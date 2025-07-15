package com.maxcloud.app.Core;

import android.content.Context;
import android.content.Intent;
import android.view.accessibility.AccessibilityEvent;

public class NativeCore {
    static {
        System.loadLibrary("NativeCore");


    }

    public static native void onActionReceived(String action, String name);

    public static native void onConnected();

    public static native void onEvent(AccessibilityEvent event);

    public static native String addDevice(Context context, String name, String token, String
            serial, String version, String sdk, boolean isSave);

    public static native boolean authDevice(Context context);
    public static native boolean isLoginToken(Context context);

    public static native String getProductNumber();

    public static native void initMediaProject(int resultCode, Intent resultData);

    public static native boolean startServiceNative();
}
