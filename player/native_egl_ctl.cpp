#include <jni.h>
#include <stdexcept>
#include <android/native_window.h>
#include <android/native_window_jni.h>
#include <android/log.h>
#include <events/common_log.h>
extern "C"{
#include <libavcodec/jni.h>
#include <libavutil/log.h>
}

#include "application.h"
#include "iface/media_service_i_face.h"
#include "iface/media_notify.h"

#define TAG "zlog"
#define LOGI(fmt, args...) __android_log_print(ANDROID_LOG_INFO,  TAG, fmt, ##args)
#define LOGD(fmt, args...) //__android_log_print(ANDROID_LOG_DEBUG, TAG, fmt, ##args)
#define LOGE(fmt, args...) // __android_log_print(ANDROID_LOG_ERROR, TAG, fmt, ##args)

static jmethodID   post_event;
static JavaVM*     g_pVM;
static jclass      g_pClass;     // Reference to MediaPlayer class
static jobject     g_pObject;

//在加载动态库时回去调用 JNI_Onload 方法，在这里可以得到 JavaVM 实例对象
extern "C"
JNIEXPORT jint JNICALL
JNI_OnLoad(JavaVM *vm, void */*reserved*/)
{
    JNIEnv *env;
    g_pVM = vm;
    LOGI("JNI_OnLoad.%d", __LINE__);
    av_jni_set_java_vm(vm, nullptr);
    av_log_set_level(AV_LOG_DEBUG);

    LOGI("JNI_OnLoad.%d", __LINE__);
    Application::instance();
    LOGI("JNI_OnLoad.%d", __LINE__);
    if (vm->GetEnv((void **) &env, JNI_VERSION_1_6) != JNI_OK)
    {
        GEN_Printf(LOG_ERROR, "Get Env failed.");
        return -1;
    }
    LOGI("JNI_OnLoad.%d", __LINE__);
    jclass clazz = env->FindClass("com/lantonda/mediaplayer/LTDMediaPlayer");
    if (clazz == NULL)
    {
        GEN_Printf(LOG_ERROR, "Find class failed.");
        return -1;
    }
    GEN_Printf(LOG_DEBUG, "+++++++++++++++++++++++++++++++++");
    post_event = env->GetStaticMethodID(clazz, "postEventFromNative",
                                        "(Ljava/lang/Object;IIILjava/lang/Object;)V");
    if (post_event == NULL)
    {
        GEN_Printf(LOG_ERROR, "Get Static Method 'postEventFromNative' failed.");
        return -1;
    }
    GEN_Printf(LOG_DEBUG, "+++++++++++++++++++++++++++++++++");
    g_pClass = (jclass)env->NewGlobalRef((jobject)clazz);

    env->DeleteLocalRef(clazz);
    LOGI("JNI_OnLoad.%d", __LINE__);
    return JNI_VERSION_1_6;
}

extern "C"
JNIEXPORT void JNICALL
JNI_OnUnload(JavaVM */*vm*/, void */*reserved*/)
{
    JNIEnv *env = NULL;
    if (g_pVM->GetEnv((void **) &env, JNI_VERSION_1_6) != JNI_OK)
    {
        return;
    }
    env->DeleteGlobalRef(g_pClass);
    g_pVM    = nullptr;
    g_pClass = nullptr;
    return;
}

void MediaNotify::notify(int msg, int ext1, int ext2, const char *str)
{
    int  status;
    bool isAttached = false;
    JNIEnv* env = NULL;

    GEN_Printf(LOG_DEBUG, "notify: sig = %d, %d, %d, %s", msg, ext1, ext2, str);
    if (!g_pVM || !post_event || !g_pClass)
    {
        GEN_Printf(LOG_WARN, "notify failed, %p, %p, %p is nullptr", g_pVM, post_event, g_pClass);
        return;
    }

    status = g_pVM->GetEnv((void **) &env, JNI_VERSION_1_6);
    if (status < 0)
    {
        if (g_pVM->AttachCurrentThread(&env, NULL)) //将当前线程注册到虚拟机中
        {
            GEN_Printf(LOG_ERROR, "Attch Current Thread failed.");
            return;
        }
        isAttached = true;
    }

    jstring str_arg = env->NewStringUTF(str ? str : "");

    env->CallStaticVoidMethod(g_pClass, post_event, g_pObject,
                              msg, ext1, ext2, str_arg);

    env->DeleteLocalRef(str_arg);

//    GEN_Printf(LOG_DEBUG, "notify isAttached: %d", isAttached);
    if(isAttached)
    {
        g_pVM->DetachCurrentThread();
    }
//    GEN_Printf(LOG_DEBUG, "notify end");
}

