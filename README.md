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

1. This is the old project. If you really need to understand and build it, then do it by yourself, this project is no longer maintained.
2. 

# Used to create

OpenGL (GLESv2), EGL



