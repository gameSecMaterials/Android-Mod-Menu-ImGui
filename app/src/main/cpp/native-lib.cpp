#include "obfuscator.hpp"
#include <GLES2/gl2.h>
#include "EGL/egl.h"
#include <android/asset_manager_jni.h>
#include "admu_ut.h"
// Data
static EGLDisplay           g_EglDisplay = EGL_NO_DISPLAY;
static EGLSurface           g_EglSurface = EGL_NO_SURFACE;
static EGLContext           g_EglContext = EGL_NO_CONTEXT;
static char                 g_LogTag[] = "ozMod_egl";
void Render();
long long currentTimeInMilliseconds();
bool setupGraphics(int w, int h);
int startTime, screenWidth, screenHeight;
int currentLang = 1;
const char* reqToastText;
bool toastRequested = false;
void RequestToast(const char* input){
    toastRequested = true;
    reqToastText = input;
}

#include <string>
#include <android/input.h>
#include <android/log.h>
#define LOGI(...) ((void)__android_log_print(ANDROID_LOG_INFO,"ozMod", __VA_ARGS__))
#define  LOGE(...)  __android_log_print(ANDROID_LOG_ERROR,"ozMod",__VA_ARGS__)
static void checkGlError(const char* op) {
    for (GLint error = glGetError(); error; error
                                                    = glGetError()) {
        LOGI("after %s() glError (0x%x)\n", op, error);
    }
}
bool HandleInputEvent(JNIEnv *jenv,
int motionEvent, int x, int y,
int p);
#include <vector>

#include "Substrate/CydiaSubstrate.h"
#include "imgui_impl_android.h"
#include "Mods.h"
#include "ModMenuSurface.h"
static jobject getGlobalContext(JNIEnv *env)
{

    jclass activityThread = env->FindClass("android/app/ActivityThread");
    jmethodID currentActivityThread = env->GetStaticMethodID(activityThread, "currentActivityThread", "()Landroid/app/ActivityThread;");
    jobject at = env->CallStaticObjectMethod(activityThread, currentActivityThread);

    jmethodID getApplication = env->GetMethodID(activityThread, "getApplication", "()Landroid/app/Application;");
    jobject context = env->CallObjectMethod(at, getApplication);
    return context;
}

jobject activity, myAssets;
class ImDrawData;

 void ImGui_ImplAndroidGLES2_RenderDrawLists(ImDrawData*data);

#include "GLES3/gl3.h"


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
    // TODO: implement HandleInputEvent()
    // TODO: Synchronization issue, potentially drop events

    // Need to make sure the coordinate (x,y) here is relative to the top left of the surface
    // This is how the behavior of GLSurfaceView.onTouchEvent
    // If the coordinate is obtained in a different way. This should be adjusted accordingly
    if (motionEvent < TOUCH_ACTION_COUNT) {
        float velocity_y = (float)((float)y - g_LastTouchEvent.y) / 100.f;
        g_LastTouchEvent = {.action = (TOUCH_ACTION) motionEvent, .x = static_cast<float>(x), .y = static_cast<float>(y), .pointers = p, .y_velocity = velocity_y};
    }
    else {
        LOGE("invalid action index: %d", motionEvent);
    }

    LOGI("x: %d y: %d", x, y);
    return true;
}



long long currentTimeInMilliseconds()
{
    struct timeval tv;
    gettimeofday(&tv, nullptr);
    return ((tv.tv_sec * 1000) + (tv.tv_usec / 1000));
}
bool setupGraphics(int width, int height);



ImVec4 imClearColor;
bool showTestWindow;
bool showAnotherWindow;

static void printGLString(const char *name, GLenum s) {
    const char *v = (const char *) glGetString(s);
    LOGI("GL %s = %s\n", name, v);
}
GLuint gProgram;
GLuint gvPositionHandle;
GLuint vbo;
const GLfloat gTriangleVertices[] = { 0.0f, 1.0f, -1.0f, -1.0f,
                                      1.0f, -1.0f };

auto gVertexShader =
        "attribute vec2 vPosition;\n"
        "void main() {\n"
        "  gl_Position = vec4(vPosition.x, vPosition.y, 0.0, 1.0);\n"
        "}\n";

auto gFragmentShader =
        "precision mediump float;\n"
        "void main() {\n"
        "  gl_FragColor = vec4(0.0, 1.0, 0.0, 1.0);\n"
        "}\n";

GLuint loadShader(GLenum shaderType, const char* pSource) {
    GLuint shader = glCreateShader(shaderType);
    if (shader) {
        glShaderSource(shader, 1, &pSource, nullptr);
        glCompileShader(shader);
        GLint compiled = 0;
        glGetShaderiv(shader, GL_COMPILE_STATUS, &compiled);
        if (!compiled) {
            GLint infoLen = 0;
            glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &infoLen);
            if (infoLen) {
                char* buf = (char*) malloc(infoLen);
                if (buf) {
                    glGetShaderInfoLog(shader, infoLen, nullptr, buf);
                    LOGE("Could not compile shader %d:\n%s\n",
                         shaderType, buf);
                    free(buf);
                }
                glDeleteShader(shader);
                shader = 0;
            }
        }
    }
    return shader;
}

