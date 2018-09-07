package com.yjyz.iot.share.service.impl;

import java.util.Date;
import java.util.HashMap;
import java.util.List;
import java.util.Map;
import java.util.UUID;

import javax.annotation.Resource;

import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.beans.factory.annotation.Value;
import org.springframework.stereotype.Service;

import com.alibaba.fastjson.JSONObject;
import com.yjyz.iot.comm.ConstParm;
import com.yjyz.iot.device.dao.DevAccountMapper;
import com.yjyz.iot.device.dao.DevConsumeMapper;
import com.yjyz.iot.device.dao.DevControlMapper;
import com.yjyz.iot.device.dao.DevRealTimeMapper;
import com.yjyz.iot.device.dao.DeviceInfoMapper;
import com.yjyz.iot.device.entity.DevAccount;
import com.yjyz.iot.device.entity.DevConsume;
import com.yjyz.iot.device.entity.DevControl;
import com.yjyz.iot.device.entity.DevRealTime;
import com.yjyz.iot.device.entity.DeviceInfo;
import com.yjyz.iot.mq.producer.topic.TopicSender;
import com.yjyz.iot.ota.dao.OTAFileInfoMapper;
import com.yjyz.iot.ota.entity.OTAFileInfo;
import com.yjyz.iot.product.dao.ProdInfoMapper;
import com.yjyz.iot.product.entity.ProdInfo;
import com.yjyz.iot.security.dao.PowAppInfoMapper;
import com.yjyz.iot.security.entity.PowAppInfo;
import com.yjyz.iot.share.service.IM26DeviceService;

@Service("m26DeviceService")
public class M26DeviceServiceImpl implements IM26DeviceService {
	@Resource
	PowAppInfoMapper appInfoDao;
	@Resource
	ProdInfoMapper prodInfoDao;
	@Resource
	DeviceInfoMapper deviceInfoDao;
	@Resource
	OTAFileInfoMapper otaFileInfoDao;
	@Resource
	DevRealTimeMapper devRealTimeDao;
	@Resource
	DevAccountMapper devAccountDao;
	@Resource
	DevControlMapper devControlDao;
	@Resource
	DevConsumeMapper devConsumeDao;
	@Autowired
	private TopicSender topicSender;

	@Value("${cfg.sys.device.owner_type}")
	private String ONWER_TYPE;

	@Override
	public DeviceInfo activate(DeviceInfo deviceInfo) throws Exception {
		DeviceInfo info = this.deviceInfoDao.selectByMAC(deviceInfo.getDeviceMac());
		if (info == null) {
			// get product info
			ProdInfo prodInfo = this.prodInfoDao.selectByPrimaryKey(deviceInfo.getProductId());
			PowAppInfo appInfo = this.appInfoDao.selectByPrimaryKey(prodInfo.getAppId());

			deviceInfo.setDeviceId(UUID.randomUUID().toString());
			deviceInfo.setDeviceName(prodInfo.getName());
			
			this.deviceInfoDao.insertSelective(deviceInfo);
			DevAccount devAccount = new DevAccount();
			devAccount.setDeviceId(deviceInfo.getDeviceId());

			devAccount.setOwnerCode(appInfo.getFirmCreditCode());
			devAccount.setOwnerType(ONWER_TYPE);
			devAccount.setOwnerName(appInfo.getFirmName());
			devAccount.setLatitude(appInfo.getFirmLatitude());
			devAccount.setLongitude(appInfo.getFirmLongitude());

			this.devAccountDao.insertSelective(devAccount);
		} else {
			deviceInfo.setDeviceId(info.getDeviceId());
			this.deviceInfoDao.updateByPrimaryKeySelective(deviceInfo);
		}
		return deviceInfo;
	}

	@Override
	public OTAFileInfo otaCheck(DeviceInfo deviceInfo) throws Exception {
		DeviceInfo info = this.deviceInfoDao.selectByPrimaryKey(deviceInfo.getDeviceId());
		if (info != null) {
			this.deviceInfoDao.updateByPrimaryKeySelective(deviceInfo);

			OTAFileInfo otaCheck = new OTAFileInfo();
			otaCheck.setProductId(info.getProductId());
			otaCheck.setFirmware(deviceInfo.getFirmware());
			otaCheck.setModulename(deviceInfo.getModuleName());
			otaCheck.setFirmware(deviceInfo.getFirmwareType());

			OTAFileInfo otaFile = this.otaFileInfoDao.selectByOTACheck(otaCheck);

			if (otaFile != null) {
				int clientVer = Integer.valueOf(deviceInfo.getFirmware().split("@")[1]);
				int serverVer = Integer.valueOf(otaFile.getFirmware().split("@")[1]);
				if (clientVer < serverVer) {

					return otaFile;
				}
			}
		}
		return null;
	}

