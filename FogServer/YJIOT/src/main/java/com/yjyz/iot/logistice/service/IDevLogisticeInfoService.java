package com.yjyz.iot.logistice.service;

import java.util.List;

import com.yjyz.iot.logistice.entity.DevLogisticeInfo;

public interface IDevLogisticeInfoService {

	/**
	 * @name:addLogisticeInfo
	 * @TODO:增加物流信息
	 * @date:2017年12月25日 上午10:19:35
	 * @param info
	 * @param userId
	 * @return
	 * @throws Exception
	 *             boolean
	 */
	DevLogisticeInfo addLogisticeInfo(DevLogisticeInfo info, String userId) throws Exception;

	/**
	 * @name:updLogisticeInfo
	 * @TODO:修改物流信息
	 * @date:2017年12月25日 上午10:19:49
	 * @param info
	 * @return
	 * @throws Exception
	 *             boolean
	 */
	boolean updLogisticeInfo(DevLogisticeInfo info) throws Exception;

	/**
	 * @name:selLogisticeInfo
	 * @TODO:查询设备物流信息
	 * @date:2017年12月25日 上午10:20:01
	 * @param info
	 * @return
	 * @throws Exception
	 *             List<DevLogisticeInfo>
	 */
	List<DevLogisticeInfo>  selectByDeviceMac(String deviceMac) throws Exception;

}