package com.yjyz.iot.share.dto;

public class ActivateDto {
	public String product_id;
	public String mac;
	public String devicepw;
	public String mxchipsn;
	public String regIp;

	public String getRegIp() {
		return regIp;
	}

	public void setRegIp(String regIp) {
		this.regIp = regIp;
	}

	public String getProduct_id() {
		return product_id;
	}

	public void setProduct_id(String product_id) {
		this.product_id = product_id;
	}

	public String getMac() {
		return mac;
	}

	public void setMac(String mac) {
		this.mac = mac;
	}

	public String getDevicepw() {
		return devicepw;
	}

	public void setDevicepw(String devicepw) {
		this.devicepw = devicepw;
	}

	public String getMxchipsn() {
		return mxchipsn;
	}

	public void setMxchipsn(String mxchipsn) {
		this.mxchipsn = mxchipsn;
	}

}