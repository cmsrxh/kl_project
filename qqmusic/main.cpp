#include <events/common_log.h>
#include "ui_proc.h"
#include "image_frame.h"
#include "application.h"
#include <unistd.h>
#include <QGuiApplication>
#include <QQmlApplicationEngine>

#define QT_MESSAGE_PATTERN "[%{time hh:mm:ss.zzz}][%{file}.%{line}] %{message}"

UIProc *gInstance;

static QObject *controllerProvider (QQmlEngine *, QJSEngine *)
{
    return gInstance;
}

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);

    qSetMessagePattern(QT_MESSAGE_PATTERN);

    QQmlApplicationEngine engine;

    gInstance = new UIProc;

    Application::init_all(INIT_NEED_NOSIGNAL, "default", "./zlog.conf");

    Application::instance()->initialize();

//    qmlRegisterSingletonType<QObject> ("Hongjing.HMI.KL", 1, 0, "Controller", controllerProvider);
//    qmlRegisterType<ImageFrameCpp, 1>("ImageFrame", 1, 0, "ImageFrame");

//    gInstance->init(engine.rootContext());

//    engine.load(QUrl(QStringLiteral("qrc:/main.qml")));

//    if (engine.rootObjects().isEmpty())
//    {
//        return -1;
//    }

    int res = app.exec();

    Application::uninit_all();

    qDebug() << "Main Thread exit.";
    Application::instance()->poweroff();
    return res;
}
