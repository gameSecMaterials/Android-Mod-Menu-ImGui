#include "obfuscator.hpp"
#include <GLES2/gl2.h>
#include "EGL/egl.h"

#include <android/asset_manager_jni.h>
JavaVM *publicJVM;
#include "sys/time.h"
void Render();
static void UpdateInput();
int inputChar = 0;
long long currentTimeInMilliseconds(){
    struct timeval tv;
    gettimeofday(&tv, nullptr);
    return ((tv.tv_sec * 1000) + (tv.tv_usec / 1000));
}

bool setupGraphics(int width, int height);

int startTime, screenWidth, screenHeight;
int currentLang = 1;

jobject activity;
class ImDrawData;

void ImGui_ImplAndroidGLES2_RenderDrawLists(ImDrawData*data);

#include "GLES3/gl3.h"

#include <string>
#include <android/input.h>
#include <android/log.h>
#define LOGI(...) ((void)__android_log_print(ANDROID_LOG_INFO,"ozMod", __VA_ARGS__))
#define  LOGE(...)  __android_log_print(ANDROID_LOG_ERROR,"ozMod",__VA_ARGS__)
#define  LOGD(...)  __android_log_print(ANDROID_LOG_DEBUG,"ozMod",__VA_ARGS__)
#define  LOGDE(...)  __android_log_print(ANDROID_LOG_DEFAULT,"ozMod",__VA_ARGS__)
#define  LOGW(...)  __android_log_print(ANDROID_LOG_WARN,"ozMod",__VA_ARGS__)
#define  LOGV(...)  __android_log_print(ANDROID_LOG_VERBOSE,"ozMod",__VA_ARGS__)
#define  LOGF(...)  __android_log_print(ANDROID_LOG_FATAL,"ozMod",__VA_ARGS__)
#define  LOGU(...)  __android_log_print(ANDROID_LOG_UNKNOWN,"ozMod",__VA_ARGS__)
#define  LOGS(...)  __android_log_print(ANDROID_LOG_SLIENT,"ozMod",__VA_ARGS__)
static void checkGlError(const char* op) {
    for (GLint error = glGetError(); error; error
                                                    = glGetError()) {
        LOGI("after %s() glError (0x%x)\n", op, error);
    }
}
bool HandleInputEvent(JNIEnv *jenv,
int motionEvent, int x, int y,
int p);
jobject UnityPlayer_CurrentActivity_fid;
void diplayKeyboard(JNIEnv* newEnv) {
    LOGI("Display keybrd");
    JNIEnv* env = NULL;
    if(newEnv != nullptr){
        env = newEnv;
    } else {
        jint jni_return = publicJVM->GetEnv((void**)&env, JNI_VERSION_1_6);
        if (jni_return == JNI_ERR){
            return;
        }
        jni_return = publicJVM->AttachCurrentThread(&env, NULL);
        if (jni_return != JNI_OK){
            return;
        }
    }

    jclass aycls = env->FindClass("android/app/Activity");
    jmethodID gss = env->GetMethodID(aycls, "getSystemService",
                                     "(Ljava/lang/String;)Ljava/lang/Object;");
    jobject ss = env->CallObjectMethod(UnityPlayer_CurrentActivity_fid, gss, env->NewStringUTF("input_method"));
    jclass imcls = env->FindClass("android/view/inputmethod/InputMethodManager");
    jmethodID tgsifm = env->GetMethodID(imcls, "toggleSoftInput", "(II)V");
    env->CallVoidMethod(ss, tgsifm, 2,0);
}
#include <vector>
#include "Substrate/CydiaSubstrate.h"
#include "imgui_impl_android.h"
#include "Mods.h"

static jobject getGlobalContext(JNIEnv *env)
{
    jclass activityThread = env->FindClass("android/app/ActivityThread"); //Don't care about that error. It didn't find class because its hidden android core class.
    jmethodID currentActivityThread = env->GetStaticMethodID(activityThread, ozObfuscate("currentActivityThread"), ozObfuscate("()Landroid/app/ActivityThread;"));
    jobject at = env->CallStaticObjectMethod(activityThread, currentActivityThread);

    jmethodID getApplication = env->GetMethodID(activityThread, ozObfuscate("getApplication"), ozObfuscate("()Landroid/app/Application;"));
    jobject context = env->CallObjectMethod(at, getApplication);
    return context;
}

 bool m_needClearMousePos;
typedef enum {
    TOUCH_ACTION_DOWN = 0,
    TOUCH_ACTION_UP,
    TOUCH_ACTION_MOVE,
    TOUCH_ACTION_COUNT
} TOUCH_ACTION;

typedef struct {
    TOUCH_ACTION action;
    float x;
    float y;
    int pointers;
    float y_velocity;
    float x_velocity;
}TOUCH_EVENT;
 TOUCH_EVENT g_LastTouchEvent;
static void UpdateInput() {
    // Update buttons
    ImGuiIO &io = ImGui::GetIO();

    // Mouse buttons: 0=left, 1=right, 2=middle + extras. ImGui itself mostly only uses left button
    // (BeginPopupContext** are using right button). Others buttons allows us to track if the mouse
    // is being used by your application + available to user as a convenience via IsMouse** API.
    // TOUCH_ACTION_MOVE -> MousePos
    // TOUCH_ACTION_DOWN -> MouseDown[0] true, left button
    // TOUCH_ACTION_UP -> MouseDown[0] false. left button
    // TOUCH_ACTION_POINTER_DOWN -> multi finger as scroll, set MouseWheel. MouseWheelH not used
    if (TOUCH_ACTION_DOWN <= g_LastTouchEvent.action &&
        g_LastTouchEvent.action <= TOUCH_ACTION_MOVE) {

        io.MousePos.x = g_LastTouchEvent.x;
        io.MousePos.y = g_LastTouchEvent.y;
    }
    switch (g_LastTouchEvent.action) {
        case TOUCH_ACTION_MOVE:
            if (g_LastTouchEvent.pointers > 1) {
                io.MouseWheel = g_LastTouchEvent.y_velocity;
                io.MouseDown[0] = false;
            }
            else {
                io.MouseWheel = 0;
            }
            break;
        case TOUCH_ACTION_DOWN:
            io.MouseDown[0] = true;
            break;
        case TOUCH_ACTION_UP:
            io.MouseDown[0] = false;
            //g_KeyEventQueues[event_key_code].push(event_action);
            break;
        default:
            break;
    }
}

