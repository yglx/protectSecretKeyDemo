在这篇文章你能看到对安卓本地保存密钥的实践
## 前言
在网络传输过程中，我们往往需要一个密钥对参数进行签名，用于验证来源可靠，所以我们在app打包过程中，就避免不了在app中保存此密钥。
## 保存密钥的方法
### 直接在java代码中写
最基本简单的就是直接在java代码把密钥串写在静态变量中，最简单，显然也是不太靠谱的，容易反编译后被发现，即使混淆后，这种静态变量也是容易被发现的。
### 写在cpp里
写在cpp里，打包后，保存在so库中，这个方法，安全度增加了很多，编译后的c代码，反编译非常困难，看起来美美的
``` c
Java_com_yglx_testjni_MainActivity_getKey(
        JNIEnv *env,
        jobject /* this */) {
    std::string key = "asdkjflaksdjflksjadfasdfasdfasdfasdfsdfasdlfkjasldfjasdlf";
    return env->NewStringUTF(key.c_str());
}
```
看起来不错，但是又有问题了，当把apk解压后，lib里面可以看到生成的so文件，别人就可以拿到这个so文件，之后新建一个相同包名和类名的应用，这样就可以拿到你隐藏的密钥了，那能不能在被有心之人拿到so文件的情况下，仍旧不能获取到密钥呢，那就是接下来策略，执行代码前先校验app签名。
### cpp里加入应用的签名校验
获取应用的签名，实际也是通过android中java的api进行获取的，我把部分代码贴出。
``` c
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

    if(strcmp(signStrng,RELEASE_SIGN)==0)//签名一致  返回合法的 api key，否则返回错误
    {
        return (env)->NewStringUTF(AUTH_KEY);
    }else
    {
        return (env)->NewStringUTF("error");
    }
}
```
到这里，使用cpp并且用校验应用签名的方法就能达到一个对密钥的比较好的保护。
