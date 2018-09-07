package com.yjyz.iot.notice.dao;

import com.yjyz.iot.notice.entity.NoticeConfig;

public interface NoticeConfigMapper {
    int deleteByPrimaryKey(String appId);

    int insert(NoticeConfig record);

    int insertSelective(NoticeConfig record);

    NoticeConfig selectByPrimaryKey(String appId);

    int updateByPrimaryKeySelective(NoticeConfig record);

    int updateByPrimaryKey(NoticeConfig record);
}