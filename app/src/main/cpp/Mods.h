//
// Created by ozMod on 27.11.2021.
//

#define targetLibName OBFUSCATE("libil2cpp.so")

void *get_il2cpp() {
    void *mod = 0;
    while (!mod) {
        mod = dlopen(targetLibName, RTLD_LAZY);
    }

    return mod;
}
#include <unistd.h>
    #include <pthread.h>
//#include "libil2cpp.so.h"
#include "ozModSharedLibraryManager.h"
int hookCount = 0;
struct ESPEnemy {
    bool ESP = true;
    const char * name = {0};
    int id = -1;
    ImColor color;
    void * object;
    ImVec2 screenPosition, boxSize = ImVec2(0,0);
    ImVec4 physicalPosition, physicalSize;
    float distance;
};
struct Enemy {
    const char * name = {0};
    int id = -1;
    ImColor color;
    void * object;
    ImVec4 physicalPosition, physicalSize;
    float distance;
};
struct EnemyEditor {
    std::vector<Enemy *> enemies;

}EnemyEditor;
struct ESP{
    bool drawBehindMenu=true;

    bool ESP = false;
    bool Lines = true, Boxes = true, Texts = true;
    bool boxBg =true;
    float boxSize = 1, lineOffsetY = 0, textSize = 30;
    std::vector<ESPEnemy *> enemies;
    int lastEnemyId = -1;
    ESPEnemy * AddEnemy(){
        ESPEnemy * enemy = new ESPEnemy();
        lastEnemyId ++;
        enemy->id = lastEnemyId;
        enemies.push_back(enemy);
        return enemy;
    }
    void Init(){
        ESPEnemy * enemy = AddEnemy();
        enemy->name = "Granny";
        enemy->color = ImColor(255, 0,0,255);
        ESPEnemy * enemy1 = AddEnemy();
        enemy1->name = "Grandpa";
        enemy1->color = ImColor(0, 255,0,255);
        ESPEnemy * enemy2 = AddEnemy();
        enemy2->name = "Slendrina";
        enemy2->color = ImColor(0, 0,255,255);
    }

}ESP;
bool fog, fpsbypass, wireframe, clrfilter;
ImColor clf, wfClr;
float wfLW = 1;
bool changeForwSpeed, changeBackSpeed;
bool fly,igncol, ignbt;
float gravity, frSpeed, bckSpeed, fov = 60, ortoSize = 3, nearCF = 1, camAspect = 1.889f, timescale = 1, fogd = 1;
bool allCantAttack, grCantAttack, grpCantAttack, crCantAttack, slCantAttack;
bool allInvis, grInvis, grpInvis, crInvis, slInvis;
bool igntdy;
bool ortoCam, freezeCamSummary, freezeCamPosCaptured;
void * slendrina;


struct Vec2 //Эта структура отлично справляется с получением координат джойстика.  Тот Vector2 херня - он не работает!)
{
    float x, y;
};
struct Vector3{
    float x,y,z;
    Vector3 operator/(float f) {
        Vector3 v = {
                x/f,
                y/f,
                z/f
        };
        return v;
    }
    Vector3 operator*(float f) {
        Vector3 v = {
                x*f,
                y*f,
                z*f
        };
        return v;
    }
    Vector3 operator+(Vector3 fx) {
        Vector3 v = {
                x+fx.x,
                y+fx.y,
                z+fx.z
        };
        return v;
    }
    Vector3 operator-(Vector3 fx) {
        Vector3 v = {
                x-fx.x,
                y-fx.y,
                z-fx.z
        };
        return v;
    }
};
/*
 * This method was decompiled and recreated from Unity Core
 * With IDA Pro PseudoCode.
 */
float UnityEngine_Vector3__Distance(
        Vector3 a,
        Vector3 b) {
    return sqrtf(
            (float)((float)((float)(a.y - b.y) * (float)(a.y - b.y))
                    + (float)((float)(a.x - b.x) * (float)(a.x - b.x)))
            + (float)((float)(a.z - b.z) * (float)(a.z - b.z)));
}

void (*IgnoreLayerCollision)(int layer1, int layer2, bool ignore);
void (*old_Transform_LookAt)(void *instance, void *target);

void (*set_useGravity)(void*, bool value);
void* (*il2cpp_resolve_icall_0)(const char *);
void* (*il2cpp_type_get_object)( void * type);
void* il2cpp_getMethod(const char *a1){
   void* result;
   result = il2cpp_resolve_icall_0(a1);
   return result;
}

