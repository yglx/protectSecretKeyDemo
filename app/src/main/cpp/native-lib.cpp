#include <jni.h>
#include <string>
#include <android/log.h>
#include <string.h>
static jclass contextClass;
static jclass signatureClass;
static jclass packageNameClass;
static jclass packageInfoClass;

const char* RELEASE_SIGN = "308203773082025fa00302010202043a1a879c300d06092a864886f70d01010b0500306c3110300e06035504061307556e6b6e6f776e3110300e06035504081307556e6b6e6f776e3110300e06035504071307556e6b6e6f776e3110300e060355040a1307556e6b6e6f776e3110300e060355040b1307556e6b6e6f776e3110300e06035504031307556e6b6e6f776e301e170d3138303531343033333634395a170d3138303831323033333634395a306c3110300e06035504061307556e6b6e6f776e3110300e06035504081307556e6b6e6f776e3110300e06035504071307556e6b6e6f776e3110300e060355040a1307556e6b6e6f776e3110300e060355040b1307556e6b6e6f776e3110300e06035504031307556e6b6e6f776e30820122300d06092a864886f70d01010105000382010f003082010a028201010096d13f2c224138855eb93378d2561f20aed0dfcaec80731ed48a2c2eb4544d5b1b1e28b3227d362c6810127bc4776aac5874b1516681419829ffab6b882709fc9c929c01bb1fd5f73b36120212627c1d35fa4b18b53503b599171416c46a7aa50ac77d8d3ec5ef29d164c26c81d1d2bd56f765d220b0050e8991e2e4397249d354e2b5608b1c8e37d35323d933c684ae836fb17113a351c04aabc79c222d58d4a15e6e81748ea803b7b3f7608046544f5aaef9a8cf9cb06286739b759fd24efa1d82c9617b654746411c44418fe7ebd3538e9220368a1dace1ea6f318899606cd25ec79c34cfadfc76d65bfe250b5ee0e01071d76e4afa73ac6633ab61aabd9b0203010001a321301f301d0603551d0e04160414b7bced8318a7fb3d08b39efe6c2d4fb67f813085300d06092a864886f70d01010b050003820101004022db7b589ce7953648aad4be98c6aec4d7cc938ba630e0611e578051a7badd1b3e755f7d76a82c154fe5f8b3464df81bdf4206b022cc95df6d0d5598ae3333412064b3a3bd5c27deceb2d298d9247d6f6218d3fd6989239a989b5341624cd1cdb5f4de0890cc86ea1cb793e8962f8c7cf01c119a6c59ee3cb1c1a508b1ca502a9a5885031fd7d08ed402b2f1f9656628f6a6d201f898a405880dda62c519c5251eb8351b8e7f6b29106b5d18cb5069d9f4d23bd04c25b116a3da210d05492dcc42c2a70f5398b9aefafeddca5c00d3fb43cc812ea7fecb4828096b8d9867818988bd01d4a4a93bd1b1916909689c06697a24fe3de570f253439a876d9ffe61";
const char *AUTH_KEY = "asdkflasasdfasdfsdfasfdasdffasdfasd";

extern "C" JNIEXPORT jstring

JNICALL
Java_com_yglx_testjni_MainActivity_getKey(
        JNIEnv *env,
        jobject /* this */) {
    return env->NewStringUTF(AUTH_KEY);
}

JNIEXPORT jint JNICALL JNI_OnLoad (JavaVM* vm,void* reserved){

    JNIEnv* env = NULL;
    jint result=-1;
    if(vm->GetEnv((void**)&env, JNI_VERSION_1_4) != JNI_OK)
        return result;

    contextClass = (jclass)env->NewGlobalRef((env)->FindClass("android/content/Context"));
    signatureClass = (jclass)env->NewGlobalRef((env)->FindClass("android/content/pm/Signature"));
    packageNameClass = (jclass)env->NewGlobalRef((env)->FindClass("android/content/pm/PackageManager"));
    packageInfoClass = (jclass)env->NewGlobalRef((env)->FindClass("android/content/pm/PackageInfo"));

    __android_log_print(ANDROID_LOG_DEBUG, "jw", "sss");
    return JNI_VERSION_1_4;
}

extern "C"
JNIEXPORT jstring JNICALL
Java_com_yglx_testjni_MainActivity_getSafeKey(JNIEnv *env, jobject instance,jobject contextObject) {
    jmethodID getPackageManagerId = (env)->GetMethodID(contextClass, "getPackageManager","()Landroid/content/pm/PackageManager;");
    jmethodID getPackageNameId = (env)->GetMethodID(contextClass, "getPackageName","()Ljava/lang/String;");
    jmethodID signToStringId = (env)->GetMethodID(signatureClass, "toCharsString","()Ljava/lang/String;");
    jmethodID getPackageInfoId = (env)->GetMethodID(packageNameClass, "getPackageInfo","(Ljava/lang/String;I)Landroid/content/pm/PackageInfo;");
    jobject packageManagerObject =  (env)->CallObjectMethod(contextObject, getPackageManagerId);
    jstring packNameString =  (jstring)(env)->CallObjectMethod(contextObject, getPackageNameId);
    jobject packageInfoObject = (env)->CallObjectMethod(packageManagerObject, getPackageInfoId,packNameString, 64);
    jfieldID signaturefieldID =(env)->GetFieldID(packageInfoClass,"signatures", "[Landroid/content/pm/Signature;");
    jobjectArray signatureArray = (jobjectArray)(env)->GetObjectField(packageInfoObject, signaturefieldID);
    jobject signatureObject =  (env)->GetObjectArrayElement(signatureArray,0);

    const char* signStrng =  (env)->GetStringUTFChars((jstring)(env)->CallObjectMethod(signatureObject, signToStringId),0);
    __android_log_print(ANDROID_LOG_DEBUG, "jw_sign","%s",signStrng);
    __android_log_print(ANDROID_LOG_DEBUG, "jw_sign2","%s",signStrng+1023);

    __android_log_print(ANDROID_LOG_DEBUG,"jw_rels","%s",RELEASE_SIGN);
    __android_log_print(ANDROID_LOG_DEBUG,"jw_","sdf:%zd,sfs:%zd",strlen(signStrng),strlen(RELEASE_SIGN));

    if(strcmp(signStrng,RELEASE_SIGN)==0)//签名一致  返回合法的 api key，否则返回错误
    {
        return (env)->NewStringUTF(AUTH_KEY);
    }else
    {
        return (env)->NewStringUTF("error");
    }
}