bool    HandleInputEvent(JNIEnv *jenv,
                                                                 int motionEvent, int x, int y,
                                                                 int p) {
    // Need to make sure the coordinate (x,y) here is relative to the top left of the surface
    // This is how the behavior of GLSurfaceView.onTouchEvent
    // If the coordinate is obtained in a different way. This should be adjusted accordingly
    if (motionEvent < TOUCH_ACTION_COUNT) {
        float velocity_y = (float)((float)y - g_LastTouchEvent.y) / 100.f;
        g_LastTouchEvent = {.action = (TOUCH_ACTION) motionEvent, .x = static_cast<float>(x), .y = static_cast<float>(y), .pointers = p, .y_velocity = velocity_y};
    }
    else {
        //LOGE("invalid action index: %d", motionEvent);
    }
    //LOGI("x: %d y: %d", x, y);
    return true;
}

ImVec4 imClearColor;
bool showTestWindow;
bool showAnotherWindow;

static void printGLString(const char *name, GLenum s) {
    const char *v = (const char *) glGetString(s);
    LOGI(ozObfuscate("GL %s = %s\n"), name, v);
}
GLuint gProgram;
GLuint gvPositionHandle;
GLuint vbo;
const GLfloat gTriangleVertices[] = { 0.0f, 1.0f, -1.0f, -1.0f,
                                      1.0f, -1.0f };
void ozTheme()
{
    ImGuiStyle* style = &ImGui::GetStyle();

    style->WindowBorderSize = 3;
    style->WindowTitleAlign = ImVec2(0.5, 0.5);
    style->WindowMinSize = ImVec2(900, 430);
    style->Colors[ImGuiCol_Border] = ImVec4(1.0f, 0.0f, 0.0f, 1.0f);
    style->Colors[ImGuiCol_BorderShadow] = ImVec4(1.0f, 0.1f, 0.1f, 1.0f);

    style->Colors[ImGuiCol_CheckMark] = ImVec4(1.0f, 0.0f, 0.0f, 1.00f);
   // style->FramePadding = ImVec2(8, 6);
    style->Colors[ImGuiCol_Button] = ImColor(31, 30, 31, 255);
    style->Colors[ImGuiCol_ButtonActive] = ImColor(41, 40, 41, 255);
    style->Colors[ImGuiCol_ButtonHovered] = ImColor(41, 40, 41, 255);
    style->Colors[ImGuiCol_TitleBg]                = ImVec4(0.04f, 0.04f, 0.04f, 1.00f);
    style->Colors[ImGuiCol_TitleBgActive]          = ImVec4(75.0f/255.0f, 0.0f, 130.0f/255.0f, 1.00f);
    style->Colors[ImGuiCol_TitleBgCollapsed]       = ImVec4(0.00f, 0.00f, 0.00f, 0.51f);
    style->Colors[ImGuiCol_Separator] = ImColor(70, 70, 70, 255);
    style->Colors[ImGuiCol_SeparatorActive] = ImColor(76, 76, 76, 255);
    style->Colors[ImGuiCol_SeparatorHovered] = ImColor(76, 76, 76, 255);

    style->Colors[ImGuiCol_FrameBg] = ImColor(37, 36, 0, 255);
    style->Colors[ImGuiCol_FrameBgActive] = ImColor(37, 36, 37, 255);
    style->Colors[ImGuiCol_FrameBgHovered] = ImColor(37, 36, 37, 255);
    //style->Colors[ImGuiCol_ScrollbarBg] = ImColor(37, 36, 37, 255);
    style->Colors[ImGuiCol_Header] = ImColor(0, 0, 0, 0);
    style->Colors[ImGuiCol_HeaderActive] = ImColor(0, 0, 0, 255);
    style->Colors[ImGuiCol_HeaderHovered] = ImColor(46, 46, 46, 255);
}

// Unfortunately, the native KeyEvent implementation has no getUnicodeChar() function.
// Therefore, we implement the processing of KeyEvents in MainActivity.kt and poll
// the resulting Unicode characters here via JNI and send them to Dear ImGui.
static int UpdateKeyboardInput(JNIEnv*java_env)
{
    // Send the actual characters to Dear ImGui
    ImGuiIO& io = ImGui::GetIO();

    if (inputChar != 0){
        io.AddInputCharacter(inputChar);
        inputChar = 0;
    }
    return 0;
}
#include "tabs.h"

