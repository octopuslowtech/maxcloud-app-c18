package com.maxcloud.app;

import android.content.BroadcastReceiver;
import android.content.Context;
import android.content.Intent;
import android.os.Build;
import android.os.Bundle;
import android.util.Log;

import com.jaredrummler.android.device.DeviceName;
import com.maxcloud.app.Core.MainService;
import com.maxcloud.app.Core.NativeCore;
import com.maxcloud.app.Extensions.AppExt;
import com.maxcloud.app.UI.MainActivity;

import org.json.JSONObject;


import java.util.concurrent.CountDownLatch;
import java.util.concurrent.TimeUnit;

public class AdbCaller extends BroadcastReceiver {
    private static final String ADB_PING_PONG = App.PACKAGE + ".PING_PONG";
    private static final String ADB_LOGIN_DEVICE = App.PACKAGE + ".LOGIN_DEVICE";
    private static final String ADB_FACTORY_RESET = App.PACKAGE + ".FACTORY_RESET";

    private final String TAG = "AdbCaller";

    @Override
    public void onReceive(Context context, Intent intent) {
        Log.d(TAG, "onReceive: " + intent.getAction());
        String action = intent.getAction();
        if (action.equals(ADB_PING_PONG)) {
            JSONObject json = new JSONObject();

            boolean isLogin = NativeCore.isLoginToken(context);

            putJson("IS_LOGIN", isLogin, json);
            putJson("IP_ADDRESS", AppExt.getLocalIpAddress(), json);
            putJson("APP_VERSION", BuildConfig.VERSION_NAME, json);

            if (!isLogin) {
                setResultExtras(json);
                return;
            }

            putJson("IS_RUNNING", MainService.isRunning, json);
            putJson("PRODUCT_NUMBER", NativeCore.getProductNumber(), json);

            setResultExtras(json);
        }

        if (action.equals(ADB_LOGIN_DEVICE)) {
            CountDownLatch latch = new CountDownLatch(1);
            new Thread() {
                @Override
                public void run() {
                    super.run();

                    JSONObject json = new JSONObject();
                    String token = intent.getStringExtra("token");

                    if (token.isEmpty()) {
                        Log.d("AdbCaller", "onReceive: token is empty");
                        putJson("MESSAGE", "Token is empty", json);
                        setResultExtras(json);
                        return;
                    }
                    try {
                        if(NativeCore.authDevice(context)){
                            putJson("MESSAGE", "Device is already logged in", json);
                            setResultExtras(json);
                            return;
                        }

                        String response = NativeCore.addDevice(context,
                                DeviceName.getDeviceName(),
                                token,
                                AppExt.getSerialNumber(context),
                                BuildConfig.VERSION_NAME, Build.VERSION.RELEASE,
                                true
                        );

                        if (response.isEmpty()) {
                            Intent intent = new Intent(context, MainActivity.class);
                            intent.setFlags(Intent.FLAG_ACTIVITY_NEW_TASK | Intent.FLAG_ACTIVITY_CLEAR_TASK);
                            App.getContext().startActivity(intent);

                            putJson("MESSAGE", "LOGIN_SUCCESS", json);
                        }
                        else{
                            putJson("MESSAGE", "LOGIN_FAILED_" + response, json);
                        }
                    } catch (Exception e) {
                        putJson("MESSAGE", "LOGIN_FAILED_" + e.getMessage(), json);
                    }

                    setResultExtras(json);

                    latch.countDown();
                }
            }.start();

            try {
                latch.await(15, TimeUnit.SECONDS);
            } catch (Throwable th) {
            }
        }
        if (action.equals(ADB_FACTORY_RESET)) {
            boolean isSuccess = AppExt.luoncomotcohoilangaymai(context);
            JSONObject json = new JSONObject();
            putJson("FACTORY_RESET", isSuccess, json);
            setResultExtras(json);
        }
    }

    private void setResultExtras(JSONObject json) {
        try {
            String result = json.toString();

            setResultData(result);

            Bundle bundle = getResultExtras(true);
            bundle.putString("result", result);
        } catch (Exception e) {
            Log.e("AdbCaller", "setResultExtras: ", e);
        }
    }

    private static void putJson(String key, Object value, JSONObject json) {
        try {
            json.put(key, value);
        } catch (Exception e) {
            Log.e("AdbCaller", "putJson: ", e);
        }
    }
}
