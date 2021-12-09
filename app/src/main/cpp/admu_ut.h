//
// Created by ozMod on 27.11.2021.
//

struct {

    void Make(JNIEnv *env, jobject thiz, const char *text, int length) {

        jstring jstr = env->NewStringUTF(text);
        jclass toast = env->FindClass(ozObfuscate("android/widget/Toast"));
        jmethodID methodMakeText =
                env->GetStaticMethodID(
                        toast,
                        ozObfuscate("makeText"),

                        ozObfuscate("(Landroid/content/Context;Ljava/lang/CharSequence;I)Landroid/widget/Toast;"));
        if (methodMakeText == NULL) {

            return;
        }
        //The last int is the length on how long the toast should be displayed
        //0 = Short, 1 = Long
        jobject toastobj = env->CallStaticObjectMethod(toast, methodMakeText,
                                                       thiz, jstr, length);

        jmethodID methodShow = env->GetMethodID(toast, ozObfuscate("show"), ozObfuscate("()V"));
        if (methodShow == NULL) {

            return;
        }
        env->CallVoidMethod(toastobj, methodShow);
    }
}Toast;

