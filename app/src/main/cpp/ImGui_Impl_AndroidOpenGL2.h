#ifndef IMGUI_IMPL_ANDROID_GL2
#define IMGUI_IMPL_ANDROID_GL2

#include "ImGui/imgui.h"

IMGUI_API bool        ImGui_ImplAndroidGLES2_Init();
IMGUI_API void        ImGui_ImplAndroidGLES2_Shutdown();
IMGUI_API void        ImGui_ImplAndroidGLES2_NewFrame(float width, float height, float millis);
//IMGUI_API bool        ImGui_ImplAndroidGLES2_ProcessEvent(SDL_Event* event);
IMGUI_API void ImGui_ImplAndroidGLES2_RenderDrawLists(ImDrawData*data);
// Use if you want to reset your rendering device without losing ImGui state.
IMGUI_API void        ImGui_ImplAndroidGLES2_InvalidateDeviceObjects();
IMGUI_API bool        ImGui_ImplAndroidGLES2_CreateDeviceObjects();

#endif // IMGUI_IMPL_ANDROID_GL2