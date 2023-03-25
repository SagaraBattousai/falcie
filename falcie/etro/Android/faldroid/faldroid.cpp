#include <jni.h>
#include <string>

extern "C" JNIEXPORT jstring JNICALL
Java_com_calotechnologies_faldroid_blockchain_Blockchain_testFunc(JNIEnv *env, jobject /* AkA this */)
{
    std::string hello = "Hello from JNI";
    return env->NewStringUTF(hello.c_str());
}