bool setupGraphics(int w, int h) {
    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    ImGui::StyleColorsDark();
    ozTheme();
    ImGui::GetStyle().WindowRounding = 10;
    ImGui::GetStyle().Alpha = 1;
    ImGui::GetStyle().WindowMinSize = ImVec2(200,200);
    io.BackendPlatformName = ("imgui_impl_android");
    // Disable loading/saving of .ini file from disk.
    // FIXME: Consider using LoadIniSettingsFromMemory() / SaveIniSettingsToMemory() to save in appropriate location for Android.
    io.IniFilename =  nullptr;
    ImGui_ImplAndroidGLES2_Init();
    // Setup keyboard input
    io.KeyMap[ImGuiKey_Tab] = AKEYCODE_TAB;
    io.KeyMap[ImGuiKey_LeftArrow] = AKEYCODE_DPAD_LEFT;   // also covers physical keyboard arrow key
    io.KeyMap[ImGuiKey_RightArrow] = AKEYCODE_DPAD_RIGHT; // also covers physical keyboard arrow key
    io.KeyMap[ImGuiKey_UpArrow] = AKEYCODE_DPAD_UP;       // also covers physical keyboard arrow key
    io.KeyMap[ImGuiKey_DownArrow] = AKEYCODE_DPAD_DOWN;   // also covers physical keyboard arrow key
    io.KeyMap[ImGuiKey_PageUp] = AKEYCODE_PAGE_UP;
    io.KeyMap[ImGuiKey_PageDown] = AKEYCODE_PAGE_DOWN;
    io.KeyMap[ImGuiKey_Home] = AKEYCODE_MOVE_HOME;
    io.KeyMap[ImGuiKey_End] = AKEYCODE_MOVE_END;
    io.KeyMap[ImGuiKey_Insert] = AKEYCODE_INSERT;

    io.KeyMap[ImGuiKey_Backspace] = 67;
    io.KeyMap[ImGuiKey_Space] = AKEYCODE_SPACE;
    io.KeyMap[ImGuiKey_Enter] = 66;
    io.KeyMap[ImGuiKey_Escape] = AKEYCODE_ESCAPE;
    io.KeyMap[ImGuiKey_KeyPadEnter] = AKEYCODE_NUMPAD_ENTER;
    io.KeyMap[ImGuiKey_A] = AKEYCODE_A;
    io.KeyMap[ImGuiKey_C] = AKEYCODE_C;
    io.KeyMap[ImGuiKey_V] = AKEYCODE_V;
    io.KeyMap[ImGuiKey_X] = AKEYCODE_X;
    io.KeyMap[ImGuiKey_Y] = AKEYCODE_Y;
    io.KeyMap[ImGuiKey_Z] = AKEYCODE_Z;

    ImGui::GetStyle().ScaleAllSizes(3);
    ImFontConfig font_cfg;
    font_cfg.SizePixels = 22.0f;
    font_cfg.GlyphRanges = io.Fonts->GetGlyphRangesCyrillic();
    io.Fonts->AddFontDefault(&font_cfg);

    //setup GL drawing
    glGenBuffers(1, &vbo);
    checkGlError("glGenBuffer");

    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    checkGlError("glBindBuffer");

    glBufferData(GL_ARRAY_BUFFER, 6*sizeof(GLfloat), gTriangleVertices, GL_STATIC_DRAW);
    checkGlError("glBufferData");

    glVertexAttribPointer(gvPositionHandle, 2, GL_FLOAT, GL_FALSE, 0, (GLvoid*)nullptr);
    checkGlError("glVertexAttribPointer");

    glEnableVertexAttribArray(gvPositionHandle);
    checkGlError("glVertexAttribArray");

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    checkGlError("glBindBuffer");

    glViewport(0,0,w,h);

    return true;
}




void button(const char* label, int& currentTab, int newTab, ImVec2 size)
{
    if (ImGui::Button(label, size))
        currentTab = newTab;
}

void line(int newId)
{
    std::string id = ("imguipp_line_" + std::to_string(newId));
    ImGui::PushStyleColor(ImGuiCol_ChildBg, IM_COL32(0, 0, 0, 0));
    {
        ImGui::BeginChild(id.c_str(), ImVec2(ImGui::GetContentRegionAvail().x, 1), false);
        ImGui::Separator();
        ImGui::EndChild();
    }
    ImGui::PopStyleColor();
}
void center_text(const char* text, int lineId , bool separator)
{
    if (text == nullptr)
        return;

    ImGui::Spacing();
    ImGui::SameLine((ImGui::GetContentRegionAvail().x / 2) - (ImGui::CalcTextSize(text).x / 2));
    ImGui::Text("%s", text);
    ImGui::Spacing();

    if (true == separator)
        line(lineId);
}

void center_text_ex(const char* text, float width_available, int lineId,  bool separator)
{
    if (text == nullptr)
        return;

    ImGui::Spacing();
    ImGui::SameLine((width_available / 2) - (ImGui::CalcTextSize(text).x / 2));
    ImGui::Text("%s", text);
    ImGui::Spacing();

    if (true == separator)
        line(lineId);
}

namespace other
{
    float get_window_size_x()
    {
        return ImGui::GetWindowSize().x;
    }

    float get_window_size_y()
    {
        return ImGui::GetWindowSize().y;
    }

    ImVec2 get_window_size()
    {
        return ImGui::GetWindowSize();
    }

}

ImVec4 to_vec4(float r, float g, float b, float a)
{
    return ImVec4(r / 255.0, g / 255.0, b / 255.0, a / 255.0);
}
static bool show_app_console = false;
static bool show_app_log = false;
static bool WantTextInputLast = false;
bool show_paint = false;
bool showStyleEditor;
#define GAME_NAME "Granny 3"
#define GAME_VER "1.1.1"
class g_FontTexture;
float testCol, testCol2, testCol3,testCol4;

void chkBoxHandler(int i , bool a){
       if(IgnoreLayerCollision){
           IgnoreLayerCollision(0,8, a);
       }

    "Жмот";
}

