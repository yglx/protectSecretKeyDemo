#include <jni.h>
#include <string>

extern "C" JNIEXPORT jstring

JNICALL
Java_com_yglx_testjni_MainActivity_getKey(
        JNIEnv *env,
        jobject /* this */) {
    std::string key = "asdkjflaksdjflksjadfasdfasdfasdfasdfsdfasdlfkjasldfjasdlf";
    return env->NewStringUTF(key.c_str());
}
