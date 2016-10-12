#include "android_core.h"
#include <string>

using std::string;

namespace RailClimberPlatform {

    int volume = 50;

    void soundInit() {
        JNIEnv *env;
        g_jvm->GetEnv((void**) &env, JNI_VERSION_1_6);
        jclass c = env->FindClass("net/holak/railclimber/RailClimberNative");
        jmethodID plsi = env->GetStaticMethodID(c, "SoundInit", "()V");
        env->CallStaticVoidMethod(c, plsi);
    }

    void soundCleanup() {
        JNIEnv *env;
        g_jvm->GetEnv((void**) &env, JNI_VERSION_1_6);
        jclass c = env->FindClass("net/holak/railclimber/RailClimberNative");
        jmethodID plsc = env->GetStaticMethodID(c, "SoundCleanup", "()V");
        env->CallStaticVoidMethod(c, plsc);
    }

    void *loadSound(const string &filename) {
        int *i = new int;
        JNIEnv *env;
        g_jvm->GetEnv((void**) &env, JNI_VERSION_1_6);
        jstring fn = env->NewStringUTF(filename.c_str());
        jclass c = env->FindClass("net/holak/railclimber/RailClimberNative");
        jmethodID plsl = env->GetStaticMethodID(c, "SoundLoad", "(Ljava/lang/String;)I");
        *i = env->CallStaticIntMethod(c, plsl, fn);
        env->DeleteLocalRef(fn);
        return i;
    }

    void playSound(void *data) {
        int i = *((int*) data);
        JNIEnv *env;
        g_jvm->GetEnv((void**) &env, JNI_VERSION_1_6);
        jclass c = env->FindClass("net/holak/railclimber/RailClimberNative");
        jmethodID plsp = env->GetStaticMethodID(c, "SoundPlay", "(I)V");
        env->CallStaticVoidMethod(c, plsp, i);
    }

    void unloadSound(void *data) {
        int *iptr = (int*) data;
        int i = *iptr;
        JNIEnv *env;
        g_jvm->GetEnv((void**) &env, JNI_VERSION_1_6);
        jclass c = env->FindClass("net/holak/railclimber/RailClimberNative");
        jmethodID plsu = env->GetStaticMethodID(c, "SoundUnload", "(I)V");
        env->CallStaticVoidMethod(c, plsu, i);
        delete iptr;
    }

    void loadSilence() {
    }

    void playSilence() {
    }

    void stopSilence() {
    }

    int getVolume() {
        return volume;
    }

    void setVolume(int vol) {
        if (vol < 0) vol = 0;
        if (vol > 100) vol = 100;
        volume = vol;
    }

}
