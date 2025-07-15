package com.maxcloud.app.UI;

import android.content.Intent;

import android.media.projection.MediaProjectionManager;
import android.net.Uri;
import android.net.VpnService;
import android.os.Bundle;
import android.provider.Settings;
import android.util.Log;
import android.widget.Toast;

import androidx.activity.EdgeToEdge;
import androidx.annotation.NonNull;
import androidx.appcompat.app.AppCompatActivity;

import com.hjq.permissions.OnPermissionCallback;
import com.hjq.permissions.Permission;
import com.hjq.permissions.XXPermissions;
import com.hjq.toast.Toaster;
import com.marsad.stylishdialogs.StylishAlertDialog;
import com.maxcloud.app.Core.NativeCore;
import com.maxcloud.app.Extensions.AppExt;

import java.util.ArrayList;
import java.util.Arrays;
import java.util.List;

public class PermissionActivity extends AppCompatActivity {
    private static final int REQUEST_VPN_PERMISSION = 1;

    private static final int REQUEST_SCREENSHOT = 151004;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        EdgeToEdge.enable(this);

        handleIntent();

    }

    private void handleIntent() {
        Intent intent = getIntent();
        if (intent == null) {
            finish();
            return;
        }

        String action = intent.getStringExtra("action");
        if (action == null) {
            Toast.makeText(this, "No action provided", Toast.LENGTH_SHORT).show();
            finish();
            return;
        }

        switch (action) {
            case "VPN_SERVICE_PERMISSION":
                requestVPNService();
                break;
            case "MEDIA_PROJECTION_PERMISSION":
                requestMediaProjectionPermission();
                break;
            case "OVERLAY_PERMISSION":
                requestOverlayPermission();
                finish();
                break;
            case "REQUEST_IGNORE_BATTERY_OPTIMIZATIONS":
                requestIgnoreBatteryOptimize();
                break;
            default:
                Toast.makeText(this, "Unknown action: " + action, Toast.LENGTH_SHORT).show();
                finish();
                break;
        }
    }


    private void requestIgnoreBatteryOptimize() {
        if (XXPermissions.isGranted(this, Permission.REQUEST_IGNORE_BATTERY_OPTIMIZATIONS)) {
            onPermissionGranted();
            return;
        }
        XXPermissions.with(this)
                .permission(Permission.REQUEST_IGNORE_BATTERY_OPTIMIZATIONS)
                .unchecked()
                .request(new OnPermissionCallback() {
                    @Override
                    public void onGranted(@NonNull List<String> permissions, boolean allGranted) {
                        onPermissionGranted();
                    }

                    @Override
                    public void onDenied(@NonNull List<String> permissions, boolean never) {
                        onPermissionDenied();
                    }
                });
    }

    private void requestVPNService() {
        Intent vpnIntent = VpnService.prepare(this);
        if (vpnIntent != null)
            startActivityForResult(vpnIntent, REQUEST_VPN_PERMISSION);
        else
            onPermissionGranted();
    }

    private void requestOverlayPermission() {
        Intent intent = new Intent(Settings.ACTION_MANAGE_OVERLAY_PERMISSION,
                Uri.parse("package:" + getPackageName()));
        intent.addFlags(Intent.FLAG_ACTIVITY_NEW_TASK);
        startActivity(intent);
    }


    private static MediaProjectionManager mediaProjectionManager;

    private void requestMediaProjectionPermission() {
        mediaProjectionManager = (MediaProjectionManager) getSystemService(MEDIA_PROJECTION_SERVICE);
        startActivityForResult(mediaProjectionManager.createScreenCaptureIntent(), REQUEST_SCREENSHOT);
    }

    @Override
    protected void onActivityResult(int requestCode, int resultCode, Intent resultData) {
        super.onActivityResult(requestCode, resultCode, resultData);

        if (requestCode == REQUEST_SCREENSHOT) {
            if (resultCode == RESULT_OK) {
                NativeCore.initMediaProject(resultCode, resultData);
                onPermissionGranted();
            } else {
                requestMediaProjectionPermission();
            }
        }

        if (requestCode == REQUEST_VPN_PERMISSION) {
            if (resultCode == RESULT_OK) {
                onPermissionGranted();
            } else {
                requestMediaProjectionPermission();
            }
        }
    }

    private void onPermissionGranted() {
        finish();

    }

    private void onPermissionDenied() {
        Toaster.showLong("Permission denied");
        finish();
    }

}
