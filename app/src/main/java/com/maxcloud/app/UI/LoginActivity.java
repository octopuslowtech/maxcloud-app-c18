package com.maxcloud.app.UI;

import android.content.Context;
import android.content.Intent;
import android.graphics.Color;
import android.net.Uri;
import android.os.AsyncTask;
import android.os.Build;
import android.os.Bundle;
import android.util.Log;
import android.view.View;
import android.widget.Button;
import android.widget.CheckBox;


import androidx.activity.result.ActivityResultLauncher;
import androidx.annotation.NonNull;
import androidx.appcompat.app.AppCompatActivity;

import com.elvishew.xlog.XLog;
import com.google.android.material.textfield.TextInputLayout;

import com.google.zxing.integration.android.IntentIntegrator;

import com.hjq.permissions.OnPermissionCallback;
import com.hjq.permissions.Permission;
import com.hjq.permissions.XXPermissions;
import com.jaredrummler.android.device.DeviceName;
import com.journeyapps.barcodescanner.ScanContract;
import com.journeyapps.barcodescanner.ScanOptions;
import com.marsad.stylishdialogs.StylishAlertDialog;
import com.maxcloud.app.BuildConfig;

import com.maxcloud.app.Core.NativeCore;
import com.maxcloud.app.Extensions.AppExt;
import com.maxcloud.app.R;

import org.jetbrains.annotations.Nullable;


import java.util.List;

public class LoginActivity extends AppCompatActivity {
    TextInputLayout deviceName, deviceToken;
    Button btnScanQR;
    CheckBox ckbSaveDevice;
    StylishAlertDialog stylishDialog;

