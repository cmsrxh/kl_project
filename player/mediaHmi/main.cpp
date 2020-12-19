#include <events/common_log.h>
#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QtQml>
#include <QTextCodec>
#include <QQuickView>
#include "manage/application.h"
#include "media_call_back.h"
#include "media_hmi_i_face.h"
#include "media_hmi_ctrl.h"
//#include "sys_device_client_proxy.h"

#define QT_MESSAGE_PATTERN "[%{time hh:mm:ss.zzz}][Media][%{file}.%{line}] %{message}"

static QObject *controllerProvider (QQmlEngine *, QJSEngine *)
{
    return mediaApp;
}
static void _qt_log_out(const char *file, int file_len, const char *func, int func_len, int line, int level, const char *format, ...)
{
    char buffer[512];
    va_list args;
    va_start(args, format);
    vsnprintf(buffer, sizeof(buffer), format, args);
    va_end(args);

    switch (level) {
    case LOG_DUMP:
    {
        QMessageLogger(basename(file), line, func).debug("%s", buffer);
        break;
    }
    case LOG_DEBUG:
    {
        QMessageLogger(basename(file), line, func).info("%s", buffer);
        break;
    }
    case LOG_INFO:
    {
        QMessageLogger(basename(file), line, func).info("%s", buffer);
        break;
    }
    case LOG_WARN:
    {
        QMessageLogger(basename(file), line, func).warning("%s", buffer);
        break;
    }
    case LOG_ERROR:
    {
        QMessageLogger(basename(file), line, func).critical("%s", buffer);
        break;
    }
    default:
    {
        QMessageLogger(basename(file), line, func).critical("%s", buffer);
        break;
    }
    }
    UNUSED(file_len);
    UNUSED(func_len);
}


int main(int argc, char**argv)
{
    // 重新定向打印,使用Qt打印
    __logprintf = _qt_log_out;

#ifdef PLATFORM_QUANZHI
//    char buff[64];
//    sprintf(buff, "echo %d, > /sys/my_sysfs_test/write", getpid());
//    system(buff);
//#ifdef NEED_GDBUS
//    GDBusMainLoop::instance()->start();
//#endif
#endif

    QGuiApplication app(argc, argv);
    QQuickView view;
    QTextCodec *codec = QTextCodec::codecForName("UTF-8");
    QTextCodec::setCodecForLocale(codec);
    MediaCallBack *callback = new MediaCallBack;
    mediaApp;
//    SysDeviceClientProxy devProxy;
//    devProxy.registerNotifyMsg(SysDeviceClientProxy::REG_HOT_DEVICE_NOTIFY);
//    devProxy.setCallback(new SysDeviceCallBack);

    MediaHMIIFace::instance()->setCallback(callback);
    MediaHMIIFace::instance()->registerNotifyMsg();

    qSetMessagePattern(QT_MESSAGE_PATTERN);

    qmlRegisterSingletonType<QObject> ("HongJing.HMI.Media", 1, 0, "Controller", controllerProvider);

    Application::instance()->initialize();
    mediaApp->init(&view);

    QObject::connect(callback, SIGNAL(mediaNotifySig(int,int,int,QString)), mediaApp, SLOT(onRecvNotify(int,int,int,QString)));

#ifdef PLATFORM_PC
    view.setColor (QColor(Qt::black));
#endif
#ifdef PLATFORM_QUANZHI
    view.setColor (QColor(Qt::transparent));
#endif

    view.setSource(QUrl(QStringLiteral("qrc:/qml/multiMedia/MediaMain.qml")));

    view.show ();

    app.exec();

    delete callback;
	return 0;
}
