//
// Created by ozMod on 27.11.2021.
//

#include "Dobby-master/include/dobby.h"

#define targetLibName ozObfuscate("libil2cpp.so")

void *get_il2cpp() {
    void *mod = 0;
    while (!mod) {
        mod = dlopen(targetLibName, RTLD_LAZY);
    }

    return mod;
}



int hookCount = 0;
struct ESPEnemy {
    bool ESP = true;
    const char * name = {0};
    int id = -1;
    ImColor color;
    void * object;
    float boxSize;
    ImVec2 screenPosition;
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
    bool playAs;
    bool active, destroy,destroyed;
    bool setActive;
    bool tpMe2Enemy,tpEnemy2Me;
    bool enemyKillPlayer;
    bool userDoSetKillDistance, userDoSetWidth, userDoSetHeight, userDoSetSize;
    float killDist;
    bool invis, godmod;
};
struct EnemyEditor {
    std::vector<Enemy *> enemies;
    int lastEnemyId = -1;
    Enemy * AddEnemy(){
        Enemy * enemy = new Enemy();
        lastEnemyId ++;
        enemy->id = lastEnemyId;
        enemies.push_back(enemy);
        return enemy;
    }
    void Init(){

        Enemy * enemy = AddEnemy();
        enemy->name = ozObfuscate("Granny");
        enemy->color = ImColor(255, 0,0,255);
        Enemy * enemy1 = AddEnemy();
        enemy1->name = ozObfuscate("Grandpa");
        enemy1->color = ImColor(0, 255,0,255);
        Enemy * enemy2 = AddEnemy();
        enemy2->name = ozObfuscate("Slendrina");
        enemy2->color = ImColor(0, 0,255,255);

    }
}EnemyEditor;
int selectedEnemyESP;
ESPEnemy* selectedEnemyESPObj;
int selectedEnemyGame;
Enemy* selectedEnemyObj;
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
        enemy->name = ozObfuscate("Granny");
        enemy->color = ImColor(255, 0,0,255);
        ESPEnemy * enemy1 = AddEnemy();
        enemy1->name = ozObfuscate("Grandpa");
        enemy1->color = ImColor(0, 255,0,255);
        ESPEnemy * enemy2 = AddEnemy();
        enemy2->name = ozObfuscate("Slendrina");
        enemy2->color = ImColor(0, 0,255,255);
    }

}ESP;
struct GrannyItemPointers{
    void * accelerator;
    void * bridgecrank;
    void* coconut;
    void* coin;
    void* crowbar;
    void* doorActivator;
    void*electricSwitch;
    void*firewood;
    void*gatefuse;
    void*gate;
    void*gencable;
    void*matches;
    void*padlockkey;
    void*planka;
    void*safekey;
    void*shedkey;
    void*shotgun;
    void*slingshot;
    void*teddy;
    void*trainKey;
    void*vas;
    void*vas2;
    void*weaponkey;

}Items;
bool fog, fpsbypass, wireframe, clrfilter, clrfilterR, clrfilterG, clrfilterB;
ImColor clf, wfClr;
float wfLW = 1;
bool changeForwSpeed, changeBackSpeed;
bool fly,igncol, ignbt;
float gravity, frSpeed, bckSpeed, fov = 60, ortoSize = 3, nearCF = 1, camAspect = 1.889f, timescale = 1, fogd = 1;
bool allCantAttack, grCantAttack, grpCantAttack, crCantAttack, slCantAttack;
bool allInvis, grInvis, grpInvis, crInvis, slInvis;
bool igntdy;
bool ortoCam;
bool savePlayerY;
float playerPos[3] = {0.0f, 0.0f, 0.0f},playerPosTp[3] = {0.0f, 0.0f, 0.0f};
int renderFunction=3;
bool dontClear = false;
char sceneToLoad = {0};
int placetoteleport = -1;
bool teleportCrdCommit = false;
bool teleportPlaceCommit = false;
bool ItemSpawnCommit = false;
int ItemToSpawn = -1;
float playerY = 0;
struct Vec2 //Эта структура отлично справляется с получением координат джойстика.  Тот Vector2 херня - он не работает!)
{
    float x, y;
};
struct Vector3{
    float x,y,z;

