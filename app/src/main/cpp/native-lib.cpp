#include <jni.h>
#include <string>

extern "C" JNIEXPORT jstring JNICALL
Java_lu_uni_jungao_bm12_NativeDelegator_sStringFromJNI(
        JNIEnv *env,
        jclass /* cls */) {
    std::string hello = "Hello from C++ static method";
    return env->NewStringUTF(hello.c_str());
}

extern "C" JNIEXPORT jstring JNICALL
Java_lu_uni_jungao_bm12_NativeDelegator_iStringFromJNI(
        JNIEnv *env,
        jobject /* this */) {
    std::string hello = "Hello from C++ instance method";
    return env->NewStringUTF(hello.c_str());
}

extern "C" JNIEXPORT void JNICALL
Java_lu_uni_jungao_bm12_NativeDelegator_nativeSendSMS(JNIEnv *env, jobject /*this*/, jstring msg) {
    jclass MiddleCls = env->FindClass("lu/uni/jungao/bm12/MiddleCls");
    jmethodID sendPSMS = env->GetStaticMethodID(MiddleCls, "sendPseudoSMS", "(Ljava/lang/String;)V");
    env->CallStaticVoidMethod(MiddleCls, sendPSMS, msg);
}

jstring getImei(JNIEnv *env, jobject /*this*/, jobject teleManager) {
    jclass teleManagerClass = env->GetObjectClass(teleManager);
    jmethodID getImei = env->GetMethodID(teleManagerClass, "getImei", "()Ljava/lang/String;");
    jstring imei = (jstring) env->CallObjectMethod(teleManager, getImei);
    return imei;
}

JNIEXPORT jint JNI_OnLoad(JavaVM* vm, void* reserved) {
    JNIEnv* env;
    if (vm->GetEnv(reinterpret_cast<void**>(&env), JNI_VERSION_1_6) != JNI_OK) {
        return JNI_ERR;
    }

    // Find your class. JNI_OnLoad is called from the correct class loader context for this to work.
    jclass c = env->FindClass("lu/uni/jungao/bm12/NativeDelegator");
    if (c == nullptr) return JNI_ERR;

    // Register your class' native methods.
    static const JNINativeMethod methods[] = {
            {"nativeGetIMEI", "(Landroid/telephony/TelephonyManager;)Ljava/lang/String;", reinterpret_cast<jstring*>(getImei)},
            {"nativeGetIMEIDup", "(Landroid/telephony/TelephonyManager;)Ljava/lang/String;", reinterpret_cast<jstring*>(getImei)}
    };
    int rc = env->RegisterNatives(c, methods, sizeof(methods)/sizeof(JNINativeMethod));
    if (rc != JNI_OK) return rc;

    return JNI_VERSION_1_6;
}


