package com.maxcloud.app.Core.Airplane;

import android.content.Intent;
import android.os.Bundle;
import android.service.voice.VoiceInteractionSession;
import android.service.voice.VoiceInteractionSessionService;


public class MyInteractionSessionService extends VoiceInteractionSessionService {
    @Override
    public VoiceInteractionSession onNewSession(Bundle bundle) {
        return new VoiceInteractionSession(this) {
            @Override
            public void onShow(Bundle args, int showFlags) {
                super.onShow(args, showFlags);

                if (args == null) return;

                String action = args.getString("action");
                if (action == null || action.isEmpty()) return;

                Intent intent = new Intent("android.settings.VOICE_CONTROL_AIRPLANE_MODE");
                intent.putExtra("airplane_mode_enabled", MyInteractionService.TURN_ON.equals(action));
                startVoiceActivity(intent);
            }
        };
    }
}
