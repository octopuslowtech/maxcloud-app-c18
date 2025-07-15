package com.maxcloud.app.Extensions;

import android.accounts.Account;
import android.accounts.AccountManager;
import android.annotation.SuppressLint;
import android.app.Activity;
import android.app.Application;
import android.app.WallpaperManager;
import android.app.admin.DevicePolicyManager;
import android.content.ActivityNotFoundException;
import android.content.BroadcastReceiver;
import android.content.ClipData;
import android.content.ClipboardManager;
import android.content.ComponentName;
import android.content.Context;
import android.content.Intent;
import android.content.pm.ApplicationInfo;
import android.content.pm.PackageInfo;
import android.content.pm.PackageManager;
import android.content.pm.ResolveInfo;
import android.content.res.Configuration;
import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
import android.graphics.Canvas;
import android.graphics.Color;
import android.graphics.LinearGradient;
import android.graphics.Paint;
import android.graphics.Point;
import android.graphics.Shader;
import android.graphics.Typeface;
import android.graphics.drawable.Drawable;
import android.hardware.camera2.CameraCharacteristics;
import android.hardware.camera2.CameraManager;
import android.net.ConnectivityManager;
import android.net.NetworkInfo;
import android.net.Uri;
import android.os.Build;
import android.os.Bundle;
import android.os.LocaleList;
import android.os.PowerManager;
import android.provider.Browser;
import android.provider.Settings;
import android.text.TextUtils;
import android.util.Base64;
import android.util.DisplayMetrics;
import android.util.Log;
import android.view.Display;
import android.view.WindowManager;
import android.view.accessibility.AccessibilityManager;
import android.view.inputmethod.InputMethodManager;

import androidx.core.content.ContextCompat;
import androidx.core.content.FileProvider;

import com.elvishew.xlog.XLog;
import com.hjq.permissions.XXPermissions;
import com.maxcloud.app.App;

import com.maxcloud.app.BuildConfig;
import com.maxcloud.app.Core.MainService;
import com.maxcloud.app.R;


import org.json.JSONArray;
import org.json.JSONException;
import org.json.JSONObject;

import java.io.BufferedReader;
import java.io.ByteArrayOutputStream;
import java.io.DataOutputStream;
import java.io.File;
import java.io.FileInputStream;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.InputStreamReader;
import java.lang.reflect.Method;
import java.net.InetAddress;
import java.net.NetworkInterface;
import java.util.ArrayList;
import java.util.Enumeration;
import java.util.Iterator;
import java.util.List;
import java.util.Locale;
import java.util.concurrent.CountDownLatch;
import java.util.concurrent.TimeUnit;


public class AppExt {
    public static final String TAG = "Device";

    public static boolean isDebugging() {
        return BuildConfig.DEBUG;
    }

    public static void unlockScreen(Context context) {
        try {
            PowerManager powerManager = (PowerManager) context.getSystemService(Context.POWER_SERVICE);
            if (!powerManager.isInteractive()) {
                PowerManager.WakeLock wakeLock = powerManager.newWakeLock(PowerManager.FULL_WAKE_LOCK |
                        PowerManager.ACQUIRE_CAUSES_WAKEUP |
                        PowerManager.ON_AFTER_RELEASE, "appname::WakeLock");
                wakeLock.acquire();
                wakeLock.release();
            }
        } catch (Exception ex) {
            XLog.e("isScreenOn " + ex.getMessage());
        }
    }

    public static boolean isRootAccess() {
        return executeRoot("echo octopus").length() > 0;
    }

    public static String executeRoot(String commands) {
        String result = "";
        Process shell = null;
        DataOutputStream out = null;
        BufferedReader in = null;
        try {
            shell = Runtime.getRuntime().exec("su");
            out = new DataOutputStream(shell.getOutputStream());
            in = new BufferedReader(new InputStreamReader(shell.getInputStream()));

            if (commands.contains("\n")) {
                String[] listCmds = commands.split("\n");
                for (String cmd : listCmds) {
                    out.writeBytes(cmd + "\n");
                    out.flush();
                }
            } else {
                out.writeBytes(commands + "\n");
                out.flush();
            }
            out.writeBytes("exit\n");
            out.flush();

            String line;
            StringBuilder sb = new StringBuilder();
            while ((line = in.readLine()) != null) {
                sb.append(line).append("\n");
            }
            result = sb.toString();

            shell.waitFor();


        } catch (Exception ex) {
            Log.e(TAG, "executeRoot: " + ex);
        } finally {
            try {
                if (out != null) {
                    out.close();
                }
                if (in != null) {
                    in.close();
                }
            } catch (Exception e) {
            }
        }
        return result;
    }