/*
 * Method Signature of WorldToScreenPoint is: UnityEngine.Camera::WorldToScreenPoint_Injected(UnityEngine.Vector3&,UnityEngine.Camera/MonoOrStereoscopicEye,UnityEngine.Vector3&)
 *
 */
/*
 * This method was decompiled and recreated from Unity Core
 * With IDA Pro PseudoCode.
 */
Vector3 (*WorldToScreenPoint)   (void *instance,Vector3 position);
//UnityEngine.Camera::set_orthographic(System.Boolean)
void set_orthographic(void* instance, bool value){
    if(!il2cpp_resolve_icall_0){
        return;
    }
    ((void (*)(void*, bool))il2cpp_getMethod("UnityEngine.Camera::set_orthographic(System.Boolean)"))(instance, value);
}
//Signature
void set_fieldOfView(void* instance, float value){
    if(!il2cpp_resolve_icall_0){
        return;
    }
                ((void (*)(void*, float))il2cpp_getMethod( "UnityEngine.Camera::set_fieldOfView(System.Single)"))(instance, value);
}

void set_orthographicSize(void*instance, float value){
    if(!il2cpp_resolve_icall_0){
        return;
    }
    ((void (*)(void*, float))il2cpp_getMethod( "UnityEngine.Camera::set_orthographicSize(System.Single)"))(instance, value);

}
void set_nearClipPlane(void*instance, float value){

    if(!il2cpp_resolve_icall_0){
        return;
    }
    ((void (*)(void*, float))il2cpp_getMethod( "UnityEngine.Camera::set_nearClipPlane(System.Single)"))(instance, value);

}

void (*set_timeScale)(float value);
void set_aspect(void*instance, float value){
    if(!il2cpp_resolve_icall_0){
        return;
    }
        ((void (*)(void*, float))il2cpp_getMethod( "UnityEngine.Camera::set_aspect(System.Single)"))(instance, value);

}
void *Camera_main(){
    void* (*result)();
    if(!il2cpp_resolve_icall_0){
        return nullptr;
    }
    result = (void* (*)())il2cpp_getMethod("UnityEngine.Camera::get_current()");
    if(result == nullptr){
        return nullptr;
    }
    return result();
}

void *(*StartCoroutine)(void * t, void *);
void *(*get_gameObject)(void * t);
void (*SetQualityLevel)(int, bool);
void *(*get_transform)(void * t);
Vector3 (*get_forward)(void * t);
Vector3 (*get_right)(void * t);
Vector3 (*get_pos)(void * t);
//Vector3 (*WorldToScreenPoint)(void*,Vector3 position);
void (*set_pos)(void * t, Vector3 w);
void (*set_enabled) (void* i,bool value);
void Transform_LookAt(void *instance, void *target) {

    //if (instance != NULL && antifocus) {
    //    return;
    //  }
    old_Transform_LookAt(instance, target);

}

void *(*old_EnemyAIGranny_PC)(void *instance);

void *EnemyAIGranny_PC(void *instance) {
    LOGI("Frame EnemyAIGranny_PC");
    if (instance != nullptr && (grCantAttack || allCantAttack) ) {
        *(bool *) ((uint64_t) instance + 0x100) = false;
        *(bool *) ((uint64_t) instance + 0xED) = false;
        return nullptr;
    }
    if (instance != nullptr && (grInvis || allInvis) ) {
        *(bool *) ((uint64_t) instance + 0xB4) = false;  //invis
        return nullptr;
    }
    return old_EnemyAIGranny_PC(instance);

}

void *(*old_EnemyAIGrandpa_PC)(void *instance);

void *EnemyAIGrandpa_PC(void *instance) {
    LOGI("Frame EnemyAIGrandpa_PC");
    if (instance != nullptr && (grpCantAttack || allCantAttack) ) {
        *(bool *) ((uint64_t) instance + 0x108) = false;//player get caught
        *(bool *) ((uint64_t) instance + 0xED) = false;//attacking
        return nullptr;
    }
    if (instance != nullptr && (grpInvis || allInvis) ) {
        *(bool *) ((uint64_t) instance + 0xA4) = false;  //invis
        return nullptr;
    }
    if(instance != nullptr && igntdy){
        *(bool *) ((uint64_t) instance + 0x187) = false;
    }
    return old_EnemyAIGranny_PC(instance);

}

void (*old_EnemyAIGranny_FixedUpdate)(void *instance);

