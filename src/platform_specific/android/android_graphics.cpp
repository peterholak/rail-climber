#include "platform/graphics.h"
#include "android_core.h"
#include <android/bitmap.h>
#include <jni.h>
#include "Logging.h"

namespace RailClimberPlatform {

    unsigned int loadTexture(const char *filename, int *retW, int *retH, bool hRepeat, bool vRepeat) {
        GLuint texture;

        JNIEnv *env;
        g_jvm->GetEnv((void**) &env, JNI_VERSION_1_6);
        jclass c = env->FindClass("net/holak/railclimber/RailClimberNative");
        jmethodID bmpload = env->GetStaticMethodID(c, "LoadBitmap", "(Ljava/lang/String;)Landroid/graphics/Bitmap;");
        jstring jfilename = env->NewStringUTF(filename);
        jobject bmpobj = env->CallStaticObjectMethod(c, bmpload, jfilename);

        AndroidBitmapInfo abinfo;
        AndroidBitmap_getInfo(env, bmpobj, &abinfo);

        *retW = abinfo.width;
        *retH = abinfo.height;

        glGenTextures(1, &texture);
        glBindTexture(GL_TEXTURE_2D, texture);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, hRepeat ? GL_REPEAT : GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, vRepeat ? GL_REPEAT : GL_CLAMP_TO_EDGE);

        void *pixels;
        AndroidBitmap_lockPixels(env, bmpobj, &pixels);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, abinfo.width, abinfo.height, 0, GL_RGBA, GL_UNSIGNED_BYTE, pixels);
        AndroidBitmap_unlockPixels(env, bmpobj);

        env->DeleteLocalRef(bmpobj);
        env->DeleteLocalRef(jfilename);

        return texture;
    }

    RCLFontType loadFont(const char *filename, int ptsize) {
        RCLFontType font = new FontStruct();
        font->name = filename;
        font->ptsize = ptsize;
        return font;
    }

    void unloadFont(RCLFontType font) {
        delete font;
    }

    unsigned int textureFromText(RCLFontType font, int color, const char *text, int *retW, int *retH) {
        GLuint texture;

        JNIEnv *env;
        g_jvm->GetEnv((void**) &env, JNI_VERSION_1_6);
        jclass c = env->FindClass("net/holak/railclimber/RailClimberNative");
        jmethodID textload = env->GetStaticMethodID(c, "CreateTextBitmap", "(Ljava/lang/String;IILjava/lang/String;)Landroid/graphics/Bitmap;");
        jstring jfilename = env->NewStringUTF(font->name.c_str());
        jstring jtext = env->NewStringUTF(text);
        jobject bmpobj = env->CallStaticObjectMethod(c, textload, jfilename, font->ptsize, color, jtext);

        AndroidBitmapInfo abinfo;
        AndroidBitmap_getInfo(env, bmpobj, &abinfo);

        *retW = abinfo.width;
        *retH = abinfo.height;

        glGenTextures(1, &texture);
        glBindTexture(GL_TEXTURE_2D, texture);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        void *pixels;
        AndroidBitmap_lockPixels(env, bmpobj, &pixels);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, abinfo.width, abinfo.height, 0, GL_RGBA, GL_UNSIGNED_BYTE, pixels);
        AndroidBitmap_unlockPixels(env, bmpobj);

        env->DeleteLocalRef(bmpobj);
        env->DeleteLocalRef(jfilename);
        env->DeleteLocalRef(jtext);

        return texture;
    }

    unsigned int multiLineText(RCLFontType font, int color, const char *text, int w, int *retH, MultiLineJustify justify) {
        GLuint texture;

        JNIEnv *env;
        g_jvm->GetEnv((void**) &env, JNI_VERSION_1_6);
        jclass c = env->FindClass("net/holak/railclimber/RailClimberNative");
        jmethodID textload = env->GetStaticMethodID(c, "MultiLineTextBitmap", "(Ljava/lang/String;IILjava/lang/String;I)Landroid/graphics/Bitmap;");
        jstring jfilename = env->NewStringUTF(font->name.c_str());
        jstring jtext = env->NewStringUTF(text);
        jobject bmpobj = env->CallStaticObjectMethod(c, textload, jfilename, font->ptsize, color, jtext, w);

        AndroidBitmapInfo abinfo;
        AndroidBitmap_getInfo(env, bmpobj, &abinfo);

        *retH = abinfo.height;

        glGenTextures(1, &texture);
        glBindTexture(GL_TEXTURE_2D, texture);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        void *pixels;
        AndroidBitmap_lockPixels(env, bmpobj, &pixels);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, abinfo.width, abinfo.height, 0, GL_RGBA, GL_UNSIGNED_BYTE, pixels);
        AndroidBitmap_unlockPixels(env, bmpobj);

        env->DeleteLocalRef(bmpobj);
        env->DeleteLocalRef(jfilename);
        env->DeleteLocalRef(jtext);

        return texture;
    }

    void textMetrics(RCLFontType font, const char *text, int *retW, int *retH) {

    }

}
