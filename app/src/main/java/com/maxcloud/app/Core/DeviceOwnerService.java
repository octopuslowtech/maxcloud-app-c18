package com.maxcloud.app.Core;

import android.app.admin.DeviceAdminReceiver;
import android.content.Context;
import android.content.Intent;

public class DeviceOwnerService extends DeviceAdminReceiver {
    @Override
    public void onEnabled(Context context, Intent intent) {
    }

    @Override
    public void onDisabled(Context context, Intent intent) {
    }
}
