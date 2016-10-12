#include "android_core.h"
#include <sys/types.h>
#include <sys/time.h>
#include <string.h>
#include <string>
#include <jni.h>
#include "leveltool/LevelTool.h"
#include <android/log.h>

using std::string;

namespace RailClimberPlatform {

    JavaVM *g_jvm;

    void log(int level, const char *fmt, va_list argList) {
        int len = strlen(fmt);
        char *tmpbfr = new char[len + 2];
        strncpy(tmpbfr, fmt, len + 2);
        tmpbfr[len] = '\n';
        tmpbfr[len + 1] = 0;
        android_LogPriority logPriority = ANDROID_LOG_DEBUG;
        if (level == 1)
            logPriority = ANDROID_LOG_INFO;
        else if (level == 2)
            logPriority = ANDROID_LOG_INFO;
        else if (level == 3)
            logPriority = ANDROID_LOG_WARN;
        else if (level == 4)
            logPriority = ANDROID_LOG_ERROR;
        __android_log_vprint(logPriority, "RailClimber", tmpbfr, argList);
        delete[] tmpbfr;
    }

    char *loadFile(const char *assetName, int *lenret) {
        JNIEnv *env;
        g_jvm->GetEnv((void**) &env, JNI_VERSION_1_6);
        jstring fn = env->NewStringUTF(assetName);
        jclass c = env->FindClass("net/holak/railclimber/RailClimberNative");
        jmethodID ltf = env->GetStaticMethodID(c, "LoadTextFile", "(Ljava/lang/String;)Ljava/lang/String;");
        jstring strObj = (jstring) env->CallStaticObjectMethod(c, ltf, fn);

        int len = env->GetStringUTFLength(strObj);
        char *buf = new char[len + 100];
        buf[len] = 0;

        const char *str = env->GetStringUTFChars(strObj, nullptr);
        memcpy(buf, str, len + 10);
        env->ReleaseStringUTFChars(strObj, str);

        env->DeleteLocalRef(strObj);
        env->DeleteLocalRef(fn);

        if (lenret)
            *lenret = len;
        return buf;
    }

    void quit() {
        JNIEnv *env;
        g_jvm->GetEnv((void**) &env, JNI_VERSION_1_6);
        jclass c = env->FindClass("net/holak/railclimber/RailClimberNative");
        jmethodID plquit = env->GetStaticMethodID(c, "PlatformQuit", "()V");
        env->CallStaticVoidMethod(c, plquit);
    }

    char **getLevels() {
        return nullptr;
    }

    RCLSettingsType loadSettings(const char *filename) {
        return nullptr;
    }

    void setSetting(RCLSettingsType settings, const char *section, const char *key, const char *value) {
        JNIEnv *env;
        g_jvm->GetEnv((void**) &env, JNI_VERSION_1_6);
        jclass c = env->FindClass("net/holak/railclimber/RailClimberNative");
        jmethodID getMethod = env->GetStaticMethodID(c, "SetSetting", "(Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;)V");
        jstring jsSection = env->NewStringUTF(section);
        jstring jsKey = env->NewStringUTF(key);
        jstring jsValue = env->NewStringUTF(value);
        env->CallStaticVoidMethod(c, getMethod, jsSection, jsKey, jsValue);

        env->DeleteLocalRef(jsSection);
        env->DeleteLocalRef(jsKey);
        env->DeleteLocalRef(jsValue);
    }

    std::string getSetting(RCLSettingsType settings, const char *section, const char *key) {
        JNIEnv *env;
        g_jvm->GetEnv((void**) &env, JNI_VERSION_1_6);
        jclass c = env->FindClass("net/holak/railclimber/RailClimberNative");
        jmethodID getMethod = env->GetStaticMethodID(c, "GetSetting", "(Ljava/lang/String;Ljava/lang/String;)Ljava/lang/String;");
        jstring jsSection = env->NewStringUTF(section);
        jstring jsKey = env->NewStringUTF(key);
        jstring strobj = (jstring) env->CallStaticObjectMethod(c, getMethod, jsSection, jsKey);
        if (strobj == nullptr)
            return std::string();
        const char *str = env->GetStringUTFChars(strobj, nullptr);
        std::string s = str;
        env->ReleaseStringUTFChars(strobj, str);

        env->DeleteLocalRef(strobj);
        env->DeleteLocalRef(jsSection);
        env->DeleteLocalRef(jsKey);

        return s;
    }

    void unloadSettings(RCLSettingsType settings) {

    }

    void vibrate(int duration) {
        JNIEnv *env;
        g_jvm->GetEnv((void**) &env, JNI_VERSION_1_6);
        jclass c = env->FindClass("net/holak/railclimber/RailClimberNative");
        jmethodID vib = env->GetStaticMethodID(c, "Vibrate", "(I)V");
        env->CallStaticVoidMethod(c, vib, duration);
    }

    string getLanguageCode() {
        /*JNIEnv *env;
        g_jvm->GetEnv((void**)&env, JNI_VERSION_1_6);
        jclass c = env->FindClass("net/holak/railclimber/RailClimberNative");
        jmethodID glc = env->GetStaticMethodID(c, "GetLangCode", "()Ljava/lang/String;");
        jstring strObj = (jstring)env->CallStaticObjectMethod(c, glc);
        const char *str = env->GetStringUTFChars(strObj, nullptr);
        string s = str;
        env->ReleaseStringUTFChars(strObj, str);
        env->DeleteLocalRef(strObj);
        return s;*/
        return "en";
    }

    long long getTicks() {
        timeval tp;
        gettimeofday(&tp, nullptr);
        return tp.tv_sec * 1000 + tp.tv_usec / 1000;
    }

}