    public static boolean installAPK(String packageName, Context context) {
        try {
            File apkFile = new File(packageName);

            Uri apkUri = FileProvider.getUriForFile(context, context.getApplicationContext().getPackageName() + ".provider", apkFile);
            Intent intent = new Intent(Intent.ACTION_VIEW);
            intent.setDataAndType(apkUri, "application/vnd.android.package-archive");
            intent.addFlags(Intent.FLAG_ACTIVITY_NEW_TASK);
            intent.addFlags(Intent.FLAG_GRANT_READ_URI_PERMISSION);

            List<ResolveInfo> resolvedIntentActivities = context.getPackageManager().queryIntentActivities(intent, 0);
            for (ResolveInfo resolveInfo : resolvedIntentActivities)
                context.grantUriPermission(resolveInfo.activityInfo.packageName, apkUri, Intent.FLAG_GRANT_READ_URI_PERMISSION);

            context.startActivity(intent);


            return true;
        } catch (Exception ex) {
            XLog.e("installAPK: " + ex.getMessage());
        }
        return false;
    }

    public static boolean navigateTik(String uriString, String packageName, boolean isBrowser, Context context) {
        Intent intentTikTok = new Intent(Intent.ACTION_VIEW, Uri.parse(uriString));
        intentTikTok.addFlags(Intent.FLAG_ACTIVITY_NEW_TASK);
        intentTikTok.setPackage(packageName);
        try {
            if (isBrowser)
                intentTikTok.putExtra(Browser.EXTRA_APPLICATION_ID, packageName);

            context.startActivity(intentTikTok);
            return true;
        } catch (ActivityNotFoundException e) {
            intentTikTok.setPackage(null);
            context.startActivity(intentTikTok);
        } catch (Exception ex) {
        }
        return false;
    }

    public static boolean navigateFB(Context context, String uriString) {
        try {
            Intent intent = new Intent(Intent.ACTION_VIEW);
            intent.setComponent(ComponentName.unflattenFromString("com.facebook.katana/.IntentUriHandler"));
            intent.addFlags(Intent.FLAG_ACTIVITY_NEW_TASK);
            intent.setData(Uri.parse(uriString));
            context.startActivity(intent);
            return true;
        } catch (Exception ex) {
            Log.e(TAG, "navigateFB: " + ex.getMessage());
        }
        return false;
    }

    public static boolean startActivity(String action, Context context) {
        try {
            Intent intent = new Intent(action);
            intent.addFlags(Intent.FLAG_ACTIVITY_NEW_TASK);
            context.startActivity(intent);
            return true;
        } catch (Exception ex) {
            XLog.e("startActivity " + ex.getMessage());
        }
        return false;
    }

    public static String getSerialNumber(Context context) {
        String serialNumber = "";
        try {
            Class<?> c = Class.forName("android.os.SystemProperties");
            Method get = c.getMethod("get", String.class);
            serialNumber = (String) get.invoke(c, "gsm.sn1");
            if (serialNumber.isEmpty() || serialNumber.equals("unknown"))
                serialNumber = (String) get.invoke(c, "ril.serialnumber");
            if (serialNumber.isEmpty() || serialNumber.equals("unknown"))
                serialNumber = (String) get.invoke(c, "ro.serialno");
            if (serialNumber.isEmpty() || serialNumber.equals("unknown"))
                serialNumber = (String) get.invoke(c, "sys.serialnumber");
            if (serialNumber.isEmpty() || serialNumber.equals("unknown"))
                serialNumber = Build.SERIAL;
            if (serialNumber.isEmpty() || serialNumber.equals("unknown"))
                serialNumber = Build.getSerial();
        } catch (Exception e) {
            e.printStackTrace();
        }
        if (serialNumber == "unknown")
            serialNumber = "";

        if (serialNumber.isEmpty())
            serialNumber = Settings.Secure.getString(context.getContentResolver(), Settings.Secure.ANDROID_ID);

        return serialNumber;
    }

