package com.yjyz.iot.share.action;

import java.util.Date;
import java.util.List;
import java.util.Map;
import java.util.UUID;

import org.apache.commons.logging.Log;
import org.apache.commons.logging.LogFactory;
import org.springframework.beans.BeanUtils;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.transaction.annotation.Transactional;
import org.springframework.web.bind.annotation.CrossOrigin;
import org.springframework.web.bind.annotation.RequestBody;
import org.springframework.web.bind.annotation.RequestHeader;
import org.springframework.web.bind.annotation.RequestMapping;
import org.springframework.web.bind.annotation.RequestMethod;
import org.springframework.web.bind.annotation.RestController;

import com.yjyz.iot.comm.ConstParm;
import com.yjyz.iot.comm.RetInfoDto;
import com.yjyz.iot.device.dto.DevCommDto;
import com.yjyz.iot.device.entity.DevAccount;
import com.yjyz.iot.device.entity.DevConsume;
import com.yjyz.iot.device.entity.DeviceInfo;
import com.yjyz.iot.device.service.IDevRealTimeService;
import com.yjyz.iot.device.service.IDeviceService;
import com.yjyz.iot.dict.entity.DictCharge;
import com.yjyz.iot.security.entity.PowClientUser;
import com.yjyz.iot.security.utils.ClientJwtToken;
import com.yjyz.iot.security.utils.ClientJwtUtil;
import com.yjyz.iot.share.dto.ClientCommDto;
import com.yjyz.iot.share.dto.DevConsumeDto;
import com.yjyz.iot.share.service.IClientService;

/**
 * @class :ClientController
 * @TODO :微信端访问接口
 * @author:Herolizhen
 * @date :2017年9月14日下午6:04:12
 */

// 3.用户注册， 需要增加IP地址限制
@CrossOrigin
@RestController
@RequestMapping("/client")
public class ClientController {

	private static Log log = LogFactory.getLog(ClientController.class);
	@Autowired
	private IClientService clientService;
	@Autowired
	private IDeviceService deviceService;
	@Autowired
	private IDevRealTimeService devRealTimeService;

	@Autowired
	ClientJwtUtil jwtUtil;

	@Transactional
	@RequestMapping(value = "/register", method = RequestMethod.POST)
	public RetInfoDto register(@RequestBody PowClientUser clientUser) {
		RetInfoDto info = new RetInfoDto();
		try {
			PowClientUser user = this.clientService.register(clientUser);
			ClientJwtToken jwtToken = new ClientJwtToken(user.getClientId());
			String token = jwtUtil.createJWT(jwtToken);
			info.meta.put("message", "register user sucess.");
			info.meta.put("code", ConstParm.SUCESS_CODE);
			info.data.put("token", token);
		} catch (Exception e) {
			log.error(e);
			info.meta.put("message", "register user fail.");
			info.meta.put("code", ConstParm.ERR_CODE_JWT);
		}
		return info;
	}

	/**
	 * @name:getJwtToken
	 * @TODO:
	 * @date:2017年10月26日 下午4:19:03
	 * @param clientUserDto
	 * @return RetInfoDto
	 */
	@Transactional
	@RequestMapping(value = "/getJwtToken", method = RequestMethod.POST)
	public RetInfoDto getJwtToken(@RequestBody PowClientUser clientUser) {
		RetInfoDto info = new RetInfoDto();
		try {
			if (clientUser.getOpenid().isEmpty()) {
				info.meta.put("message", "get JwtToken fail, no openid set.");
				info.meta.put("code", ConstParm.ERR_NO_CLIENT);
				return info;
			}
			PowClientUser user = this.clientService.getUserByOpenId(clientUser);
			ClientJwtToken jwtToken = new ClientJwtToken(user.getClientId());
			String token = jwtUtil.createJWT(jwtToken);
			info.meta.put("message", "get JwtToken sucess.");
			info.meta.put("code", ConstParm.SUCESS_CODE);
			info.data.put("token", token);
		} catch (Exception e) {
			log.error(e);
			info.meta.put("message", "get JwtToken fail.");
			info.meta.put("code", ConstParm.ERR_CODE_JWT);
		}
		return info;
	}