	@Override
	public boolean uploadData(String deviceId, boolean isSave, String jsonData) throws Exception {

		if (isSave && jsonData.isEmpty()) {
			throw new Exception();
		}

		String tableName = "T_RT_" + deviceId.replaceAll("\\-", "");
		String jsonString = "{".concat(jsonData).concat("}");

		if (isSave) {
			int hasTable = this.devRealTimeDao.existTable(tableName);
			if (hasTable == 0) {
				JSONObject obj = JSONObject.parseObject(jsonString);
				StringBuffer colDefBuffer = new StringBuffer();
				colDefBuffer.append("dtime timestamp(3) not null DEFAULT CURRENT_TIMESTAMP(3),device_id varchar(36),");
				for (String key : obj.keySet()) {
					colDefBuffer.append("col_" + key.toLowerCase() + " decimal(10,4),");
				}
				colDefBuffer.append("primary key (dtime)");
				DevRealTime drt = new DevRealTime();
				drt.setColDefString(colDefBuffer.toString());
				drt.setTableName(tableName);
				this.devRealTimeDao.createTable(drt);
			}

			JSONObject obj = JSONObject.parseObject(jsonString);
			StringBuffer insertBuffer = new StringBuffer();
			insertBuffer.append("(");
			for (String key : obj.keySet()) {
				insertBuffer.append("col_" + key.toLowerCase() + ",");
			}
			insertBuffer.append("device_id) values(");
			for (String key : obj.keySet()) {
				insertBuffer.append(obj.get(key) + ",");
			}

			insertBuffer.append("'" + deviceId + "') ");
			DevRealTime drt = new DevRealTime();
			drt.setInsertString(insertBuffer.toString());
			drt.setTableName(tableName);
			int ret = this.devRealTimeDao.insertData(drt);
			if (ret != 1) {
				return false;
			}
		}
		// 更新设备在线时间
		// DevAccount devAccount = this.devAccount.selectByPrimaryKey(deviceId);
		DevAccount devAccount = new DevAccount();
		devAccount.setDeviceId(deviceId);
		devAccount.setLastTime(new Date(System.currentTimeMillis()));
		devAccount.setIsOnline(true);
		this.devAccountDao.updateByPrimaryKeySelective(devAccount);
		topicSender.send("d2c." + deviceId + ".status", jsonString);
		return true;
	}

	@Override
	public Map<String, Object> syncStatus(String deviceId, String jsonData) throws Exception {

		// 发送设备端最新数据
		if (jsonData != null) {
			String jsonString = "{".concat(jsonData).concat("}");
			topicSender.send("d2c." + deviceId + ".status", jsonString);
		}
		// 更新设备在线
		DevAccount devAccount = new DevAccount();
		devAccount.setDeviceId(deviceId);
		devAccount.setLastTime(new Date(System.currentTimeMillis()));
		devAccount.setIsOnline(true);
		this.devAccountDao.updateByPrimaryKeySelective(devAccount);

		// 获取最新状态
		Map<String, Object> retData = new HashMap<String, Object>();
		List<DevControl> list = this.devControlDao.selectLastStatusByDid(deviceId);

		if (list.size() == 0) {
			retData.put("SF", ConstParm.DICT_SHARE_M26_NODATA);
		} else {
			retData.put("SF", ConstParm.DICT_SHARE_M26_DATA);
			Map<String, Object> sd = new HashMap<String, Object>();
			for (int i = 0; i < list.size(); i++) {
				DevControl dc = list.get(i);
				try {
					sd.put(dc.getCmd(), Integer.parseInt(dc.getParm().toString()));
				} catch (Exception e) {
					sd.put(dc.getCmd(), dc.getParm());
				}
			}
			retData.put("SD", sd);
			this.devControlDao.updateStatusByDid(deviceId);
		}

		// 获取未消费筹码
		DevConsume dc = this.devConsumeDao.selectUnConOrder(deviceId);
		if (dc == null) {
			retData.put("CF", ConstParm.DICT_SHARE_M26_NODATA);
		} else {
			retData.put("CF", ConstParm.DICT_SHARE_M26_DATA);
			Map<String, Object> cd = new HashMap<String, Object>();
			cd.put("C", dc.getChips());
			cd.put("T", 1);
			cd.put("O", dc.getOrderNo());
			cd.put("S", dc.getStatus());
			retData.put("CD", cd);
		}

		return retData;
	}

	@Override
	public boolean chipsConfirm(String deviceId, String orderNo) throws Exception {
		DevConsume devConsume = this.devConsumeDao.selectByOrderNo(orderNo);
		if (devConsume == null) {
			return false;
		}

		long curr_ts = System.currentTimeMillis();
		devConsume.setBillBeing(new Date(curr_ts));
		devConsume.setBillExport(new Date(curr_ts + devConsume.getChips() * 60 * 1000));
		devConsume.setStatus("2");

		int ret = this.devConsumeDao.updateByPrimaryKeySelective(devConsume);
		if (ret != 1) {
			return false;
		}

		return true;
	}

}