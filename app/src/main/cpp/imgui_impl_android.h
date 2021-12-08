

#ifndef IMGUI_IMPL_ANDROID_GL2
#define IMGUI_IMPL_ANDROID_GL2

#include "imgui/imgui.h"
#pragma  once
#include "imgui/imgui_internal.h"


static unsigned  int      g_FontTexture = 0;
IMGUI_API bool        ImGui_ImplAndroidGLES2_Init();
IMGUI_API void        ImGui_ImplAndroidGLES2_Shutdown();
IMGUI_API void        ImGui_ImplAndroidGLES2_NewFrame(int width, int height, unsigned int millis);
//IMGUI_API bool        ImGui_ImplAndroidGLES2_ProcessEvent(SDL_Event* event);

// Use if you want to reset your rendering device without losing ImGui state.
IMGUI_API void        ImGui_ImplAndroidGLES2_InvalidateDeviceObjects();
IMGUI_API bool        ImGui_ImplAndroidGLES2_CreateDeviceObjects();

#endif // IMGUI_IMPL_ANDROID_GL2