void EnemyAIGranny_FixedUpdate(void *instance) {
    LOGI("Frame EnemyAIGranny_FixedUpdate");
    if(instance == nullptr){
        return;
    }
    if (true && (grInvis || allInvis) ){
        LOGI("Frame EnemyAIGranny_FixedUpdate: grInvis || allInvis");
        *(bool *) ((uint64_t) instance + 0xB4) = false;
    }
    if (true && (grCantAttack || allCantAttack) ){
        LOGI("Frame EnemyAIGranny_FixedUpdate: grCantAttack || allInvis");
        *(float *) ((uint64_t) instance + 0xC8) = 0;
        *(float *) ((uint64_t) instance + 0xC4) = 99999;
        *(bool *) ((uint64_t) instance + 0xED) = false;
    }
    if(true && igntdy){
        LOGI("Frame EnemyAIGranny_FixedUpdate: igntdy");
        *(bool *) ((uint64_t) instance + 0x190) = false;
    }
    old_EnemyAIGranny_FixedUpdate(instance);
    if (true && (grInvis || allInvis) ){
        LOGI("Frame EnemyAIGranny_FixedUpdate: grInvis || allInvis");
        *(bool *) ((uint64_t) instance + 0xB4) = false;
    }
    if(true && igntdy){
        LOGI("Frame EnemyAIGranny_FixedUpdate: igntdy");
        *(bool *) ((uint64_t) instance + 0x190) = false;
    }
    if (true && (grCantAttack || allCantAttack) ) {
        LOGI("Frame EnemyAIGranny_FixedUpdate: (grCantAttack || allCantAttack)");
        *(float *) ((uint64_t) instance + 0xC8) = 0;
        *(float *) ((uint64_t) instance + 0xC4) = 99999;
        *(bool *) ((uint64_t) instance + 0xED) = false;
    }
}

void (*old_EnemyAIGrandpa_FixedUpdate)(void *instance);

void EnemyAIGrandpa_FixedUpdate(void *instance) {
    LOGI("Frame EnemyAIGrandpa_FixedUpdate");
    if(instance == nullptr){
        return;
    }
    if (true && (grpInvis || allInvis) ){
        LOGI("Frame EnemyAIGranny_FixedUpdate: grInvis || allInvis");
        *(bool *) ((uint64_t) instance + 0xA4) = false;
    }
    if (true && (grpCantAttack || allCantAttack) ){
        *(float *) ((uint64_t) instance + 0xB8) = 0;
        *(float *) ((uint64_t) instance + 0xB4) = 99999;
        *(bool *) ((uint64_t) instance + 0xED) = false;
    }

    old_EnemyAIGranny_FixedUpdate(instance);
    if (true && (grpInvis || allInvis) ){
        *(bool *) ((uint64_t) instance + 0xA4) = false;
    }
    if (true && (grpCantAttack || allCantAttack) ) {
        *(float *) ((uint64_t) instance + 0xB8) = 0;
        *(float *) ((uint64_t) instance + 0xB4) = 99999;
        *(bool *) ((uint64_t) instance + 0xED) = false;
    }
}
bool ESPTransformCheck(void * go){
    return go != nullptr && (get_gameObject && get_transform && get_pos) && get_transform((go));
}

void GrannyESP(void * granny, void * player){
    if(granny != nullptr){
        if(player){
            if(!(ESPTransformCheck(player) && ESPTransformCheck(granny))){
                LOGE("Can't get transform of Player or Granny!");
                return;
            }
            Vector3 cpos = get_pos(get_transform((player)));
            Vector3 cposG = get_pos(get_transform((granny)));
            //cposG.y += 0.7f;
            ESPEnemy * enemy = ESP.enemies[0];

            if(!(enemy)){
                LOGE("Can't get Enemy!");
                return;
            }
            if(!enemy->ESP){
                return;
            }
         //   LOGE("The name of enemy is %c", enemy->name);
            // enemy->distance = Vector3::Distance(cpos, cposG);
            enemy->physicalPosition = ImVec4(cposG.x, cposG.y, cposG.z, 0);
            if(!(Camera_main && Camera_main())){
                LOGE("Can't get Camera!");
                return;
            }
            if(!(WorldToScreenPoint)){
                LOGE("Can't get WorldToScreenPoint!");
                return;
            }
            Vector3 fm = WorldToScreenPoint(Camera_main(), cposG);
            if (fm.z <= 1.0f){
                return;
            }//  проверка чтоб не рисовало за экраном
            enemy->distance = UnityEngine_Vector3__Distance(cpos, cposG);
            //enemy->screenPosition = ImVec2(-999, -999);
            enemy->screenPosition = ImVec2( fm.x - 150 + enemy->distance, (screenHeight - fm.y) - (30+400) + enemy->distance);
             enemy->boxSize.x = 150 - (enemy->distance * 2);
            enemy->boxSize.y = 250 - (enemy->distance * 2);

        }

    }
}