void DrawContentFor(int i){

    if(Settings::Tab == 0){
        switch(i){
            case 0:
                ImGui::PushStyleColor(ImGuiCol_Text, IM_COL32(0, 255, 255, 255));
                ImGui::Text("%s", (const char *)("Hi! This Mod Menu have many categories and features."));
                ImGui::PopStyleColor();
                ImGui::PushStyleColor(ImGuiCol_Text, IM_COL32(0, 255, 0, 255));
                ImGui::Text("%s", (const char *)(" Just explore and enjoy!"));
                ImGui::PopStyleColor();
                break;
            case 1:
                ImGui::SetWindowFontScale(0.8);
                ImGui::Text("%s", (const char *)("Creator: ozMod"));
                ImGui::Text("%s", (const char *)(" Helper: FCS Leader"));
                ImGui::PushStyleColor(ImGuiCol_Text, IM_COL32(255, 0, 0, 255));
                ImGui::Text("%s", (const char *)("  Also thanks to:"));
                ImGui::PopStyleColor();

                ImGui::PushStyleColor(ImGuiCol_Text, IM_COL32(0, 255, 0, 255));
                ImGui::Text("%s", (const char *)("    Ciber Hacker & geokar2006 - for code ideas and ByNameModding"));
                ImGui::PopStyleColor();
                ImGui::PushStyleColor(ImGuiCol_Text, IM_COL32(255, 165, 0, 255));
                ImGui::Text("%s", (const char *)("    Omar Ocornut - for ImGui (GUI Realization)"));
                      ImGui::PopStyleColor();
                ImGui::PushStyleColor(ImGuiCol_Text, IM_COL32(255, 50, 65, 255));
                ImGui::Text("%s", (const char *)("    adamyaxley - for Protection and obfuscation (FCS Leader also)"));
                ImGui::PopStyleColor();
                ImGui::PushStyleColor(ImGuiCol_Text, IM_COL32(0, 0, 255, 255));
                ImGui::Text("%s", (const char *)("    Oifox or Octowolve (IDK) - for OpenGL hooking"));
                ImGui::PopStyleColor();
                ImGui::SetWindowFontScale(1);
                break;
            case 2:
                ImGui::ShowDemoWindow();
                break;
            case 3:
                ImGuiIO& io = ImGui::GetIO();
                ImGui::Separator();
                ImGui::Text(("Dear ImGui Version: %s"), ImGui::GetVersion());
                ImGui::Text(("Mod Menu FPS: %d (%f Frames)"), (int)io.Framerate, 1000 / io.Framerate);
                  ImGui::Separator();
                ImGui::Text(("Game Name: %s"), GAME_NAME);
                ImGui::Text(("Game Version: %s"), GAME_VER);
                ImGui::Separator();
                ImGui::PushStyleColor(ImGuiCol_Text, IM_COL32(255, 0, 0, 255));
                ImGui::Text(("Dear ImGui Version: %s. Modded by ozMod."), IMGUI_VERSION);
                ImGui::Text("%s", (const char *)("Using Android Implementation For ImGui In Unity Player."));
                ImGui::PopStyleColor();
                break;
        }
    }

    if(Settings::Tab == 2){
        switch(i) {
            case 0:
                ImGui::Checkbox(("Draw ESP Behind Mod Menu"), &ESP.drawBehindMenu);
                break;
            case 1:
                ImGui::Checkbox(("Enable Box"), &ESP.Boxes);
                ImGui::Checkbox(("Enable Line"), &ESP.Lines);
                ImGui::Checkbox(("Enable Text"), &ESP.Texts);
                ImGui::Separator();
                ImGui::Checkbox(("Enable Box Background"), &ESP.boxBg);
                ImGui::SliderFloat(("Boxes Sizes"), &ESP.boxSize, 1, 3);
                ImGui::Separator();
                ImGui::SliderFloat(("Line Offset Y"), &ESP.lineOffsetY, 0, screenHeight);
                ImGui::Separator();
                ImGui::SliderFloat(("Text Size"), &ESP.textSize, 20, 50);
                break;
            case 2:
                const char* items[] = { ("Granny"), ("Grandpa"), ("Slendrina"), ("Monster") };

                if(ImGui::Combo("Select Enemy", &selectedEnemyESP, items, IM_ARRAYSIZE(items))){
                    selectedEnemyESPObj = ESP.enemies[selectedEnemyESP];
                }
                if(selectedEnemyESPObj != nullptr) {
                    if (ImGui::Button("Disable/Enable")) {
                        selectedEnemyESPObj->ESP = !selectedEnemyESPObj->ESP;
                    }
                    ImGui::ColorEdit4("Color", (float *) &selectedEnemyESPObj->color);

                }

                break;
        }
    }
    if(Settings::Tab == 1) {
        switch (i) {
            case 0:
                const char* items[] = {
                        ("Granny"),
                        ("Grandpa"),
                        ("Slendrina"),
                        ("Monster")
                };

                if(ImGui::Combo(("Select Enemy"), &selectedEnemyGame, items, IM_ARRAYSIZE(items))){
                    selectedEnemyObj = EnemyEditor.enemies[selectedEnemyGame];
                }
               if(!selectedEnemyObj->destroyed){
                   if (ImGui::Button(("Set Active"), ImVec2(260 - 15, 81))){
                      if(selectedEnemyObj){
                          selectedEnemyObj->active = !selectedEnemyObj->active;
                          selectedEnemyObj->setActive = true;
                      }
                   }
                }

                ImGui::SameLine();
                if (ImGui::Button(("Destroy"), ImVec2(260 - 15, 81))){
                    if(selectedEnemyObj && !selectedEnemyObj->destroyed){
                        selectedEnemyObj->destroy = true;
                        selectedEnemyObj->destroyed = true;
                    }
                }
                if(!selectedEnemyObj->destroyed) {
                    ImGui::SameLine();
                    if (ImGui::Button(("Play As Enemy"), ImVec2(260 - 15, 81))) {
                        if (selectedEnemyObj) {
                            selectedEnemyObj->playAs = true;
                        }
                    }
                    if (ImGui::Button(("TP Me to Enemy"), ImVec2(260 - 15, 81))) {
                        if (selectedEnemyObj) {
                            selectedEnemyObj->tpMe2Enemy = true;
                        }
                    }
                    ImGui::SameLine();
                    if (ImGui::Button(("TP Enemy to Me"), ImVec2(260 - 15, 81))) {
                        if (selectedEnemyObj) {
                            selectedEnemyObj->tpEnemy2Me = true;
                        }
                    }
                    ImGui::SameLine();
                    if (ImGui::Button(("Enemy Kill Me"), ImVec2(260 - 15, 81))) {
                        if (selectedEnemyObj) {
                            selectedEnemyObj->enemyKillPlayer = true;
                        }
                    }
                    if (ImGui::Button(("Set Width"), ImVec2(260 - 15, 81))) {
                       //unimplemented
                    }
                    ImGui::SameLine();
                    if (ImGui::Button(("Set Height"), ImVec2(260 - 15, 81))) {
                        //unimplemented
                    }
                    ImGui::SameLine();
                    if (ImGui::Button(("Set Size"), ImVec2(260 - 15, 81))) {
                        //unimplemented
                    }
                    if (ImGui::Button(("God Mode"), ImVec2(260 - 15, 81))) {
                        if (selectedEnemyGame == 0) {
                            grCantAttack = true;
                        }
                        if (selectedEnemyGame == 1) {
                            grpCantAttack = true;
                        }
                        if (selectedEnemyGame == 2) {
                            slCantAttack = true;
                        }
                    }
                    ImGui::SameLine();
                    if (ImGui::Button(("Invisible"), ImVec2(260 - 15, 81))) {
                        if (selectedEnemyGame == 0) {
                            grInvis = true;
                        }
                        if (selectedEnemyGame == 1) {
                            grpInvis = true;
                        }
                        if (selectedEnemyGame == 2) {
                            slInvis = true;
                        }
                    }
                    ImGui::SameLine();
                    if (ImGui::Button(("Attack Distance"), ImVec2(260 - 15, 81))) {

                    }
                }
                break;
        }
    }
    if(Settings::Tab == 3){
        switch(i) {
            case 0:
                ImGui::Checkbox(("Enable FOG"), &fog);
                if(ImGui::SliderFloat(("Fog Distance"), &fogd, 0, 15)){

                }
                break;
            case 1:
                 if(ImGui::Button(("Set High Quality"))){
                     SetQualityLevel(1, true);
                 }

                if(ImGui::Button(("Set Low Quality"))){
                    SetQualityLevel(0, true);
                }
                break;
            case 2:
                ImGui::Checkbox(("Bypass FPS Limit"), &fpsbypass);
                ImGui::Separator();
                ImGui::Checkbox(("Slendrina Wireframe"), &chamsSlendrina);
                ImGui::Checkbox(("Enable Wireframe"), &wireframe);
                if(ImGui::SliderFloat(("Wireframe size"), &wfLW, 0, 5)){

                }

                ImGui::Separator();
                //ImGui::ColorEdit4("Wireframe color", nullptr);
                ImGui::Checkbox(("Enable Color Filter"), &clrfilter);
               // ImGui::ColorEdit4("Color Filter", nullptr);
                break;
        }
    }
    if(Settings::Tab == 4){
        switch(i){
            case 0:
                ImGui::Checkbox(("Make Enemy Ignore Teddy"), &igntdy);
                ImGui::Separator();
                ImGui::Checkbox(("All Cannot Attack"), &allCantAttack);
                if(!allCantAttack) {
                    ImGui::Checkbox(("Granny Can't Attack (Grandpa Also)"), &grCantAttack);
                    ImGui::Checkbox(("Grandpa Can't Attack"), &grpCantAttack);
                    ImGui::Checkbox(("Crocodile Can't Attack"), &crCantAttack);
                    ImGui::Checkbox(("Slendrina Can't Attack"), &slCantAttack);
                }
                ImGui::Separator();
                ImGui::Checkbox(("No One Can See Me"), &allInvis);
                if(!allInvis){
                    ImGui::Checkbox(("Invisible From Granny"), &grInvis);
                    ImGui::Checkbox(("Invisible From Grandpa"), &grpInvis);
                    ImGui::Checkbox(("Invisible From Crocodile"), &crInvis);
                    ImGui::Checkbox(("Invisible From Robot"), &slInvis);
                }

                break;
            case 1:

                 if(ImGui::Checkbox(("Disable Collisions"), &igncol)){
                     IgnoreLayerCollision(0,8, igncol);
                 }
                 ImGui::Checkbox(("Fly"), &fly);
                 if(ImGui::SliderFloat(("Front Speed"), &frSpeed, 0, 100)){
                     changeForwSpeed = true;
                 }
                 if(ImGui::SliderFloat(("Back Speed"), &bckSpeed, 0, 100)){
                     changeBackSpeed = true;
                 }
                 ImGui::Checkbox(("Ignore Bear Traps"), &ignbt);

                break;
            case 2:
                ImGui::SetWindowFontScale(0.8);
                if(ImGui::SliderFloat(("Field Of View"), &fov, 0, 150)){
                    if(Camera_main && Camera_main() && set_fieldOfView){
                        set_fieldOfView(Camera_main(), fov);
                    }
                }
                if(ImGui::SliderFloat(("Ortographic View Size"), &ortoSize, 3, 30)){
                    if(Camera_main && Camera_main() && set_orthographicSize ){
                        set_orthographicSize(Camera_main(), ortoSize);
                    }
                }


                ImGui::Separator();
                if(ImGui::SliderFloat(("Near Clip Plane "), &nearCF, 0.3f, 100)){
                    if(Camera_main && Camera_main() && set_nearClipPlane){
                        set_nearClipPlane(Camera_main(), nearCF);
                    }
                }
                if(ImGui::SliderFloat(("Camera Aspect Size"), &camAspect, 1, 50)){
                    if(Camera_main && Camera_main() && set_aspect){
                        set_aspect(Camera_main(), camAspect);
                    }
                }
                if(ImGui::Checkbox(("Ortographic View"), &ortoCam)){
                    if(Camera_main && Camera_main() && set_orthographic){
                        set_orthographic(Camera_main(), ortoCam);
                    }
                }



                ImGui::Separator();
               // ImGui::Checkbox("Freeze Camera Position", &freezeCamSummary);
                break;
            case 3:

                break;
            case 4:

                break;
        }
    }


}
void cmdExecuteSystem(const char* input){
    printf("%s", input);
}
void
DrawESPElement(ImDrawList *list,ImVec2 fromWhere, const char *name, ImColor elClr, ImVec2 toWhere, bool enableBg,
               float addV2, float distance) {
    //l:ImVec2(ImVec2(screenWidth / 2, screenHeight / 2))
    //
    if(ESP.Lines){
        list->AddLine(fromWhere, toWhere, elClr);
    }
    const char * dstr = std::string(std::string(("Distance: ")) + std::to_string(distance)).c_str();
    if(ESP.Boxes) {
        if (enableBg) {
            //  elClr.Value = ImVec4(elClr.Value.x / 4,elClr.Value.y,elClr.Value.z, elClr.Value.w / 2   );
            list->AddRectFilled(ImVec2(toWhere.x - 150, toWhere.y),
                                ImVec2((toWhere.x + 150) * addV2, (toWhere.y + 400) * addV2),
                                ImColor(elClr.Value.x, elClr.Value.y, elClr.Value.z, 0.15));
        }
        list->AddRect(ImVec2(toWhere.x - 150, toWhere.y),
                      ImVec2((toWhere.x + 150) * addV2, (toWhere.y + 400) * addV2), elClr, 0, 0,
                      enableBg ? 2 : 1);
    }
    if(ESP.Texts){
        list->AddText(ImGui::GetFont(), ESP.textSize, ImVec2(toWhere.x-60, toWhere.y + 10), enableBg ? ImColor(elClr.Value.x/2.f, elClr.Value.y/2.f, elClr.Value.z/2.f, 1.0f) : elClr, name, nullptr, 0.0f, nullptr);
        list->AddText(ImGui::GetFont(), ESP.textSize, ImVec2(toWhere.x-150, toWhere.y + 410), enableBg ? ImColor(elClr.Value.x, elClr.Value.y, elClr.Value.z, 1.0f) : elClr, dstr, nullptr, 0.0f, nullptr);
    }
}
//btw void* and void * does not have diffrence.
ImDrawList* getDrawList(){
    ImDrawList *drawList;
    if(ESP.drawBehindMenu){
        drawList = ImGui::GetBackgroundDrawList();
    } else {
        drawList = ImGui::GetForegroundDrawList(ImGui::GetMainViewport());
    }
    return drawList;
}

