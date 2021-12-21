#include "obfuscator.hpp" //Compile-Time String Obfuscator

#include <vector> //std vector
#include <ctime> // time header to support timeval structure
#include <unistd.h> //uni std
#include <pthread.h> // pthread header
#include <string>//std string
#include <set> //std set (bionic linker)
#include <iostream>
#include <locale>
#include <codecvt>
#include <sstream>
#include <dlfcn.h>
bool isLGL = false;
bool isALegalUse = true;

//Hooking software
#include "Substrate/CydiaSubstrate.h"
#include "Dobby-master/include/dobby.h"

float currentTimeInMilliseconds(){
    struct timeval tv;
    gettimeofday(&tv, nullptr);
    return ((tv.tv_sec * 1000) + (tv.tv_usec / 1000));
}

#include "ImGui_Impl_AndroidOpenGL2.h" // my gui implementation

#include "EGL/egl.h"  // EGL
#include "GLES3/gl3.h"

//android
#include <android/asset_manager_jni.h> //JNI + JNI Asset Manager
#include <android/native_window.h> // native window header is used to
#include <android/input.h> //input consts
#include <android/log.h>

#define GAME_NAME ozObfuscate ("Granny 3")
#define GAME_VER ozObfuscate("1.1.1")
#define PKG_NAME ozObfuscate("com.DVloper.Granny3")
//android logger
#define ENABLE_LOGS true //disable on production deployment
#define LOGCAT_TAG ozObfuscate("ozMod")
#define LOGI(...) if(ENABLE_LOGS){ __android_log_print(ANDROID_LOG_INFO,LOGCAT_TAG, __VA_ARGS__); }
#define  LOGE(...)  if(ENABLE_LOGS){ __android_log_print(ANDROID_LOG_ERROR,LOGCAT_TAG,__VA_ARGS__); }
#define  LOGD(...)  if(ENABLE_LOGS){ __android_log_print(ANDROID_LOG_DEBUG,LOGCAT_TAG,__VA_ARGS__); }
#define  LOGDE(...) if(ENABLE_LOGS){  __android_log_print(ANDROID_LOG_DEFAULT,LOGCAT_TAG,__VA_ARGS__);}
#define  LOGW(...)  if(ENABLE_LOGS){ __android_log_print(ANDROID_LOG_WARN,LOGCAT_TAG,__VA_ARGS__); }
#define  LOGV(...)  if(ENABLE_LOGS){ __android_log_print(ANDROID_LOG_VERBOSE,LOGCAT_TAG,__VA_ARGS__); }
#define  LOGF(...)  if(ENABLE_LOGS){ __android_log_print(ANDROID_LOG_FATAL,LOGCAT_TAG,__VA_ARGS__); }
#define  LOGU(...)  if(ENABLE_LOGS){ __android_log_print(ANDROID_LOG_UNKNOWN,LOGCAT_TAG,__VA_ARGS__); }
#define  LOGS(...)  if(ENABLE_LOGS){ __android_log_print(ANDROID_LOG_SLIENT,LOGCAT_TAG,__VA_ARGS__); }


/*
	Все координаты GUI-элементов задаются
	относительно разрешения 1920x1080
*/
#define MULT_X	0.000520833333334f	// 1/1920
#define MULT_Y	0.00092592592f 	// 1/1080
#define SCREEN_FRUSTUMM 2.461999


int currentCategory = 0;
void Render();
void UpdateInput();
bool setupGraphics(int width, int height);
int inputChar = 0;
float startTime, screenWidth, screenHeight;
int RenderTicks = 0;


unsigned long findLibraryBaseAddress(const char *library) {
    const char *filename;
    char buffer[1024] = { 0 };
    FILE *fp = nullptr;
    unsigned long address = 0;
    filename = ozObfuscate("/proc/self/maps");
    fp = fopen(filename, ozObfuscate("rt"));
    if (fp == nullptr) {
        LOGE("%s", "Access Denied to Open /proc/self/maps!")
        return 0;
    }
    while (fgets(buffer, sizeof(buffer), fp)) {
        if (strstr(buffer, library)) {
            address = (unsigned long) strtoul(buffer, nullptr, 16);
            fclose(fp);
            return address;
        }
    }
}

unsigned long getRealAddrOfLibrary(const char *libraryName, unsigned long relativeAddr) {
    uintptr_t baseAddress = findLibraryBaseAddress(libraryName);
    return baseAddress == 0 ? 0 : ((unsigned long)baseAddress + relativeAddr);
}

bool isLibraryLoaded(const char *libraryName) {
    char line[512] = {0};
    __sFILE* fp = fopen(ozObfuscate("/proc/self/maps"), ozObfuscate("rt"));
    if (fp != nullptr) {
        while (fgets(line, sizeof(line), fp)) {
            if (strstr(line, libraryName)) {
                return true;
            }
        }
        fclose(fp);
    }
    return false;
}

#include "Mods.h" // Mods Header
#include "ImGui/imgui_internal.h"

jobject getGlobalContext(JNIEnv *env)
{
    jclass activityThread = env->FindClass("android/app/ActivityThread");
    jmethodID currentActivityThread = env->GetStaticMethodID(activityThread, ozObfuscate("currentActivityThread"), ozObfuscate("()Landroid/app/ActivityThread;"));
    jobject at = env->CallStaticObjectMethod(activityThread, currentActivityThread);
    jmethodID getApplication = env->GetMethodID(activityThread, ozObfuscate("getApplication"), ozObfuscate("()Landroid/app/Application;"));
    return env->CallObjectMethod(at, getApplication);
} //get context by ActivityThread.currentActivityThread.getApplication(). android.app.ActivityThread is a hidden java class in android core

JavaVM *publicJVM; //Public Java Virtual Machine

//localization
int currentLang = 0;
#define ozSelectLanguage(en, ru) currentLang == 1 ? ozObfuscate(ru) : ozObfuscate(en)

jobject activity;
class ImDrawData;

bool HandleInputEvent(JNIEnv *env, int motionEvent, int x, int y, int p);

jobject UnityPlayer_CurrentActivity_fid;