GLuint createProgram(const char* pVertexSource, const char* pFragmentSource) {
    GLuint vertexShader = loadShader(GL_VERTEX_SHADER, pVertexSource);
    if (!vertexShader) {
        return 0;
    }

    GLuint pixelShader = loadShader(GL_FRAGMENT_SHADER, pFragmentSource);
    if (!pixelShader) {
        return 0;
    }

    GLuint program = glCreateProgram();
    if (program) {
        glAttachShader(program, vertexShader);
        checkGlError("glAttachShader");
        glAttachShader(program, pixelShader);
        checkGlError("glAttachShader");
        glLinkProgram(program);
        GLint linkStatus = GL_FALSE;
        glGetProgramiv(program, GL_LINK_STATUS, &linkStatus);
        if (linkStatus != GL_TRUE) {
            GLint bufLength = 0;
            glGetProgramiv(program, GL_INFO_LOG_LENGTH, &bufLength);
            if (bufLength) {
                char* buf = (char*) malloc(bufLength);
                if (buf) {
                    glGetProgramInfoLog(program, bufLength, nullptr, buf);
                    LOGE("Could not link program:\n%s\n", buf);
                    free(buf);
                }
            }
            glDeleteProgram(program);
            program = 0;
        }
    }
    return program;
}
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
int ShowSoftKeyboardInput(JNIEnv*java_env){


    jclass native_activity_clazz = java_env->FindClass("androidx/viewbinding/systemui/SystemOverlaySurface");
    if (native_activity_clazz == nullptr){
        LOGE("cant get unity");
        return -3;
    }


    jmethodID method_id = java_env->GetStaticMethodID(native_activity_clazz, "ShowKeyboard", "()V");
    if (method_id == nullptr){
        LOGE("cant get show  keybord");
        return -4;
    }

     LOGI("I showing it...");
    java_env->CallStaticVoidMethod(native_activity_clazz, method_id);



    return 0;
}

// Unfortunately, the native KeyEvent implementation has no getUnicodeChar() function.
// Therefore, we implement the processing of KeyEvents in MainActivity.kt and poll
// the resulting Unicode characters here via JNI and send them to Dear ImGui.
static int UpdateKeyboardInput(JNIEnv*java_env)
{
    // Send the actual characters to Dear ImGui
    ImGuiIO& io = ImGui::GetIO();

    if (Surface.inputChar != 0){
        io.AddInputCharacter(Surface.inputChar);
        Surface.inputChar = 0;
    }
    return 0;
}
// Helper to retrieve data placed into the assets/ directory (android/app/src/main/assets)
static int GetAssetData(JNIEnv*env, const char* filename, void** outData)
{
    int num_bytes = 0;

    AAsset* asset_descriptor = AAssetManager_open(AAssetManager_fromJava(env, myAssets), filename, AASSET_MODE_BUFFER);
    if (asset_descriptor)
    {
        num_bytes = AAsset_getLength(asset_descriptor);
        *outData = IM_ALLOC(num_bytes);
        int64_t num_bytes_read = AAsset_read(asset_descriptor, *outData, num_bytes);
        AAsset_close(asset_descriptor);
        IM_ASSERT(num_bytes_read == num_bytes);
    }
    return num_bytes;
}
#include "tabs.h"
void logEgl()// Initialize EGL
// This is mostly boilerplate code for EGL...
{
    g_EglDisplay = eglGetDisplay(EGL_DEFAULT_DISPLAY);
    if (g_EglDisplay == EGL_NO_DISPLAY){
        __android_log_print(ANDROID_LOG_ERROR, "ozMod_egl", "%s", "GL State: eglGetDisplay == NULL");

    } else {
        __android_log_print(ANDROID_LOG_VERBOSE, "ozMod_egl", "%s", "GL State: eglGetDisplay not NULL");
    }


    if (eglInitialize(g_EglDisplay, nullptr, 0) != EGL_TRUE){
        __android_log_print(ANDROID_LOG_ERROR, "ozMod_egl", "%s", "GL State: eglInitialize is not fine :(");

    } else {
        __android_log_print(ANDROID_LOG_VERBOSE, "ozMod_egl", "%s", "GL State: eglInitialize fine");
    }

    const EGLint egl_attributes[] = { EGL_BLUE_SIZE, 8, EGL_GREEN_SIZE, 8, EGL_RED_SIZE, 8, EGL_DEPTH_SIZE, 24, EGL_SURFACE_TYPE, EGL_WINDOW_BIT, EGL_NONE };
    EGLint num_configs = 0;
    if (eglChooseConfig(g_EglDisplay, egl_attributes, nullptr, 0, &num_configs) != EGL_TRUE){
        __android_log_print(ANDROID_LOG_ERROR, "ozMod_egl", "%s", "GL State: eglChooseConfig");

    } else {
        __android_log_print(ANDROID_LOG_VERBOSE, "ozMod_egl", "%s", "GL State: eglChooseConfig");
    }
    // Get the first matching config
    EGLConfig egl_config;
    eglChooseConfig(g_EglDisplay, egl_attributes, &egl_config, 1, &num_configs);
    EGLint egl_format;
    eglGetConfigAttrib(g_EglDisplay, egl_config, EGL_NATIVE_VISUAL_ID, &egl_format);

    const EGLint egl_context_attributes[] = { EGL_CONTEXT_CLIENT_VERSION, 3, EGL_NONE };
    g_EglContext = eglCreateContext(g_EglDisplay, egl_config, EGL_NO_CONTEXT, egl_context_attributes);

    if (g_EglContext == EGL_NO_CONTEXT){
        __android_log_print(ANDROID_LOG_ERROR, "ozMod_egl", "%s", "GL State: EGL Context");

    } else {
        __android_log_print(ANDROID_LOG_VERBOSE, "ozMod_egl", "%s", "GL State: EGL Context");
    }
    // g_EglSurface = eglCreateWindowSurface(g_EglDisplay, egl_config, g_App->window, NULL);
    // eglMakeCurrent(g_EglDisplay, g_EglSurface, g_EglSurface, g_EglContext);
}

