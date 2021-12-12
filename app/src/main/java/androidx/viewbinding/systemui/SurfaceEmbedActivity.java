package androidx.viewbinding.systemui;
import com.unity3d.player.UnityPlayer;
import android.app.Activity;
import android.os.Bundle;
public class SurfaceEmbedActivity extends Activity {
    // Used to load the 'imgui' library on application startup.
    public void onCreate(Bundle bundle) {
        super.onCreate(bundle);

        requestWindowFeature(1);
        getWindow().setFlags(1024, 1204);
        UnityPlayer unityPlayer = new UnityPlayer(this);
        setContentView(unityPlayer);
        SystemOverlaySurface.Start(this, unityPlayer);
    }

    // TODO: This can be moved to native layer, to avoid dependency on java

}