void displayKeyboard(JNIEnv* newEnv) {
    LOGI("Display keybrd")
    JNIEnv* env = nullptr;
    if(newEnv != nullptr){
        env = newEnv;
    } else {
        jint jni_return = publicJVM->GetEnv((void**)&env, JNI_VERSION_1_6);
        if (jni_return == JNI_ERR){
            return;
        }
        jni_return = publicJVM->AttachCurrentThread(&env, nullptr);
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


typedef enum { TOUCH_ACTION_DOWN = 0, TOUCH_ACTION_UP, TOUCH_ACTION_MOVE } TOUCH_ACTION;

typedef struct {
    TOUCH_ACTION action;
    float x;
    float y;
    int pointers;
    float y_velocity;
    float x_velocity;
}TOUCH_EVENT;
 TOUCH_EVENT g_LastTouchEvent;
 void UpdateInput() {
    // Update buttons
    ImGuiIO &io = ImGui::GetIO();

    // Mouse buttons: 0=left, 1=right, 2=middle + extras. ImGui itself mostly only uses left button
    // (BeginPopupContext** are using right button). Others buttons allows us to track if the mouse
    // is being used by your application + available to user as a convenience via IsMouse** API.
    // TOUCH_ACTION_MOVE -> MousePos
    // TOUCH_ACTION_DOWN -> MouseDown[0] true, left button
    // TOUCH_ACTION_UP -> MouseDown[0] false. left button
    // TOUCH_ACTION_POINTER_DOWN -> multi finger as scroll, set MouseWheel. MouseWheelH not used


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

bool  HandleInputEvent(JNIEnv *env, int motionEvent, int x, int y, int p) {
    // Need to make sure the coordinate (x,y) here is relative to the top left of the surface
    // This is how the behavior of GLSurfaceView.onTouchEvent
    // If the coordinate is obtained in a different way. This should be adjusted accordingly
    LOGI("Touch %d_%d", motionEvent, p)
    float velocity_y = (float)((float)y - g_LastTouchEvent.y) / 100.f;
    g_LastTouchEvent = {.action = (TOUCH_ACTION) motionEvent, .x = static_cast<float>(x), .y = static_cast<float>(y), .pointers = p, .y_velocity = velocity_y};
    ImGuiIO &io = ImGui::GetIO();
    io.MousePos.x = g_LastTouchEvent.x;
    io.MousePos.y = g_LastTouchEvent.y;
    if(motionEvent == 2){
        if (g_LastTouchEvent.pointers > 1) {
            io.MouseWheel = g_LastTouchEvent.y_velocity;
            io.MouseDown[0] = false;
        }
        else {
            io.MouseWheel = 0;
        }
    }
    if(motionEvent == 0){
        io.MouseDown[0] = true;
    }
    if(motionEvent == 1){
        io.MouseDown[0] = false;
    }
    return true;
}
void ozTheme()
{
    ImGuiStyle* style = &ImGui::GetStyle();
    style->WindowBorderSize = 3;

    style->WindowTitleAlign = ImVec2(0.5, 0.5);
    style->WindowMinSize = ImVec2(900, 430);
    style->FrameRounding = 12.0f;
    style->GrabRounding = 12.0f;
    style->Colors[ImGuiCol_BorderShadow] = ImVec4(1.0f, 0.1f, 0.1f, 1.0f);
    style->Colors[ImGuiCol_CheckMark] = ImVec4(1.0f, 0.0f, 0.0f, 1.00f);
    style->Colors[ImGuiCol_Button] = ImColor(31, 30, 31, 255);
    style->Colors[ImGuiCol_ButtonActive] = ImColor(41, 40, 41, 255);
    style->Colors[ImGuiCol_ButtonHovered] = ImColor(41, 40, 41, 255);
    style->Colors[ImGuiCol_TitleBg]                = ImVec4(0.04f, 0.04f, 0.04f, 1.00f);
    style->Colors[ImGuiCol_TitleBgActive]          = ImVec4(75.0f/255.0f, 0.0f, 130.0f/255.0f, 1.00f);
    style->Colors[ImGuiCol_TitleBgCollapsed]       = ImVec4(0.00f, 0.00f, 0.00f, 0.51f);
    style->Colors[ImGuiCol_Separator] = ImColor(70, 70, 70, 255);
    style->Colors[ImGuiCol_SeparatorActive] = ImColor(76, 76, 76, 255);
    style->Colors[ImGuiCol_SeparatorHovered] = ImColor(76, 76, 76, 255);
    style->Colors[ImGuiCol_FrameBgHovered] = ImColor(37, 36, 37, 255);
    style->Colors[ImGuiCol_Header] = ImColor(0, 0, 0, 0);
    style->Colors[ImGuiCol_HeaderActive] = ImColor(0, 0, 0, 255);
    style->Colors[ImGuiCol_HeaderHovered] = ImColor(46, 46, 46, 255);
}
bool setupGraphics(float w, float h) {
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

    ImFontConfig font_cfg;
    font_cfg.SizePixels = 22.0f;
    font_cfg.GlyphRanges = io.Fonts->GetGlyphRangesCyrillic();
    io.Fonts->AddFontDefault(&font_cfg);
    return true;
}

ImVec4 to_vec4(float r, float g, float b, float a)
{
    return ImVec4(r / 255.0f, g / 255.0f, b / 255.0f, a / 255.0f);
}
static bool show_app_console = false;
static bool show_app_log = false;
static bool WantTextInputLast = false;
bool show_paint = false;
bool showStyleEditor;

float testCol, testCol2, testCol3,testCol4;

void DrawContentFor(int i){
    float spacing = ImGui::GetStyle().ItemInnerSpacing.x;
    if(currentCategory == 0){
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
                ImGui::Text("%s", (const char *)("    adamyaxley, FCS Leader - for Protection and obfuscation (FCS Leader also)"));
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
                ImGui::Text(("Game Name: %s"), (const char*)GAME_NAME);
                ImGui::Text(("Game Version: %s"), (const char*)GAME_VER);
                ImGui::Separator();
                ImGui::PushStyleColor(ImGuiCol_Text, IM_COL32(255, 0, 0, 255));
                ImGui::Text(("Dear ImGui Version: %s. Modded by ozMod."), IMGUI_VERSION);
                ImGui::Text("%s", (const char *)("Using Android Implementation For ImGui In Unity Player."));
                ImGui::PopStyleColor();
                break;
        }
    }

    if(currentCategory == 2){
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
    const char* items[] = {
            ozSelectLanguage("Granny", u8"Бабка"),
            ozSelectLanguage("Grandpa", u8"Дед"),
            ozSelectLanguage("Slendrina", u8"Слендрина"),
            ozSelectLanguage("Crocodile", u8"Крокодил")
    };
    const char* placelist[] = {
            ozSelectLanguage("Main Room", u8"Главная комната"), //10.810 -5.538 17.600
            ozSelectLanguage("Train", u8"К поезду"),// 11.810001 -12.538391 10.6
            ozSelectLanguage("Outside", u8"За дом"), //0.522 5 40
            ozSelectLanguage("Shop Backroom", u8"Задняя комната магазина"), // 22.889 -16.162 -0.053
            ozSelectLanguage("Old Bedroom", u8"Старая комната для отдыха") //-8.307 14.511 2.244

    };
    const char* itemlist[] = {
            ozSelectLanguage("Crow", u8"Воробушек"),
            ozSelectLanguage("Granny", u8"Бабка"),
            ozSelectLanguage("Grandpa", u8"Дед"),
            ozSelectLanguage("Fire", u8"Огонь"),
            ozSelectLanguage("Gate", u8"Ворота"),
            ozSelectLanguage("Accelerator", u8"Акселератор"),
            ozSelectLanguage("Bridge Crank", u8"Мостовой кривошип"),
            ozSelectLanguage("Coconut", u8"Кокос"),
            ozSelectLanguage("Coin", u8"Монета"),
            ozSelectLanguage("Crowbar", u8"Лом // Монитровка"),
            ozSelectLanguage("Door Activatior", u8"Активатор дверей"),
            ozSelectLanguage("Electric Switch", u8"Электрический переключатель"),
            ozSelectLanguage("Firewood", u8"Дрова"),
            ozSelectLanguage("Gate Fuse", u8"Предохранитель ворот"),
            ozSelectLanguage("Generator Cable", u8"Кабель от генератора"),
            ozSelectLanguage("Matches", u8"Спички"),
            ozSelectLanguage("Padlock Key", u8"Синий ключ"),
            ozSelectLanguage("Plank", u8"Доска"),
            ozSelectLanguage("Safe Key", u8"Ключ от сейфа"),
            ozSelectLanguage("Shed Key", u8"Ключ от сарая"),
            ozSelectLanguage("Shotgun", u8"Дробовик"),
            ozSelectLanguage("Slingshot", u8"Рогатка"),
            ozSelectLanguage("Teddy", u8"Михаил петрович"),
            ozSelectLanguage("Train Key", u8"Ключ от поезда"),
            ozSelectLanguage("Vas", u8"Ваза"),
            ozSelectLanguage("Vas 2", u8"Вторая ваза"),
            ozSelectLanguage("Weapon Key", u8"Ключ от арболета"),

    };
    if(currentCategory == 1) {
        switch (i) {
            case 0:


                if(ImGui::Combo(ozSelectLanguage("Select Enemy", u8"Выбрать врага"), &selectedEnemyGame, items, IM_ARRAYSIZE(items))){
                    selectedEnemyObj = EnemyEditor.enemies[selectedEnemyGame];
                }

               if(!selectedEnemyObj->destroyed){
                   if (ImGui::Button(ozSelectLanguage("ON/OFF", u8"Выключить"), ImVec2(260 - 15, 81))){
                      if(selectedEnemyObj){
                          selectedEnemyObj->active = !selectedEnemyObj->active;
                          selectedEnemyObj->setActive = true;
                      }
                   }
                }

                ImGui::SameLine();

                if (ImGui::Button(ozSelectLanguage("Destroy", u8"Удалить с игры"), ImVec2(260 - 15, 81))){
                    if(selectedEnemyObj && !selectedEnemyObj->destroyed){
                        selectedEnemyObj->destroy = true;

                    }
                }
                if(!selectedEnemyObj->destroyed) {
                    ImGui::SameLine();
                    if (ImGui::Button(ozSelectLanguage("Play As Enemy", u8"Играть за врага"), ImVec2(260 - 15, 81))) {
                        if (selectedEnemyObj) {
                            selectedEnemyObj->playAs = true;
                        }
                    }

                    if (ImGui::Button(ozSelectLanguage("TP Enemy To Me", u8"Телепорт врага ко мне"), ImVec2(260 - 15, 81))) {
                        if (selectedEnemyObj) {
                            selectedEnemyObj->tpMe2Enemy = true;
                        }
                    }
                    ImGui::SameLine();
                    if (ImGui::Button(ozSelectLanguage("TP Me To Enemy", u8"Телепорт к врагу"), ImVec2(260 - 15, 81))) {
                        if (selectedEnemyObj) {
                            selectedEnemyObj->tpEnemy2Me = true;
                        }
                    }
                    ImGui::SameLine();
                    if (ImGui::Button(ozSelectLanguage("Enemy Kill Me", u8"Враг меня убъет"), ImVec2(260 - 15, 81))) {
                        if (selectedEnemyObj) {
                            selectedEnemyObj->enemyKillPlayer = true;
                        }
                    }
                    if(ImGui::Checkbox(ozSelectLanguage("Can't Attack", u8"Бессмертие"), &selectedEnemyObj->invis)){
                        if (selectedEnemyGame == 0) {
                            grCantAttack = !grCantAttack;

                        }
                        if (selectedEnemyGame == 1) {
                            grpCantAttack = !grpCantAttack;

                        }
                        if (selectedEnemyGame == 2) {
                            slCantAttack = !slCantAttack;

                        }
                    }
                    if(ImGui::Checkbox(ozSelectLanguage("Invisible", u8"Невидимость"), &selectedEnemyObj->invis)){
                        if (selectedEnemyGame == 0) {
                            grInvis = !grInvis;

                        }
                        if (selectedEnemyGame == 1) {
                            grpInvis = !grpInvis;

                        }
                        if (selectedEnemyGame == 2) {
                            slInvis = !slInvis;

                        }
                    }

                        ImGui::SliderFloat(ozSelectLanguage("Set attack Distance", u8"Радиус атаки"), &selectedEnemyObj->killDist, 0, 15);
                }
                break;
            case 2:
                ImGui::PushItemWidth(-1.0f);
                if (ImGui::Button(ozSelectLanguage("Teleport Me to Granny", u8"Я -> Бабка"))) {
                    EnemyEditor.enemies[0]->tpMe2Enemy = true;
                }
                ImGui::SameLine();
                if (ImGui::Button(ozSelectLanguage("Teleport Granny To Me", u8"Бабка -> Я"))) {
                    EnemyEditor.enemies[0]->tpEnemy2Me = true;
                }

                if (ImGui::Button(ozSelectLanguage("Teleport Me to Grandpa", u8"Я -> Дед"))) {
                    EnemyEditor.enemies[1]->tpMe2Enemy = true;
                }
                ImGui::SameLine();
                if (ImGui::Button(ozSelectLanguage("Teleport Grandpa To Me", u8"Дед -> Я"))) {
                    EnemyEditor.enemies[1]->tpEnemy2Me = true;
                }
                if (ImGui::Button(ozSelectLanguage("Teleport Me to Slendrina", u8"Я -> Дед"))) {

                }
                ImGui::SameLine();
                if (ImGui::Button(ozSelectLanguage("Teleport Slendrina To Me", u8"Дед -> Я"))) {

                }
                ImGui::PopItemWidth();
                ImGui::Separator();
                ImGui::InputFloat3(ozSelectLanguage("Player Pos", u8"Координаты игрока"), playerPosTp);

                ImGui::Text("%s",ozSelectLanguage("Player X:", u8"Координата X:"));
                ImGui::SameLine();

                // Arrow buttons with Repeater


                ImGui::PushButtonRepeat(true);
                if (ImGui::ArrowButton("##left", ImGuiDir_Left)) {
                    playerPosTp[0]--;
                    teleportCrdCommit = true;
                }
                ImGui::SameLine(0.0f, spacing);
                if (ImGui::ArrowButton("##right", ImGuiDir_Right)) {
                    playerPosTp[0]++;
                    teleportCrdCommit = true;
                }

                ImGui::PopButtonRepeat();
                ImGui::SameLine();
                ImGui::Text("%f", playerPosTp[0]);


                ImGui::Text("%s",ozSelectLanguage("Player Y:", u8"Координата X:"));
                ImGui::SameLine();

                // Arrow buttons with Repeater


                ImGui::PushButtonRepeat(true);
                if (ImGui::ArrowButton("##left1", ImGuiDir_Left)) {
                    playerPosTp[1]--;
                    teleportCrdCommit = true;
                }
                ImGui::SameLine(0.0f, spacing);
                if (ImGui::ArrowButton("##right1", ImGuiDir_Right)) {
                    playerPosTp[1]++;
                    teleportCrdCommit = true;
                }

                ImGui::PopButtonRepeat();
                ImGui::SameLine();
                ImGui::Text("%f", playerPosTp[1]);

                ImGui::Text("%s",ozSelectLanguage("Player Z:", u8"Координата Z:"));
                ImGui::SameLine();

                // Arrow buttons with Repeater


                ImGui::PushButtonRepeat(true);
                if (ImGui::ArrowButton("##left2", ImGuiDir_Left)) {
                    playerPosTp[2]--;
                    teleportCrdCommit = true;
                }
                ImGui::SameLine(0.0f, spacing);
                if (ImGui::ArrowButton("##right2", ImGuiDir_Right)) {
                    playerPosTp[2]++;
                    teleportCrdCommit = true;
                }

                ImGui::PopButtonRepeat();
                ImGui::SameLine();
                ImGui::Text("%f", playerPosTp[2]);

                ImGui::Separator();

                if(ImGui::Combo(ozSelectLanguage("Places to Teleport", u8"Места для телепортации"), &placetoteleport, placelist, IM_ARRAYSIZE(placelist))){
                    teleportPlaceCommit = true;
                }

                break;
            case 3:
                if(ImGui::Combo(ozSelectLanguage("Items List", u8"Выбрать предмет для создания"), &ItemToSpawn, itemlist, IM_ARRAYSIZE(itemlist))){

                    ItemSpawnCommit = true;

                }
            break;
                case 4:
                       //ImGui::InputText(ozSelectLanguage("Custom scene", u8"Кастомная сцена"), &(sceneToLoad), 15);
                       //ImGui::PushItemWidth(-1.0f);
                      // if (ImGui::Button(ozSelectLanguage("Go to scene", u8"Перейти к сцене"))) {
                       //     LoadScene(new_il2cpp_string(sceneToLoad));
                      // }
                      // ImGui::PopItemWidth();
                      ImGui::Text("will put more stuff here soon");
                       ImGui::Separator();
                ImGui::PushItemWidth(-1.0f);
                if (ImGui::Button(ozSelectLanguage("Game Intro", u8"Интро"))) {
                    LoadScene(new_il2cpp_string("SplashScreen"));
                }

                if (ImGui::Button(ozSelectLanguage("Main Menu", u8"Главное меню"))) {
                    LoadScene(new_il2cpp_string("Menu"));
                }
                if (ImGui::Button(ozSelectLanguage("Game Scene", u8"Сцена игры"))) {
                    LoadScene(new_il2cpp_string("Scene"));
                }


                ImGui::PopItemWidth();

                break;
        }
    }
    if(currentCategory == 3){
        switch(i) {
            case 0:
                if(ImGui::Checkbox(ozSelectLanguage("Enable FOG", u8"Включить туман"), &fog)){
                    SetFOG(fog);
                }
                if(ImGui::SliderFloat(ozSelectLanguage("FOG Distance", u8"Радиус тумана"), &fogd, 0, 15)){
                       SetFOGDist(fogd);
                }
                break;
            case 1:
                 if(ImGui::Button(ozSelectLanguage("Set High Quality", u8"Высокое качество"))){
                     SetQualityLevel(1, true);
                 }

                if(ImGui::Button(ozSelectLanguage("Set Low Quality", u8"Низкое качество"))){
                    SetQualityLevel(0, true);
                }
                break;
            case 2:
                if(ImGui::Checkbox(ozSelectLanguage("Unlimited FPS", u8"Обойти ограничение FPS (2000 кадров)"), &fpsbypass)){
                        SetFrameRateLimit(fpsbypass ? 2000 : 30);
                   }
                ImGui::Separator();
                ImGui::Checkbox(ozSelectLanguage("Disable Frame Clear", u8"Отключить авто-удаление отрисовки"), &dontClear);
                ImGui::Checkbox(ozSelectLanguage("Slendrina Wireframe", u8"Включить вайр-фрейм для слендрины"), &chamsSlendrina);
                ImGui::Checkbox(ozSelectLanguage("Enable Wireframe", u8"Включить вайр-фрейм"), &wireframe);
                if(ImGui::SliderFloat(ozSelectLanguage("Wireframe Size", u8"Размер Вайр-фрейма"), &wfLW, 0, 5)){

                }
                ImGui::Separator();
                //ImGui::ColorEdit4("Wireframe color", nullptr);

                ImGui::Checkbox(ozSelectLanguage("Enable Color Filter", u8"Включить фильтрацию цвета"), &clrfilter);
                ImGui::Checkbox("R", &clrfilterR);
                ImGui::SameLine();
                ImGui::Checkbox("G", &clrfilterG);
                ImGui::SameLine();
                ImGui::Checkbox("B", &clrfilterB);
               // ImGui::ColorEdit4("Color Filter", nullptr);
                break;
        }
    }
    if(currentCategory == 4){
        switch(i){
            case 0:
                ImGui::Separator();
                ImGui::Checkbox(ozSelectLanguage("No one can attack me", u8"Никто меня не убьет"), &allCantAttack);
                if(!allCantAttack) {
                    ImGui::Checkbox(ozSelectLanguage("Granny Can't attack", u8"Бессмертие от бабки"), &grCantAttack);
                    ImGui::Checkbox(ozSelectLanguage("Grandpa Can't attack", u8"Бессмертие от деда"), &grpCantAttack);
                    ImGui::Checkbox(ozSelectLanguage("Crocodile Can't attack", u8"Бессмертие от крокодила"), &crCantAttack);
                    ImGui::Checkbox(ozSelectLanguage("Slendrina Can't attack", u8"Бессмертие от слендрины"), &slCantAttack);
                }
                ImGui::Separator();
                ImGui::Checkbox(ozSelectLanguage("No one can see me", u8"Все слепые"), &allInvis);
                if(!allInvis){
                    ImGui::Checkbox(ozSelectLanguage("Invisible from Granny", u8"Невидимость для бабки"), &grInvis);
                    ImGui::Checkbox(ozSelectLanguage("Invisible from Grandpa", u8"Невидимость для деда"), &grpInvis);
                    ImGui::Checkbox(ozSelectLanguage("Invisible from Crocodile", u8"Невидимость для крокодила"), &crInvis);
                    ImGui::Checkbox(ozSelectLanguage("Invisible from Robot", u8"Невидимость для робота в метро"), &slInvis);
                }

                break;
            case 1:

                 if(ImGui::Checkbox(("Disable Collisions"), &igncol)){
                     savePlayerY = true;
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
        drawList = ImGui::GetForegroundDrawList();
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
                    DrawESPElement(getDrawList(), ImVec2(ImGui::GetIO().DisplaySize.x / 2, ESP.lineOffsetY), me->name, me->color,
                                   me->screenPosition, ESP.boxBg,1, me->distance);
                }
            }
        }
    }
}
float menuSizeXMax = 1200, menuSizeY = 483;
ImVec4 strokeColor = ImVec4(1.0f, 0.0f, 0.0f, 1.0f);
//Render the menu
void Render(){
    RenderTicks++;
   // LOGD("Rendering ImGui. Render Ticks: %d Display Size: %f, %f", RenderTicks, screenWidth,screenHeight)
    ImGuiIO& io = ImGui::GetIO();

    if(!isALegalUse){
        //Creating A new ImGui Frame for our backend
        ImGui_ImplAndroidGLES2_NewFrame(ImGui::GetIO().DisplaySize.x, ImGui::GetIO().DisplaySize.y, currentTimeInMilliseconds());
        //rgb change border color
        ImGuiStyle* style = &ImGui::GetStyle();
        if(strokeColor.x < 1.0f){
            strokeColor.x+=0.01f;
        } else{
            if(strokeColor.y < 1.0f){
                strokeColor.y+=0.01f;
            } else {
                if(strokeColor.z < 1.0f){
                    strokeColor.z+=0.01f;
                } else {
                    strokeColor = ImVec4(1.0f, 0.0f, 0.0f, 1.0f);
                }
            }
        }
        style->Colors[ImGuiCol_Border] = strokeColor;
        ImGui::Begin(ozObfuscate("[System] Cannot Load Mod Menu"), nullptr);
        ImGui::Text("%s",(const char*)ozObfuscate("Invalid Modification Detected! Please use the original instance of APK."));
        if(isLGL){
            ImGui::Spacing();

            ImGui::Text("%s",(const char*)ozObfuscate(u8"ЛГЛ БОТЫ!!!"));

        }

        ImGui::End();
        ImGui::Render(); // Render ImGui
        ImGui_ImplAndroidGLES2_RenderDrawLists(ImGui::GetDrawData());
        ImGui::EndFrame(); //End ImGui Frame
        return;
    } else {
        if(ImGui::GetIO().DisplaySize.x < 2340 || ImGui::GetIO().DisplaySize.x < 1080){
            ImGui::GetStyle().ScaleAllSizes(ImGui::GetIO().DisplaySize.x * MULT_X * SCREEN_FRUSTUMM);
        } else {
            ImGui::GetStyle().ScaleAllSizes(3);
        }
        ImGui::GetStyle().WindowRounding = 10;
        ImGui::GetStyle().WindowBorderSize = 3;
        ImGui::GetStyle().FrameRounding = 14.0f;
        if(!ImGui::get_cmdExecutorHaveVal()){
            ImGui::set_cmdExecutor(&cmdExecuteSystem);
        }

    }
    //Creating A new ImGui Frame for our backend
    ImGui_ImplAndroidGLES2_NewFrame(ImGui::GetIO().DisplaySize.x, ImGui::GetIO().DisplaySize.y, currentTimeInMilliseconds());
    ImDrawList* drawList = getDrawList();
    //Example using text over screen
    drawList->AddText(ImGui::GetFont(), 30, ImVec2(100.f, 100.f), ImColor(255, 255, 0, 255), "Player Stats.", nullptr, 0.0f, nullptr);
    //Rendering ESP
    RenderESPSummary();
    //matrix selector
    if(ImGui::GetIO().DisplaySize.x <= 1360){
        menuSizeXMax = 1200-420;
        LOGD("420 %f",screenWidth)
    } else {
        if(ImGui::GetIO().DisplaySize.x <= 1440){
            LOGD("275 %f",screenWidth)
            menuSizeXMax = 1200-275;
        }
    }
    if(ImGui::GetIO().DisplaySize.x >= 2600){
        menuSizeXMax = 1200+300;
        menuSizeY = 483+150;
    }

    //Open animation
    if(ImGui::get_openAnimationState() < menuSizeXMax){

        ImGui::set_openAnimationState(ImGui::get_openAnimationState()+60);
    }
    if(ImGui::get_openAnimationState() > menuSizeXMax){
        ImGui::set_openAnimationState(menuSizeXMax);
    }
    //rgb change border color
    ImGuiStyle* style = &ImGui::GetStyle();
    if(strokeColor.x < 1.0f){
        strokeColor.x+=0.01f;
    } else{
        if(strokeColor.y < 1.0f){
            strokeColor.y+=0.01f;
        } else {
            if(strokeColor.z < 1.0f){
                strokeColor.z+=0.01f;
            } else {
                strokeColor = ImVec4(1.0f, 0.0f, 0.0f, 1.0f);
            }
        }
    }
    style->Colors[ImGuiCol_Border] = strokeColor;
    //Set Animation to window
    std::string wName = std::string( ozObfuscate("Mod Menu for ")) + (const char*)GAME_NAME + "(" + (const char*)GAME_VER + ")" + std::string(ozObfuscate(" - by ozMod"));

    if(currentLang == 1){
        wName = std::string( ozObfuscate("Мод-меню для игры ")) + (const char*)GAME_NAME + " (" + (const char*)GAME_VER + ")" + std::string(ozObfuscate(" - создал ozMod"));
    }
    ImGui::Begin( wName.c_str(), nullptr, ImGuiWindowFlags_MenuBar);
    ImGui::FindWindowByName(wName.c_str())->SizeFull.x = ImGui::get_openAnimationState();
    ImGui::FindWindowByName(wName.c_str())->SizeFull.y=menuSizeY;
    if (ImGui::BeginMenuBar())
    {
        if (ImGui::BeginMenu(ozSelectLanguage("Tools", u8"Инструменты")))
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
        if (ImGui::BeginMenu(ozSelectLanguage("Style Editor", u8"Персонализация")))
        {
            if (ImGui::MenuItem(ozSelectLanguage("Open Editor", u8"Открыть редактор"), ozObfuscate("Ctrl+O"))) {
                showStyleEditor = true;
            }
            if (ImGui::MenuItem(ozSelectLanguage("Close Editor", u8"Закрыть редактор"), ozObfuscate("Ctrl+O"))) {
                showStyleEditor = false;

            }
            if (ImGui::MenuItem(ozSelectLanguage("Reset All", u8"Сброс"), ozObfuscate("Ctrl+R")))  {
                 ImGui::StyleColorsClassic();
                 ozTheme();
            }
            ImGui::EndMenu();
        }
        if (ImGui::BeginMenu(ozSelectLanguage("Features", u8"Функции")))
        {
            if (ImGui::MenuItem("Test", "")) {

            }

            ImGui::EndMenu();
        }
        if (ImGui::BeginMenu(ozSelectLanguage("Help", u8"Помощь")))
        {
            if (ImGui::MenuItem(ozSelectLanguage("Help", u8"Помощь"), ("Ctrl+M"))) {

            }
            if (ImGui::MenuItem(ozObfuscate("About ImGui"), ("Ctrl+G"))) {

            }

            ImGui::EndMenu();
        }
        if (ImGui::BeginMenu(ozSelectLanguage("Language", u8"Язык")))
        {
            if (ImGui::MenuItem(ozSelectLanguage("Russian", u8"Русский"), ("Ctrl+M"))) {
                  currentLang = 1;
            }
            if (ImGui::MenuItem(ozSelectLanguage("English", u8"Английский"), ("Ctrl+G"))) {
                currentLang = 0;
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
    if(!showStyleEditor) {
        ImGui::Columns(2);
        ImGui::SetColumnOffset(1, 260);
        {
            //Left side

            static ImVec4 active = to_vec4(41, 40, 100, 255);
            static ImVec4 inactive = to_vec4(31, 30, 31, 255);

            ImGui::PushStyleColor(ImGuiCol_Button, currentCategory == 1 ? active : inactive);
            if (ImGui::Button(ozSelectLanguage("General", u8"Главное"), ImVec2(230 - 15, 81))) {
                currentCategory = 1;
            }


            ImGui::PushStyleColor(ImGuiCol_Button, currentCategory == 2 ? active : inactive);
            if (ImGui::Button(ozSelectLanguage("ESP", u8"ESP / ЕСП"), ImVec2(230 - 15, 81))) {
                currentCategory = 2;
            }


            ImGui::PushStyleColor(ImGuiCol_Button, currentCategory == 3 ? active : inactive);
            if (ImGui::Button(ozSelectLanguage("Unity", u8"Движок игры (Unity)"), ImVec2(230 - 15, 81)))
                currentCategory = 3;


            ImGui::PushStyleColor(ImGuiCol_Button, currentCategory == 4 ? active : inactive);
            if (ImGui::Button(ozSelectLanguage("Player", u8"Игрок"), ImVec2(230 - 15, 81)))
                currentCategory = 4;

            ImGui::PopStyleColor(4);

            ImGui::SetCursorPosY(ImGui::GetWindowHeight() - 30);
        }

        ImGui::NextColumn();

        //Right side
        {


            static ImGuiTextFilter filter;
            static std::vector<std::string> resources;
            if (currentCategory == 0) {

                resources =
                        {
                                std::string(ozSelectLanguage("Getting Started", u8"С чего начать")),
                                std::string(ozSelectLanguage("About Mod Menu", u8"Про модификацию")),
                                std::string(ozSelectLanguage("ImGui Demo Window", u8"Демонстрационное окно ImGui")),
                                std::string(ozSelectLanguage("Metrics and debugger", u8"Откладка и статистика")),

                        };

            }
            if (currentCategory == 1) {
                resources =
                        {
                                std::string(ozSelectLanguage("Enemy Control Panel", u8"Управление врагами")),
                                //std::string(ozSelectLanguage("Enemy Control Panel", u8"Управление врагами")), // camera menu
                                std::string(ozSelectLanguage("Object Editor", u8"Редактор объектов")),
                                std::string(ozSelectLanguage("Teleport Menu", u8"Меню телепортации")),
                                std::string(ozSelectLanguage("Item Spawner", u8"Спавнер предметов")),
                                std::string(ozSelectLanguage("Scene Manager", u8"Управление сценами"))
                        };
            }
            if (currentCategory == 2) {
                resources =
                        {
                                std::string(ozSelectLanguage("ESP Layer", u8"Слой ESP")),
                                std::string(ozSelectLanguage("ESP Style", u8"Внешний вид ESP")),
                                std::string(ozSelectLanguage("ESP Enemies", u8"Менеджер врагов ESP"))
                        };
            }
            if (currentCategory == 3) {
                resources =
                        {
                                std::string(ozSelectLanguage("Light", u8"Настройки света")),
                                std::string(ozSelectLanguage("Quality Settings", u8"Качество")),
                                std::string(ozSelectLanguage("Engine Core", u8"Движок игры"))
                        };
            }

            if (currentCategory == 4) {
                resources =
                        {
                                std::string(ozSelectLanguage("Anti-Enemy", u8"Бессмертие / Невидимость")),
                                std::string(ozSelectLanguage("Physics", u8"Физика")),
                                std::string(ozSelectLanguage("Camera", u8"Камера")),
                                std::string(ozSelectLanguage("Player Stats", u8"Статистика игрока на экране")),
                                std::string(ozSelectLanguage("No-Clip Settings", u8"Настройки полета (если он включен)")),
                        };
            }
            filter.Draw(ozSelectLanguage("Search", u8"Поиск"), 240);
            ImGui::SameLine();
            if (ImGui::Button(ozSelectLanguage("Clear", u8"Очистить"))) {
                filter.Clear();
            }
            if (ImGui::ListBoxHeader("##ResourcesList",
                                     ImVec2(ImGui::GetContentRegionAvail().x + 20,
                                            ImGui::GetContentRegionAvail().y))) {
                if (currentCategory == 3) {
                    if (ImGui::SliderFloat(ozSelectLanguage("Time Scale", u8"Управление временем"), &timescale, 0, 5) && set_timeScale) {
                        set_timeScale(timescale);

                    }
                }

                if (currentCategory == 2) {
                    ImGui::Checkbox(ozSelectLanguage("Enable ESP", u8"Включить ESP"), &ESP.ESP);
                }
                int i = 0;
                for (const auto &resource : resources) {
                    if (filter.PassFilter(resource.c_str())) {
                        if (ImGui::TreeNode(resource.c_str())) {
                            DrawContentFor(i);
                            ImGui::TreePop();
                        }
                    }
                    i++;
                }
                ImGui::ListBoxFooter();
            }

        }
    }

    ImGui::End();
    ImGui::Render(); // Render ImGui
    ImGui_ImplAndroidGLES2_RenderDrawLists(ImGui::GetDrawData());
    ImGui::EndFrame(); //End ImGui Frame
    ImGui::GetStyle().ResetAllSizes();
}



bool (*old_nativeInjectEvent )(JNIEnv*, jobject ,jobject event);
bool hook_nativeInjectEvent(JNIEnv* env, jobject instance,jobject event){
        jclass tchcls = env->FindClass(ozObfuscate("android/view/MotionEvent"));
        if(!tchcls){
            LOGI("Can't find MotionEvent!") //на логах можно ; не ставить :)
        }

        jclass kycls = env->FindClass(ozObfuscate("android/view/KeyEvent"));
         if(!kycls){
            LOGI("Can't find KeyEvent!")
         }
        jmethodID ga = env->GetMethodID(kycls, ozObfuscate("getAction"),
                                    ozObfuscate("()I"));
        if(!ga){
            LOGI("Can't find KeyEvent.getAction!")
         }
        if(env->IsInstanceOf(event, kycls) && env->CallIntMethod(event, ga) == 0) {
            LOGI("Key Event Found!")
            jmethodID gkc = env->GetMethodID(kycls, ozObfuscate("getKeyCode"), ozObfuscate("()I"));
            if(!gkc){
                LOGI("Can't find KeyEvent.getKeyCode!")
            }
            jmethodID guc = env->GetMethodID(kycls, ozObfuscate("getUnicodeChar"), ozObfuscate("(I)I"));
            if(!guc){
                LOGI("Can't find KeyEvent.getUnicodeChar!")
            }
            jmethodID gms = env->GetMethodID(kycls, ozObfuscate("getMetaState"), ozObfuscate("()I")); //I need it for ctrl, alt, etc.
            if(!gms){
                LOGI("Can't find KeyEvent.getMetaState!")
            }
            int keyCode = env->CallIntMethod(event, gkc);
            ImGuiIO& io = ImGui::GetIO();
            switch (keyCode) {
                case 19:
                    LOGI("Key Code: 19")
                    ImGui::GetIO().AddInputCharacter(keyCode);
                    break;
                case 20:
                       LOGI("Key Code: 20")
                    ImGui::GetIO().AddInputCharacter(keyCode);
                    break;
                case 21:
                    LOGI("Key Code: 21")
                    ImGui::GetIO().AddInputCharacter(keyCode);
                    break;
                case 22:
                    LOGI("Key Code: 22")
                    ImGui::GetIO().AddInputCharacter(keyCode);
                    break;
                case 61:
                    LOGI("Key Code: 61")
                    ImGui::GetIO().AddInputCharacter(keyCode);
                    break;
                case 66:
                    LOGI("Key Code: Enter")
                    ImGui::GetIO().AddInputCharacter(keyCode);
                    break;
                case 67:
                    LOGI("Key Code: Del")
                    ImGui::GetIO().AddInputCharacter(keyCode);
                    break;
                case 92:
                    LOGI("Key Code: 92")
                    ImGui::GetIO().AddInputCharacter(keyCode);
                    break;
                case 93:
                    LOGI("Key Code: 93")
                    ImGui::GetIO().AddInputCharacter(keyCode);
                    break;
                case 111:
                    LOGI("Key Code: 111")
                    ImGui::GetIO().AddInputCharacter(keyCode);
                    break;
                case 112:
                    LOGI("Key Code: 112")
                    ImGui::GetIO().AddInputCharacter(keyCode);
                    break;
                case 122:
                    LOGI("Key Code: 122")
                    ImGui::GetIO().AddInputCharacter(keyCode);
                    break;
                case 123:
                    LOGI("Key Code: 123")
                    ImGui::GetIO().AddInputCharacter(keyCode);
                    break;
                case 124:
                    LOGI("Key Code: 124")
                    ImGui::GetIO().AddInputCharacter(keyCode);
                    break;
                default:
                    LOGI("Key Code: Other")
                     ImGui::GetIO().AddInputCharacter(env->CallIntMethod(event, guc, env->CallIntMethod(event, gms)));
                    break;
            }
            return old_nativeInjectEvent(env, instance, event);
        }
         LOGD("Processing Touch Event! (not sure)")
        if(!env->IsInstanceOf(event, tchcls)){
            return old_nativeInjectEvent(env, instance, event);
        }
        LOGD("Processing Touch Event!")
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

void ProtectJavaClasses(JNIEnv*env, jobject obj);
void SearchActivity (JNIEnv * globalEnv){
    jclass prcls = globalEnv->FindClass("com/unity3d/player/UnityPlayer");
    if(prcls != nullptr){
        LOGW("Soft Input: UnityPlayer.java is found")
        jfieldID UnityPlayer_CurrentActivity_fid2 = globalEnv->GetStaticFieldID(prcls, "currentActivity","Landroid/app/Activity;");
        if(UnityPlayer_CurrentActivity_fid2 != nullptr){
            LOGW("Soft Input: UnityPlayer.currentActivity is found")
            jobject resultObject=globalEnv->GetStaticObjectField(prcls, UnityPlayer_CurrentActivity_fid2);
            if(resultObject!=nullptr){
                LOGW("Soft Input: Get Current Activity Success!")
                UnityPlayer_CurrentActivity_fid = globalEnv->NewGlobalRef(resultObject);
                //some protection soon maybe xd
                ProtectJavaClasses(globalEnv, UnityPlayer_CurrentActivity_fid);
            }
        }
    }
}
jint (*old_RegisterNatives )(JNIEnv*, jclass, JNINativeMethod*,jint);
jint hook_RegisterNatives(JNIEnv* env, jclass destinationClass, JNINativeMethod* methods,
                          jint totalMethodCount){

    int currentNativeMethodNumeration;
    for (currentNativeMethodNumeration = 0; currentNativeMethodNumeration < totalMethodCount; ++currentNativeMethodNumeration )
    {
        if (!strcmp(methods[currentNativeMethodNumeration].name, ozObfuscate("nativeInjectEvent")) ){
            DobbyHook(methods[currentNativeMethodNumeration].fnPtr, (void*)hook_nativeInjectEvent, (void **)&old_nativeInjectEvent);
        }
    }
    SearchActivity(env);
    return old_RegisterNatives(env, destinationClass, methods, totalMethodCount);
}

int32_t (*old_ANativeWindow_getWidth)(ANativeWindow* window);
int32_t new_ANativeWindow_getWidth(ANativeWindow* window){
    int32_t returnValue = old_ANativeWindow_getWidth(window);
    LOGI("Recieved Display Width: %d", returnValue)
    screenWidth = returnValue;
    ImGui::GetIO().DisplaySize.x = screenWidth;
    return returnValue;
}

int32_t (*old_ANativeWindow_getHeight)(ANativeWindow* window);
int32_t new_ANativeWindow_getHeight(ANativeWindow* window) {
    int32_t returnValue = old_ANativeWindow_getHeight(window);
    LOGI("Recieved Display Height: %d", returnValue)
    screenHeight = returnValue;
    ImGui::GetIO().DisplaySize.y = screenHeight;
    return returnValue;
}

void StartBackend(JNIEnv* env){
    //Setup Start time
    startTime = currentTimeInMilliseconds();
    //Default Display Size
    screenWidth = 2340;
    screenHeight = 1080;

    //Setup
    setupGraphics(screenWidth, screenHeight);

    //код рендера в другом файле. если поместить его здесь то будет краш

    //Display size
    DobbyHook((void*)ANativeWindow_getWidth, (void*)new_ANativeWindow_getWidth, (void **)&old_ANativeWindow_getWidth);
    DobbyHook((void*)ANativeWindow_getHeight, (void*)new_ANativeWindow_getHeight, (void **)&old_ANativeWindow_getHeight);

    //Input
    DobbyHook((void*)env->functions->RegisterNatives, (void*)hook_RegisterNatives, (void **)&old_RegisterNatives);

}

/*
jclass (*oFindClassHook)(JNIEnv* env, const char* name);
jclass FindClassHook(JNIEnv *env, const char*name)
{
    bool infected = false;

    if(std::string(name).find(ozObfuscate("uk/lgl")) != std::string::npos){
        infected = true;
    }
    if(std::string(name).find(ozObfuscate("com/android/support/Preferences")) != std::string::npos){
        infected = true;
    }
    if(std::string(name).find(ozObfuscate("com/android/support/Menu")) != std::string::npos){
        infected = true;
    }
    if(std::string(name).find(ozObfuscate("FloatingModMenuService")) != std::string::npos){
        infected = true;
    }
    if(std::string(name).find(ozObfuscate("keysystem")) != std::string::npos){
        infected = true;
    }
    if(std::string(name).find(ozObfuscate("tdm/ahz")) != std::string::npos){
        infected = true;
    }

if(infected){
infected = false;
LOGF("Unused branch found!!")
exit(0);
return nullptr;
}
return oFindClassHook(env, name);
}
 */
void ProtectJavaClasses(JNIEnv *env, jobject obj){
    LOGI("Protecting Classes..")

    jclass cls = env->GetObjectClass(obj);
    jmethodID mid = env->GetMethodID(cls,ozObfuscate("getPackageManager") ,ozObfuscate("()Landroid/content/pm/PackageManager;"));
    jobject packageManager = env->CallObjectMethod(obj, mid);

    // this.getPackageName()
    mid = env->GetMethodID(cls, ozObfuscate("getPackageName"), ozObfuscate("()Ljava/lang/String;"));//
    jstring packageName = (jstring) env->CallObjectMethod(obj, mid);

    // packageManager->getPackageInfo(packageName, GET_SIGNATURES);
    cls = env->GetObjectClass(packageManager);
    mid = env->GetMethodID(cls, ozObfuscate("getPackageInfo"), ozObfuscate("(Ljava/lang/String;I)Landroid/content/pm/PackageInfo;"));
    jint flags = 0x00000040;
    jobject packageInfo = env->CallObjectMethod(packageManager, mid, packageName, flags);
    // packageInfo->signatures
    cls = env->GetObjectClass(packageInfo);
    jfieldID fid = env->GetFieldID( cls, ozObfuscate("signatures"), ozObfuscate("[Landroid/content/pm/Signature;"));
    jobject signatures = env->GetObjectField( packageInfo, fid);
    // signatures[0]
    jobject signature = env->GetObjectArrayElement((jobjectArray)(signatures), 0);
    // signature->toByteArray()
    cls = env->GetObjectClass(signature);
    mid = env->GetMethodID(cls, ozObfuscate("toByteArray"), ozObfuscate("()[B"));
    jbyteArray appSig = (jbyteArray)env->CallObjectMethod(signature, mid);
    // X509Certificate appCertificate = X509Certificate.getInstance(appSignature.toByteArray());
    cls = env->FindClass(ozObfuscate("javax/security/cert/X509Certificate" ));
    if(cls !=nullptr) {
        jmethodID mid_static = env->GetStaticMethodID( cls, ozObfuscate("getInstance"), ozObfuscate("([B)Ljavax/security/cert/X509Certificate;"));
        if (mid_static != nullptr) {
            jobject  cerObj = (jstring) env->CallStaticObjectMethod(cls, mid_static,appSig );//
            jclass tmpCls = env->GetObjectClass(cerObj);
            jmethodID mid = env->GetMethodID(tmpCls, ozObfuscate("getIssuerDN"),ozObfuscate("()Ljava/security/Principal;"));
            jobject tmpObj = env->CallObjectMethod(cerObj, mid);
            cls = env->GetObjectClass(tmpObj);
            jmethodID mid2 = env->GetMethodID(cls, ozObfuscate("toString"), ozObfuscate("()Ljava/lang/String;"));
            jstring ow = (jstring) env->CallObjectMethod(tmpObj, mid2);//get app Owner
            const char *tmpOW = env->GetStringUTFChars(ow, nullptr);
            const char * validSig = ozObfuscate("CN=Resystem Team, OU=Resystem Team, O=Resystem Team, L=SH, ST=SH, C=CN");
            if(!strcmp(tmpOW, validSig)){
                LOGD("Frida-Agent: Validation script loaded")
            } else {
                isALegalUse = false;
                LOGE("Unable to find file '/patch/app/69/validation/branch_info.asm'! ")
            }
        }
    }


}
extern "C"
JNIEXPORT jint JNICALL
JNI_OnLoad(JavaVM *vm, void *reserved) {
    LOGI("JNI_OnLoad")
    //setup jni
    publicJVM = vm;
    JNIEnv *globalEnv;
    publicJVM->GetEnv((void **) &globalEnv, JNI_VERSION_1_6);

    //setup our imgui backend
    StartBackend(globalEnv);

    //init mods hooks
    InitMods();
    return JNI_VERSION_1_6;
}