void GrandpaESP(void * grandpa, void * player){
    if(grandpa != nullptr){
        if(player){
            if(!(ESPTransformCheck(player) && ESPTransformCheck(grandpa))){
                LOGE("Can't get transform of Player or Granny!");
                return;
            }
            Vector3 cpos = get_pos(get_transform((player)));
            Vector3 cposG = get_pos(get_transform((grandpa)));
            //cposG.y += 0.7f;
            ESPEnemy * enemy = ESP.enemies[1];
            if(!(enemy)){
                LOGE("Can't get Enemy!");
                return;
            }
            if(!enemy->ESP){
                return;
            }
          //  LOGE("The name of enemy is %c", enemy->name);
            // enemy->distance = Vector3::Distance(cpos, cposG);
            enemy->physicalPosition = ImVec4(cposG.x, cposG.y, cposG.z, 0);
            if(!(Camera_main && Camera_main())){
                LOGE("Can't get Camera!");
                return;
            }
            if(!(WorldToScreenPoint)){
                LOGE("Can't get WorldToScreenPoint!");
                return;
            }

            Vector3 fm = WorldToScreenPoint(Camera_main(), cposG);
            if (fm.z <= 1.0f){
                return;
            }//  проверка чтоб не рисовало за экраном
            enemy->distance = UnityEngine_Vector3__Distance(cpos, cposG);
            //enemy->screenPosition = ImVec2(-999, -999);
            enemy->screenPosition = ImVec2( fm.x - 150  + enemy->distance, (screenHeight - fm.y) - (30+400)  + enemy->distance);
            enemy->boxSize.x = 150- (enemy->distance * 2);
            enemy->boxSize.y = 250- (enemy->distance * 2);

        }

    }
}

