package com.yjyz.iot.stats.service.impl;

import java.util.List;
import java.util.Map;

import javax.annotation.Resource;

import org.springframework.beans.factory.annotation.Value;
import org.springframework.stereotype.Service;

import com.yjyz.iot.stats.dao.AccountQueryMapper;
import com.yjyz.iot.stats.entity.QueryParm;
import com.yjyz.iot.stats.service.IAccountQueryService;

@Service("accountQueryService")
public class AccountQueryServiceImpl implements IAccountQueryService {
	@Resource
	AccountQueryMapper accountQueryDao;
	@Value("${cfg.sys.device_ttl}")
	private long DEV_TTL;

	@Override
	public List<Map<String, Object>> deviceAccountQuery(QueryParm parm) throws Exception {
		if (parm.getDeviceId() == null || parm.getDeviceId() == "") {
			parm.setDeviceId("all");
		}
		parm.setTtl(DEV_TTL);
		return this.accountQueryDao.deviceAccountQuery(parm);
	}

	@Override
	public List<Map<String, Object>> deviceOfferMonthQuery(QueryParm parm) throws Exception {
		return this.accountQueryDao.deviceOfferMonthQuery(parm);
	}

	@Override
	public List<Map<String, Object>> deviceOfferWeekQuery(QueryParm parm) throws Exception {
		return this.accountQueryDao.deviceOfferWeekQuery(parm);
	}

	@Override
	public List<Map<String, Object>> merchantAccountQuery(QueryParm parm) throws Exception {
		if (parm.getMerchantName() == null || parm.getMerchantName() == "") {
			parm.setMerchantName("all");
		}
		return this.accountQueryDao.merchantAccountQuery(parm);
	}

	@Override
	public List<Map<String, Object>> merchantOfferMonthQuery(QueryParm parm) throws Exception {
		return this.accountQueryDao.merchantOfferMonthQuery(parm);
	}

	@Override
	public List<Map<String, Object>> merchantOfferWeekQuery(QueryParm parm) throws Exception {
		return this.accountQueryDao.merchantOfferWeekQuery(parm);
	}
}