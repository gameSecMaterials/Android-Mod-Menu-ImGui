#include "Mods.h"

#define DO_API(r, n, p) auto n = (r (*) p)dlsym(get_il2cpp(),#n)
typedef struct Il2CppType Il2CppType;
typedef struct Il2CppAssembly Il2CppAssembly;
typedef struct Il2CppClass Il2CppClass;
typedef std::vector<const Il2CppClass*> TypeVector;
typedef struct MethodInfo MethodInfo;
typedef struct Il2CppDomain Il2CppDomain;
typedef struct Il2CppAppDomain Il2CppAppDomain;
typedef struct Il2CppImage Il2CppImage;
typedef struct Il2CppAppDomainSetup Il2CppAppDomainSetup;
typedef struct Il2CppObject Il2CppObject;
typedef struct MonitorData MonitorData;
typedef struct Il2CppNameToTypeDefinitionIndexHashTable Il2CppNameToTypeDefinitionIndexHashTable;
typedef struct Il2CppObject
{
    Il2CppClass *klass;
    MonitorData *monitor;
} Il2CppObject;
typedef struct Il2CppImage
{
    const char* name;
    const char *nameNoExt;
    int32_t assemblyIndex;
    int32_t typeStart;
    uint32_t typeCount;
    int32_t exportedTypeStart;
    uint32_t exportedTypeCount;
    int32_t entryPointIndex;
    Il2CppNameToTypeDefinitionIndexHashTable* nameToClassHashTable;
    uint32_t token;
} Il2CppImage;
typedef struct Il2CppAppContext
{
    Il2CppObject obj;
    int32_t domain_id;
    int32_t context_id;
    void* static_data;
} Il2CppAppContext;
typedef struct Il2CppDomain
{
    Il2CppAppDomain* domain;
    Il2CppAppDomainSetup* setup;
    Il2CppAppContext* default_context;
    const char* friendly_name;
    uint32_t domain_id;
    volatile int threadpool_jobs;
} Il2CppDomain;
typedef struct Il2CppAssemblyName
{
    int32_t nameIndex;
    int32_t cultureIndex;
    int32_t hashValueIndex;
    int32_t publicKeyIndex;
    uint32_t hash_alg;
    int32_t hash_len;
    uint32_t flags;
    int32_t major;
    int32_t minor;
    int32_t build;
    int32_t revision;
    uint8_t publicKeyToken[8];
} Il2CppAssemblyName;
typedef struct Il2CppAssembly
{
    int32_t imageIndex;
    int32_t customAttributeIndex;
    int32_t referencedAssemblyStart;
    int32_t referencedAssemblyCount;
    Il2CppAssemblyName aname;
} Il2CppAssembly;
typedef struct Il2CppArrayType
{
    const Il2CppType* etype;
    uint8_t rank;
    uint8_t numsizes;
    uint8_t numlobounds;
    int *sizes;
    int *lobounds;
} Il2CppArrayType;
typedef struct Il2CppGenericInst
{
    uint32_t type_argc;
    const Il2CppType **type_argv;
} Il2CppGenericInst;

typedef struct Il2CppGenericContext
{
    const Il2CppGenericInst *class_inst;
    const Il2CppGenericInst *method_inst;
} Il2CppGenericContext;
typedef void(*Il2CppMethodPointer)();
typedef struct VirtualInvokeData
{
    Il2CppMethodPointer methodPtr;
    const MethodInfo* method;
} VirtualInvokeData;