    public static void openVoiceAssistantSettings(Context context) {
        startActivity(Settings.ACTION_VOICE_INPUT_SETTINGS, context);
    }

    public static boolean isLanguageSupported(Context context) {
        String[] supportedLanguages = {"en_US", "en_GB", "en"};
        Configuration config = context.getResources().getConfiguration();
        String locale = config.locale.toString();
        for (String supportedLanguage : supportedLanguages) {
            if (supportedLanguage.equals(locale)) {
                return true;
            }
        }
        return false;
    }

    public static boolean isSupportedAbi() {
        String[] supportedAbis;

        if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.LOLLIPOP) {
            supportedAbis = Build.SUPPORTED_ABIS;
        } else {
            supportedAbis = new String[]{Build.CPU_ABI, Build.CPU_ABI2};
        }

        for (String abi : supportedAbis) {
            if ("armeabi-v7a".equals(abi) || "arm64-v8a".equals(abi))
                return true;
        }
        return false;
    }

    public static boolean isNetworkAvailable(Context context) {
        ConnectivityManager connectivityManager
                = (ConnectivityManager) context.getSystemService(Context.CONNECTIVITY_SERVICE);
        NetworkInfo activeNetworkInfo = connectivityManager.getActiveNetworkInfo();

        return activeNetworkInfo != null && activeNetworkInfo.isConnected();
    }

    public static boolean isAccessibilityServiceEnable(Context context) {
//        try {
//            int accessibilityEnabled = Settings.Secure.getInt(context.getContentResolver(), Settings.Secure.ACCESSIBILITY_ENABLED);
//            if (accessibilityEnabled == 1) {
//                String services = Settings.Secure.getString(context.getContentResolver(), Settings.Secure.ENABLED_ACCESSIBILITY_SERVICES);
//                if (services != null)
//                    return services.toLowerCase().contains(context.getPackageName().toLowerCase());
//            }
//        } catch (Settings.SettingNotFoundException e) {
//        }
//        return false;
        return MainService.isRunning;
    }

    public static boolean isDeveloperModeOn(Context context) {
        return Settings.Global.getInt(
                context.getContentResolver(),
                Settings.Global.DEVELOPMENT_SETTINGS_ENABLED,
                0
        ) != 0;
    }

    public static void openAccessibilitySettings(Context context) {
        startActivity(Settings.ACTION_ACCESSIBILITY_SETTINGS, context);
    }

    public static void openKeyboardSwitch(Context context) {
        try {
            InputMethodManager inputMethodManager = (InputMethodManager) context.getSystemService(Context.INPUT_METHOD_SERVICE);
            inputMethodManager.showInputMethodPicker();
        } catch (Exception unused) {
        }
    }

    public static boolean isInputMethodEnabled(Context context) {
        try {
            String enabledInputMethods = Settings.Secure.getString(
                    context.getContentResolver(),
                    Settings.Secure.ENABLED_INPUT_METHODS
            );
            if (!TextUtils.isEmpty(enabledInputMethods)) {
                return enabledInputMethods.contains("com.maxcloud.keyboard.latin.LatinIME");
            }
        } catch (Exception e) {
            Log.e("KeyboardUtils", "Error checking if input method is enabled", e);
        }
        return false;
    }

    public static void restartOctopus() {
        Intent intent = new Intent(Intent.ACTION_MAIN);
        intent.addCategory(Intent.CATEGORY_LAUNCHER);
        intent.setFlags(Intent.FLAG_ACTIVITY_NEW_TASK);

        String packageName = App.getContext().getPackageName();
        PackageManager pm = App.getContext().getPackageManager();
        Intent info = pm.getLaunchIntentForPackage(packageName);

        if (info != null) {
            ComponentName componentName = info.getComponent();
            Intent mainIntent = Intent.makeRestartActivityTask(componentName);
            App.getContext().startActivity(mainIntent);
            System.exit(0);
        }
    }

    public static boolean isAppInstalled(Context context, String packageName) {
        try {
            context.getPackageManager().getPackageInfo(packageName, 0);
            return true;
        } catch (PackageManager.NameNotFoundException e) {
            return false; // Ứng dụng chưa được cài đặt
        }
    }

    public static void finishApp(Activity activity) {
        activity.finish();
        System.exit(0);
    }

    public static boolean changeLanguage(Context context) {
        try {
            ArrayList<Locale> locales = new ArrayList<>();
            locales.add(new Locale("en", "US"));

            Configuration configuration = getSystemConfiguration();

            if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.N) {
                configuration.setLocales(new LocaleList(locales.toArray(new Locale[0])));
            } else {
                configuration.locale = locales.get(0);
            }

            configuration.getClass().getField("userSetLocale").setBoolean(configuration, true);

            Class<?> activityManagerClass = Class.forName("android.app.ActivityManager" +
                    (Build.VERSION.SDK_INT >= 26 ? "" : "Native"));
            Method getService = activityManagerClass.getMethod(
                    Build.VERSION.SDK_INT >= 26 ? "getService" : "getDefault"
            );
            Object activityManager = getService.invoke(activityManagerClass);
            Method updateConfiguration = Build.VERSION.SDK_INT >= 26
                    ? Class.forName("android.app.IActivityManager").getMethod("updateConfiguration", Configuration.class)
                    : activityManagerClass.getMethod("updateConfiguration", Configuration.class);

            updateConfiguration.invoke(activityManager, configuration);
            return true;
        } catch (Exception e) {
            Log.e(TAG, "Failed to change language", e);
        }
        return false;
    }

    private static Configuration getSystemConfiguration() throws Exception {
        Class<?> activityManagerClass = Class.forName("android.app.ActivityManager" +
                (Build.VERSION.SDK_INT >= 26 ? "" : "Native"));
        Method getService = activityManagerClass.getMethod(
                Build.VERSION.SDK_INT >= 26 ? "getService" : "getDefault"
        );
        Object activityManager = getService.invoke(activityManagerClass);
        Method getConfiguration = Build.VERSION.SDK_INT >= 26
                ? Class.forName("android.app.IActivityManager").getMethod("getConfiguration")
                : activityManagerClass.getMethod("getConfiguration");
        return (Configuration) getConfiguration.invoke(activityManager);
    }

    public static String sendBroadcastAndWait(Context context, String str) {
        try {
            final CountDownLatch countDownLatch = new CountDownLatch(1);
            final String[] strArr = {null};
            BroadcastReceiver broadcastReceiver = new BroadcastReceiver() {
                @Override
                public void onReceive(Context context, Intent intent) {
                    Bundle resultExtras = getResultExtras(true);
                    if (resultExtras != null) {
                        strArr[0] = resultExtras.getString("result");
                    } else {
                        strArr[0] = "";
                    }
                    countDownLatch.countDown();
                }
            };
            JSONObject jSONObject = new JSONObject(str);
            Intent intent = new Intent(jSONObject.getString("action"));
            if (jSONObject.has("package")) {
                intent.setPackage(jSONObject.getString("package"));
            }
            if (jSONObject.has("package") && jSONObject.has("class_name")) {
                intent.setClassName(jSONObject.getString("package"), jSONObject.getString("class_name"));
            }
            if (jSONObject.has("extra")) {
                JSONObject jSONObject2 = jSONObject.getJSONObject("extra");
                Iterator<String> keys = jSONObject2.keys();
                while (keys.hasNext()) {
                    String next = keys.next();
                    Object obj = jSONObject2.get(next);
                    if (obj instanceof String) {
                        intent.putExtra(next, (String) obj);
                    } else if (obj instanceof Integer) {
                        intent.putExtra(next, (Integer) obj);
                    } else if (obj instanceof Boolean) {
                        intent.putExtra(next, (Boolean) obj);
                    } else if (obj instanceof Double) {
                        intent.putExtra(next, (Double) obj);
                    } else if (obj instanceof Float) {
                        intent.putExtra(next, (Float) obj);
                    } else if (obj instanceof Long) {
                        intent.putExtra(next, (Long) obj);
                    }
                }
            }
            context.sendOrderedBroadcast(intent, null, broadcastReceiver, null, -1, null, null);
            if (countDownLatch.await(jSONObject.getInt("timeout"), TimeUnit.SECONDS)) {
                return strArr[0];
            }
        } catch (Exception e) {

        }
        return null;
    }

    public static boolean startActivityByIntent(Context context, String str) {
        try {
            JSONObject jsonObject = new JSONObject(str);
            Intent intent = new Intent();

            if (jsonObject.has("action")) {
                intent.setAction(jsonObject.getString("action"));
            }

            if (jsonObject.has("package") && jsonObject.has("class_name")) {
                ComponentName componentName = new ComponentName(
                        jsonObject.getString("package"),
                        jsonObject.getString("class_name")
                );
                intent.setComponent(componentName);
            }

            if (jsonObject.has("extra")) {
                JSONObject jSONObject2 = jsonObject.getJSONObject("extra");
                Iterator<String> keys = jSONObject2.keys();
                while (keys.hasNext()) {
                    String next = keys.next();
                    Object obj = jSONObject2.get(next);
                    if (obj instanceof String) {
                        intent.putExtra(next, (String) obj);
                    } else if (obj instanceof Integer) {
                        intent.putExtra(next, (Integer) obj);
                    } else if (obj instanceof Boolean) {
                        intent.putExtra(next, (Boolean) obj);
                    } else if (obj instanceof Double) {
                        intent.putExtra(next, (Double) obj);
                    } else if (obj instanceof Float) {
                        intent.putExtra(next, (Float) obj);
                    } else if (obj instanceof Long) {
                        intent.putExtra(next, (Long) obj);
                    }
                }
            }

            if (jsonObject.has("category")) {
                intent.addCategory(jsonObject.getString("category"));
            }

            if (jsonObject.has("new_task")) {
                intent.setFlags(Intent.FLAG_ACTIVITY_NEW_TASK | Intent.FLAG_ACTIVITY_CLEAR_TASK);
            } else
                intent.setFlags(Intent.FLAG_ACTIVITY_NEW_TASK);


            context.startActivity(intent);

            return true;
        } catch (Exception e) {
            Log.e(TAG, "startActivityByIntent: " + e.getMessage());
            return false;
        }
    }

    public static void toggleFlash(Context context) {
        CameraManager cameraManager = (CameraManager) context.getSystemService(Context.CAMERA_SERVICE);
        try {
            String cameraId = cameraManager.getCameraIdList()[0];
            CameraCharacteristics characteristics = cameraManager.getCameraCharacteristics(cameraId);

            Boolean hasFlash = characteristics.get(CameraCharacteristics.FLASH_INFO_AVAILABLE);

            if (hasFlash != null && hasFlash) {
                cameraManager.setTorchMode(cameraId, true);
            }
        } catch (Exception e) {
            e.printStackTrace();
        }
    }

    public static void setClipboard(Context context, String text) {
        ClipboardManager clipboard = (ClipboardManager) context.getSystemService(Context.CLIPBOARD_SERVICE);

        ClipData clip = ClipData.newPlainText("note", text);

        if (clipboard != null) {
            clipboard.setPrimaryClip(clip);
        }
    }

    public static String getClipboard(Context context) {
        ClipboardManager clipboard = (ClipboardManager) context.getSystemService(Context.CLIPBOARD_SERVICE);

        if (clipboard != null && clipboard.hasPrimaryClip()) {
            ClipData clip = clipboard.getPrimaryClip();

            if (clip != null && clip.getItemCount() > 0) {
                return clip.getItemAt(0).getText().toString();
            }
        }
        return null;
    }

    public static String readFile(File goodFile) {
        StringBuilder content = new StringBuilder();
        try {
            BufferedReader reader = new BufferedReader(new InputStreamReader(new FileInputStream(goodFile)));
            String line;
            while ((line = reader.readLine()) != null) {
                content.append(line);
            }
            reader.close();
        } catch (IOException e) {
            e.printStackTrace();
        }
        return content.toString();
    }

    public static boolean setWallpaper(Context context, String imagePath) {
        try {
            WallpaperManager wallpaperManager = WallpaperManager.getInstance(context);
            Bitmap bitmap = BitmapFactory.decodeFile(imagePath);

            wallpaperManager.setBitmap(bitmap);
            return true;
        } catch (Exception e) {
            XLog.e("setWallpaper: " + e.getMessage());
        }
        return false;
    }

    public static boolean createWallpaper(Context context, String text, String imagePath) {
        try {
            // Lấy kích thước màn hình
            DisplayMetrics displayMetrics = context.getResources().getDisplayMetrics();
            int width = displayMetrics.widthPixels;
            int height = displayMetrics.heightPixels;

            if (width > height) {
                int temp = width;
                width = height;
                height = temp;
            }

            // Tạo bitmap với kích thước màn hình
            Bitmap bitmap = Bitmap.createBitmap(width, height, Bitmap.Config.ARGB_8888);
            Canvas canvas = new Canvas(bitmap);

            // Vẽ nền đen
            Paint paintBackground = new Paint();
            paintBackground.setColor(text.equals("expired") ? Color.RED : Color.BLACK);
            canvas.drawRect(0, 0, width, height, paintBackground);

            // Lấy màu từ resources
            int startColor = ContextCompat.getColor(context, R.color.min2);
            int endColor = ContextCompat.getColor(context, R.color.min1);

            // Gradient cho chữ chính
            Shader textShader = new LinearGradient(0, 0, width, 0,
                    new int[]{startColor, endColor},
                    null, Shader.TileMode.CLAMP);

            // Tạo Paint cho chữ chính
            Paint paintText = new Paint();
            paintText.setTextAlign(Paint.Align.CENTER);
            paintText.setTypeface(Typeface.DEFAULT_BOLD); // Chữ đậm
            paintText.setShader(textShader); // Áp dụng gradient

            // Xác định kích thước chữ chính lớn nhất có thể
            float textSize = width * 0.6f; // Bắt đầu với 60% chiều rộng màn hình
            paintText.setTextSize(textSize);

            // Giảm dần textSize nếu chữ bị tràn
            while (paintText.measureText(text) > width * 0.9 || (textSize * 1.2) > height) {
                textSize -= 5;
                paintText.setTextSize(textSize);
                if (textSize <= 20) break;
            }

            // Tọa độ giữa màn hình cho chữ chính
            float xPos = width / 2f;
            float yPos = height / 2f - (paintText.descent() + paintText.ascent()) / 2;

            // Vẽ chữ chính
            canvas.drawText(text, xPos, yPos, paintText);

            Paint paintSmallText = new Paint();
            paintSmallText.setShader(textShader); // Áp dụng gradient
            paintSmallText.setTextAlign(Paint.Align.CENTER);
            paintSmallText.setTypeface(Typeface.SANS_SERIF);

            // ✅ Scale kích thước chữ nhỏ theo màn hình
            float smallTextSize = width * 0.05f; // 5% chiều rộng màn hình
            paintSmallText.setTextSize(smallTextSize);

            float smallTextX = width / 2f;
            float smallTextY = height * 0.8f; // Đặt vị trí ở 8/10 màn hình

            canvas.drawText("onox", smallTextX, smallTextY, paintSmallText);

            File file = new File(imagePath);

            // Ghi bitmap vào file
            FileOutputStream outputStream = new FileOutputStream(file);
            bitmap.compress(Bitmap.CompressFormat.JPEG, 100, outputStream);
            outputStream.flush();
            outputStream.close();

            return true;
        } catch (IOException e) {
            e.printStackTrace();
            return false;
        }
    }

    public static String getLocalIpAddress() {
        try {
            for (Enumeration<NetworkInterface> en = NetworkInterface.getNetworkInterfaces(); en.hasMoreElements(); ) {
                NetworkInterface intf = en.nextElement();

                // Bỏ qua giao diện VPN và các giao diện không hoạt động
                if (intf.isLoopback() || !intf.isUp() || intf.isVirtual() ||
                        intf.getName().contains("tun") || intf.getName().contains("tap") ||
                        intf.getDisplayName().contains("VPN")) {
                    continue;
                }

                for (Enumeration<InetAddress> enumIpAddr = intf.getInetAddresses(); enumIpAddr.hasMoreElements(); ) {
                    InetAddress inetAddress = enumIpAddr.nextElement();
                    if (!inetAddress.isLoopbackAddress() && inetAddress instanceof java.net.Inet4Address) {
                        return inetAddress.getHostAddress();
                    }
                }
            }
        } catch (Exception e) {
            e.printStackTrace();
        }
        return null;
    }

    @SuppressLint("PrivateApi")
    public static String getProcessName() {
        if (Build.VERSION.SDK_INT >= 28)
            return Application.getProcessName();
        try {
            Class<?> activityThread = Class.forName("android.app.ActivityThread");
            String methodName = "currentProcessName";
            Method getProcessName = activityThread.getDeclaredMethod(methodName);
            return (String) getProcessName.invoke(null);
        } catch (Exception e) {
            return BuildConfig.APPLICATION_ID;
        }
    }

    public static boolean luoncomotcohoilangaymai(Context context) {
        DevicePolicyManager dpm = (DevicePolicyManager) context.getSystemService(Context.DEVICE_POLICY_SERVICE);
        if (dpm == null || !dpm.isDeviceOwnerApp(context.getPackageName())) {
            return false;
        }

        try {
            dpm.wipeData(DevicePolicyManager.WIPE_EXTERNAL_STORAGE);
            return true;
        } catch (SecurityException e) {
        }

        return false;
    }

    public static Point getScreenSize(Context context) {
        WindowManager windowManager = (WindowManager) context.getSystemService(Context.WINDOW_SERVICE);
        Display display = windowManager.getDefaultDisplay();

        Point screenSize = new Point();
        display.getRealSize(screenSize);

        if (screenSize.x > screenSize.y) {
            // Landscape: return as is (width > height)
            return new Point(screenSize.x, screenSize.y);
        } else {
            // Portrait: swap to make width > height
            return new Point(screenSize.y, screenSize.x);
        }
    }
    public static String getPackagesInstalledWithIcon(Context context) {
        try {
            PackageManager packageManager = context.getPackageManager();

            List<ApplicationInfo> installedApps = packageManager.getInstalledApplications(PackageManager.GET_META_DATA);

            List<ApplicationInfo> thirdPartyApps = new ArrayList<>();
            for (ApplicationInfo appInfo : installedApps) {
                if ((appInfo.flags & ApplicationInfo.FLAG_SYSTEM) == 0) {
                    thirdPartyApps.add(appInfo);
                }
            }

            JSONArray jsonArray = new JSONArray();

            for (ApplicationInfo appInfo : thirdPartyApps) {
                try {
                    JSONObject jsonObject = new JSONObject();

                    jsonObject.put("packageName", packageManager.getApplicationLabel(appInfo).toString());

                    jsonObject.put("packageSpec", appInfo.packageName);

                    String base64Image = drawableToBase64(packageManager.getApplicationIcon(appInfo));
                    jsonObject.put("packageIcon", base64Image != null ? base64Image : "");

                    jsonArray.put(jsonObject);
                } catch (Exception e) {
                    Log.e(TAG, "Error processing package: " + appInfo.packageName, e);
                }
            }

            return jsonArray.toString();
        } catch (Exception ex) {
            Log.e(TAG, "getPackagesInstalledWithIcon error", ex);
            return "";
        }
    }

    private static String drawableToBase64(Drawable drawable) {
        try {
            Bitmap bitmap;
            if (drawable.getIntrinsicWidth() <= 0 || drawable.getIntrinsicHeight() <= 0) {
                bitmap = Bitmap.createBitmap(100, 100, Bitmap.Config.ARGB_8888);
            } else {
                bitmap = Bitmap.createBitmap(
                        drawable.getIntrinsicWidth(),
                        drawable.getIntrinsicHeight(),
                        Bitmap.Config.ARGB_8888
                );
            }

            Canvas canvas = new Canvas(bitmap);
            drawable.setBounds(0, 0, canvas.getWidth(), canvas.getHeight());
            drawable.draw(canvas);

            ByteArrayOutputStream byteArrayOutputStream = new ByteArrayOutputStream();
            bitmap.compress(Bitmap.CompressFormat.PNG, 100, byteArrayOutputStream);
            byte[] byteArray = byteArrayOutputStream.toByteArray();

            return Base64.encodeToString(byteArray, Base64.DEFAULT);
        } catch (Exception e) {
            Log.e(TAG, "Error converting Drawable to Base64", e);
            return null;
        }
    }

    public static boolean isPermissionGranted(Context context, String permissionName){
        try{
            return XXPermissions.isGranted(context, permissionName);
        }
        catch (Exception ex){
            Log.e(TAG, "isPermissionGranted error", ex);
            return false;
        }
    }


}
