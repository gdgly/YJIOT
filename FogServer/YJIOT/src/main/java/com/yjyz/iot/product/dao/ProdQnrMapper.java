package com.yjyz.iot.product.dao;

import com.yjyz.iot.product.entity.ProdQnr;

public interface ProdQnrMapper {
	int deleteByPrimaryKey(String qnrId);

	int insert(ProdQnr record);

	int insertSelective(ProdQnr record);

	ProdQnr selectByPrimaryKey(String qnrId);

	int updateByPrimaryKeySelective(ProdQnr record);

	int updateByPrimaryKey(ProdQnr record);
}