typedef struct Il2CppClass
{
    const Il2CppImage* image;
    void* gc_desc;
    const char* name;
    const char* namespaze;
    Il2CppType byval_arg;
    Il2CppType this_arg;
    Il2CppClass* element_class;
    Il2CppClass* castClass;
    Il2CppClass* declaringType;
    Il2CppClass* parent;
    Il2CppGenericClass *generic_class;
    Il2CppMetadataTypeHandle typeMetadataHandle;
    const Il2CppInteropData* interopData;
    Il2CppClass* klass;
    FieldInfo* fields;
    const EventInfo* events;
    const PropertyInfo* properties;
    const MethodInfo** methods;
    Il2CppClass** nestedTypes;
    Il2CppClass** implementedInterfaces;
    Il2CppRuntimeInterfaceOffsetPair* interfaceOffsets;
    void* static_fields;
    const void* rgctx_data;
    struct Il2CppClass** typeHierarchy;
    void *unity_user_data;
    uint32_t initializationExceptionGCHandle;
    uint32_t cctor_started;
    uint32_t cctor_finished;
    __attribute__((aligned(8))) size_t cctor_thread;
    void* genericContainerHandle;
    uint32_t instance_size;
    uint32_t actualSize;
    uint32_t element_size;
    int32_t native_size;
    uint32_t static_fields_size;
    uint32_t thread_static_fields_size;
    int32_t thread_static_fields_offset;
    uint32_t flags;
    uint32_t token;
    uint16_t method_count;
    uint16_t property_count;
    uint16_t field_count;
    uint16_t event_count;
    uint16_t nested_type_count;
    uint16_t vtable_count;
    uint16_t interfaces_count;
    uint16_t interface_offsets_count;
    uint8_t typeHierarchyDepth;
    uint8_t genericRecursionDepth;
    uint8_t rank;
    uint8_t minimumAlignment;
    uint8_t naturalAligment;
    uint8_t packingSize;
    uint8_t initialized_and_no_error : 1;
    uint8_t valuetype : 1;
    uint8_t initialized : 1;
    uint8_t enumtype : 1;
    uint8_t is_generic : 1;
    uint8_t has_references : 1;
    uint8_t init_pending : 1;
    uint8_t size_inited : 1;
    uint8_t has_finalize : 1;
    uint8_t has_cctor : 1;
    uint8_t is_blittable : 1;
    uint8_t is_import_or_windows_runtime : 1;
    uint8_t is_vtable_initialized : 1;
    uint8_t has_initialization_error : 1;
    VirtualInvokeData vtable[32];
} Il2CppClass;
typedef struct Il2CppGenericClass
{
    const Il2CppType* type;
    Il2CppGenericContext context;
    Il2CppClass *cached_class;
} Il2CppGenericClass;
typedef enum Il2CppTypeEnum
{
    IL2CPP_TYPE_END = 0x00,
    IL2CPP_TYPE_VOID = 0x01,
    IL2CPP_TYPE_BOOLEAN = 0x02,
    IL2CPP_TYPE_CHAR = 0x03,
    IL2CPP_TYPE_I1 = 0x04,
    IL2CPP_TYPE_U1 = 0x05,
    IL2CPP_TYPE_I2 = 0x06,
    IL2CPP_TYPE_U2 = 0x07,
    IL2CPP_TYPE_I4 = 0x08,
    IL2CPP_TYPE_U4 = 0x09,
    IL2CPP_TYPE_I8 = 0x0a,
    IL2CPP_TYPE_U8 = 0x0b,
    IL2CPP_TYPE_R4 = 0x0c,
    IL2CPP_TYPE_R8 = 0x0d,
    IL2CPP_TYPE_STRING = 0x0e,
    IL2CPP_TYPE_PTR = 0x0f,
    IL2CPP_TYPE_BYREF = 0x10,
    IL2CPP_TYPE_VALUETYPE = 0x11,
    IL2CPP_TYPE_CLASS = 0x12,
    IL2CPP_TYPE_VAR = 0x13,
    IL2CPP_TYPE_ARRAY = 0x14,
    IL2CPP_TYPE_GENERICINST = 0x15,
    IL2CPP_TYPE_TYPEDBYREF = 0x16,
    IL2CPP_TYPE_I = 0x18,
    IL2CPP_TYPE_U = 0x19,
    IL2CPP_TYPE_FNPTR = 0x1b,
    IL2CPP_TYPE_OBJECT = 0x1c,
    IL2CPP_TYPE_SZARRAY = 0x1d,
    IL2CPP_TYPE_MVAR = 0x1e,
    IL2CPP_TYPE_CMOD_REQD = 0x1f,
    IL2CPP_TYPE_CMOD_OPT = 0x20,
    IL2CPP_TYPE_INTERNAL = 0x21,
    IL2CPP_TYPE_MODIFIER = 0x40,
    IL2CPP_TYPE_SENTINEL = 0x41,
    IL2CPP_TYPE_PINNED = 0x45,
    IL2CPP_TYPE_ENUM = 0x55
} Il2CppTypeEnum;

typedef const struct ___Il2CppMetadataTypeHandle* Il2CppMetadataTypeHandle;
typedef const struct ___Il2CppMetadataGenericParameterHandle* Il2CppMetadataGenericParameterHandle;
typedef struct Il2CppType
{
    union
    {
        void* dummy;
        int32_t __klassIndex;
        Il2CppMetadataTypeHandle typeHandle;
        const Il2CppType *type;
            Il2CppArrayType *array;
        int32_t __genericParameterIndex;
        Il2CppMetadataGenericParameterHandle genericParameterHandle;
        Il2CppGenericClass *generic_class;
    } data;
    unsigned int attrs : 16;
    Il2CppTypeEnum type : 8;
    unsigned int num_mods : 6;
    unsigned int byref : 1;
    unsigned int pinned : 1;
} Il2CppType;

void (*Image$$GetTypes_t)(const Il2CppImage* image, bool exportedOnly_UNUSED_IN_IL2CPP_SRC, TypeVector* target);

