package com.yjyz.iot.security.dao;

import java.util.List;

import com.yjyz.iot.security.entity.PowRole;
import com.yjyz.iot.security.entity.PowSysUser;
import com.yjyz.iot.security.entity.PowUserRoleKey;

public interface PowUserRoleMapper {
	int deleteByPrimaryKey(PowUserRoleKey key);

	int insert(PowUserRoleKey record);

	int insertSelective(PowUserRoleKey record);

	List<PowRole> selectRoleByUser(PowUserRoleKey record);

	List<PowSysUser> selectUserByRole(PowUserRoleKey record);
	
	int deleteByUserName(PowUserRoleKey key);
}