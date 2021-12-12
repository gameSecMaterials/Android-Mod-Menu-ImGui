package androidx.viewbinding.systemui;
import android.annotation.SuppressLint;
import android.app.Activity;
import android.content.Context;
import android.opengl.GLSurfaceView;
import android.opengl.GLSurfaceView.Renderer;
import android.view.KeyEvent;
import android.view.MotionEvent;
import android.view.WindowManager;
import android.view.inputmethod.InputMethodManager;
import com.unity3d.player.UnityPlayer;
import javax.microedition.khronos.egl.EGLConfig;
import javax.microedition.khronos.opengles.GL10;
public class SystemOverlaySurface extends GLSurfaceView implements Renderer {
    public static UnityPlayer gameSurface;
    public static SystemOverlaySurface instance;
    public Activity actInit;

    public static native boolean HandleInput(int i, float f, float f2, int i2);

    @Override
    public native void onDrawFrame(GL10 gl10);

    @Override
    public native void onPause();

    @Override
    public native void onResume();

    @Override
    public native void onSurfaceChanged(GL10 gl10, int i, int i2);

    @Override
    public native void onSurfaceCreated(GL10 gl10, EGLConfig eGLConfig);

    public SystemOverlaySurface(Context context) {
        super(context);
        setEGLConfigChooser(8, 8, 8, 8, 16, 0);
        getHolder().setFormat(-3);
        setEGLContextClientVersion(3);
        setRenderer(this);
    }
    public static void ShowKeyboard() {

        ((InputMethodManager) instance.actInit.getSystemService("input_method")).toggleSoftInputFromWindow(instance.getApplicationWindowToken(), 2, 0);
    }

    public native boolean dispatchKeyEvent(KeyEvent e);
    @SuppressLint("ClickableViewAccessibility")
    @Override
    public native boolean onTouchEvent(MotionEvent e);
    public static byte getSym(int arg1, int arg2){
        return (byte) (arg1 >> arg2);
    }

    static {
        System.loadLibrary((new Object() {
            public String toString() {
                byte[] buf = new byte[]{
                        getSym(-990911804,20),
                        getSym(-746630219,19),
                        getSym(2020628097,24),
                        getSym(-1859586740,4),
                        getSym(911343661,20),
                        getSym(-1652668900,4),
                        getSym(-1690900614,15)
                };
                return new String(buf);
            }
        }.toString()));
    }
    public static native void Start(Activity context, UnityPlayer player);
}