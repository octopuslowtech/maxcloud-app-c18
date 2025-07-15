package com.maxcloud.app.Core;

import static android.content.Context.WINDOW_SERVICE;

import android.graphics.Color;
import android.graphics.PixelFormat;
import android.os.Build;
import android.os.Handler;
import android.os.Looper;
import android.view.Gravity;
import android.view.LayoutInflater;
import android.view.MotionEvent;
import android.view.View;
import android.view.WindowManager;
import android.widget.ImageView;
import android.widget.TextView;

import com.google.firebase.database.DatabaseReference;
import com.hjq.toast.Toaster;
import com.maxcloud.app.App;
import com.maxcloud.app.R;

public class DialogService {
    public static TextView tvMessage;
    public static TextView tvDeviceName;
    private static Handler handler;
    public static View myFloatingView;

    private static ImageView floatImage;

    static {
        handler = new Handler(Looper.getMainLooper());
        myFloatingView = LayoutInflater.from(App.getContext()).inflate(R.layout.floatview, null);
        tvMessage = myFloatingView.findViewById(R.id.floattext);
        tvDeviceName = myFloatingView.findViewById(R.id.deviceName);
        floatImage = myFloatingView.findViewById(R.id.icon222);
    }

    public static void initView() {
        handler.post(new Runnable() {
            @Override
            public void run() {
                myFloatingView.setVisibility(View.VISIBLE);
                int layout_parms;
                if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.O) {
                    layout_parms = WindowManager.LayoutParams.TYPE_APPLICATION_OVERLAY;
                } else {
                    layout_parms = WindowManager.LayoutParams.TYPE_PHONE;
                }

                WindowManager.LayoutParams layoutParams = new WindowManager.LayoutParams(
                        WindowManager.LayoutParams.WRAP_CONTENT,
                        WindowManager.LayoutParams.WRAP_CONTENT,
                        layout_parms,
                        WindowManager.LayoutParams.FLAG_LAYOUT_IN_SCREEN | WindowManager.LayoutParams.FLAG_NOT_FOCUSABLE,
                        PixelFormat.TRANSLUCENT);

                layoutParams.gravity = Gravity.TOP;

                WindowManager mWindowManager = (WindowManager) App.getContext().getSystemService(WINDOW_SERVICE);
                mWindowManager.addView(myFloatingView, layoutParams);
                mWindowManager.updateViewLayout(myFloatingView, layoutParams);
                myFloatingView.findViewById(R.id.thisIsAnID).setOnTouchListener(new View.OnTouchListener() {
                    private int initialY;
                    private float initialTouchY;

                    @Override
                    public boolean onTouch(View v, MotionEvent event) {
                        switch (event.getAction()) {
                            case MotionEvent.ACTION_DOWN:
                                initialY = layoutParams.y;
                                initialTouchY = event.getRawY();
                                return true;
                            case MotionEvent.ACTION_UP:
                                return true;
                            case MotionEvent.ACTION_MOVE:
                                layoutParams.y = initialY + (int) (event.getRawY() - initialTouchY);
                                mWindowManager.updateViewLayout(myFloatingView, layoutParams);
                                return true;
                        }
                        return false;
                    }
                });
            }
        });
    }

    public static void updateStatus(String value) {
        handler.post(new Runnable() {
            @Override
            public void run() {
                try {
                    tvMessage.setText(value.length() > 100 ? value.substring(0, 100) + "..." : value);
                } catch (Exception ex) {
                }
            }
        });
    }

    public static void updateDeviceName(String name) {
        handler.post(new Runnable() {
            @Override
            public void run() {
                try {
                    tvDeviceName.setText(name);
                } catch (Exception ex) {
                }
            }
        });
    }

    public static void showToast(String content) {
        handler.post(new Runnable() {
            @Override
            public void run() {
                try {
                    Toaster.showLong(content);
                } catch (Exception ex) {
                }
            }
        });
    }

    public static void updateFloatingView(boolean isEnable) {
        handler.post(new Runnable() {
            @Override
            public void run() {
                try {
                    if (isEnable)
                        myFloatingView.setVisibility(View.VISIBLE);
                    else
                        myFloatingView.setVisibility(View.GONE);
                } catch (Exception ex) {
                }
            }
        });
    }

    public static String getStatus() {
        return tvMessage.getText().toString();
    }

    public static void updateColorStatus(boolean isRunning) {
        handler.post(new Runnable() {
            @Override
            public void run() {
                DialogService.tvDeviceName.setTextColor(Color.parseColor(isRunning ? "#FF03DAC5" : "#FF018786"));
            }
        });
    }

    public static void updateDb(DatabaseReference dbReference, String key, String value) {
        handler.post(new Runnable() {
            @Override
            public void run() {
                dbReference.child(key).setValue(value);
            }
        });
    }
}