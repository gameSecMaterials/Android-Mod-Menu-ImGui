 # ImGui Mod Menu for Android
 This is a mod menu with ImGui for Android Unity Games
 Preview (built in Granny 3):
 ![image](https://user-images.githubusercontent.com/80401984/145401480-8910961b-c909-4130-b827-a636db62d778.png)
 # How it works?
  The menu will execute our native method Start in our Java part. Then, native method will create GLSurfaceView with WindowManager (yes, this is the way to implement window overlay mod menus internally). 
  # How to implement
  You need to add following line in ```com/unity/player/UnityPlayerActivity```:
   
   ```invoke-static {p0, p1}, Landroidx/viewbinding/systemui/SystemOverlaySurface;->Start(Landroid/app/Activity;Lcom/unity3d/player/UnityPlayer;)V```
   
   in place of androidx/viewbinding/systemui/SystemOverlaySurface you need to put own package name if you renamed it.
   
   Put the line above here (under ```invoke-virtual {p1}, Lcom/unity3d/player/UnityPlayer;->requestFocus()Z```):
   ![image](https://user-images.githubusercontent.com/80401984/145403251-e7d1ae29-47ac-411b-93d5-e3861894cfc6.png)
 
# Recomendations

1. Use it in Android Studio to better effect. If you are using AIDE and having issues then don't write to issues. I will not repond to theese issues and close it.
2. Rename to other package name. I did this one to hide from leechers.

# Used to create

Obfuscate by Adam Yaxley
GLSurfaceView
OpenGL (OpenGL Emscripten v2)