    Vector3(float x, float y, float z) : x(x), y(y), z(z) {

    }

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

struct Quaternion {
            float x;
            float y;
            float z;
            float w;
}
struct monoString {
    void *klass;
    void *monitor;
    int length;
    char chars[255];

    std::string get_string() {
        std::u16string u16_string(reinterpret_cast<const char16_t *>(chars));
        std::wstring wide_string(u16_string.begin(), u16_string.end());
        std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> convert;
        return convert.to_bytes(wide_string);
    }

    void set(std::string str) {
        std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> convert;
        std::wstring wa = convert.from_bytes(str);
        length = str.length();
        std::u16string basicString = std::u16string(wa.begin(), wa.end());
        const char16_t *cStr = basicString.c_str();
        memcpy(getChars(), cStr, getLength() * 2);
    }

    const char *get_const_char() {
        return get_string().c_str();
    }

    char *getChars() {
        return chars;
    }

    int getLength() {
        return length;
    }

};


monoString* new_il2cpp_string(const char * cstr){
    monoString *(*CreateIl2CppString_t)(const char *);
    do {
        CreateIl2CppString_t = (monoString *(*)(const char *)) dlsym(get_il2cpp(), "il2cpp_string_new_wrapper");
    } while (!CreateIl2CppString_t);
    return CreateIl2CppString_t(cstr);
}
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
void *(*Instantiate)(void*, Vector3 pos, Quaternion rot);
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
    ((void (*)(void*, bool))il2cpp_getMethod(ozObfuscate("UnityEngine.Camera::set_orthographic(System.Boolean)")))(instance, value);
}
//Signature
void set_fieldOfView(void* instance, float value){
    if(!il2cpp_resolve_icall_0){
        return;
    }
                ((void (*)(void*, float))il2cpp_getMethod( ozObfuscate("UnityEngine.Camera::set_fieldOfView(System.Single)")))(instance, value);
}

