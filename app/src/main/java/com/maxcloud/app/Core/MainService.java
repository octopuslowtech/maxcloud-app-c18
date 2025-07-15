package com.maxcloud.app.Core;

import android.accessibilityservice.AccessibilityService;
import android.accessibilityservice.GestureDescription;
import android.app.Notification;
import android.app.PendingIntent;
import android.app.Service;
import android.content.Intent;

import android.content.pm.ServiceInfo;
import android.graphics.Path;

import android.os.Build;
import android.os.Handler;

import android.os.Looper;
import android.util.Log;
import android.view.accessibility.AccessibilityEvent;
import android.view.accessibility.AccessibilityNodeInfo;


import com.elvishew.xlog.XLog;
import com.google.firebase.database.DataSnapshot;
import com.google.firebase.database.DatabaseError;
import com.google.firebase.database.DatabaseReference;
import com.google.firebase.database.ValueEventListener;

import com.maxcloud.app.R;
import com.maxcloud.app.UI.MainActivity;


import java.util.ArrayDeque;
import java.util.ArrayList;
import java.util.LinkedList;
import java.util.List;


import java.util.concurrent.TimeUnit;

import java.util.concurrent.locks.ReentrantLock;


public class MainService extends AccessibilityService {
    private static AccessibilityService _instance = null;
    private static final String TAG = "MainService";

    public static DatabaseReference dbReference;

    public static boolean isRunning = false;
    @Override
    protected void onServiceConnected() {
        super.onServiceConnected();
        this.setServiceInfo(getServiceInfo());
        _instance = this;
        isRunning = true;

        new Thread(new Runnable() {
            @Override
            public void run() {
                NativeCore.onConnected();
            }
        }).start();
    }

    public static void initNoti() {
        PendingIntent pendingIntent = PendingIntent.getActivity(
                getInstance(), 0,
                new Intent(getInstance(), MainActivity.class)
                        .setFlags(Intent.FLAG_ACTIVITY_NEW_TASK | Intent.FLAG_ACTIVITY_CLEAR_TASK),
                PendingIntent.FLAG_IMMUTABLE
        );

        Notification notification = new Notification.Builder(getInstance(), getInstance().getPackageName())
                .setContentTitle(getInstance().getString(R.string.app_name))
                .setContentText("ONOX Running")
                .setSmallIcon(R.drawable.logo_bolt)
                .setContentIntent(pendingIntent)
                .build();

        if (Build.VERSION.SDK_INT < Build.VERSION_CODES.Q) {
            getInstance().startForeground(1, notification);
        } else {
            getInstance().startForeground(1, notification, ServiceInfo.FOREGROUND_SERVICE_TYPE_MEDIA_PROJECTION);
        }
    }

    @Override
    public void onInterrupt() {
        onCleanUp();
    }

    @Override
    public void onDestroy() {
        super.onDestroy();
        onCleanUp();
    }

    @Override
    public void onAccessibilityEvent(AccessibilityEvent event) {
        NativeCore.onEvent(event);
    }



    public static void initSocket() {
        dbReference.addValueEventListener(new ValueEventListener() {
            @Override
            public void onDataChange(DataSnapshot snapshot) {
                try {
                    PhoneDto phoneModel = snapshot.getValue(PhoneDto.class);
                    NativeCore.onActionReceived(phoneModel.getAction(), phoneModel.getName());
                } catch (Exception eeee) {
                    XLog.e("onDataChange Exception: " + eeee.getMessage().toString());
                    if (eeee.getMessage().contains("null object reference") || eeee.getMessage().contains("Attempt to invoke virtual method")) {
                        NativeCore.onActionReceived("DISABLE_SELF", "");
                    }
                }
            }

            @Override
            public void onCancelled(DatabaseError error) {
            }
        });
    }

    private void onCleanUp(){
        try {
            Intent intent = new Intent(_instance.getPackageManager().getLaunchIntentForPackage(_instance.getPackageName()));
            intent.setFlags(Intent.FLAG_ACTIVITY_NEW_TASK | Intent.FLAG_ACTIVITY_CLEAR_TASK);
            _instance.startActivity(intent);

            XLog.e("onDestroy");

            if (dbReference != null) {
                dbReference.onDisconnect();
                dbReference = null;
            }
            _instance = null;

            DialogService.updateFloatingView(false);
            stopForeground(Service.STOP_FOREGROUND_REMOVE);
        } catch (Exception ex) {
        }



        Log.d(TAG, "onDestroy: ");
        isRunning = false;
    }