void SlendrinaESP(void * slend, void * player){
    if(slend != nullptr){
        if(player){
            if(!(ESPTransformCheck(player) && ESPTransformCheck(slend))){
                LOGE("Can't get transform of Player or Granny!");
                return;
            }
            Vector3 cpos = get_pos(get_transform((player)));
            Vector3 cposG = get_pos(get_transform((slend)));
            //cposG.y += 0.7f;
            ESPEnemy * enemy = ESP.enemies[2];
            if(!(enemy)){
                LOGE("Can't get Enemy!");
                return;
            }
            if(!enemy->ESP){
                return;
            }
            //LOGE("The name of enemy is %c", enemy->name);
            // enemy->distance = Vector3::Distance(cpos, cposG);
            enemy->physicalPosition = ImVec4(cposG.x, cposG.y, cposG.z, 0);
            if(!(Camera_main && Camera_main())){
                LOGE("Can't get Camera!");
                return;
            }
            if(!(WorldToScreenPoint)){
                LOGE("Can't get WorldToScreenPoint!");
                return;
            }

            Vector3 fm = WorldToScreenPoint(Camera_main(), cposG);
            if (fm.z <= 1.0f){
                return;
            }//  проверка чтоб не рисовало за экраном

            //enemy->screenPosition = ImVec2(-999, -999);
            enemy->distance = UnityEngine_Vector3__Distance(cpos, cposG);
            enemy->screenPosition = ImVec2( fm.x - 150 + enemy->distance, (screenHeight - fm.y) - (30+400) + enemy->distance );
            enemy->boxSize.x = 150 - (enemy->distance * 2);
            enemy->boxSize.y = 250 - (enemy->distance * 2);

        }

    }
}
void (*old_FPSControllerNEW_FixedUpdate)(void *instance);
void FPSControllerNEW_FixedUpdate(void *instance) {
    LOGI("Frame Player");
    if(instance == nullptr){
        return;
    }
    void *granny = *(void **) ((uint64_t) instance + 0xA0); //Объект бабки)
    void *player = *(void **) ((uint64_t) instance + 0x80); //Объект игрока
    void *grandpa = *(void **) ((uint64_t) instance + 0xA4); //Объект деда) )
    if(changeForwSpeed){
        *(float*) ((uint64_t) instance + 0x1C) = frSpeed;
        changeForwSpeed = false;
    }
    if(changeBackSpeed){
        *(float*) ((uint64_t) instance + 0x20) = bckSpeed;
        changeBackSpeed = false;
    }
    void *plCharchter = *(void **) ((uint64_t) instance + 0x3C); //Объект джойстика
    if(plCharchter!= nullptr){
        set_enabled(plCharchter, !fly);
    }
    if(granny != nullptr){
        Vector3 cpos = get_pos(get_transform((player)));
        Vector3 cposG = get_pos(get_transform((granny)));
        if(UnityEngine_Vector3__Distance(cpos, cposG) <= 5){

        }
    }
    if(ESP.ESP){
        GrannyESP(granny, player);
        GrandpaESP(grandpa, player);
       // SlendrinaESP(slendrina, player);
    }


    old_FPSControllerNEW_FixedUpdate(instance); //Вызвать функцию игры
    if(!fly && igncol) {
        void *joystick = *(void **) ((uint64_t) instance + 0x10); //Объект джойстика
        float forwardSpeed = *(float*) ((uint64_t) instance + 0x1C);
        float backwardSpeed = *(float*) ((uint64_t) instance + 0x20);
        Vec2 inputDickk_BackingField = *(Vec2 *) ((uint64_t) joystick + 0x14);
        float x = inputDickk_BackingField.x; //кодината X - влево/вправо
        float y = inputDickk_BackingField.y; //кодината Y - вверх/вниз
        void *cam = Camera_main(); //Камера игрока
        if (x != 0) {
            float nocSpeedRatio = (x < 0 ? backwardSpeed : forwardSpeed);
            Vector3 playerPosition = get_pos(
                    get_transform(player)); ///сначала просто получаем координаты игрока
          Vector3 addition;
            addition = get_right(get_transform(get_gameObject(cam))) * (x/1);
            addition.y = 0;
            playerPosition = playerPosition + addition; //новая позиция игрока ///наш Vector3 равный позиции игрока изменен
            playerPosition.y = get_pos(
                   get_transform(player)).y;

            set_pos(get_transform(player), playerPosition); //теперь игрок на новой позиции
        }

        if (y != 0) {
            float nocSpeedRatio = (y < 0 ? backwardSpeed : forwardSpeed);
            Vector3 playerPosition = get_pos(
                    get_transform(player)); ///сначала просто получаем координаты игрока
            Vector3 addition;
            addition = get_forward(get_transform(get_gameObject(cam))) * (y/1 );
            addition.y = 0;
            playerPosition = playerPosition +  addition; //новая позиция игрока ///наш Vector3 равный позиции игрока изменен
            playerPosition.y = get_pos(
                    get_transform(player)).y;
            set_pos(get_transform(player), playerPosition); //теперь игрок на новой позиции
        }
        return;
    }
  if(fly) {

        void *joystick = *(void **) ((uint64_t) instance + 0x10); //Объект джойстика
        float forwardSpeed = *(float*) ((uint64_t) instance + 0x1C);
        float backwardSpeed = *(float*) ((uint64_t) instance + 0x20);

        Vec2 inputDickk_BackingField = *(Vec2 *) ((uint64_t) joystick + 0x14);
        float x = inputDickk_BackingField.x; //кодината X - влево/вправо
        float y = inputDickk_BackingField.y; //кодината Y - вверх/вниз
        void *cam = Camera_main(); //Камера игрока
        if (x != 0) {
            float nocSpeedRatio = (x < 0 ? backwardSpeed : forwardSpeed);
            Vector3 playerPosition = get_pos(
                    get_transform(player)); ///сначала просто получаем координаты игрока
            playerPosition = playerPosition + get_right(get_transform(get_gameObject(cam))) * (x/nocSpeedRatio); //новая позиция игрока ///наш Vector3 равный позиции игрока изменен
            set_pos(get_transform(player), playerPosition); //теперь игрок на новой позиции
        }

        if (y != 0) {
            float nocSpeedRatio = (y < 0 ? backwardSpeed : forwardSpeed);
            Vector3 playerPosition = get_pos(
                    get_transform(player)); ///сначала просто получаем координаты игрока
            playerPosition = playerPosition +  get_forward(get_transform(get_gameObject(cam))) * (y/nocSpeedRatio ); //новая позиция игрока ///наш Vector3 равный позиции игрока изменен
            set_pos(get_transform(player), playerPosition); //теперь игрок на новой позиции
        }
    }

}
void (*old_BearTrap_OnTriggerEnter)(void *instance, void *other);

void BearTrap_OnTriggerEnter(void *instance, void *other) {

    if (instance != nullptr) {
        if (ignbt) {
            return;
        }
    }
    return old_BearTrap_OnTriggerEnter(instance, other);
}
void (*old_startSlendrina)(void *instance);

void startSlendrina(void *instance) {

    if (instance != nullptr) {

    }
     old_startSlendrina(instance);
}
void (*old_Slendrina_FixedUpdate)(void *instance);

