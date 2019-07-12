#include <jni.h>
#include <string>

#include "include/inlineHook.h"
#include "hooktest.h"

extern "C" JNIEXPORT jstring JNICALL
Java_com_wizd_inlinehooktest_MainActivity_stringFromJNI(
        JNIEnv *env,
        jobject /* this */) {
    std::string hello = "Hello from C++";

    testHook();

    return env->NewStringUTF(hello.c_str());
}
extern "C"
JNIEXPORT void JNICALL
Java_com_wizd_xposedinlinehook_HookClass_testNative(JNIEnv *env, jclass type) {

//    testHook();
    hookRecvfrom();

}