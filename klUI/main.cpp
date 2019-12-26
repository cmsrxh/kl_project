#include "kl_ui_proc.h"
#include "application.h"
#include <events/common_log.h>
#include <unistd.h>
#include <QGuiApplication>
#include <QQmlApplicationEngine>

#define QT_MESSAGE_PATTERN "[%{time hh:mm:ss.zzz}][%{file}.%{line}] %{message}"

KLUIProc *gInstance;

static QObject *controllerProvider (QQmlEngine *, QJSEngine *)
{
    return gInstance;
}

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);

    qSetMessagePattern(QT_MESSAGE_PATTERN);

    QQmlApplicationEngine engine;

    gInstance = new KLUIProc;

    Application::init_all(0/*INIT_NEED_NOSIGNAL*/, "default", "./zlog.conf");

    Application::instance()->initialize();

    qmlRegisterSingletonType<QObject> ("Hongjing.HMI.KL", 1, 0, "Controller", controllerProvider);

    gInstance->init(engine.rootContext());

    engine.load(QUrl(QStringLiteral("qrc:/main.qml")));

    if (engine.rootObjects().isEmpty())
    {
        return -1;
    }

    int res = app.exec();

    Application::uninit_all();
    return res;
}