void RenderESPSummary(){
    if (!ESP.enemies.empty() && ESP.ESP) {
        for (int i = 0; i < ESP.enemies.size(); i++) {
            ESPEnemy *me = ESP.enemies[i];
            if(me->ESP && me->screenPosition.x > 0){
                if(i==3 && me->physicalPosition.x == 0 ){
                    getDrawList()->AddText(ImGui::GetFont(), 30, ImVec2(0,0), ImColor(255, 255, 0, 255), "Can't find Slendrina!", nullptr, 0.0f, nullptr);
                } else {
                    DrawESPElement(getDrawList(), ImVec2(screenWidth / 2, ESP.lineOffsetY), me->name, me->color,
                                   me->screenPosition, ESP.boxBg,1, me->distance);
                }
            }
        }
    }
}
//Render the menu
void Render(){
    ImGuiIO& io = ImGui::GetIO();
    if(!ImGui::get_cmdExecutorHaveVal()){
        ImGui::set_cmdExecutor(&cmdExecuteSystem);
    }
    //Creating A new ImGui Frame for our backend
    ImGui_ImplAndroidGLES2_NewFrame(screenWidth, screenHeight, currentTimeInMilliseconds()-startTime);
    ImDrawList* drawList = getDrawList();
    //Example using text over screen
    drawList->AddText(ImGui::GetFont(), 30, ImVec2(100.f, 100.f), ImColor(255, 255, 0, 255), "Player Stats.", nullptr, 0.0f, nullptr);
    //Rendering ESP
    RenderESPSummary();

    //Open animation
    if(ImGui::get_openAnimationState() < 1200){

        ImGui::set_openAnimationState(ImGui::get_openAnimationState()+60);
    }
    if(ImGui::get_openAnimationState() > 1200){
        ImGui::set_openAnimationState(1200);
    }
    //Set Animation to window
    ImGui::SetNextWindowBgAlpha(ImGui::get_openAnimationState());
    ImGui:: SetNextWindowSize(ImVec2(ImGui::get_openAnimationState(),488));
    std::string wName = std::string( ozObfuscate("Mod Menu for ")) + GAME_NAME + "(" + GAME_VER + ")" + std::string(ozObfuscate(" - by ozMod"));
    ImGui::Begin( wName.c_str(), nullptr, ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_AlwaysAutoResize);
    if (ImGui::BeginMenuBar())
    {
        if (ImGui::BeginMenu(ozObfuscate("Tools")))
        {
            ImGui::MenuItem(ozObfuscate("Paint"), ozObfuscate("Ctrl+P"), &show_paint);
            ImGui::MenuItem(ozObfuscate("Metrics/Debugger"), nullptr);
            //ImGui::MenuItem(ozObfuscate("Console"), nullptr, &show_app_console); // Wanna do later
            ImGui::MenuItem(ozObfuscate("Game Logs"), nullptr, &show_app_log);
            ImGui::MenuItem(ozObfuscate("Style Editor"), ozObfuscate("Ctrl+O"), &showStyleEditor);
            if (ImGui::MenuItem(ozObfuscate("Close"), ozObfuscate("Ctrl+C")))  {

            }
            ImGui::EndMenu();
        }
        if (ImGui::BeginMenu(ozObfuscate("Style Editor")))
        {
            if (ImGui::MenuItem(ozObfuscate("Open Editor"), ozObfuscate("Ctrl+O"))) {
                showStyleEditor = true;
            }
            if (ImGui::MenuItem(ozObfuscate("Close Editor"), ozObfuscate("Ctrl+O"))) {
                showStyleEditor = false;
            }
            if (ImGui::MenuItem(ozObfuscate("Reset All"), ozObfuscate("Ctrl+R")))  {
                 ImGui::StyleColorsClassic();
                 ozTheme();
            }
            ImGui::EndMenu();
        }
        if (ImGui::BeginMenu(ozObfuscate("Features")))
        {
            if (ImGui::MenuItem("Test", "")) {

            }

            ImGui::EndMenu();
        }
        if (ImGui::BeginMenu(ozObfuscate("Help")))
        {
            if (ImGui::MenuItem(ozObfuscate("About Mod Menu"), ("Ctrl+M"))) {

            }
            if (ImGui::MenuItem(ozObfuscate("About ImGui"), ("Ctrl+G"))) {

            }

            ImGui::EndMenu();
        }
        ImGui::EndMenuBar();
    }

    if (show_app_console){
        ImGui::ShowExampleAppConsole(&show_app_console);
    }
    if (show_app_log) {
        ImGui::ShowExampleAppLog(&show_app_log);
    }
    if (show_paint)  {
        ImGui::ShowExampleAppCustomRendering(&show_app_log);
    }
    if(showStyleEditor){
        ImGui::ShowStyleEditor();
    }
    ImGui::Columns(2);
    ImGui::SetColumnOffset(1, 260);
    {
        //Left side

        static ImVec4 active = to_vec4(41, 40, 100, 255);
        static ImVec4 inactive = to_vec4(31, 30, 31, 255);

        ImGui::PushStyleColor(ImGuiCol_Button, Settings::Tab == 1 ? active : inactive);
        if (ImGui::Button(ozObfuscate("General"), ImVec2(230 - 15, 81))){
            Settings::Tab = 1;
        }



        ImGui::PushStyleColor(ImGuiCol_Button, Settings::Tab == 2 ? active : inactive);
        if (ImGui::Button(ozObfuscate("ESP"), ImVec2(230 - 15, 81))){
            Settings::Tab = 2;
        }



        ImGui::PushStyleColor(ImGuiCol_Button, Settings::Tab == 3 ? active : inactive);
        if (ImGui::Button( ozObfuscate("Unity"), ImVec2(230 - 15, 81)))
            Settings::Tab = 3;


        ImGui::PushStyleColor(ImGuiCol_Button, Settings::Tab == 4 ? active : inactive);
        if (ImGui::Button(ozObfuscate("Player"), ImVec2(230 - 15, 81)))
            Settings::Tab = 4;

        ImGui::PopStyleColor(4);

        ImGui::SetCursorPosY(ImGui::GetWindowHeight() - 30);
    }

    ImGui::NextColumn();

    //Right side
    {


        static ImGuiTextFilter filter;
        static std::vector<std::string> resources;
        if(Settings::Tab==0) {

            resources =
                    {
                            std::string(ozObfuscate("Getting Started")),
                                        std::string( ozObfuscate("About Mod Menu")),
                            std::string(ozObfuscate("ImGui Demo Window")),
                            std::string(ozObfuscate("Game Analytics/Metrics")),

                    };

        }
        if(Settings::Tab==1){
            resources =
                    {
                            std::string(ozObfuscate("Enimies Admin Panel")),
                                        std::string( ozObfuscate("Camera Menu")),
                            std::string(ozObfuscate("Object Editor")),
                                        std::string(ozObfuscate("Teleport Menu")),
                                                    std::string(ozObfuscate("Items Spawner")),
                                                                std::string(ozObfuscate("Clones Spawner"))
                    };
        }
        if(Settings::Tab==2){
            resources =
                    {
                            std::string(ozObfuscate("ESP Layer")),
                                        std::string(ozObfuscate("ESP Style")),
                                                                std::string(ozObfuscate("ESP Enemies"))
                    };
        }
        if(Settings::Tab==3){
            resources =
                    {
                            std::string(ozObfuscate("Light")),
                                                    std::string(ozObfuscate("Quality Settings")),
                                                                            std::string(ozObfuscate("Engine Core"))
                    };
        }

        if(Settings::Tab==4){
            resources =
                    {
                            std::string(ozObfuscate("Anti Enemy")),
                                                    std::string(ozObfuscate("Physics")),
                                                                            std::string(ozObfuscate("Camera")),
                                                                                                    std::string(ozObfuscate("Player Stats")),
                                                                                                                            std::string(ozObfuscate("No-Clip Settings")),
                    };
        }
        filter.Draw(u8" Поиск", 240);
        ImGui::SameLine();
        if(ImGui::Button("Clear")){
            filter.Clear();
        }
        ImGui::PushStyleColor(ImGuiCol_FrameBg, IM_COL32(0, 0, 0, 0));

        if (ImGui::ListBoxHeader("##ResourcesList", ImVec2(ImGui::GetContentRegionAvail().x + 20, ImGui::GetContentRegionAvail().y)))
        {
            if(Settings::Tab == 3){
                if(ImGui::SliderFloat("Time Scale", &timescale, 0, 5) && set_timeScale){
                    set_timeScale(timescale);

                }
            }
            if(Settings::Tab == 2){
                ImGui::Checkbox("Enable ESP", &ESP.ESP);
            }
            int i = 0;
            for (const auto& resource : resources)
            {
                if (filter.PassFilter(resource.c_str()))
                {
                    if (ImGui::TreeNode(resource.c_str())){
                        DrawContentFor(i);
                        ImGui::TreePop();
                    }
                }
                i++;
            }
            ImGui::ListBoxFooter();
        }
        ImGui::PopStyleColor();
    }
//ShowDemoWindow
    //ImGui::PushItemWidth(ImGui::GetWindowWidth() * 0.65f);    // 2/3 of the space for widget and 1/3 for labels
    // Right align, keep 140 pixels for labels
    ImGui::End();
    ImGui::EndFrame(); //End ImGui Frame
    ImGui::Render(); // Render ImGui
    ImGui_ImplAndroidGLES2_RenderDrawLists(ImGui::GetDrawData()); //Draw ImGui Draw Data with our Android Backend
}

