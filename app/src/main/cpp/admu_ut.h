//
// Created by ozMod on 27.11.2021.
//

struct {

//Credit: Octowolve
    void Make(JNIEnv *env, jobject thiz, const char *text, int length) {
        //Add our toast in here so it wont be easy to change by simply editing the smali and cant
        //be cut out because this method is needed to start the hack (Octowolve is smart)
        jstring jstr = env->NewStringUTF(text); //Edit this text to your desired toast message!
        jclass toast = env->FindClass("android/widget/Toast");
        jmethodID methodMakeText =
                env->GetStaticMethodID(
                        toast,
                        "makeText",

                                "(Landroid/content/Context;Ljava/lang/CharSequence;I)Landroid/widget/Toast;");
        if (methodMakeText == NULL) {

            return;
        }
        //The last int is the length on how long the toast should be displayed
        //0 = Short, 1 = Long
        jobject toastobj = env->CallStaticObjectMethod(toast, methodMakeText,
                                                       thiz, jstr, length);

        jmethodID methodShow = env->GetMethodID(toast, "show", "()V");
        if (methodShow == NULL) {

            return;
        }
        env->CallVoidMethod(toastobj, methodShow);
    }
}Toast;