void set_orthographicSize(void*instance, float value){
    if(!il2cpp_resolve_icall_0){
        return;
    }
    ((void (*)(void*, float))il2cpp_getMethod( ozObfuscate("UnityEngine.Camera::set_orthographicSize(System.Single)")))(instance, value);

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
void SetFrameRateLimit(int v){
    void (*result)(int);
    if(!il2cpp_resolve_icall_0){
        return;
    }
    result = (void (*)(int))il2cpp_getMethod("UnityEngine.Application::set_targetFrameRate(System.Int32)");
    if(result == nullptr){
        return;
    }
    return result(v);
}
void SetFOG(bool v){
    void (*result)(bool);
    if(!il2cpp_resolve_icall_0){
        return;
    }
    result = (void (*)(bool))il2cpp_getMethod("UnityEngine.RenderSettings::set_fog(System.Boolean)");
    if(result == nullptr){
        return;
    }
    return result(v);
}
void SetFOGDist(float v){
    void (*result)(float);
    if(!il2cpp_resolve_icall_0){
        return;
    }
    result = (void (*)(float))il2cpp_getMethod("UnityEngine.RenderSettings::set_fogEndDistance(System.Single)");
    if(result == nullptr){
        return;
    }
    return result(v);
}
void (*Destroy)(void* obj);
void (*SetApplicationAvg)(int val); //2000 - no limit //Method Signature: UnityEngine.Application::set_targetFrameRate(System.Int32)
void (*SetActive)(void*,bool value);
void *(*StartCoroutine)(void * t, void *);
void *(*get_gameObject)(void * t);
void (*SetQualityLevel)(int, bool);
void *(*LoadScene)(monoString*);
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
            enemy->screenPosition = ImVec2( fm.x - 150, (screenHeight - fm.y) - (30+400) );


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
            enemy->screenPosition = ImVec2( fm.x - 150 , (screenHeight - fm.y) - (30+400) );


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
            } //  проверка чтоб не рисовало за экраном

            //enemy->screenPosition = ImVec2(-999, -999);
            enemy->distance = UnityEngine_Vector3__Distance(cpos, cposG);
            enemy->screenPosition = ImVec2( fm.x - 150, (screenHeight - fm.y) - (30+400) );

        }

    }
}
void (*old_FPSControllerNEW_FixedUpdate)(void *instance);
void FPSControllerNEW_FixedUpdate(void *instance) {
    LOGI("Frame Player");
    if(instance == nullptr){
        return;
    }

    void *player = *(void **) ((uint64_t) instance + 0x80); //Объект игрока
    void *granny = *(void **) ((uint64_t) instance + 0xA0); //Объект бабки)
    void *grandpa = *(void **) ((uint64_t) instance + 0xA4); //Объект деда) )

   if(player){
       void *plyrTransform = get_transform(player);
       if (plyrTransform) {
           Vector3 posn = get_pos(plyrTransform);
           if (teleportCrdCommit) {
               LOGI("Wanna to move player");
               posn.x = playerPosTp[0];
               posn.y = playerPosTp[1];
               posn.z = playerPosTp[2];
               set_pos(plyrTransform, posn);
               teleportCrdCommit = false;
           }
           if(teleportPlaceCommit){
               Vector3 target = Vector3(0.0f, 0.0f, 0.0f);
               switch(placetoteleport){
                   case 0:
                       target.x = 10.810f;
                       target.y = -5.538;
                       target.z = 17.600;
                   break;
                   case 1:
                       target.x = 11.810001f;
                       target.y = -12.538391;
                       target.z = 17.600;
                       break;
                   case 2:
                       target.x = 0.522f;
                       target.y = 5;
                       target.z = 40;
                       break;
                   case 3:
                       target.x = 22.889f;
                       target.y = -16.162;
                       target.z = -0.053;
                       break;
                   case 4:
                       target.x = -8.307f;
                       target.y = 14.511;
                       target.z = 2.444;
                       break;
               }
               set_pos(plyrTransform, target);
               teleportPlaceCommit = false;
           }
           playerPos[0] = posn.x;
           playerPos[1] = posn.y;
           playerPos[2] = posn.z;
           playerPosTp[0] = posn.x;
           playerPosTp[1] = posn.y;
           playerPosTp[2] = posn.z;
       }
   }

    Enemy* enemy = EnemyEditor.enemies[0];
    Enemy* enemy1 = EnemyEditor.enemies[1];
    if(enemy){
        if(enemy->setActive){
            if(granny){
                SetActive(granny, enemy->active);
            }
            enemy->setActive = false;
        }

        if(enemy->tpEnemy2Me){
            if(player){
                LOGW("Very Cool that player is found");
                if( granny){
                    LOGW("Very Cool that Granny found");
                    void*Granny_transform = get_transform(granny);
                    void*Player_transform = get_transform(player);
                    if(Granny_transform){
                        LOGW("Why it not teleport to me then");
                        set_pos(Player_transform,get_pos(Granny_transform));
                    }
                }

            }
            enemy->tpEnemy2Me = false;
        }
        if(enemy->tpMe2Enemy){

            if(player){

                if(granny){
                    void*Granny_transform = get_transform(granny);
                     void*Player_transform = get_transform(player);
                    if(Granny_transform){
                        set_pos(Player_transform,get_pos(Granny_transform));
                    }

                }
            }
            enemy->tpMe2Enemy = false;
        }
    }
    if(enemy1){
        if(enemy1->setActive){
            if(grandpa){
                SetActive(grandpa, enemy1->active);
            }
            enemy1->setActive = false;
        }

        if(enemy1->tpEnemy2Me){
            if(player){
                LOGW("Very Cool that player is found");
                if( grandpa){
                    LOGW("Very Cool that Granny found");
                    void*Granny_transform = get_transform(grandpa);
                    void*Player_transform = get_transform(player);
                    if(Granny_transform){
                        LOGW("Why it not teleport to me then");
                        set_pos(Player_transform,get_pos(Granny_transform));
                    }
                }

            }
            enemy1->tpEnemy2Me = false;
        }
        if(enemy1->tpMe2Enemy){

            if(player){

                if(grandpa){
                    void*Granny_transform = get_transform(grandpa);
                    void*Player_transform = get_transform(player);
                    if(Granny_transform){
                        set_pos(Player_transform,get_pos(Granny_transform));
                    }
                }
            }
            enemy1->tpMe2Enemy = false;
        }
    }
    if(teleportPlaceCommit){
        LOGD("Commit Teleport Place: %d", placetoteleport)
        teleportPlaceCommit = false;
    }
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
        if(UnityEngine_Vector3__Distance(cpos, cposG) <= 5){}
    }
    if(ESP.ESP){
        GrannyESP(granny, player);
        GrandpaESP(grandpa, player);
    }
    old_FPSControllerNEW_FixedUpdate(instance); //Вызвать функцию игры
    if(savePlayerY){
        if(player){
            playerY = get_pos(get_transform(get_gameObject(player))).y;
        }
        savePlayerY = false;
    }
    if(igncol && playerY != 0){
        Vector3 freezeYPos = get_pos(get_transform(player));
        freezeYPos.y = playerY;
        set_pos(get_transform(player), freezeYPos);
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
            playerPosition = playerPosition + get_right(get_transform(get_gameObject(cam))) * (x); //новая позиция игрока ///наш Vector3 равный позиции игрока изменен
            set_pos(get_transform(player), playerPosition); //теперь игрок на новой позиции
        }

        if (y != 0) {
            float nocSpeedRatio = (y < 0 ? backwardSpeed : forwardSpeed);
            Vector3 playerPosition = get_pos(
                    get_transform(player)); ///сначала просто получаем координаты игрока
            playerPosition = playerPosition +  get_forward(get_transform(get_gameObject(cam))) * (y); //новая позиция игрока ///наш Vector3 равный позиции игрока изменен
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
void (*old_Granny_FixedUpdate)(void *instance);

void Granny_FixedUpdate(void *instance) {

    if (instance == nullptr) {

       return;
    }
    Enemy* enemy = EnemyEditor.enemies[0];
    if(enemy){
       if(enemy->enemyKillPlayer){
           *(bool *) ((uint64_t) instance + 0x100) = true;
           *(bool *) ((uint64_t) instance + 0xED) = false;
           StartCoroutine(instance, old_EnemyAIGranny_PC(instance));
           enemy->enemyKillPlayer = false;
       }


        if(enemy->destroy){

            void*Granny = get_gameObject(instance);
            if(Granny){
               Destroy(Granny);
                enemy->destroyed = true;
            }
            enemy->destroy = false;
        }
    }
    if (grInvis || allInvis){
        *(bool *) ((uint64_t) instance + 0xA4) = false;
    }
    LOGI("Granny_FixedUpdate");
        old_Granny_FixedUpdate(instance);

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
    if(((count < 2123 || count > 3000) /* Slendrina macros*/ && chamsSlendrina) && !wireframe && !clrfilter && mode == GL_TRIANGLES){
        return;
    }
    if (wireframe || chamsSlendrina) {
        mode = GL_LINES;
        glLineWidth(wfLW);
        glDepthRangef(1, 0.5);
        glColorMask(1, 0, 0, 0);
        glEnable(GL_BLEND);
        glDisable(GL_TEXTURE_3D);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE);
    }
         //restore gl state
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
        LOGE("Cannot load symbol 'glDrawElements': %s", dlsym_error)
        return;
    }else{
        DobbyHook((void*)(p_glDrawElements), (void*)(new_glDrawElements), (void**)(&old_glDrawElements));
    }
}