std::string readHexStrFromMem(const void *addr, size_t len) {
    char temp[len];
    memset(temp, 0, len);
    const size_t bufferLen = len * 2 + 1;
    char buffer[bufferLen];
    memset(buffer, 0, bufferLen);
    std::string ret;
    if (memcpy(temp, addr, len) == NULL)
        return ret;
    for (int i = 0; i < len; i++) {
        sprintf(&buffer[i * 2], ("%02X"), (unsigned char) temp[i]);
    }
    ret += buffer;
    return ret;
}
/*
 * Branch decoding based on
 * https://github.com/aquynh/capstone/
*/
std::string revhexstr(std::string hex) {
    std::string out;
    for (unsigned int i = 0; i < hex.length(); i += 2) out = hex.substr(i, 2) + out;
    return out;
}
std::string fixhex(std::string str) {
    std::string out = str;
    std::string::size_type tmp;
    if (out.find(std::string(("0x"))) != -1) {
        tmp = out.find(std::string(("0x")));
        out.replace(tmp, 2, std::string(("")));
    }
    for (int i = out.length() - 1; i >= 0; --i) {
        if (out[i] == ' ')
            out.erase(i, 1);
    }
    return out;
}
unsigned int HexStr2DWORD(std::string hex) {
    return strtoull(hex.c_str(), NULL, 16);
}
bool Is_B_BL_Hex_arm64(std::string hex) {
    unsigned int hexw = HexStr2DWORD(revhexstr(fixhex(hex)));
    return (hexw & 0xFC000000) == 0x14000000 || (hexw & 0xFC000000) == 0x94000000;
}
bool Decode_Branch_or_Call_Hex(std::string hex, unsigned int offset, unsigned int *outOffset) {
    bool arm = false;

    unsigned int insn = HexStr2DWORD(revhexstr(fixhex(hex)));
    unsigned int imm = ((insn & (((uint32_t)1 << 24) - 1) << 0) >> 0) << 2;
    unsigned int SignExtendedImm = (int32_t)(imm << (32 - 26)) >> (32 - 26);
        *outOffset = SignExtendedImm + offset + (Is_B_BL_Hex_arm64(hex) ? 0 : 8);

    return true;
}
unsigned int FindNext_B_BL_offset(unsigned int start, int index = 1) {

    int offset = 0;
    std::string curHex = readHexStrFromMem((const void*)start, 4);
    unsigned int outOffset = 0;
    bool out = true;
    while (!(out = Decode_Branch_or_Call_Hex(curHex, start + offset, &outOffset)) || index != 1) {
        offset += 4;
        curHex = readHexStrFromMem((const void*)(start + offset), 4);
        if (out)
            index--;
    }
    return outOffset;

}
void Image$$GetTypes(const Il2CppImage* image, bool exportedOnly_UNUSED_IN_IL2CPP_SRC, TypeVector* target){
    if (!Image$$GetTypes_t){
        DO_API(const Il2CppImage*, il2cpp_get_corlib, ());
        DO_API(Il2CppClass*, il2cpp_class_from_name, (const Il2CppImage * image, const char* namespaze, const char *name));
        DO_API(const MethodInfo*, il2cpp_class_get_method_from_name, (Il2CppClass * klass, const char* name, int argsCount));
        auto corlib = il2cpp_get_corlib();
        Dl_info info;
        dladdr((void*)il2cpp_get_corlib, &info);
        unsigned long startAddr = (unsigned long)info.dli_fbase;
        auto assemblyClass = il2cpp_class_from_name(corlib, ("System.Reflection"), ("Assembly"));
        unsigned long GetTypesAdr = (unsigned long) il2cpp_class_get_method_from_name(assemblyClass, ("GetTypes"), 1)->methodPointer;
        Image$$GetTypes_t = (void (*)(const Il2CppImage *, bool, TypeVector *))FindNext_B_BL_offset(FindNext_B_BL_offset(FindNext_B_BL_offset(GetTypesAdr, 1), 1+1), 1);

    }

    Image$$GetTypes_t(image, exportedOnly_UNUSED_IN_IL2CPP_SRC, target);
}
Il2CppClass *outputClass = nullptr;
void LoadClass(std::string _namespace, std::string _name, std::string dllname) {
    Il2CppClass *thisclass = nullptr;

    DO_API(const Il2CppAssembly*, il2cpp_domain_assembly_open, (Il2CppDomain * domain, const char *name));
    DO_API(Il2CppDomain*, il2cpp_domain_get, ());
    DO_API(const Il2CppImage*, il2cpp_assembly_get_image, (const Il2CppAssembly * assembly));
    Il2CppImage *dll = il2cpp_assembly_get_image(il2cpp_domain_assembly_open(il2cpp_domain_get(), dllname.c_str()));
    int try_count = 0;
    do {
        auto dom = il2cpp_domain_get();
        if (dom) {
            TypeVector clases;
            Image$$GetTypes(dll, false, &clases);
            for (const Il2CppClass cls : clases){
                if (!cls) continue;
                if (CheckObj(cls->name) == _name && CheckObj(cls->namespaze) == _namespace){
                    thisclass = (Il2CppClass*)cls;
                }
            }
        }

        if (try_count == 10 && !thisclass) {
            LOGI("Can't find that class!");
            return;
        } else if (!thisclass) {
            try_count++;
        }
    } while (!thisclass);
    outputClass = thisclass;
    if (!outputClass){
        LOGI("Can't find that class!");
    }
}