bool setupGraphics(int w, int h) {

    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGui::StyleColorsDark();
    // Disable loading/saving of .ini file from disk.
    // FIXME: Consider using LoadIniSettingsFromMemory() / SaveIniSettingsToMemory() to save in appropriate location for Android.
    ozTheme();
    ImGui_ImplAndroidGLES2_Init();
    ImGuiIO& io = ImGui::GetIO();
    io.IniFilename =    nullptr;
    ImGui::GetStyle().WindowRounding = 10;
  //  ImGui::GetStyle().ItemSpacing = ImVec2(10,10);
    ImGui::GetStyle().Alpha = 1;
    ImGui::GetStyle().WindowMinSize = ImVec2(200,200);
    io.BackendPlatformName = "imgui_impl_android";

    // Keyboard mapping. Dear ImGui will use those indices to peek into the io.KeysDown[] array.
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

    imClearColor = ImColor(114, 144, 154);
    showTestWindow = false;
    showAnotherWindow = false;
    //glViewport(0, 0, w, h);
    printGLString("Version", GL_VERSION);
    printGLString("Vendor", GL_VENDOR);
    printGLString("Renderer", GL_RENDERER);
    printGLString("Extensions", GL_EXTENSIONS);

    LOGI("setupGraphics(%d, %d)", w, h);
    // We load the default font with increased size to improve readability on many devices with "high" DPI.
    // FIXME: Put some effort into DPI awareness.
    // Important: when calling AddFontFromMemoryTTF(), ownership of font_data is transfered by Dear ImGui by default (deleted is handled by Dear ImGui), unless we set FontDataOwnedByAtlas=false in ImFontConfig

    io.IniFilename =    nullptr;
    ImGui::GetStyle().WindowRounding = 10;
    //  ImGui::GetStyle().ItemSpacing = ImVec2(10,10);
    ImGui::GetStyle().Alpha = 1;
    ImGui::GetStyle().WindowMinSize = ImVec2(200,200);
    io.BackendPlatformName = "imgui_impl_android";
    ImGui::GetStyle().ScaleAllSizes(3);
    ImFontConfig font_cfg;
    font_cfg.SizePixels = 22.0f;
    font_cfg.GlyphRanges = io.Fonts->GetGlyphRangesCyrillic();
    io.Fonts->AddFontDefault(&font_cfg);
    /*
    gProgram = createProgram(gVertexShader, gFragmentShader);
    if (!gProgram) {
        LOGE("Could not create program.");
        return false;
    }
    gvPositionHandle = glGetAttribLocation(gProgram, "vPosition");
    checkGlError("glGetAttribLocation");
    LOGI("glGetAttribLocation(\"vPosition\") = %d\n",
         gvPositionHandle);
    */

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


float getx()
{
    return ImGui::GetContentRegionAvail().x;
}

float gety()
{
    return ImGui::GetContentRegionAvail().y;
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

void linevertical()
{
    ImGui::SameLine();
    ImGui::SeparatorEx(ImGuiSeparatorFlags_Vertical);
    ImGui::SameLine();
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
                ImGui::Text("%s", (const char *)ozObfuscate("Hi! This Mod Menu have many categories and features."));
                ImGui::PopStyleColor();
                ImGui::PushStyleColor(ImGuiCol_Text, IM_COL32(0, 255, 0, 255));
                ImGui::Text("%s", (const char *)ozObfuscate(" Just explore and enjoy!"));
                ImGui::PopStyleColor();
                break;
            case 1:
                ImGui::SetWindowFontScale(0.8);
                ImGui::Text("%s", (const char *)ozObfuscate("Creator: ozMod"));
                ImGui::Text("%s", (const char *)ozObfuscate(" Helper: FCS Leader"));
                ImGui::PushStyleColor(ImGuiCol_Text, IM_COL32(255, 0, 0, 255));
                ImGui::Text("%s", (const char *)ozObfuscate("  Also thanks to:"));
                ImGui::PopStyleColor();

                ImGui::PushStyleColor(ImGuiCol_Text, IM_COL32(0, 255, 0, 255));
                ImGui::Text("%s", (const char *)ozObfuscate("    Ciber Hacker & geokar2006 - for code ideas and ByNameModding"));
                ImGui::PopStyleColor();
                ImGui::PushStyleColor(ImGuiCol_Text, IM_COL32(255, 165, 0, 255));
                ImGui::Text("%s", (const char *)ozObfuscate("    Omar Ocornut - for ImGui (GUI Realization)"));
                      ImGui::PopStyleColor();
                ImGui::PushStyleColor(ImGuiCol_Text, IM_COL32(255, 50, 65, 255));
                ImGui::Text("%s", (const char *)ozObfuscate("    adamyaxley - for Protection and obfuscation (FCS Leader also)"));
                ImGui::PopStyleColor();
                ImGui::PushStyleColor(ImGuiCol_Text, IM_COL32(0, 0, 255, 255));
                ImGui::Text("%s", (const char *)ozObfuscate("    Oifox or Octowolve (IDK) - for OpenGL hooking"));
                ImGui::PopStyleColor();
                ImGui::SetWindowFontScale(1);
                break;
            case 2:
                ImGui::ShowDemoWindow();
                break;
            case 3:
                ImGuiIO& io = ImGui::GetIO();
                ImGui::Separator();
                ImGui::Text(ozObfuscate("Dear ImGui Version: %s"), ImGui::GetVersion());
                ImGui::Text(ozObfuscate("Mod Menu FPS: %d (%f Frames)"), (int)io.Framerate, 1000 / io.Framerate);
                  ImGui::Separator();
                ImGui::Text(ozObfuscate("Game Name: %s"), GAME_NAME);
                ImGui::Text(ozObfuscate("Game Version: %s"), GAME_VER);
                ImGui::Separator();
                ImGui::PushStyleColor(ImGuiCol_Text, IM_COL32(255, 0, 0, 255));
                ImGui::Text(ozObfuscate("Dear ImGui Version: %s. Modded by ozMod."), IMGUI_VERSION);
                ImGui::Text("%s", (const char *)ozObfuscate("Using Android Implementation For ImGui In Unity Player."));
                ImGui::PopStyleColor();
                break;
        }
    }

    if(Settings::Tab == 2){
        switch(i) {
            case 0:
                ImGui::Checkbox(ozObfuscate("Draw ESP Behind Mod Menu"), &ESP.drawBehindMenu);
                break;
            case 1:
                ImGui::Checkbox(ozObfuscate("Enable Box"), &ESP.Boxes);
                ImGui::Checkbox(ozObfuscate("Enable Line"), &ESP.Lines);
                ImGui::Checkbox(ozObfuscate("Enable Text"), &ESP.Texts);
                ImGui::Separator();
                ImGui::Checkbox(ozObfuscate("Enable Box Background"), &ESP.boxBg);
                ImGui::SliderFloat(ozObfuscate("Boxes Sizes"), &ESP.boxSize, 1, 3);
                ImGui::Separator();
                ImGui::SliderFloat(ozObfuscate("Line Offset Y"), &ESP.lineOffsetY, 0, screenHeight);
                ImGui::Separator();
                ImGui::SliderFloat(ozObfuscate("Text Size"), &ESP.textSize, 20, 50);
                break;
            case 2:
                const char* items[] = { ozObfuscate("Granny"), ozObfuscate("Grandpa"), ozObfuscate("Slendrina"), ozObfuscate("Monster") };

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
                        ozObfuscate("Granny"),
                        ozObfuscate("Grandpa"),
                        ozObfuscate("Slendrina"),
                        ozObfuscate("Monster")
                };

                if(ImGui::Combo(ozObfuscate("Select Enemy"), &selectedEnemyGame, items, IM_ARRAYSIZE(items))){
                    selectedEnemyObj = EnemyEditor.enemies[selectedEnemyGame];
                }
               if(!selectedEnemyObj->destroyed){
                   if (ImGui::Button(ozObfuscate("Set Active"), ImVec2(260 - 15, 81))){
                      if(selectedEnemyObj){
                          selectedEnemyObj->active = !selectedEnemyObj->active;
                          selectedEnemyObj->setActive = true;
                      }
                   }
                }

                ImGui::SameLine();
                if (ImGui::Button(ozObfuscate("Destroy"), ImVec2(260 - 15, 81))){
                    if(selectedEnemyObj && !selectedEnemyObj->destroyed){
                        selectedEnemyObj->destroy = true;
                        selectedEnemyObj->destroyed = true;
                    }
                }
                if(!selectedEnemyObj->destroyed) {
                    ImGui::SameLine();
                    if (ImGui::Button(ozObfuscate("Play As Enemy"), ImVec2(260 - 15, 81))) {
                        if (selectedEnemyObj) {
                            selectedEnemyObj->playAs = true;
                        }
                    }
                    if (ImGui::Button(ozObfuscate("TP Me to Enemy"), ImVec2(260 - 15, 81))) {
                        if (selectedEnemyObj) {
                            selectedEnemyObj->tpMe2Enemy = true;
                        }
                    }
                    ImGui::SameLine();
                    if (ImGui::Button(ozObfuscate("TP Enemy to Me"), ImVec2(260 - 15, 81))) {
                        if (selectedEnemyObj) {
                            selectedEnemyObj->tpEnemy2Me = true;
                        }
                    }
                    ImGui::SameLine();
                    if (ImGui::Button(ozObfuscate("Enemy Kill Me"), ImVec2(260 - 15, 81))) {
                        if (selectedEnemyObj) {
                            selectedEnemyObj->enemyKillPlayer = true;
                        }
                    }
                    if (ImGui::Button(ozObfuscate("Set Width"), ImVec2(260 - 15, 81))) {
                       //unimplemented
                    }
                    ImGui::SameLine();
                    if (ImGui::Button(ozObfuscate("Set Height"), ImVec2(260 - 15, 81))) {
                        //unimplemented
                    }
                    ImGui::SameLine();
                    if (ImGui::Button(ozObfuscate("Set Size"), ImVec2(260 - 15, 81))) {
                        //unimplemented
                    }
                    if (ImGui::Button(ozObfuscate("God Mode"), ImVec2(260 - 15, 81))) {
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
                    if (ImGui::Button(ozObfuscate("Invisible"), ImVec2(260 - 15, 81))) {
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
                    if (ImGui::Button(ozObfuscate("Attack Distance"), ImVec2(260 - 15, 81))) {

                    }
                }
                break;
        }
    }
    if(Settings::Tab == 3){
        switch(i) {
            case 0:
                ImGui::Checkbox(ozObfuscate("Enable FOG"), &fog);
                if(ImGui::SliderFloat(ozObfuscate("Fog Distance"), &fogd, 0, 15)){

                }
                break;
            case 1:
                 if(ImGui::Button(ozObfuscate("Set High Quality"))){
                     SetQualityLevel(1, true);
                 }

                if(ImGui::Button(ozObfuscate("Set Low Quality"))){
                    SetQualityLevel(0, true);
                }
                break;
            case 2:
                ImGui::Checkbox(ozObfuscate("Bypass FPS Limit"), &fpsbypass);
                ImGui::Separator();
                ImGui::Checkbox(ozObfuscate("Slendrina Wireframe"), &chamsSlendrina);
                ImGui::Checkbox(ozObfuscate("Enable Wireframe"), &wireframe);
                if(ImGui::SliderFloat(ozObfuscate("Wireframe size"), &wfLW, 0, 5)){

                }

                ImGui::Separator();
                //ImGui::ColorEdit4("Wireframe color", nullptr);
                ImGui::Checkbox(ozObfuscate("Enable Color Filter"), &clrfilter);
               // ImGui::ColorEdit4("Color Filter", nullptr);
                break;
        }
    }
    if(Settings::Tab == 4){
        switch(i){
            case 0:
                ImGui::Checkbox(ozObfuscate("Make Enemy Ignore Teddy"), &igntdy);
                ImGui::Separator();
                ImGui::Checkbox(ozObfuscate("All Cannot Attack"), &allCantAttack);
                if(!allCantAttack) {
                    ImGui::Checkbox(ozObfuscate("Granny Can't Attack (Grandpa Also)"), &grCantAttack);
                    ImGui::Checkbox(ozObfuscate("Grandpa Can't Attack"), &grpCantAttack);
                    ImGui::Checkbox(ozObfuscate("Crocodile Can't Attack"), &crCantAttack);
                    ImGui::Checkbox(ozObfuscate("Slendrina Can't Attack"), &slCantAttack);
                }
                ImGui::Separator();
                ImGui::Checkbox(ozObfuscate("No One Can See Me"), &allInvis);
                if(!allInvis){
                    ImGui::Checkbox(ozObfuscate("Invisible From Granny"), &grInvis);
                    ImGui::Checkbox(ozObfuscate("Invisible From Grandpa"), &grpInvis);
                    ImGui::Checkbox(ozObfuscate("Invisible From Crocodile"), &crInvis);
                    ImGui::Checkbox(ozObfuscate("Invisible From Robot"), &slInvis);
                }

                break;
            case 1:

                 if(ImGui::Checkbox(ozObfuscate("Disable Collisions"), &igncol)){
                     IgnoreLayerCollision(0,8, igncol);
                 }
                 ImGui::Checkbox(ozObfuscate("Fly"), &fly);
                 if(ImGui::SliderFloat(ozObfuscate("Front Speed"), &frSpeed, 0, 100)){
                     changeForwSpeed = true;
                 }
                 if(ImGui::SliderFloat(ozObfuscate("Back Speed"), &bckSpeed, 0, 100)){
                     changeBackSpeed = true;
                 }
                 ImGui::Checkbox(ozObfuscate("Ignore Bear Traps"), &ignbt);

                break;
            case 2:
                ImGui::SetWindowFontScale(0.8);
                if(ImGui::SliderFloat(ozObfuscate("Field Of View"), &fov, 0, 150)){
                    if(Camera_main && Camera_main() && set_fieldOfView){
                        set_fieldOfView(Camera_main(), fov);
                    }
                }
                if(ImGui::SliderFloat(ozObfuscate("Ortographic View Size"), &ortoSize, 3, 30)){
                    if(Camera_main && Camera_main() && set_orthographicSize ){
                        set_orthographicSize(Camera_main(), ortoSize);
                    }
                }


                ImGui::Separator();
                if(ImGui::SliderFloat(ozObfuscate("Near Clip Plane "), &nearCF, 0.3f, 100)){
                    if(Camera_main && Camera_main() && set_nearClipPlane){
                        set_nearClipPlane(Camera_main(), nearCF);
                    }
                }
                if(ImGui::SliderFloat(ozObfuscate("Camera Aspect Size"), &camAspect, 1, 50)){
                    if(Camera_main && Camera_main() && set_aspect){
                        set_aspect(Camera_main(), camAspect);
                    }
                }
                if(ImGui::Checkbox(ozObfuscate("Ortographic View"), &ortoCam)){
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
    const char * dstr = std::string(std::string(ozObfuscate("Distance: ")) + std::to_string(distance)).c_str();
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
void Render(){
    ImGuiIO& io = ImGui::GetIO();

    if(!ImGui::get_cmdExecutorHaveVal()){
        ImGui::set_cmdExecutor(&cmdExecuteSystem);
    }
    //Creating A new ImGui Frame for our backend
    ImGui_ImplAndroidGLES2_NewFrame(screenWidth, screenHeight, currentTimeInMilliseconds()-startTime);
    ImDrawList *drawList;
    if(ESP.drawBehindMenu){
        drawList = ImGui::GetBackgroundDrawList();
    } else {
        drawList = ImGui::GetForegroundDrawList(ImGui::GetMainViewport());
    }

    drawList->AddText(ImGui::GetFont(), 30, ImVec2(100.f, 100.f), ImColor(255, 255, 0, 255), "Player Stats.", 0, 0.0f, nullptr);

    if (!ESP.enemies.empty() && ESP.ESP) {
        for (int i = 0; i < ESP.enemies.size(); i++) {
            ESPEnemy *me = ESP.enemies[i];
            if(me->ESP && me->screenPosition.x > 0){
                if(i==3 && me->physicalPosition.x == 0 ){

                    drawList->AddText(ImGui::GetFont(), 30, ImVec2(0,0), ImColor(255, 255, 0, 255), "Can't find Slendrina!", 0, 0.0f, nullptr);

                } else {
                    DrawESPElement(drawList, ImVec2(screenWidth / 2, ESP.lineOffsetY), me->name, me->color,
                                   me->screenPosition, ESP.boxBg,1, me->distance);
                }
            }



        }
    }
    //DrawESPElement(ImVec2(screenWidth / 2, 0), "Grandpa", ImColor(0, 255, 0, 255), ImVec2(ImVec2((screenWidth / 2) + 450, screenHeight / 2)), true, ImVec2(0,0), 0);

    //DrawESPElement(ImVec2(screenWidth / 2, 0), "Slendrina", ImColor(0, 0, 255, 255), ImVec2(ImVec2((screenWidth / 2) - 450, screenHeight / 2)), true);

    // 3. Show the ImGui test window. Most of the sample code is in ImGui::ShowTestWindow()
//1200
    if(ImGui::get_openAnimationState() < 1200){

        ImGui::set_openAnimationState(ImGui::get_openAnimationState()+60);
    }
    if(ImGui::get_openAnimationState() > 1200){
        ImGui::set_openAnimationState(1200);
    }

    ImGui:: SetNextWindowSize(ImVec2(ImGui::get_openAnimationState(),488));
    //  ImGui::GetCurrentWindow()->SizeFull.x = ImGui::get_openAnimationState()
    std::string wName = std::string(ozObfuscate("Mod Menu for ")) + GAME_NAME + "(" + GAME_VER + ")" + std::string(ozObfuscate(" - by ozMod"));
    ImGui::Begin( wName.c_str(), nullptr, ImGuiWindowFlags_MenuBar |

                                                                                    ImGuiWindowFlags_AlwaysAutoResize);


    if (ImGui::BeginMenuBar())
    {
        if (ImGui::BeginMenu(ozObfuscate("Tools")))
        {
            if (ImGui::MenuItem(ozObfuscate("Paint"), ozObfuscate("Ctrl+P") , &show_paint)) {
                /* Do stuff */ }
            ImGui::MenuItem(ozObfuscate("Metrics/Debugger"), nullptr);

            ImGui::MenuItem(ozObfuscate("Console"), nullptr, &show_app_console);
            ImGui::MenuItem(ozObfuscate("Game Logs"), nullptr, &show_app_log);
            if (ImGui::MenuItem(ozObfuscate("Style Editor"), ozObfuscate("Ctrl+O"))) {
                /* Do stuff */ }
            if (ImGui::MenuItem(ozObfuscate("Close"), ozObfuscate("Ctrl+C")))  {
            }
            ImGui::EndMenu();
        }
        if (ImGui::BeginMenu(ozObfuscate("Style Editor")))
        {
            if (ImGui::MenuItem(ozObfuscate("Open Editor"), ozObfuscate("Ctrl+O"))) {
                /* Do stuff */ }

            if (ImGui::MenuItem(ozObfuscate("Reset All"), ozObfuscate("Ctrl+R")))  {

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
            if (ImGui::MenuItem(ozObfuscate("About Mod Menu"), ozObfuscate("Ctrl+M"))) {

            }
            if (ImGui::MenuItem(ozObfuscate("About ImGui"), ozObfuscate("Ctrl+G"))) {

            }

            ImGui::EndMenu();
        }
        ImGui::EndMenuBar();
    }

    if (show_app_console)             ImGui::ShowExampleAppConsole(&show_app_console);
    if (show_app_log)                 ImGui::ShowExampleAppLog(&show_app_log);
    if (show_paint)                 ImGui::ShowExampleAppCustomRendering(&show_app_log);
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
                            "Getting Started",
                            "About Mod Menu",
                            "ImGui Demo Window",
                            "Game Analytics/Metrics",

                    };

        }
        if(Settings::Tab==1){
            resources =
                    {
                            "Enimies Admin Panel",
                            "Camera Menu",
                            "Object Editor",
                            "Teleport Menu",
                            "Items Spawner",
                            "Clones Spawner"
                    };
        }
        if(Settings::Tab==2){
            resources =
                    {
                            "ESP Layer",
                            "ESP Style",
                            "ESP Enemies"
                    };
        }
        if(Settings::Tab==3){
            resources =
                    {
                            "Light",
                            "Qualiity",
                            "Engine Core"
                    };
        }

        if(Settings::Tab==4){
            resources =
                    {
                            "Anti Enemy",
                            "Physics",
                            "Camera",
                            "Player Stats",
                            "No-Clip Settings",
                    };
        }
        filter.Draw(u8" Поиск", 240);
        ImGui::SameLine();
        if(ImGui::Button("Clear")){
            filter.Clear();
        }
        ImGui::PushStyleColor(ImGuiCol_FrameBg, IM_COL32(0, 0, 0, 0));

        if (ImGui::ListBoxHeader("##ResourcesList", ImVec2(getx()+20, gety())))
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

//y=438 // x=200
   // ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);
    //glClearColor(clear_color.x * clear_color.w, clear_color.y * clear_color.w, clear_color.z * clear_color.w, clear_color.w);
   // checkGlError("glClearColor");
   // glClear( GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
   // checkGlError("glClear");
    glUseProgram(gProgram);
    checkGlError("glUseProgram");

    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    //After commentig glVertexAttribArray rendering imgui works fine
    glVertexAttribPointer(gvPositionHandle, 2, GL_FLOAT, GL_FALSE, 0, (GLvoid*)0);
    glEnableVertexAttribArray(gvPositionHandle);
      checkGlError("glDrawArrays");
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


    ImGui::EndFrame(); //End ImGui Frame

    ImGui::Render(); // Render ImGui
    //glClear(GL_BUFFER);
    ImGui_ImplAndroidGLES2_RenderDrawLists(ImGui::GetDrawData()); //Draw ImGui Draw Data with our Android Backend
    //g_FontTexture = 0;

}

extern "C"
JNIEXPORT jboolean JNICALL
Java_androidx_viewbinding_systemui_UnityPlayerImpl_onTouchEventUnused(JNIEnv *env, jobject thiz, jobject e) {
    // TODO: implement onTouchEvent()
    // Get class, super class, and super class method ID...
    jclass objcls = env->GetObjectClass(thiz);
    jclass tchcls = env->FindClass("android/view/MotionEvent");
    jclass sprcls = env->GetSuperclass(objcls);
    jmethodID methid = env->GetMethodID(sprcls, "onTouchEvent", "(Landroid/view/MotionEvent;)Z");
    jmethodID methidT = env->GetMethodID(objcls, "injectEvent", "(Landroid/view/MotionEvent;)Z");
    jmethodID id_getAct = env->GetMethodID(tchcls, "getActionMasked", "()I");
    jmethodID id_getX = env->GetMethodID(tchcls, "getX", "()F");
    jmethodID id_getY = env->GetMethodID(tchcls, "getY", "()F");
    jmethodID id_getPs = env->GetMethodID(tchcls, "getPointerCount", "()I");

    jint action = env->CallIntMethod(e,id_getAct);
    jfloat x = env->CallFloatMethod(e,id_getX);
    jfloat y = env->CallFloatMethod(e,id_getY);
    jint pointers = env->CallIntMethod(e,id_getPs);
    bool menuTouch = HandleInputEvent(env, action, x,y, pointers);
    LOGI("Touch Event Happens.");
    // Call super class method...
    return  true;
}
const char* (*oGetStringUTFChars)(JNIEnv*, jstring, jboolean*);
const char* GetStringUTFChars(JNIEnv* env, jstring str, jboolean* copy)
{
    const char * result = oGetStringUTFChars(env, str, copy);

    LOGI("JNI CALL - GetStringUTFChars: %s", result);

    return result;
}

jclass (*oFindClassHook)(JNIEnv*, const char*);
jclass FindClassHook(JNIEnv*env, const char*classn)
{
    jclass result = oFindClassHook(env, classn);
    LOGI("JNI CALL - FindClass: %s", classn);
    if(std::string(classn).find("uk/lgl") != std::string::npos){
        return nullptr;
    }
    if(std::string(classn).find("com/android/support/Preferences") != std::string::npos){
        return nullptr;
    }
    if(std::string(classn).find("com/android/support/Menu") != std::string::npos){
        return nullptr;
    }
    if(std::string(classn).find("FloatingModMenuService") != std::string::npos){
        return nullptr;
    }
    if(std::string(classn).find("keysystem") != std::string::npos){
        return nullptr;
    }
    if(std::string(classn).find("tdm/ahz") != std::string::npos){
        return nullptr;
    }
    return result;
}
jclass (*oRegisterNativesHook)(JNIEnv*, jclass, const JNINativeMethod*,
                               jint);
jclass RegisterNativesHook(JNIEnv*env, jclass clasz, const JNINativeMethod* list,
                           jint num)
{
    jclass result = oRegisterNativesHook(env, clasz, list, num);
    jclass cls = env->FindClass("java/lang/Class");
    jmethodID mid_getName = env->GetMethodID(cls, "getName", "()Ljava/lang/String;");
    jboolean b = false;
    const char * name = env->GetStringUTFChars((jstring)env->CallObjectMethod(clasz, mid_getName), &b);
    LOGI("JNI CALL - RegisterNatives for %s:", name);
    LOGI("JNI CALL -  how Much?: %d", sizeof (list));
    int num2 = 0;
    while(num2 != sizeof (list)){
        LOGI("JNI CALL -   Member #%d: %s", num2, std::string(std::string(list[num2].name) + std::string(list[num2].signature)).c_str());
        num2++;
    }
    return result;
}

void InitJNI(JNIEnv*env) {

   // MSHookFunction((void *)env->functions->GetStringUTFChars, (void *)GetStringUTFChars, (void **)&oGetStringUTFChars);
    //MSHookFunction((void *)env->functions->FindClass, (void*)FindClassHook, (void **)&oFindClassHook);
   // MSHookFunction((void *)env->functions->RegisterNatives, (void *)RegisterNativesHook, (void **)&oRegisterNativesHook);

}

__attribute__((constructor))
void lib_main() {

    InitMods();
}

extern "C"
JNIEXPORT void JNICALL
Java_androidx_viewbinding_systemui_SystemOverlaySurface_onSurfaceChanged(JNIEnv *env, jobject thiz, jobject gl10, jint w,
                                                    jint h) {
    // TODO: implement onSurfaceChanged()
    // TODO: implement ReInit()
    screenWidth = w;
    screenHeight = h;
}
extern "C"
JNIEXPORT void JNICALL
Java_androidx_viewbinding_systemui_SystemOverlaySurface_onSurfaceCreated(JNIEnv *env, jobject thiz, jobject gl,
                                                    jobject egl_config) {
    // TODO: implement onSurfaceCreated()

    startTime = currentTimeInMilliseconds();
    setupGraphics( screenWidth, screenHeight);
    //activity = env->NewGlobalRef(ctx);
}
bool pauseSurface = false;
extern "C"
JNIEXPORT void JNICALL
Java_androidx_viewbinding_systemui_SystemOverlaySurface_onDrawFrame(JNIEnv *env, jobject thiz, jobject gl) {
    // TODO: implement onDrawFrame()
    // TODO: implement Render()
     if(pauseSurface){
        return;
    }
    UpdateInput();
    UpdateKeyboardInput(env); // Calling it every frame to get keyboard inputs...
    ImGuiIO& io = ImGui::GetIO();
    // FIXME: do not call this every frame because of JNI overhead
    if (io. WantTextInput && !WantTextInputLast){
        ShowSoftKeyboardInput(env);
    }
    WantTextInputLast = io.WantTextInput;
    Render();
}
extern "C"
JNIEXPORT jboolean JNICALL
Java_androidx_viewbinding_systemui_SystemOverlaySurface_HandleInput(JNIEnv *env, jclass clazz, jint a, jfloat x,
                                               jfloat y, jint p) {
    // TODO: implement HandleInput()
    return HandleInputEvent(env, a,x,y,p);
}

extern "C"
JNIEXPORT void JNICALL
Java_androidx_viewbinding_systemui_SystemOverlaySurface_onPause(JNIEnv *env, jobject clazz) {
    // TODO: implement onPause()
    pauseSurface = true;
}
extern "C"
JNIEXPORT void JNICALL
Java_androidx_viewbinding_systemui_SystemOverlaySurface_onResume(JNIEnv *env, jobject thiz) {
    // TODO: implement onResume()
    pauseSurface = false;
}
int getSurfaceGravity() {
    // TODO: implement getSurfaceGravity()
    return 8388659;
}


extern "C"
JNIEXPORT jboolean JNICALL
Java_androidx_viewbinding_systemui_SystemOverlaySurface_onTouchEvent(JNIEnv *env, jobject thiz,
                                                                     jobject e) {
 return Surface.Input(env, e);
}

extern "C"
    JNIEXPORT void JNICALL
Java_androidx_viewbinding_systemui_SystemOverlaySurface_Start(JNIEnv *env, jclass clazz, jobject activityContextArg,jobject player) {
    // TODO: implement nativeInit()
        logEgl();

        Surface.Start(env, activityContextArg, player);
}

extern "C"
JNIEXPORT jboolean JNICALL
Java_androidx_viewbinding_systemui_SystemOverlaySurface_dispatchKeyEvent(JNIEnv *env, jobject thiz,
                                                                         jobject e) {
    // TODO: implement dispatchKeyEvent()
    return Surface.dispacthKeyEvent(env, e);
}



extern "C"
JNIEXPORT jint JNICALL
JNI_OnLoad(JavaVM *vm, void *reserved) {
    JNIEnv *globalEnv;
    vm->GetEnv((void **) &globalEnv, JNI_VERSION_1_6);
    // Register your class native methods. Build and ecompile the app and see the signature
    // This is to hide function names from disassembler
    // See more: https://developer.android.com/training/articles/perf-jni#native-libraries
    InitJNI(globalEnv);

    return JNI_VERSION_1_6;
}
