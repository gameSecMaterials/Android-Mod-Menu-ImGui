package com.unity3d.player;

import android.app.Activity;
import android.content.Context;
import android.graphics.Canvas;
import android.graphics.Color;
import android.util.Log;
import android.view.InputEvent;
import android.view.MotionEvent;
import android.view.View;
import android.widget.FrameLayout;
import android.widget.TextView;



public class UnityPlayer extends FrameLayout {

    // Used by Unity Player Activity

    /* JADX INFO: additional move instructions added (1) to help type inference */
    /* JADX WARN: Type inference failed for: r7v0, types: [com.unity3d.player.IUnityPlayerLifecycleEvents] */
    /* JADX DEBUG: Failed to find minimal casts for resolve overloaded methods, cast all args instead
     method: ClspMth{android.app.AlertDialog.Builder.setPositiveButton(java.lang.CharSequence, android.content.DialogInterface$OnClickListener):android.app.AlertDialog$Builder}
     arg types: [java.lang.String, com.unity3d.player.UnityPlayer$1]
     candidates:
	 ClspMth{android.app.AlertDialog.Builder.setPositiveButton(int, android.content.DialogInterface$OnClickListener):android.app.AlertDialog$Builder}
	 ClspMth{android.app.AlertDialog.Builder.setPositiveButton(java.lang.CharSequence, android.content.DialogInterface$OnClickListener):android.app.AlertDialog$Builder} */
    /* JADX WARNING: Unknown variable types count: 1 */

    public  boolean injectEvent(InputEvent inputEvent) {
        return true;
    }
    //private final native boolean nativeRender();
    public static Activity currentActivity;
    public UnityPlayer(Context context) {
        super(context);

        setBackgroundColor(Color.TRANSPARENT);
        //	UnityIntegrationModMenuService.CreateMenu(this, context);
    }
    public View getView(){
        return this;
    }
}