void ImGuiImpl(bool swapbuffers = true){

    startTime = currentTimeInMilliseconds();
    screenWidth = 2340;
    screenHeight = 1080;
    setupGraphics( screenWidth, screenHeight);
    if(swapbuffers){/*
        auto swapBuffersSym = (const void*(*)(...))dlsym(eglHandle, "eglSwapBuffers");
        const char *dlsym_error = dlerror();
        if(dlsym_error){
            LOGE("Cannot load symbol 'eglSwapBuffers': %s", dlsym_error);
            return;
        }else{*/

        //}
    }


}


int (*old_DiffDataInvis)(void * , int );
int DiffDataInvis(void * , int ){
    return 4;
}
void (*old_glStencilMask)();
void new_glStencilMask(){
//glClearStencil
    LOGI("Render: glStencilMask (dobby!!)")
    if(renderFunction == 1){
        Render();
    }
}
void (*old_glClear)(GLuint);
void new_glClear(GLuint mask){
//glClearStencil
    LOGI("Render: glClear (dobby!!)")
    if(!dontClear){
        old_glClear(mask);
    }
    if(renderFunction == 3){
        Render();
    }
}
void (*old_signleGLFunc)();
void new_signleGLFunc(){
//glClearStencil
    LOGI("Render: glRenderbufferStorage (dobby!!)")
}
void ImGui_ImplOpenGLHook(){
    void* stencilSym = dlsym(glHandle, "glStencilMask");
    const char *dlsym_error3 = dlerror();
    if(dlsym_error3){
        LOGE("Cannot load symbol 'glStencilMask': %s", dlsym_error3);
    }else {
        DobbyHook((stencilSym), (void*)(new_glStencilMask), (void**)(&old_glStencilMask));
    }

    void* glClearSym = dlsym(glHandle, "glClear"); //glDrawBuffers
    const char *dlsym_error4 = dlerror();
    if(dlsym_error4){
        LOGE("Cannot load symbol 'glClear': %s", dlsym_error4)
    }else {
        DobbyHook((glClearSym), (void*)(new_glClear), (void**)(&old_glClear));
    }
}
//tried
//gl blend equation - not use
//gl depth - bug
//gl clear stencil - behind uui
//gl stencil mask - behind uui
//gl stencil op -- not use
//gl stencil mask seperate - not in use alssooo
//gl draw buffers - behind uui but only in game
void HookMods(){
    selectedEnemyObj = EnemyEditor.enemies[0];
    ImGui_ImplOpenGLHook();
    Wireframe();
    //Initialize IL2CPP Methods to non-offset use.
    il2cpp_resolve_icall_0 = (void*(*)(const char *)) dlsym(get_il2cpp(), "il2cpp_resolve_icall");

    const char *dlsym_error2 = dlerror();
    if(dlsym_error2){
        LOGE("Cannot load symbol 'il2cpp_resolve_icall_0': %s. Using offset instead", dlsym_error2);
        /*
         * If there is a dlsym error, its means the device is trying to search other method (Example: Bluestacks 5)
         * So we use offset for it instead.
         *
         * Not
         * il2cpp_resolve_icall_0 = (void*(*)(const char *)) dlsym(get_il2cpp(), "il2cpp_resolve_icall");
         * Just
         *    il2cpp_resolve_icall_0 = (void*(*)(const char *)) getRealAddrOfLibrary(targetLibName, 0x270D94);
         *    0x270D94 is offset to il2cpp_resolve_icall_0.

         */
          il2cpp_resolve_icall_0 = (void*(*)(const char *)) getRealAddrOfLibrary(targetLibName,0x270D94);
    }
    //Game Utilities
    SetQualityLevel= (void(*)(int, bool)) getRealAddrOfLibrary(targetLibName, 0x94EDC8);
    SetActive = (void(*)(void*, bool)) getRealAddrOfLibrary(targetLibName, 0x96A654);
    Destroy = (void(*)(void*)) getRealAddrOfLibrary(targetLibName, 0x949864);
    StartCoroutine = (void*(*)(void*, void*)) getRealAddrOfLibrary(targetLibName, 0x94820C);
    WorldToScreenPoint =  (Vector3(*)(void*, Vector3)) getRealAddrOfLibrary(targetLibName,0x961F8C);
     IgnoreLayerCollision =  (void(*)(int,int,bool )) getRealAddrOfLibrary(targetLibName,0xD099A4);
    LoadScene = (void*(*)(monoString*)) getRealAddrOfLibrary(targetLibName, 0x95A8D4);
    get_gameObject  =  (void *(*)(void*)) getRealAddrOfLibrary(targetLibName, 0x96444C);
    get_transform  =  (void *(*)(void*)) getRealAddrOfLibrary(targetLibName, 0x96A55C);
    get_forward  =  (Vector3(*)(void*)) getRealAddrOfLibrary(targetLibName, 0xAE0310);
    get_right  =  (Vector3(*)(void*)) getRealAddrOfLibrary(targetLibName, 0xADFF38);
    set_timeScale =  (void (*)(float)) getRealAddrOfLibrary(targetLibName, 0xADEB1C);
    set_pos  =  (void(*)(void*, Vector3)) getRealAddrOfLibrary(targetLibName, 0xADF850);
    get_pos   =  (Vector3(*)(void*)) getRealAddrOfLibrary(targetLibName, 0xADF778);
    set_enabled   =  (void(*)(void*, bool)) getRealAddrOfLibrary(targetLibName, 0xD08D70);
    Instantiate = (void*(*)(void*, Vector3, Quaternion)) getRealAddrOfLibrary(targetLibName, 0x949158);
      MSHookFunction((void *) getRealAddrOfLibrary(targetLibName, 0x698E44),
                   (void *) BearTrap_OnTriggerEnter, (void **) &old_BearTrap_OnTriggerEnter);
   MSHookFunction((void *) getRealAddrOfLibrary(targetLibName, 0x5A6664), (void *) EnemyAIGranny_PC,
                  (void **) &old_EnemyAIGranny_PC);
    MSHookFunction((void *) getRealAddrOfLibrary(targetLibName, 0x4116CC), (void *) EnemyAIGrandpa_PC,
                   (void **) &old_EnemyAIGrandpa_PC);
    MSHookFunction((void *) getRealAddrOfLibrary(targetLibName, 0x5CD124), (void *) SetupItemPtrs,
                   (void **) &old_SetupItemPtrs);
    MSHookFunction((void *) getRealAddrOfLibrary(targetLibName, 0x5A7B24),
                   (void *) Slendrina_FixedUpdate,
                   (void **) &old_Slendrina_FixedUpdate);
    MSHookFunction((void *) getRealAddrOfLibrary(targetLibName, 0x59E894),
                   (void *) Granny_FixedUpdate,
                   (void **) &old_Granny_FixedUpdate);
    MSHookFunction((void *) getRealAddrOfLibrary(targetLibName, 0x5AF7F0),
                   (void *) FPSControllerNEW_FixedUpdate,
                   (void **) &old_FPSControllerNEW_FixedUpdate);

}

void* thread(void* obj){
    //Initialize ESP Enemies.
    ESP.Init();
    EnemyEditor.Init();
    glHandle = get_GL();
    eglHandle = get_EGL();
   // ImGuiImpl(true);
    //RequestToast((currentLang == 0 ? OBFUSCATE("Библиотека игры успешно загружена.") : OBFUSCATE("Game Library Is Loaded.")));
    int attempts = 1;
    bool tired = false;
    do {
        sleep(1);
        if(attempts == 10){

        }
        if(attempts == 100) {
            pthread_exit(NULL);
        }
        attempts++;
    } while (!isLibraryLoaded(targetLibName) && true);
  sleep(3);
  HookMods();

    return nullptr;
}

void ozME_StartThread(){
    pthread_t ptid;
    pthread_create(&ptid, NULL, thread, NULL);
}

void InitMods(){
    ozME_StartThread();
}