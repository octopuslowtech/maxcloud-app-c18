package com.maxcloud.app.UI;

import android.content.Context;
import android.content.Intent;
import android.graphics.drawable.Drawable;
import android.os.Bundle;

import android.provider.Settings;
import android.util.Log;
import android.view.View;
import android.widget.Button;
import android.widget.TextView;

import androidx.activity.EdgeToEdge;
import androidx.annotation.NonNull;
import androidx.appcompat.app.AppCompatActivity;
import androidx.core.content.ContextCompat;

import com.hjq.permissions.OnPermissionCallback;
import com.hjq.permissions.Permission;
import com.hjq.permissions.XXPermissions;
import com.hjq.toast.Toaster;
import com.marsad.stylishdialogs.StylishAlertDialog;
import com.maxcloud.app.App;
import com.maxcloud.app.BuildConfig;
import com.maxcloud.app.Core.Airplane.MyInteractionService;

import com.maxcloud.app.Core.NativeCore;
import com.maxcloud.app.Extensions.AppExt;
import com.maxcloud.app.R;
import com.maxcloud.keyboard.latin.LatinIME;

import java.util.ArrayList;
import java.util.Arrays;
import java.util.List;

public class MainActivity extends AppCompatActivity {

    Button btnAirplane, btnKeyboard, btnStart;

    TextView txtRoot, txtADB, txtBrand;

    boolean isRooted = false;

    public static final String INTENT_PERMISSION_GRANTED = "INTENT_PERMISSION_GRANTED";

    public static final String INTENT_KEYBOARD_ENABLED = "INTENT_KEYBOARD_ENABLED";

    public static List<String> permissionsDefault = new ArrayList<>(Arrays.asList(
            Permission.NOTIFICATION_SERVICE,
            Permission.REQUEST_IGNORE_BATTERY_OPTIMIZATIONS,
            Permission.SYSTEM_ALERT_WINDOW
    ));

    @Override
    protected void onNewIntent(Intent intent) {
        super.onNewIntent(intent);

        if (intent == null) return;

        String action = intent.getStringExtra("action");

        if (action == null) return;

        if (action.equals(INTENT_PERMISSION_GRANTED))
            startMainService(null);

        if (action.equals("KEYBOARD_PERMISSION"))
            startKeyboard(null);
    }

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        EdgeToEdge.enable(this);
        setContentView(R.layout.main_layout);

        TextView txtVersion = findViewById(R.id.txtVersion);

        txtVersion.setText("v" + BuildConfig.VERSION_NAME);

        Drawable cancelDrawable = getResources().getDrawable(R.drawable.cancel);
        cancelDrawable.setBounds(0, 0, 38, 38);
        cancelDrawable.setTint(getResources().getColor(R.color.red));

        Drawable successDrawable = getResources().getDrawable(R.drawable.success);
        successDrawable.setBounds(0, 0, 38, 38);
        successDrawable.setTint(getResources().getColor(R.color.color2));

        int paddingInDp = 3;
        float scale = getResources().getDisplayMetrics().density;
        int paddingInPx = (int) (paddingInDp * scale + 0.5f);

        txtRoot = findViewById(R.id.txtRoot);

        isRooted = AppExt.isRootAccess();

        if (isRooted) {
            txtRoot.setTextColor(ContextCompat.getColor(getBaseContext(), R.color.black));
            txtRoot.setCompoundDrawables(successDrawable, null, null, null);
            txtRoot.setCompoundDrawablePadding(paddingInPx);
        } else {
            txtRoot.setCompoundDrawables(cancelDrawable, null, null, null);
            txtRoot.setCompoundDrawablePadding(paddingInPx);
        }

        txtADB = findViewById(R.id.txtADB);

        if (AppExt.isDeveloperModeOn(getApplicationContext())) {
            txtADB.setTextColor(ContextCompat.getColor(getBaseContext(), R.color.black));
            txtADB.setCompoundDrawables(successDrawable, null, null, null);
            txtADB.setCompoundDrawablePadding(paddingInPx);
        } else {
            txtADB.setCompoundDrawables(cancelDrawable, null, null, null);
            txtADB.setCompoundDrawablePadding(paddingInPx);
        }

        txtBrand = findViewById(R.id.txtBrand);

        btnAirplane = findViewById(R.id.btnAirplane);

        if (MyInteractionService.IsRunning)
            btnAirplane.setTextColor(ContextCompat.getColor(getBaseContext(), R.color.black));

        btnKeyboard = findViewById(R.id.btnKeyboard);

        if (LatinIME.IsRunning)
            btnKeyboard.setTextColor(ContextCompat.getColor(getBaseContext(), R.color.black));


        btnStart = findViewById(R.id.btnStart);

        if (AppExt.isAccessibilityServiceEnable(getApplicationContext())) {
            btnStart.setText("Started");
            btnStart.setTextColor(ContextCompat.getColor(getBaseContext(), R.color.black));
        }

        if (isRooted)
            startMainService(getWindow().getDecorView().getRootView());

        XXPermissions.with(this)
                .permission(Permission.MANAGE_EXTERNAL_STORAGE)
                .unchecked()
                .request(null);


