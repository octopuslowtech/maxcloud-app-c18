package com.maxcloud.app.Core.Airplane;

import android.content.BroadcastReceiver;
import android.content.Context;
import android.content.Intent;
import android.content.IntentFilter;
import android.os.Build;
import android.os.Bundle;
import android.service.voice.VoiceInteractionService;
import android.util.Log;

import androidx.annotation.RequiresApi;

import com.maxcloud.app.App;

public class MyInteractionService extends VoiceInteractionService  {
    private static final String TAG = "MyInteractionService";
    public static boolean IsRunning;
    private BroadcastReceiver broadcastReceiver = null;

    public static final String TURN_ON = App.PACKAGE + ".TURN_ON_AIRPLANE";
    public static final String TURN_OFF = App.PACKAGE + ".TURN_OFF_AIRPLANE";

    @Override
    public void onReady() {
        super.onReady();
        Log.v(TAG, "onReady");
        IsRunning = true;

        if(broadcastReceiver == null) {
            IntentFilter filter = new IntentFilter();
            filter.addAction(TURN_ON);
            filter.addAction(TURN_OFF);

            broadcastReceiver = new BroadcastReceiver() {
                @RequiresApi(api = Build.VERSION_CODES.M)
                @Override
                public void onReceive(Context context, Intent intent) {
                    if (!IsRunning) return;

                    String action = intent.getAction();

                    Log.d(TAG, "onReceive: " + action);

                    Bundle bundle = new Bundle();
                    if (action.equals(TURN_ON)) {
                        bundle.putString("action", TURN_ON);
                    } else if (action.equals(TURN_OFF)) {
                        bundle.putString("action", TURN_OFF);
                    }

                    showSession(bundle, 0);
                }
            };

            if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.O)
                registerReceiver(broadcastReceiver, filter, Context.RECEIVER_EXPORTED);
            else
                registerReceiver(broadcastReceiver, filter);
        }

        Intent intent = new Intent(getPackageManager().getLaunchIntentForPackage(getPackageName()));
        intent.setFlags(Intent.FLAG_ACTIVITY_NEW_TASK | Intent.FLAG_ACTIVITY_CLEAR_TASK);
        startActivity(intent);
    }
    @Override
    public void onShutdown() {
        super.onShutdown();
        Log.v(TAG, "onShutdown");
        unregisterReceiver(broadcastReceiver);
        IsRunning = false;
    }

}