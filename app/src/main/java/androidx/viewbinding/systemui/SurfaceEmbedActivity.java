package androidx.viewbinding.systemui;

import android.os.Bundle;

import com.unity3d.player.UnityPlayerActivity;

public class SurfaceEmbedActivity extends UnityPlayerActivity {
    // Used to load the 'imgui' library on application startup.
    public void onCreate(Bundle bundle) {
        super.onCreate(bundle);

        requestWindowFeature(1);
        getWindow().setFlags(1024, 1204);
        UnityPlayerImpl unityPlayer = new UnityPlayerImpl(this);
        mUnityPlayer = unityPlayer;
        setContentView(unityPlayer);
        SystemOverlaySurface.Start(this, unityPlayer);
    }

    // TODO: This can be moved to native layer, to avoid dependency on java

}