void Slendrina_FixedUpdate(void *instance) {

    if (instance != nullptr) {
        SlendrinaESP(get_gameObject(instance), *(void**) ((uint64_t) instance + 0xE0));
    }
    if (!slCantAttack){
        old_Slendrina_FixedUpdate(instance);
    }

}
void ozHookSym(void *pVoid, void *pVoid1, void **pVoid2) {
    hookCount++;

    MSHookFunction(pVoid, pVoid1, pVoid2);
}


void * eglHandle;
void *get_EGL() {
    void *mod = 0;
    while (!mod) {
        mod = dlopen("libEGL.so", RTLD_LAZY);
    }

    return mod;
}
void * glHandle;
void *get_GL() {
    void *mod = 0;
    while (!mod) {
        mod = dlopen("libGLESv2.so", RTLD_LAZY);
    }

    return mod;
}

int (*old_glGetUniformLocation)(GLuint, const GLchar *);
GLint new_glGetUniformLocation(GLuint program, const GLchar *name) {
    //Log name to find used shaders.
    LOGI("New shader found: %s", name);
    return old_glGetUniformLocation(program, name);
}

void LogShaders(){
    auto p_glGetUniformLocation = (const void*(*)(...))dlsym(glHandle, "glGetUniformLocation");
    const char *dlsym_error = dlerror();
    if(dlsym_error){
        LOGE("Cannot load symbol 'glGetUniformLocation': %s", dlsym_error);
        return;
    }else{
        MSHookFunction(reinterpret_cast<void*>(p_glGetUniformLocation), reinterpret_cast<void*>(new_glGetUniformLocation), reinterpret_cast<void**>(&old_glGetUniformLocation));
    }
}
bool chamsSlendrina;
void (*old_glDrawElements)(GLenum mode, GLsizei count, GLenum type, const void *indices);
void new_glDrawElements(GLenum mode, GLsizei count, GLenum type, const void *indices) {
    old_glDrawElements(mode, count, type, indices);

    if(((count < 2123 || count > 3000) && chamsSlendrina) && !wireframe && !clrfilter){
        return;
    }

 //   LOGE("Count of indicies of glDrawElements: %d", count);
        if (clrfilter) {

            glDepthRangef(1, 0.5);
            glColorMask(5,0,0,0);
            glEnable(GL_BLEND);
            glBlendFunc(GL_SRC_ALPHA, GL_ONE);

        }
    if (wireframe || chamsSlendrina) {

        mode = GL_LINES;
        glLineWidth(wfLW);
        glDepthRangef(1, 0.5);
        glColorMask(1, 0, 0, 0);
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE);
    }

        if(wireframe || clrfilter || chamsSlendrina){
            old_glDrawElements(mode, count, type, indices);


            glDepthRangef(0.5, 1);

            glColorMask(1, 1, 1, 1);
            glDisable(GL_BLEND);


        }


}
void Wireframe(){
    auto p_glDrawElements = (const void*(*)(...))dlsym(glHandle, "glDrawElements");
    const char *dlsym_error = dlerror();
    if(dlsym_error){
        LOGE("Cannot load symbol 'glDrawElements': %s", dlsym_error);
        return;
    }else{
        MSHookFunction(reinterpret_cast<void*>(p_glDrawElements), reinterpret_cast<void*>(new_glDrawElements), reinterpret_cast<void**>(&old_glDrawElements));
    }
}
EGLAPI EGLSurface EGLAPIENTRY (*old_eglCreateWindowSurface )(EGLDisplay dpy, EGLConfig config, EGLNativeWindowType win, const EGLint *attrib_list);
EGLAPI EGLSurface EGLAPIENTRY new_eglCreateWindowSurface(EGLDisplay dpy, EGLConfig config, EGLNativeWindowType win, const EGLint *attrib_list){
    LOGI("eglCreateWindowSurface");
    EGLSurface rezultSurface = old_eglCreateWindowSurface(dpy, config, win, attrib_list);
    return rezultSurface;
}
EGLAPI EGLBoolean EGLAPIENTRY (*old_eglSwapBuffers) (EGLDisplay dpy, EGLSurface surface);
EGLAPI EGLBoolean EGLAPIENTRY new_eglSwapBuffers (EGLDisplay dpy, EGLSurface surface) {
    LOGI("eglSwapBuffers being invoked");

    if(!old_eglSwapBuffers(dpy, surface)){
        LOGI("eglSwapBuffers invoke error.");
        checkGlError("eglSwapBuffers");
        return false;
    }
    return true;
}
int (*old_sub_434958) ();
int  sub_434958 () {
    LOGI("Calling: sub_434958");
    return old_sub_434958();
}