jclass (*oFindClassHook)(JNIEnv*, const char*);
jclass FindClassHook(JNIEnv*env, const char*classn)
{
    jclass result = oFindClassHook(env, classn);
    if(std::string(classn).find(ozObfuscate("uk/lgl")) != std::string::npos){
        return nullptr;
    }
    if(std::string(classn).find(ozObfuscate("com/android/support/Preferences")) != std::string::npos){
        return nullptr;
    }
    if(std::string(classn).find(ozObfuscate("com/android/support/Menu")) != std::string::npos){
        return nullptr;
    }
    if(std::string(classn).find(ozObfuscate("FloatingModMenuService")) != std::string::npos){
        return nullptr;
    }
    if(std::string(classn).find(ozObfuscate("keysystem")) != std::string::npos){
        return nullptr;
    }
    if(std::string(classn).find(ozObfuscate("tdm/ahz")) != std::string::npos){
        return nullptr;
    }
    return result;
}
jint (*old_RegisterNatives )(JNIEnv*, jclass, JNINativeMethod*,jint);
bool (*old_nativeInjectEvent )(JNIEnv*, jobject ,jobject event);
bool hook_nativeInjectEvent(JNIEnv* env, jobject instance,jobject event){
        jclass tchcls = env->FindClass(ozObfuscate("android/view/MotionEvent"));
        jclass kycls = env->FindClass(ozObfuscate("android/view/KeyEvent"));
        jmethodID ga = env->GetMethodID(kycls, ozObfuscate("getAction"),
                                    ozObfuscate("()I"));
        if(env->IsInstanceOf(event, kycls) && env->CallIntMethod(event, ga) == 0) {
            LOGI("Inject KeyEvent");
            jmethodID gkc = env->GetMethodID(kycls, ozObfuscate("getKeyCode"), ozObfuscate("()I"));
            jmethodID guc = env->GetMethodID(kycls, ozObfuscate("getUnicodeChar"), ozObfuscate("()I"));
            int keyCode = env->CallIntMethod(event, gkc);
            ImGuiIO& io = ImGui::GetIO();
            switch (keyCode) {
                case 19:
                     inputChar = keyCode;
                    break;
                case 20:
                    inputChar = keyCode;
                    break;
                case 21:
                    inputChar = keyCode;
                    break;
                case 22:
                    inputChar = keyCode;
                    break;
                case 61:
                    inputChar = keyCode;
                    break;
                case 66:
                    inputChar = keyCode;
                    break;
                case 67:
                    inputChar = keyCode;
                    break;
                case 92:
                    inputChar = keyCode;
                    break;
                case 93:
                    inputChar = keyCode;
                    break;
                case 111:
                    inputChar = keyCode;
                    break;
                case 112:
                    inputChar = keyCode;
                    break;
                case 122:
                    inputChar = keyCode;
                    break;
                case 123:
                    inputChar = keyCode;
                    break;
                case 124:
                    inputChar = keyCode;
                    break;
                default:
                     ImGui::GetIO().AddInputCharacter(env->CallIntMethod(event, guc));
                    break;
            }
            return old_nativeInjectEvent(env, instance, event);
        }
        if(!env->IsInstanceOf(event, tchcls)){
            return old_nativeInjectEvent(env, instance, event);
        }
        jmethodID id_getAct = env->GetMethodID(tchcls, ozObfuscate("getActionMasked"), ozObfuscate("()I"));
        jmethodID id_getX = env->GetMethodID(tchcls, ozObfuscate("getX"), ozObfuscate("()F"));
        jmethodID id_getY = env->GetMethodID(tchcls, ozObfuscate("getY"), ozObfuscate("()F"));
        jmethodID id_getPs = env->GetMethodID(tchcls, ozObfuscate("getPointerCount"), ozObfuscate("()I"));
        HandleInputEvent(env, env->CallIntMethod(event, id_getAct),env->CallFloatMethod(event, id_getX), env->CallFloatMethod(event, id_getY), env->CallIntMethod(event, id_getPs));
        if (!ImGui::GetIO().MouseDownOwnedUnlessPopupClose[0]){
            return old_nativeInjectEvent(env, instance, event);
        }
        return false;
}
void SearchActivtiy(JNIEnv * globalEnv){
    jclass prcls = globalEnv->FindClass("com/unity3d/player/UnityPlayer");
    if(prcls != nullptr){
        jfieldID UnityPlayer_CurrentActivity_fid2 = globalEnv->GetStaticFieldID(prcls, "currentActivity","Landroid/app/Activity;");
        if(UnityPlayer_CurrentActivity_fid2 != nullptr){
            UnityPlayer_CurrentActivity_fid = globalEnv->NewGlobalRef(globalEnv->GetStaticObjectField(prcls, UnityPlayer_CurrentActivity_fid2));
            if(UnityPlayer_CurrentActivity_fid == nullptr){
                //not ok
            } else {
                //okk
            }
        }
    }
}