	/**
	 * @name:getChargeInfo
	 * @TODO:选择套餐
	 * @date:2017年9月15日 下午1:16:06
	 * @param deviceId
	 * @param Authorization
	 * @return RetDataInfoDto
	 */
	@Transactional
	@RequestMapping(value = "/getChargeInfo", method = RequestMethod.POST, consumes = "application/json;charset=UTF-8", produces = "application/json;charset=UTF-8")
	public RetInfoDto getChargeInfo(@RequestBody ClientCommDto commDto, @RequestHeader String Authorization) {
		RetInfoDto info = new RetInfoDto();
		ClientJwtToken jwtToken;
		try {
			jwtToken = jwtUtil.parseToken(Authorization);
		} catch (Exception e) {
			info.meta.put("message", "access token is wrong!");
			info.meta.put("code", ConstParm.ERR_CODE_JWT);
			return info;
		}

		try {
			List<DictCharge> chargeList = this.deviceService.getDevChargeInfo(commDto.getDeviceId());
			if (chargeList.size() == 0) {
				info.meta.put("message", "no charge list for this device.");
				info.meta.put("code", ConstParm.ERR_NO_CHARGE);
				return info;
			}
			info.meta.put("message", "get data sucess.");
			info.meta.put("code", ConstParm.SUCESS_CODE);
			info.data.put("chargelist", chargeList);

			List<DevConsume> his = this.clientService.getConsumeHistory(commDto.getDeviceId(), jwtToken.getUser_id());
			if (his.size() > 0) {
				info.data.put("consumehis", his);
			}
		} catch (Exception e) {
			log.error(e);
			info.meta.put("message", "get JwtToken fail.");
			info.meta.put("code", ConstParm.ERR_SHARE_CLIENT_GETCHARGEINFO);
		}
		return info;
	}

	/**
	 * @name:preBill
	 * @TODO:预交费
	 * @date:2017年9月15日 下午2:17:32
	 * @param devConsume
	 * @param Authorization
	 * @return RetDataInfoDto
	 */
	@Transactional
	@RequestMapping(value = "/preBill", method = RequestMethod.POST)
	public RetInfoDto preBill(@RequestBody DevConsume devConsume, @RequestHeader String Authorization) {

		RetInfoDto info = new RetInfoDto();
		ClientJwtToken jwtToken;
		try {
			jwtToken = jwtUtil.parseToken(Authorization);
		} catch (Exception e) {
			info.meta.put("message", "access token is wrong!");
			info.meta.put("code", ConstParm.ERR_CODE_JWT);
			return info;
		}
		try {
			devConsume.setOrderNo(UUID.randomUUID().toString());
			devConsume.setCreateTime(new Date());
			devConsume.setClientId(jwtToken.getUser_id());
			int ret = this.clientService.preBill(devConsume);
			if (ret == 0) {
				info.meta.put("message", "prepay fail.");
				info.meta.put("code", ConstParm.ERR_INSERT);

			} else {
				info.meta.put("message", "prepay sucess.");
				info.meta.put("code", ConstParm.SUCESS_CODE);
				info.data.put("prebill", devConsume);
			}
		} catch (Exception e) {
			log.error(e);
			info.meta.put("message", "get JwtToken fail.");
			info.meta.put("code", ConstParm.ERR_SHARE_CLIENT_PREBILL);
		}
		return info;
	}

	/**
	 * @name:consume
	 * @TODO:缴费成功确认， 开机指令
	 * @date:2017年9月15日 下午2:17:57
	 * @param devConsumeDao
	 * @param Authorization
	 * @return RetDataInfoDto
	 */
	// @Transactional
	@RequestMapping(value = "/consume", method = RequestMethod.POST)
	public RetInfoDto consume(@RequestBody DevConsumeDto dto) {

		RetInfoDto info = new RetInfoDto();
		if (dto.getOrderNo() == null) {
			info.meta.put("message", "order no is worng.");
			info.meta.put("code", ConstParm.ERR_NO_ORDER);
			return info;
		}
		try {
			DevConsume devConsume = new DevConsume();
			BeanUtils.copyProperties(dto, devConsume);
			int retNo = this.clientService.consume(dto.getOpenId(), devConsume);
			if (retNo == ConstParm.SUCESS_CODE) {
				info.meta.put("message", "consume sucess.");
				info.meta.put("code", ConstParm.SUCESS_CODE);
			} else {
				info.meta.put("message", "consume failer.");
				info.meta.put("code", retNo);
			}
		} catch (Exception e) {
			log.error(e);
			info.meta.put("message", "consume failer.");
			info.meta.put("code", ConstParm.ERR_SHARE_CLIENT_CONSUME);
		}
		return info;
	}

