package com.yjyz.iot.exam.service;

import java.util.List;

import com.yjyz.iot.exam.dto.DevExamDto;
import com.yjyz.iot.exam.entity.DevExamDetail;
import com.yjyz.iot.exam.entity.DevExamMain;

public interface IDevExamService {

	DevExamMain startExam(DevExamDto dto, String userId) throws Exception;

	List<DevExamDetail> getExamResult(DevExamDto dto) throws Exception;

	boolean updateExamMain(DevExamMain devExamMain) throws Exception;

	public boolean saveExamItem(DevExamDto dto) throws Exception;

	public boolean stopExam(DevExamMain devExamMain) throws Exception;

	public boolean saveExamItem(String examId, List<DevExamDetail> examItems) throws Exception;

	public List<DevExamMain> queryExamList(DevExamDto dto) throws Exception;

	public boolean confirmExamManu(DevExamDto dto) throws Exception;
}