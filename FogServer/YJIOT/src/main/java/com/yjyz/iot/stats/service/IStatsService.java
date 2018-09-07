package com.yjyz.iot.stats.service;

import java.util.List;
import java.util.Map;

import com.yjyz.iot.stats.entity.StatsParm;

public interface IStatsService {
	List<Map<String, Object>> devStateType(StatsParm parm)throws Exception;
	List<Map<String, Object>> userRegMonth(StatsParm parm)throws Exception;
	List<Map<String, Object>> fundMonth(StatsParm parm)throws Exception;
	List<Map<String, Object>> fundMerChantDay(StatsParm parm)throws Exception;
	List<Map<String, Object>> fundMerChantWeek(StatsParm parm)throws Exception;
	List<Map<String, Object>> fundTimesDay(StatsParm parm)throws Exception;
	List<Map<String, Object>> fundTimesWeek(StatsParm parm)throws Exception;
	List<Map<String, Object>> devSeller(StatsParm parm)throws Exception;
	List<Map<String, Object>> devOnlineState(StatsParm parm)throws Exception;
	List<Map<String, Object>> fundSellerWeek(StatsParm parm)throws Exception;
	List<Map<String, Object>> fundDevTop15(StatsParm parm)throws Exception;
	List<Map<String, Object>> fundUserTop10(StatsParm parm)throws Exception;
	List<Map<String, Object>> userRegTypeWeek(StatsParm parm)throws Exception;
	List<Map<String, Object>> devExamMain(StatsParm parm)throws Exception;
	List<Map<String, Object>> devExamItem(StatsParm parm)throws Exception;
}