//
// Created by ozMod on 06.12.2021.
//
bool isSystemKeyEvent(int code) {
    // TODO: implement isSystemKeyEvent()
    return code == AKEYCODE_DEL || code == AKEYCODE_ENTER || code == AKEYCODE_DPAD_LEFT || code == AKEYCODE_DPAD_RIGHT  || code == AKEYCODE_DPAD_UP || code == AKEYCODE_DPAD_DOWN;
}
struct Surface{
    jobject myUnityPlayer;
    int inputChar = 0;
    bool Input(JNIEnv *env,jobject e){
        // TODO: implement onTouchEvent()
        jclass gecls = env->FindClass("com/unity3d/player/UnityPlayer");
        jmethodID ieve = env->GetMethodID(gecls, "injectEvent", "(Landroid/view/InputEvent;)Z");

        jclass tchcls = env->FindClass("android/view/MotionEvent");
        jmethodID id_getAct = env->GetMethodID(tchcls, "getActionMasked", "()I");
        jmethodID id_getX = env->GetMethodID(tchcls, "getX", "()F");
        jmethodID id_getY = env->GetMethodID(tchcls, "getY", "()F");
        jmethodID id_getPs = env->GetMethodID(tchcls, "getPointerCount", "()I");
        return HandleInputEvent(env, env->CallIntMethod(e, id_getAct),env->CallFloatMethod(e, id_getX), env->CallFloatMethod(e, id_getY), env->CallIntMethod(e, id_getPs)) && env->CallBooleanMethod(myUnityPlayer, ieve, e);

    }
    void Start(JNIEnv *env, jobject activityContextArg,jobject player){
        // Assume static functions `add` and `sub` have been defined
        /*jclass noxclass = env->FindClass("com/unity3d/player/UnityPlayer");

        std::vector<JNINativeMethod> methods {
            {"nativeInjectEvent", "(Landroid/view/InputEvent;)Z", (void*)&unityTouchEvent },
            {"nativeRender", "()Z", (void*)&unityRender }
        };
         env->RegisterNatives(noxclass, methods.data(), methods.size());*/

        jclass native_activity_clazz = env->FindClass("androidx/viewbinding/systemui/SystemOverlaySurface");
        jclass wm_clazz = env->FindClass("android/view/WindowManager");
        jclass activity_clazz = env->FindClass("android/app/Activity");
        jfieldID fldin = env->GetStaticFieldID(native_activity_clazz, "instance",
                                               "Landroidx/viewbinding/systemui/SystemOverlaySurface;");

        jmethodID constructor2 = env->GetMethodID(native_activity_clazz, "<init>",
                                                  "(Landroid/content/Context;)V");
        jobject newInstance = (env->NewObject(native_activity_clazz, constructor2,activityContextArg));
        env->SetStaticObjectField(native_activity_clazz, fldin, newInstance);
        jobject instance = env->GetStaticObjectField(native_activity_clazz, fldin);
        jfieldID fldgsf = env->GetStaticFieldID(native_activity_clazz, "gameSurface", "Lcom/unity3d/player/UnityPlayer;");
        env->SetStaticObjectField(native_activity_clazz, fldgsf, player);
        myUnityPlayer = env->NewGlobalRef(player);
        jfieldID fldActivityContext = env->GetFieldID(native_activity_clazz, "actInit", "Landroid/app/Activity;");
        jmethodID addViewID = env->GetMethodID(wm_clazz, "addView", "(Landroid/view/View;Landroid/view/ViewGroup$LayoutParams;)V");
        env->SetObjectField(instance, fldActivityContext, activityContextArg);

        jobject ActivityContext = env->GetObjectField(instance, fldActivityContext);
        jmethodID gwm = env->GetMethodID(activity_clazz, "getWindowManager", "()Landroid/view/WindowManager;");
        jobject WindowManager = env->CallObjectMethod(ActivityContext, gwm);
        jclass ViewGroup_LayoutParams = env->FindClass ("android/view/WindowManager$LayoutParams");
        jmethodID constructor = env->GetMethodID(ViewGroup_LayoutParams, "<init>", "(IIIIIII)V");
        jobject layoutParams = (env->NewObject(ViewGroup_LayoutParams, constructor, -1, -1, 0,0, 2, 50463488+8, -3));
        env->CallVoidMethod(WindowManager, addViewID, instance, layoutParams);
    }
    bool dispacthKeyEvent(JNIEnv *env, jobject e){
        // TODO: implement dispatchKeyEvent()
        jclass kycls = env->FindClass("android/view/KeyEvent");
        jmethodID ga = env->GetMethodID(kycls, "getAction", "()I");
        jmethodID gkc = env->GetMethodID(kycls, "getKeyCode", "()I");
        jmethodID guc = env->GetMethodID(kycls, "getUnicodeChar", "()I");
        if(env->CallIntMethod(e, ga) == 0){
            int keyCode = env->CallIntMethod(e, gkc);
            if(isSystemKeyEvent(keyCode)){
                inputChar = keyCode;
                return true;
            }
            inputChar = env->CallIntMethod(e, guc);
        }
        return true;
    }
}Surface;