package com.maxcloud.app.Core;

public class PhoneDto {
    private String name;
    private String action;
    private String serial;

    public String getName() {
        return name;
    }

    public void setName(String name) {
        this.name = name;
    }

    public String getAction() {
        return action;
    }

    public void setAction(String action) {
        this.action = action;
    }

    public String getSerial() {
        return serial;
    }

    public void setSerial(String serial) {
        this.serial = serial;
    }

    public PhoneDto()
    {

    }

    public PhoneDto(String name, String action, String serial) {
        this.name = name;
        this.action = action;
        this.serial = serial;
    }
}
