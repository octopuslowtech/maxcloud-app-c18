-keep class com.maxcloud.app.Core.PhoneDto {*;}

-keep class com.maxcloud.app.Core.MainService {*;}

-keep class com.maxcloud.app.Core.VPNService {*;}

-keep class com.maxcloud.app.Core.DialogService {*;}


-keep class com.maxcloud.app.Core.NativeCore {*;}

-keep class com.maxcloud.app.App {*;}

-keepclassmembers class com.maxcloud.app.Core.Airplane.MyInteractionService {
    public static boolean IsRunning;
}

-keep class com.maxcloud.app.Extensions.** {*;}

-keepclassmembers class com.maxcloud.keyboard.latin.LatinIME {
    public static boolean IsRunning;
}

# 3rd party libraries

-keep class com.hjq.permissions.** {*;}
-keep class com.hjq.toast.** {*;}
-keep class com.microsoft.** {*;}

-keep class com.marsad.stylishdialogs.RotatingAnimation {
	    public <init>(...);
}

-keep class com.google.** {*;}

-keep class com.jaredrummler.android.device.DeviceName {*;}

-dontwarn org.slf4j.impl.StaticLoggerBinder

-keep class org.webrtc.** {*;}