int (*old_sub_518B10) (unsigned int *a1);
int  sub_518B10 (unsigned int *a1) {
    LOGI("Calling: sub_518B10");
    return old_sub_518B10(a1);
}
bool (*old_sub_6591E0) (int a1);
bool  sub_6591E0 (int a1) {
    LOGI("Calling: sub_6591E0");
    return old_sub_6591E0(a1);
}

void ImGuiImpl(){
   // startTime = currentTimeInMilliseconds();
    //screenWidth = 2340;
    //screenHeight = 1080;
    setupGraphics( screenWidth, screenHeight);
    auto p_glDrawElements = (const void*(*)(...))dlsym(eglHandle, "eglSwapBuffers");
    const char *dlsym_error = dlerror();
    if(dlsym_error){
        LOGE("Cannot load symbol 'eglSwapBuffers': %s", dlsym_error);
        return;
    }else{
        MSHookFunction(reinterpret_cast<void*>(p_glDrawElements), reinterpret_cast<void*>(new_eglSwapBuffers), reinterpret_cast<void**>(&old_eglSwapBuffers));
    }
}

int (*old_DiffDataInvis)(void * , int );
int DiffDataInvis(void * , int ){
    return 4;
}
void HookMods(){

   // LogShaders();
    Wireframe();
    //Initialize IL2CPP Methods to non-offset use.
    il2cpp_resolve_icall_0 = (void*(*)(const char *)) dlsym(get_il2cpp(), "il2cpp_resolve_icall");

    const char *dlsym_error = dlerror();
    if(dlsym_error){
        LOGE("Cannot load symbol 'il2cpp_resolve_icall_0': %s. Using offset instead", dlsym_error);
        /*
         * If there is a dlsym error, its means the device is trying to search other method (Example: Bluestacks 5)
         * So we use offset for it instead.
         *
         * Not
         * il2cpp_resolve_icall_0 = (void*(*)(const char *)) dlsym(get_il2cpp(), "il2cpp_resolve_icall");
         * Just
         *    il2cpp_resolve_icall_0 = (void*(*)(const char *)) getAbsoluteAddress(targetLibName, 0x270D94);
         *    0x270D94 is offset to il2cpp_resolve_icall_0.

         */
          il2cpp_resolve_icall_0 = (void*(*)(const char *)) getAbsoluteAddress(targetLibName, 0x270D94);

    }

    //Game Utilities
    SetQualityLevel= (void(*)(int, bool)) getAbsoluteAddress(targetLibName, 0x94EDC8);

    StartCoroutine = (void*(*)(void*, void*)) getAbsoluteAddress(targetLibName, 0x94820C);
    //Camera_main  =  (void *(*)()) getAbsoluteAddress(targetLibName, 0x962630);
   // set_orthographic  =  (void(*)(void*, bool)) getAbsoluteAddress(targetLibName, 0x960E4C);
    //  set_useGravity  =  (void(*)(void*, bool)) getAbsoluteAddress(targetLibName, 0xD0DDA8);
    WorldToScreenPoint =  (Vector3(*)(void*, Vector3)) getAbsoluteAddress(targetLibName, 0x961F8C);
   // set_fieldOfView  =  (void(*)(void*, float)) getAbsoluteAddress(targetLibName, 0x960BF4);
   // set_orthographicSize =  (void(*)(void*, float )) getAbsoluteAddress(targetLibName, 0x960DA4 );
   // set_nearClipPlane =  (void(*)(void*, float )) getAbsoluteAddress(targetLibName, 0x960AA4 );
  //  set_aspect =  (void(*)(void*, float )) getAbsoluteAddress(targetLibName, 0x960F44 );
    IgnoreLayerCollision =  (void(*)(int,int,bool )) getAbsoluteAddress(targetLibName, 0xD099A4 );
    get_gameObject  =  (void *(*)(void*)) getAbsoluteAddress(targetLibName, 0x96444C);
    get_transform  =  (void *(*)(void*)) getAbsoluteAddress(targetLibName, 0x96A55C);
    get_forward  =  (Vector3(*)(void*)) getAbsoluteAddress(targetLibName, 0xAE0310);
    get_right  =  (Vector3(*)(void*)) getAbsoluteAddress(targetLibName, 0xADFF38);
    set_timeScale =  (void (*)(float)) getAbsoluteAddress(targetLibName, 0xADEB1C);
    set_pos  =  (void(*)(void*, Vector3)) getAbsoluteAddress(targetLibName, 0xADF850);
    get_pos   =  (Vector3(*)(void*)) getAbsoluteAddress(targetLibName, 0xADF778);
    set_enabled   =  (void(*)(void*, bool)) getAbsoluteAddress(targetLibName, 0xD08D70);
   //CharacterController_set_radius = (void (*)(void *, float ))getAbsoluteAddress("libil2cpp.so" ,0xEAB82C); // CharacterController$$set_radius

    //Game
    MSHookFunction((void *) (int (*)())il2cpp_getMethod("UnityEngine.PlayerPrefs::GetInt(System.String,System.Int32)"),
                   (void *) DiffDataInvis,
                   (void **) &old_DiffDataInvis);
    MSHookFunction((void *) getAbsoluteAddress(targetLibName, 0x698E44),
                   (void *) BearTrap_OnTriggerEnter, (void **) &old_BearTrap_OnTriggerEnter);
   // MSHookFunction((void *) getAbsoluteAddress(targetLibName, 0x59E894),
         //          (void *) EnemyAIGranny_FixedUpdate, (void **) &old_EnemyAIGranny_FixedUpdate);
 //   MSHookFunction((void *) getAbsoluteAddress(targetLibName, 0x408718),
       //            (void *) EnemyAIGrandpa_FixedUpdate, (void **) &old_EnemyAIGrandpa_FixedUpdate);
    MSHookFunction((void *) getAbsoluteAddress(targetLibName, 0x5A6664), (void *) EnemyAIGranny_PC,
                   (void **) &old_EnemyAIGranny_PC);
    MSHookFunction((void *) getAbsoluteAddress(targetLibName, 0x4116CC), (void *) EnemyAIGrandpa_PC,
                   (void **) &old_EnemyAIGrandpa_PC);

    MSHookFunction((void *) getAbsoluteAddress(targetLibName, 0x5A7B24),
                   (void *) Slendrina_FixedUpdate,
                   (void **) &old_Slendrina_FixedUpdate);

    MSHookFunction((void *) getAbsoluteAddress(targetLibName, 0x5AF7F0),
                   (void *) FPSControllerNEW_FixedUpdate,
                   (void **) &old_FPSControllerNEW_FixedUpdate);

}

