 # ImGui Mod Menu for Android
 This is a mod menu with ImGui for Android Unity Games
 Preview (built in Granny 3):
 ![image](https://user-images.githubusercontent.com/80401984/145401480-8910961b-c909-4130-b827-a636db62d778.png)
 # How it works?
   Later.
  # How to inject
  1. Copy your builded lib to lib/armeabi-v7a
  2. Load the library on Activity Start. Tutorial:
  
     You need to add following lines in ```com/unity/player/UnityPlayerActivity```:
   
   ```
   const-string v0, "_ozMod"

    invoke-static {v0}, Ljava/lang/System;->loadLibrary(Ljava/lang/String;)V
   ```
   
   Do it like that
 ![image](https://user-images.githubusercontent.com/80401984/145727071-26bb7d51-ae83-4fdc-94f5-aa3b9421ed4b.png)

# Recomendations

1. Use it in Android Studio to better effect. If you are using AIDE and having issues then don't write to issues. I will not repond to theese issues and close it.
2. Please Rename lib name with CMakeLists.txt

# Used to create

Obfuscate by Adam Yaxley

GLSurfaceView

OpenGL (OpenGL Emscripten v2)

EGL

Il2cpp Dumper

dnSpy

Cydia Substrade

Dobby

JNI


If i'm not mentionied someone else who helped/contributed to that project, just contact me.