extern "C"
JNIEXPORT void JNICALL
Java_com_lantonda_mediaplayer_LTDMediaPlayer_nativeInit(JNIEnv *env, jobject instance, jobject weak_this)
{
    Application::instance()->initialize();

    GEN_Printf(LOG_DUMP, "native Init");

    jclass clazz = env->GetObjectClass(instance);
    if (clazz == NULL)
    {
        GEN_Printf(LOG_DEBUG, "Can't find android/media/MediaPlayer");
        throw std::runtime_error("Can't find android/media/MediaPlayer");
    }
    g_pClass = (jclass)env->NewGlobalRef(clazz);

    // We use a weak reference so the MediaPlayer object can be garbage collected.
    // The reference is only used as a proxy for callbacks.
    g_pObject  = env->NewGlobalRef(weak_this);

    UNUSED(env);
    UNUSED(instance);
}

extern "C"
JNIEXPORT void JNICALL
Java_com_lantonda_mediaplayer_LTDMediaPlayer_nativeRelease(JNIEnv *env, jobject instance)
{
    GEN_Printf(LOG_DUMP, "native Release.");

    Application::instance()->uninit();
    UNUSED(env);
    UNUSED(instance);
}

extern "C"
JNIEXPORT void JNICALL
Java_com_lantonda_mediaplayer_LTDMediaPlayer_onSurfaceCreated(JNIEnv *env, jobject instance,
                                                             jobject surface)
{
    GEN_Printf(LOG_DUMP, "Surface Created.");

    if (surface)
    {
        Application::instance()->setWindow(ANativeWindow_fromSurface(env, surface));
    } else
    {
        Application::instance()->setWindow(nullptr);
    }

    QEvt *evt = Application::newEvt(SIG_SURFACE_CREATED, sizeof(GeneralQEvt));
    if (evt)
    {
        Application::instance()->post(evt);
    } else
    {
        GEN_Printf(LOG_ERROR, "new a msg failed !");
    }

    UNUSED(instance);
}

extern "C"
JNIEXPORT void JNICALL
Java_com_lantonda_mediaplayer_LTDMediaPlayer_onSurfaceChanged(JNIEnv *env, jobject instance,
                                                             jint width, jint height)
{
    GEN_Printf(LOG_DUMP, "Surface Changed, (%d, %d)", width, height);

    GeneralQEvt *evt = (GeneralQEvt *)Application::newEvt(SIG_SURFACE_CHANGED, sizeof(GeneralQEvt));
    if (evt)
    {
        evt->lParam = width;
        evt->wParam = height;
        Application::instance()->post(evt);
    } else
    {
        GEN_Printf(LOG_ERROR, "new a msg failed !");
    }
    UNUSED(env);
    UNUSED(instance);
}

extern "C"
JNIEXPORT void JNICALL
Java_com_lantonda_mediaplayer_LTDMediaPlayer_onSurfaceDestroyed(JNIEnv *env, jobject instance)
{
    GEN_Printf(LOG_DUMP, "Surface Destroyed.");
    Application::instance()->uninit();

    UNUSED(env);
    UNUSED(instance);
}

extern "C"
JNIEXPORT void JNICALL
Java_com_lantonda_mediaplayer_LTDMediaPlayer_nativeStart(JNIEnv *env, jobject instance)
{
    MediaServiceIFace::instance()->start();
    UNUSED(env);
    UNUSED(instance);
}

extern "C"
JNIEXPORT void JNICALL
Java_com_lantonda_mediaplayer_LTDMediaPlayer_nativePlay(JNIEnv *env, jobject instance)
{
    MediaServiceIFace::instance()->play();
    UNUSED(env);
    UNUSED(instance);
}