bool (*old_nativeDraw )(JNIEnv*, jobject );
bool hook_nativeDraw(JNIEnv* env, jobject instance){
    UpdateInput();
    // Open on-screen (soft) input if requested by Dear ImGui
    static bool WantTextInputLast = false;
    ImGuiIO& io = ImGui::GetIO();
    if (io.WantTextInput && !WantTextInputLast){
        diplayKeyboard(nullptr);
    }
    WantTextInputLast = io.WantTextInput;
    Render();
    bool result = old_nativeDraw(env, instance);
    LOGI("JNI Render Thread");
    return result;
}
jint hook_RegisterNatives(JNIEnv* env, jclass destinationClass, JNINativeMethod* methods,
                          jint count){
    int i; // [sp+24h] [bp-2Ch]
    for ( i = 0; i < count; ++i )
    {
        if (!strcmp(methods[i].name, ozObfuscate("nativeInjectEvent")) ){
            MSHookFunction(methods[i].fnPtr, (void*)hook_nativeInjectEvent, (void **)&old_nativeInjectEvent);
        }
        if (!strcmp(methods[i].name, ozObfuscate("nativeRender")) ){
            LOGI("Native Render! It is on: %p", (methods[i].fnPtr));

            MSHookFunction(methods[i].fnPtr, (void*)hook_nativeDraw, (void **)&old_nativeDraw);
        }
    }
    SearchActivtiy(env);
    return old_RegisterNatives(env, destinationClass, methods, count);
}


extern "C"
JNIEXPORT jint JNICALL
JNI_OnLoad(JavaVM *vm, void *reserved) {
    publicJVM = vm;
    JNIEnv *globalEnv;
    publicJVM->GetEnv((void **) &globalEnv, JNI_VERSION_1_6);
    InitMods();
    MSHookFunction((void*)globalEnv->functions->RegisterNatives, (void*)hook_RegisterNatives, (void **)&old_RegisterNatives);
    return JNI_VERSION_1_6;
}