	/**
	 * @name:getDevInfo
	 * @TODO:软件调用 通过mac地址获取设备信息JWT认证
	 * @date:2017年9月14日 下午4:32:03
	 * @param deviceMac
	 * @param Authorization
	 * @return RetDataInfoDto
	 */
	@Transactional
	@RequestMapping(value = "/getDevInfoByMac", method = RequestMethod.POST, consumes = "application/json;charset=UTF-8", produces = "application/json;charset=UTF-8")
	public RetInfoDto getDevInfoByMac(@RequestBody ClientCommDto commDto, @RequestHeader String Authorization) {
		RetInfoDto info = new RetInfoDto();
		try {
			jwtUtil.parseToken(Authorization);
		} catch (Exception e) {
			info.meta.put("message", "access token is wrong!");
			info.meta.put("code", ConstParm.ERR_CODE_JWT);
			return info;
		}
		try {
			DeviceInfo devInfo = this.deviceService.getDevInfoByMac(commDto.getDeviceMac());
			if (devInfo == null) {
				info.meta.put("message", "devInfo  is not exist!");
				info.meta.put("code", ConstParm.ERR_NO_DEVICE);
				return info;
			}
			DevAccount devAccount = this.deviceService.getDevAccount(devInfo.getDeviceId());
			if (devAccount == null) {
				info.meta.put("message", "DeviceID  is not exist!");
				info.meta.put("code", ConstParm.ERR_NO_DEVACCOUNT);
				return info;
			}
			info.meta.put("message", "Get Device Success.");
			info.meta.put("code", ConstParm.SUCESS_CODE);
			info.data.put("devAccount", devAccount);
			info.data.put("devInfo", devInfo);
			try {
				// 获取最新的数据
				Map<String, Object> devStatus = this.devRealTimeService.selectLast(devAccount.getDeviceId());
				info.data.put("devStatus", devStatus);
			} catch (Exception e) {
				System.out.println("ESHARE Alert getDevInfoByMac Fail, DeviceId:"+ devAccount.getDeviceId());
				log.error(e);
			}
		} catch (Exception e) {
			log.error(e);
			info.meta.put("message", "getDevInfo Exception");
			info.meta.put("code", ConstParm.ERR_SHARE_CLIENT_GETDEVINFO);
		}
		return info;
	}

	/**
	 * @name:getDevInfoByOrderNo
	 * @TODO:通过订单号获取设备信息
	 * @date:2017年10月17日 下午12:17:23
	 * @param orderNo
	 * @param Authorization
	 * @return RetDataInfoDto
	 */
	@Transactional
	@RequestMapping(value = "/getOrderInfo", method = RequestMethod.POST, consumes = "application/json;charset=UTF-8", produces = "application/json;charset=UTF-8")
	public RetInfoDto getOrderInfo(@RequestBody ClientCommDto commDto) {
		RetInfoDto info = new RetInfoDto();
		try {
			DeviceInfo devInfo = this.deviceService.getDevInfoByOrderNo(commDto.getOrderNo());
			if (devInfo == null) {
				info.meta.put("message", "Device  is not exist!");
				info.meta.put("code", ConstParm.ERR_NO_DEVICE);
				return info;
			}
			DevAccount devAccount = this.deviceService.getDevAccount(devInfo.getDeviceId());
			if (devAccount == null) {
				info.meta.put("message", "DeviceID  is not exist!");
				info.meta.put("code", ConstParm.ERR_NO_DEVACCOUNT);
				return info;
			}

			PowClientUser user = this.clientService.getUserByOrderNo(commDto.getOrderNo());
			info.meta.put("message", "Get Device Success.");
			info.meta.put("code", ConstParm.SUCESS_CODE);
			info.data.put("devAccount", devAccount);
			info.data.put("devInfo", devInfo);
			try {
				// 获取最新的数据
				Map<String, Object> devStatus = this.devRealTimeService.selectLast(devAccount.getDeviceId());
				info.data.put("devStatus", devStatus);
			} catch (Exception e) {
				System.out.println("ESHARE Alert getOrderInfo Fail, DeviceId:"+ devAccount.getDeviceId());
				log.error(e);
			}
			info.data.put("user", user);
		} catch (Exception e) {
			log.error(e);
			info.meta.put("message", "orderNo  is not exist!");
			info.meta.put("code", ConstParm.ERR_SHARE_CLIENT_GETDEVINFO);
		}
		return info;
	}

