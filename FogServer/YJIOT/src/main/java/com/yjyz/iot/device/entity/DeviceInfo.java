package com.yjyz.iot.device.entity;

import java.util.Date;

public class DeviceInfo {
    private String deviceId;

    private String devicePw;

    private String deviceMac;

    private String productId;

    private String moduleName;

    private String firmware;

    private String firmwareType;

    private Boolean isActivate;

    private Boolean isRecovery;

    private String iotVersion;

    private String productModel;

    private String deviceName;

    private Date activateTime;
    
    private String regIp;

    public String getRegIp() {
		return regIp;
	}

	public void setRegIp(String regIp) {
		this.regIp = regIp;
	}

	public String getDeviceId() {
        return deviceId;
    }

    public void setDeviceId(String deviceId) {
        this.deviceId = deviceId == null ? null : deviceId.trim();
    }

    public String getDevicePw() {
        return devicePw;
    }

    public void setDevicePw(String devicePw) {
        this.devicePw = devicePw == null ? null : devicePw.trim();
    }

    public String getDeviceMac() {
        return deviceMac;
    }

    public void setDeviceMac(String deviceMac) {
        this.deviceMac = deviceMac == null ? null : deviceMac.trim();
    }

    public String getProductId() {
        return productId;
    }

    public void setProductId(String productId) {
        this.productId = productId == null ? null : productId.trim();
    }

    public String getModuleName() {
        return moduleName;
    }

    public void setModuleName(String moduleName) {
        this.moduleName = moduleName == null ? null : moduleName.trim();
    }

    public String getFirmware() {
        return firmware;
    }

    public void setFirmware(String firmware) {
        this.firmware = firmware == null ? null : firmware.trim();
    }

    public String getFirmwareType() {
        return firmwareType;
    }

    public void setFirmwareType(String firmwareType) {
        this.firmwareType = firmwareType == null ? null : firmwareType.trim();
    }

    public Boolean getIsActivate() {
        return isActivate;
    }

    public void setIsActivate(Boolean isActivate) {
        this.isActivate = isActivate;
    }

    public Boolean getIsRecovery() {
        return isRecovery;
    }

    public void setIsRecovery(Boolean isRecovery) {
        this.isRecovery = isRecovery;
    }

    public String getIotVersion() {
        return iotVersion;
    }

    public void setIotVersion(String iotVersion) {
        this.iotVersion = iotVersion == null ? null : iotVersion.trim();
    }

    public String getProductModel() {
        return productModel;
    }

    public void setProductModel(String productModel) {
        this.productModel = productModel == null ? null : productModel.trim();
    }

    public String getDeviceName() {
        return deviceName;
    }

    public void setDeviceName(String deviceName) {
        this.deviceName = deviceName == null ? null : deviceName.trim();
    }

    public Date getActivateTime() {
        return activateTime;
    }

    public void setActivateTime(Date activateTime) {
        this.activateTime = activateTime;
    }
}