    @Override
    protected void attachBaseContext(Context base) {
        super.attachBaseContext(base);
    }

    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.login_layout);

        stylishDialog = new StylishAlertDialog(this, StylishAlertDialog.PROGRESS);
        stylishDialog.getProgressHelper().setBarColor(Color.parseColor("#16badf"));
        ckbSaveDevice = findViewById(R.id.checkboxSaveDevice);

        if (!AppExt.isSupportedAbi()) {
            new StylishAlertDialog(LoginActivity.this, StylishAlertDialog.WARNING)
                    .setTitleText("Device Error")
                    .setContentText("The device does not support 32-bit or 64-bit architecture!!!")
                    .setConfirmButton("OK", new StylishAlertDialog.OnStylishClickListener() {
                        @Override
                        public void onClick(StylishAlertDialog sDialog) {
                            AppExt.finishApp(LoginActivity.this);
                        }
                    })
                    .show();
            return;
        }

        if (Build.VERSION.SDK_INT < 25) {
            new StylishAlertDialog(LoginActivity.this, StylishAlertDialog.ERROR)
                    .setTitleText("OS Error")
                    .setContentText("Sorry, this app does not support Android versions 8 or below !!!")
                    .setConfirmButton("OK", new StylishAlertDialog.OnStylishClickListener() {
                        @Override
                        public void onClick(StylishAlertDialog sDialog) {
                            AppExt.finishApp(LoginActivity.this);
                        }
                    })
                    .show();
            return;
        }

        if (!AppExt.isNetworkAvailable(this)) {
            new StylishAlertDialog(LoginActivity.this, StylishAlertDialog.WARNING)
                    .setTitleText("Network Error")
                    .setContentText("Please enable your network and try again or contact admin !!!")
                    .setConfirmButton("OK", new StylishAlertDialog.OnStylishClickListener() {
                        @Override
                        public void onClick(StylishAlertDialog sDialog) {
                            // open network activity
                            Intent intent = new Intent(android.provider.Settings.ACTION_WIRELESS_SETTINGS);
                            intent.setFlags(Intent.FLAG_ACTIVITY_NEW_TASK);
                            startActivity(intent);
                            AppExt.finishApp(LoginActivity.this);
                        }
                    })
                    .show();
            return;
        }

        if (NativeCore.authDevice(LoginActivity.this)) {
            Intent intent = new Intent(this, MainActivity.class);
            intent.setFlags(Intent.FLAG_ACTIVITY_NEW_TASK | Intent.FLAG_ACTIVITY_CLEAR_TASK);
            startActivity(intent);
            finish();
            return;
        }

        deviceName = findViewById(R.id.deviceName);
        deviceToken = findViewById(R.id.deviceToken);

        btnScanQR = findViewById(R.id.btnScanQR);
        btnScanQR.setOnClickListener(new View.OnClickListener() {
            public void onClick(View v) {
                ScanOptions options = new ScanOptions();
                options.setOrientationLocked(true);
                options.setPrompt(getString(R.string.app_desc));
                barcodeLauncher.launch(options);
            }
        });

        deviceName.getEditText().setText(DeviceName.getDeviceName());

        Button btnLogin = findViewById(R.id.btnLogin);

        btnLogin.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                XXPermissions.with(LoginActivity.this)
                        .permission(Permission.NOTIFICATION_SERVICE, Permission.MANAGE_EXTERNAL_STORAGE)
                        .unchecked()
                        .request(new OnPermissionCallback() {
                            @Override
                            public void onGranted(@NonNull List<String> permissions, boolean allGranted) {
                                handleLogin(deviceName.getEditText().getText().toString()
                                        , deviceToken.getEditText().getText().toString());
                            }
                        });
            }
        });
    }

    public void startTelegram(View view) {
        startActivity(new Intent(Intent.ACTION_VIEW, Uri.parse("https://t.me/cloudphonefarm")));
    }

    private class LoginAsync extends AsyncTask<String, Void, Void> {
        String msg = "";

        @Override
        protected Void doInBackground(String... strings) {
            boolean isChecked = Boolean.parseBoolean(strings[2]); // Save device

            try {
                String response = NativeCore.addDevice(LoginActivity.this,
                        strings[0],
                        strings[1],
                        AppExt.getSerialNumber(LoginActivity.this),
                        BuildConfig.VERSION_NAME, Build.VERSION.RELEASE,
                        isChecked
                );

                if (response.isEmpty()) {
                    Intent intent = new Intent(LoginActivity.this, MainActivity.class);
                    intent.setFlags(Intent.FLAG_ACTIVITY_NEW_TASK | Intent.FLAG_ACTIVITY_CLEAR_TASK);
                    getApplicationContext().startActivity(intent);
                    finish();
                } else msg = response;
            } catch (Exception e) {
                msg = e.getMessage();
                XLog.e("Error: ", e);
            }
            return null;
        }

        @Override
        protected void onPostExecute(Void unused) {
            super.onPostExecute(unused);
            stylishDialog.dismiss();
            if (msg.length() > 0) {
                new StylishAlertDialog(LoginActivity.this, StylishAlertDialog.ERROR)
                        .setContentText(msg)
                        .show();
            }
        }
    }

    @Override
    protected void onActivityResult(int requestCode, int resultCode, @Nullable Intent data) {
        super.onActivityResult(requestCode, resultCode, data);
        if (requestCode == 1001) {
            Intent intent = new Intent(getPackageManager().getLaunchIntentForPackage(getPackageName()));
            intent.setFlags(Intent.FLAG_ACTIVITY_NEW_TASK | Intent.FLAG_ACTIVITY_CLEAR_TASK);
            startActivity(intent);
        }
    }

    private final ActivityResultLauncher<ScanOptions> barcodeLauncher =
            registerForActivityResult(new ScanContract(), result -> {
                if (result.getContents() != null) {
                    deviceToken.getEditText().setText(result.getContents());
                    handleLogin(deviceName.getEditText().getText().toString()
                            , deviceToken.getEditText().getText().toString());
                }
            });

    void handleLogin(String name, String token) {
        if (name.isEmpty() || token.isEmpty()) {
            new StylishAlertDialog(LoginActivity.this, StylishAlertDialog.WARNING)
                    .setTitleText("Message")
                    .setContentText("Please enter data and try again!!!")
                    .setConfirmButton("OK", null)
                    .show();
            return;
        }
        stylishDialog
                .setTitleText("Processing")
                .setCancellable(false)
                .setCancelledOnTouchOutside(true)
                .show();

        new LoginAsync().execute(name, token, String.valueOf(ckbSaveDevice.isChecked()));

    }


}