	/**
	 * @name:sendCommandAdv
	 * @TODO:软件端发送设备指令JWT认证
	 * @date:2017年9月14日 下午4:39:05
	 * @param devRealDataInfo
	 * @param Authorization
	 * @return RetDataInfoDto
	 */
	@Transactional
	@RequestMapping(value = "/sendCommand", method = RequestMethod.POST)
	public RetInfoDto sendCommand(@RequestBody DevCommDto dto, @RequestHeader String Authorization) {
		RetInfoDto info = new RetInfoDto();
		ClientJwtToken jwtToken;
		try {
			jwtToken = jwtUtil.parseToken(Authorization);
		} catch (Exception e) {
			info.meta.put("message", "access token is wrong!");
			info.meta.put("code", ConstParm.ERR_CODE_JWT);
			return info;
		}

		String validSend = dto.getPayload().toString().toUpperCase();

		if (validSend.contains("CHIPS") || (validSend.contains("TYPE") && !validSend.contains("EXAMTYPE"))) {
			info.meta.put("message", "illegal Command send fail.");
			info.meta.put("code", ConstParm.ERR_ILLEGAL_COMMAND);
			return info;
		}

		try {
			boolean isOk = this.deviceService.sendCommand(dto, jwtToken.getUser_id());
			if (isOk) {
				info.meta.put("message", "Real Data Send Success.");
				info.meta.put("code", ConstParm.SUCESS_CODE);
			} else {
				info.meta.put("message", "Real Data Save False.");
				info.meta.put("code", ConstParm.ERR_SHARE_CLIENT_SENDCOMMAMD);
			}
		} catch (Exception e) {
			log.error(e);
			info.meta.put("message", "Real Data Send Fail.");
			info.meta.put("code", ConstParm.ERR_SHARE_CLIENT_SENDCOMMAMD);
		}
		return info;
	}

	/**
	 * @name:getUnCosumeBill
	 * @TODO: 根据mac地址获取支付成功且未发送筹码的缴费记录
	 * @date:2017年10月30日 下午6:58:26
	 * @param commDto
	 * @param Authorization
	 * @return RetDataInfoDto
	 */
	@Transactional
	@RequestMapping(value = "/getUnCosumeBill", method = RequestMethod.POST, consumes = "application/json;charset=UTF-8", produces = "application/json;charset=UTF-8")
	public RetInfoDto getUnCosumeBill(@RequestBody ClientCommDto commDto, @RequestHeader String Authorization) {
		RetInfoDto info = new RetInfoDto();
		try {
			jwtUtil.parseToken(Authorization);
		} catch (Exception e) {
			info.meta.put("message", "access token is wrong!");
			info.meta.put("code", ConstParm.ERR_CODE_JWT);
			return info;
		}
		try {
			DevConsume dc = this.clientService.getUnCosumeBill(commDto.getDeviceMac());
			if (dc != null) {
				info.meta.put("code", ConstParm.SUCESS_CODE);
				info.data.put("order", dc);
			} else {
				info.meta.put("message", "unconsume order is not exist!");
				info.meta.put("code", ConstParm.SUCESS_CODE);
			}
		} catch (Exception e) {
			log.error(e);
			info.meta.put("message", "unconsume order is not exist!");
			info.meta.put("code", ConstParm.ERR_SHARE_CLIENT_GETUNCOSUMEBILL);
		}
		return info;
	}

}