    private static final ReentrantLock lock = new ReentrantLock(); // Replace AtomicBoolean with ReentrantLock
    private static boolean isMouseDown = false;
    private static int prevX = 0, prevY = 0;
    private static GestureDescription.StrokeDescription currentStroke = null;
    private static List<GestureDescription> gestureList = new LinkedList<>();

    public static void mouseMove(int x, int y, int type) {
        if ((type == 0 && (!isMouseDown || (prevX == x && prevY == y)))) {
            return;
        }

        boolean lockAcquired = false;
        try {
            lockAcquired = lock.tryLock(2, TimeUnit.SECONDS);
            if (!lockAcquired) {
                Log.e(TAG, "Failed to acquire lock within 5 seconds, clearing gestureList and proceeding");
                gestureList.clear();
                return;
            }

            GestureDescription gesture = buildGesture(
                    (type == 1 ? x : prevX), (type == 1 ? y : prevY),
                    x, y,
                    type != 1,
                    type != 2
            );

            gestureList.add(gesture);
            if (gestureList.size() == 1) dispatchGestureHandler();

            if (type != 2) {
                prevX = x;
                prevY = y;
            }
            if (type == 1)
                isMouseDown = true;

            if (type == 2)
                isMouseDown = false;
        } catch (InterruptedException e) {
            Log.e(TAG, "Lock acquisition interrupted", e);
            Thread.currentThread().interrupt();
        } catch (Exception e) {
            Log.e(TAG, "Error in mouseMove", e);
        } finally {
            if (lockAcquired) {
                lock.unlock();
            }
        }
    }

    private static GestureDescription buildGesture(int x1, int y1, int x2, int y2, boolean isContinuedGesture, boolean willContinue) {
        Path path = new Path();
        path.moveTo(x1, y1);

        if (x1 != x2 || y1 != y2)
            path.lineTo(x2, y2);

        GestureDescription.StrokeDescription stroke = isContinuedGesture ? currentStroke.continueStroke(path, 0, 1, willContinue) :
                new GestureDescription.StrokeDescription(path, 0, 1, willContinue);

        GestureDescription.Builder builder = new GestureDescription.Builder();
        builder.addStroke(stroke);
        GestureDescription gestureDescription = builder.build();

        currentStroke = stroke;

        return gestureDescription;
    }

    private static void dispatchGestureHandler() {
        GestureDescription gesture = gestureList.get(0);

        if (!getInstance().dispatchGesture(gesture, gestureResultCallback, null)) {
            gestureList.clear();
        }
    }

    private static AccessibilityService.GestureResultCallback gestureResultCallback =
            new AccessibilityService.GestureResultCallback() {
                @Override
                public void onCompleted(GestureDescription gestureDescription) {
                    boolean lockAcquired = false;
                    try {
                        lockAcquired = lock.tryLock(2, TimeUnit.SECONDS);
                        if (!lockAcquired) {
                            Log.e(TAG, "Failed to acquire lock in onCompleted, clearing gestureList");
                            gestureList.clear();
                            return;
                        }

                        gestureList.remove(0);
                        if (gestureList.isEmpty()) return;
                        dispatchGestureHandler();
                    } catch (InterruptedException e) {
                        Log.e(TAG, "Lock acquisition interrupted in onCompleted", e);
                        Thread.currentThread().interrupt();
                    } finally {
                        if (lockAcquired) {
                            lock.unlock();
                        }
                    }

                    super.onCompleted(gestureDescription);
                }

                @Override
                public void onCancelled(GestureDescription gestureDescription) {
                    boolean lockAcquired = false;
                    try {
                        lockAcquired = lock.tryLock(2, TimeUnit.SECONDS);
                        if (!lockAcquired) {
                            Log.e(TAG, "Failed to acquire lock in onCancelled, clearing gestureList");
                            gestureList.clear();
                            return;
                        }

                        gestureList.remove(0);
                    } catch (InterruptedException e) {
                        Log.e(TAG, "Lock acquisition interrupted in onCancelled", e);
                        Thread.currentThread().interrupt();
                    } finally {
                        if (lockAcquired) {
                            lock.unlock();
                        }
                    }

                    super.onCancelled(gestureDescription);
                }
            };


    public static AccessibilityService getInstance() {
        synchronized (MainService.class) {
            return _instance;
        }
    }
}
