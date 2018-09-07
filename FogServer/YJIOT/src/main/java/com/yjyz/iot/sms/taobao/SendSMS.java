package com.yjyz.iot.sms.taobao;

import org.springframework.beans.factory.annotation.Value;

import com.taobao.api.DefaultTaobaoClient;
import com.taobao.api.TaobaoClient;
import com.taobao.api.request.AlibabaAliqinFcSmsNumSendRequest;
import com.taobao.api.response.AlibabaAliqinFcSmsNumSendResponse;
import com.yjyz.iot.sms.ISendSMS;
import com.yjyz.iot.sms.SMSResult;

public class SendSMS implements ISendSMS {
	@Value("${sms.taobao.url}")
	private String url;
	@Value("${sms.taobao.appKey}")
	private String appKey;
	@Value("${sms.taobao.secret}")
	private String secret;
	@Value("${sms.taobao.signName}")
	private String signName;
	@Value("${sms.taobao.templateCode}")
	private String templateCode;

	TaobaoClient client;
	AlibabaAliqinFcSmsNumSendRequest req;

	public SMSResult send(String tel, String verCode) {
		if (client == null) {
			client = new DefaultTaobaoClient(url, appKey, secret);
		}

		if (req == null) {
			req = new AlibabaAliqinFcSmsNumSendRequest();
		}
		req.setSmsType("normal");
		req.setSmsFreeSignName(signName);
		req.setSmsParamString("{\"code\":\"" + verCode + "\"}");
		req.setRecNum(tel);
		req.setSmsTemplateCode(templateCode);
		try {
			AlibabaAliqinFcSmsNumSendResponse rsp = client.execute(req);
			return new SMSResult("taobao", "0", rsp.getMsg());
		} catch (Exception e) {
			System.out.println(e.toString());
			return new SMSResult("taobao", "-1", "SMS Server Error!");
		}
	}

	public static void main(String[] args) {
		TaobaoClient client = new DefaultTaobaoClient("http://gw.api.taobao.com/router/rest", "24497634",
				"3f154f2f69c6c6e751a770fdbdbd99ec");
		AlibabaAliqinFcSmsNumSendRequest req = new AlibabaAliqinFcSmsNumSendRequest();
		req.setSmsType("normal");
		req.setSmsFreeSignName("易净优智");
		req.setSmsParamString("{\"code\":\"1234\"}");
		req.setRecNum("13439966787");
		req.setSmsTemplateCode("SMS_74790010");
		try {
			AlibabaAliqinFcSmsNumSendResponse rsp = client.execute(req);
			System.out.println(rsp.getBody());
		} catch (Exception e) {
			System.out.println(e.toString());
		}
	}

	@Override
	public SMSResult sendVerCode(String tel, String verCode) {
		// TODO Auto-generated method stub
		return null;
	}

	@Override
	public SMSResult sendAlerm(String tel, String devName) {
		// TODO Auto-generated method stub
		return null;
	}

	@Override
	public SMSResult sendAlerm(String ytxAppId, String templateCode, String tel, String devName, String message) {
		// TODO Auto-generated method stub
		return null;
	}

	@Override
	public SMSResult sendVerCode(String ytxAppId, String templateCode, String tel, String verCode) {
		// TODO Auto-generated method stub
		return null;
	}




}