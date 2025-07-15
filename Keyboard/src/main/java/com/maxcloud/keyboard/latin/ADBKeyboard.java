package com.maxcloud.keyboard.latin;

import android.inputmethodservice.InputMethodService;
import android.view.View;

import com.maxcloud.keyboard.R;

public class ADBKeyboard extends InputMethodService {
    @Override
    public View onCreateInputView() {
        View mInputView = getLayoutInflater().inflate(R.layout.adb_layout, null);
        return mInputView;
    }

    @Override
    public void onDestroy() {
        super.onDestroy();
    }

}