void* thread(void* obj){
    //Initialize ESP Enemies.
    ESP.Init();

    glHandle = get_GL();
    eglHandle = get_EGL();
    //ImGuiImpl();
    auto fp_eglNewSurface = (const void*(*)(...))dlsym(eglHandle, "eglCreateWindowSurface");
    const char *dlsym_error1 = dlerror();
    if(dlsym_error1){
        LOGE("Cannot load symbol 'eglCreateWindowSurface': %s", dlsym_error1);
    }else{
        MSHookFunction(reinterpret_cast<void*>(fp_eglNewSurface), reinterpret_cast<void*>(new_eglCreateWindowSurface), reinterpret_cast<void**>(&old_eglCreateWindowSurface));
    }
    //RequestToast((currentLang == 0 ? OBFUSCATE("Библиотека игры успешно загружена.") : OBFUSCATE("Game Library Is Loaded.")));
    int attempts = 1;
    bool tired = false;
    do {
        sleep(1);
        if(attempts == 10){
       //     RequestToast((currentLang == 0 ? ("Ошибка при загрузке библиотеки игры. Функции могут не работать или работать неправильно.") : OBFUSCATE("Unable to load Game Library. Menu FeaturesStates can work incorectly.")));
        }
        if(attempts == 100) {
     //       RequestToast((currentLang == 0 ? (
       //             "Количество попыток загрузки библиотеки игры преввысило 100. Не вижу смысла ожидать.")
      //                                     : (
      //                      "Unable to load Game Library. Attempts is >= 100. Stopping Thread.")));

            pthread_exit(NULL);
        }
        attempts++;
    } while (!isLibraryLoaded(targetLibName) && true);
    sleep(2); // Il2cpp can be not initialized

    //RequestToast((currentLang == 0 ? ("Библиотека игры успешно загружена.") : ("Game Library Is Loaded.")));
    sleep(3);
    try {

        HookMods();
    } catch (void*e) {
        int err = attempts + 5 + hookCount;
      //  RequestToast((currentLang == 0 ?  std::string(std::string(("Неизвестная ошибка: ")) + std::to_string(err)).c_str() : std::string(std::string(("Unknown Error: ")) + std::to_string(err)).c_str()));

    }
    return nullptr;
}

void ozME_StartThread(){

    pthread_t ptid;
    pthread_create(&ptid, NULL, thread, NULL);
}

void InitMods(){
    ozME_StartThread();
}