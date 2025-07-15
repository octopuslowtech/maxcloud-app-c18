package com.maxcloud.app.Extensions;

import android.media.projection.MediaProjection;

public class MediaProjectionCallback extends MediaProjection.Callback{
    @Override
    public void onCapturedContentVisibilityChanged(boolean isVisible) {

        super.onCapturedContentVisibilityChanged(isVisible);
    }

    @Override
    public void onCapturedContentResize(int width, int height) {
        super.onCapturedContentResize(width, height);
    }

    @Override
    public void onStop() {

    }
}