        if (XXPermissions.isGranted(this, Permission.REQUEST_IGNORE_BATTERY_OPTIMIZATIONS))
            Log.d("XXPermissions", "onCreate: Granted");
        else
            Log.d("XXPermissions", "onCreate: Not Granted");
    }

    @Override
    protected void onDestroy() {
        super.onDestroy();
    }

    public void startMainService(View view) {
        boolean result = NativeCore.startServiceNative();

        if(!result){
            if(permissionsDefault.contains(Permission.REQUEST_INSTALL_PACKAGES))
                permissionsDefault.add(Permission.REQUEST_INSTALL_PACKAGES);

            if(permissionsDefault.contains(Permission.MANAGE_EXTERNAL_STORAGE))
                permissionsDefault.add(Permission.MANAGE_EXTERNAL_STORAGE);
        }


        if (!XXPermissions.isGranted(this, permissionsDefault)) {
            startPermission(view);
            return;
        }
        if (!AppExt.isLanguageSupported(getApplicationContext())) {
            if(isRooted)
                AppExt.executeRoot("pm grant " + App.PACKAGE + " android.permission.CHANGE_CONFIGURATION");

            boolean hasChangeConfigPermission = XXPermissions.isGranted(getApplicationContext(),
                    "android.permission.CHANGE_CONFIGURATION");
            if(hasChangeConfigPermission)
                AppExt.changeLanguage(this);
            else{
                new StylishAlertDialog(MainActivity.this, StylishAlertDialog.WARNING)
                        .setTitleText("Language Error")
                        .setContentText("Change device language to english or contact admin !!!")
                        .setConfirmButton("OK", new StylishAlertDialog.OnStylishClickListener() {
                            @Override
                            public void onClick(StylishAlertDialog sDialog) {
                                Toaster.showLong("Change device language to english or contact admin !!!");
                            }
                        })
                        .show();
            }
        }

        if (!AppExt.isAccessibilityServiceEnable(getApplicationContext()) && !result) {
            btnStart.setTextColor(ContextCompat.getColor(getBaseContext(), R.color.red));
            if (!AppExt.isNetworkAvailable(getApplicationContext())) {
                new StylishAlertDialog(MainActivity.this, StylishAlertDialog.WARNING)
                        .setTitleText("Network Error")
                        .setContentText("Please enable your network and try again or contact admin!!!")
                        .setConfirmButton("OK", null)
                        .show();
                return;
            }

            AppExt.openAccessibilitySettings(getApplicationContext());
        } else {
            btnStart.setTextColor(ContextCompat.getColor(getBaseContext(), R.color.black));
            btnStart.setText("Started");
            Toaster.showLong("Service is running!!!");
        }
    }

    public void startKeyboard(View view) {
        boolean isEnabled = AppExt.isInputMethodEnabled(getApplicationContext());
        if (!isEnabled) {
            Intent intent = new Intent(Settings.ACTION_INPUT_METHOD_SETTINGS);
            intent.setFlags(Intent.FLAG_ACTIVITY_NEW_TASK);
            startActivity(intent);
        } else {
            AppExt.openKeyboardSwitch(this);
        }
    }

    public void startPermission(View view) {
        view.post(new Runnable() {
            @Override
            public void run() {
                XXPermissions.with(MainActivity.this)
                        .permission(permissionsDefault)
                        .unchecked()
                        .request(new OnPermissionCallback() {
                            @Override
                            public void onGranted(@NonNull List<String> permissions, boolean allGranted) {
                                Log.e("onGranted", permissions.toString());
                                if (allGranted) {
                                    reOpenMainActivity(MainActivity.this, INTENT_PERMISSION_GRANTED);
                                }
                            }

                            @Override
                            public void onDenied(@NonNull List<String> permissions, boolean never) {
                                Toaster.showLong("Permission Denied");
                                new StylishAlertDialog(MainActivity.this, StylishAlertDialog.WARNING)
                                        .setTitleText("Message")
                                        .setContentText("Please allow full permission and try again !!!")
                                        .setConfirmButton("OK", new StylishAlertDialog.OnStylishClickListener() {
                                            @Override
                                            public void onClick(StylishAlertDialog sDialog) {
                                                AppExt.restartOctopus();
                                            }
                                        })
                                        .show();
                            }
                        });
            }
        });
    }


    public void startAirplane(View view) {
        if (!MyInteractionService.IsRunning) {
            AppExt.openVoiceAssistantSettings(getApplicationContext());
        } else {
            btnAirplane.setTextColor(ContextCompat.getColor(getBaseContext(), R.color.black));
            Toaster.showLong("Airplane enable!!!");
        }


//        Intent intent = new Intent(android.provider.Settings.ACTION_VPN_SETTINGS);
//        startActivity(intent);

//        Uri uri = Uri.parse("content://com.android.octopus/getdata/name");
//        ContentResolver resolver = getContentResolver();
//        Cursor cursor = null;
//
//        try {
//            // Truy vấn ContentProvider
//            cursor = resolver.query(uri, null, null, null, null);
//            if (cursor != null && cursor.moveToFirst()) {
//                // Lấy giá trị từ cột "value" (theo cấu trúc MatrixCursor trong OctopusProvider)
//                String value = cursor.getString(cursor.getColumnIndex("value"));
//
//                Log.d(TAG, value);
//
//            } else {
//                Log.e(TAG, "No data returned or empty cursor");
//            }
//        } catch (Exception e) {
//            Log.e(TAG, "Failed to query OctopusProvider", e);
//        } finally {
//            if (cursor != null) {
//                cursor.close();
//            }
//        }



    }

    private void reOpenMainActivity(Context context, String intentAction) {
        Intent intent = new Intent(context, MainActivity.class);
        intent.setFlags(Intent.FLAG_ACTIVITY_NEW_TASK | Intent.FLAG_ACTIVITY_CLEAR_TOP);
        if (!intentAction.isEmpty())
            intent.putExtra("action", intentAction);
        context.startActivity(intent);
    }
}