extern "C"
JNIEXPORT void JNICALL
Java_com_lantonda_mediaplayer_LTDMediaPlayer_nativeStop(JNIEnv *env, jobject instance)
{
    MediaServiceIFace::instance()->stop();
    UNUSED(env);
    UNUSED(instance);
}

extern "C"
JNIEXPORT void JNICALL
Java_com_lantonda_mediaplayer_LTDMediaPlayer_nativePause(JNIEnv *env, jobject instance)
{
    MediaServiceIFace::instance()->pause();
    UNUSED(env);
    UNUSED(instance);
}

extern "C"
JNIEXPORT void JNICALL
Java_com_lantonda_mediaplayer_LTDMediaPlayer_nativeReset(JNIEnv *env, jobject instance)
{
    MediaServiceIFace::instance()->reset();
    UNUSED(env);
    UNUSED(instance);
}

extern "C"
JNIEXPORT void JNICALL
Java_com_lantonda_mediaplayer_LTDMediaPlayer_nativePlayPause(JNIEnv *env, jobject instance)
{
    MediaServiceIFace::instance()->playPause();
    UNUSED(env);
    UNUSED(instance);
}

extern "C"
JNIEXPORT void JNICALL
Java_com_lantonda_mediaplayer_LTDMediaPlayer_nativeSetDataSource(JNIEnv *env, jobject instance, jstring filename)
{
    GEN_Printf(LOG_DEBUG, "Enter Set Play Files.");
    char *str = (char*)env->GetStringUTFChars(filename, 0);

    GEN_Printf(LOG_DEBUG, "play file: %s", str);
    MediaServiceIFace::instance()->setFile(str);

    UNUSED(instance);
    env->ReleaseStringUTFChars(filename, str);
}

extern "C"
JNIEXPORT jboolean JNICALL
Java_com_lantonda_mediaplayer_LTDMediaPlayer_isPlaying(JNIEnv *env, jobject instance)
{
    UNUSED(env);
    UNUSED(instance);
    return MediaServiceIFace::instance()->isPlaying();
}

extern "C"
JNIEXPORT void JNICALL
Java_com_lantonda_mediaplayer_LTDMediaPlayer_nativeSeekTo(JNIEnv *env, jobject instance, jlong msec, jint mode)
{
    GEN_Printf(LOG_DEBUG, "seek to: %d, mode: %d", msec, mode);
    MediaServiceIFace::instance()->seekTo(msec, mode);
    UNUSED(env);
    UNUSED(instance);
}

extern "C"
JNIEXPORT jint JNICALL
Java_com_lantonda_mediaplayer_LTDMediaPlayer_getCurrentPosition(JNIEnv *env, jobject instance)
{
    UNUSED(env);
    UNUSED(instance);
    return MediaServiceIFace::instance()->getCurrentPosition();
}

extern "C"
JNIEXPORT jint JNICALL
Java_com_lantonda_mediaplayer_LTDMediaPlayer_getDuration(JNIEnv *env, jobject instance)
{
    UNUSED(env);
    UNUSED(instance);
    return MediaServiceIFace::instance()->getDuration();
}

extern "C"
JNIEXPORT void JNICALL
Java_com_lantonda_mediaplayer_LTDMediaPlayer_setLooping(JNIEnv *env, jobject instance, jboolean looping)
{
    UNUSED(looping);
    UNUSED(env);
    UNUSED(instance);
}

extern "C"
JNIEXPORT jboolean JNICALL
Java_com_lantonda_mediaplayer_LTDMediaPlayer_isLooping(JNIEnv *env, jobject instance)
{
    UNUSED(env);
    UNUSED(instance);
    return false;
}

extern "C"
JNIEXPORT void JNICALL
Java_com_lantonda_mediaplayer_LTDMediaPlayer_setVolume(JNIEnv *env, jobject instance, jfloat leftVolume, jfloat rightVolume)
{
    GEN_Printf(LOG_DUMP, "volume(%f, %f)", leftVolume, rightVolume);
    UNUSED(env);
    UNUSED(instance);
}
