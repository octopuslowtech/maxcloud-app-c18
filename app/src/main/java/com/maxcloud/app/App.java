package com.maxcloud.app;

import android.app.Application;
import android.app.NotificationChannel;
import android.app.NotificationManager;
import android.content.Context;
import android.net.ConnectivityManager;
import android.util.Log;
import android.view.Gravity;

import androidx.annotation.NonNull;

import com.elvishew.xlog.LogConfiguration;
import com.elvishew.xlog.LogLevel;
import com.elvishew.xlog.XLog;
import com.elvishew.xlog.flattener.PatternFlattener;
import com.elvishew.xlog.printer.AndroidPrinter;
import com.elvishew.xlog.printer.Printer;
import com.elvishew.xlog.printer.file.FilePrinter;
import com.elvishew.xlog.printer.file.backup.NeverBackupStrategy;
import com.elvishew.xlog.printer.file.naming.DateFileNameGenerator;
import com.hjq.toast.Toaster;
import com.hjq.toast.style.WhiteToastStyle;
import com.jaredrummler.android.device.DeviceName;

import xcrash.XCrash;

public class App extends Application {

    public static final String PACKAGE = "com.maxcloud.app";

    public static boolean isModuleActive() {
        return false;
    }

    private static Application INS;

    @Override
    public void onTrimMemory(int level) {
        super.onTrimMemory(level);
    }

    public void onCreate() {
        super.onCreate();
        INS = this;

        String nativePath = getContext().getApplicationInfo().nativeLibraryDir;
        Log.d("onCreate", "onCreate: " + nativePath);

        XCrash.InitParameters xcarsh = new XCrash.InitParameters()
                .disableJavaCrashHandler()
                .setNativeDumpAllThreads(false)
                .setNativeDumpMap(false)  // Không ghi thông tin bộ nhớ map
                .setNativeDumpFds(false)  // Không ghi thông tin file descriptors
                .setNativeDumpNetwork(false)  // Không ghi thông tin mạng
                .setNativeDumpElfHash(false) // Không ghi ELF hash
                .setLogDir("/sdcard/" + PACKAGE + "/native");

        xcrash.XCrash.init(this, xcarsh);

        initJavaLog();

        Toaster.init(this);
        Toaster.setStyle(new WhiteToastStyle());
        Toaster.setGravity(Gravity.BOTTOM, 0, 250);
        DeviceName.init(this);


        NotificationChannel channel = new NotificationChannel(
                getPackageName(),
                getString(R.string.app_name),
                NotificationManager.IMPORTANCE_DEFAULT
        );

        getSystemService(NotificationManager.class).createNotificationChannel(channel);
    }

    private static void initJavaLog() {
        Thread.setDefaultUncaughtExceptionHandler(new Thread.UncaughtExceptionHandler() {
            @Override
            public void uncaughtException(@NonNull Thread thread, @NonNull Throwable throwable) {
                XLog.e("TypeError:", throwable);
            }
        });

        LogConfiguration config = new LogConfiguration.Builder()
                .logLevel(LogLevel.ALL)
                .tag("Octopus")
                .enableBorder()
                .build();

        String outputPath = "/sdcard/" + PACKAGE + "/java/";

        Printer androidPrinter = new AndroidPrinter(true);
        PatternFlattener patternFlattener = new PatternFlattener("{d yyyy/MM/dd HH:mm:ss} {l}/{t}: {m}");
        Printer filePrinter = new FilePrinter
                .Builder(outputPath)
                .fileNameGenerator(new DateFileNameGenerator() {
                    @Override
                    public String generateFileName(int logLevel, long timestamp) {
                        return super.generateFileName(logLevel, timestamp) + "-log.txt";
                    }
                })
                .backupStrategy(new NeverBackupStrategy())
                .flattener(patternFlattener)
                .build();

        XLog.init(config, androidPrinter, filePrinter);
    }



    public static Context getContext() {
        return INS;
    }

    private static ConnectivityManager connectivity;

    public static ConnectivityManager getConnectivity() {
        if (connectivity == null) {
            connectivity = (ConnectivityManager) getContext().getSystemService(Context.CONNECTIVITY_SERVICE);
        }
        return connectivity;
    }
    // getInstance

    public static Application getInstance() {
        return INS;
    }


}
