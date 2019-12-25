#include "kl_ui_proc.h"
#include "application.h"
#include <QGuiApplication>
#include <QQmlApplicationEngine>

#define QT_MESSAGE_PATTERN "[%{time hh:mm:ss.zzz}][%{file}.%{line}] %{message}"

static KLUIProc *ginstance;

static QObject *controllerProvider (QQmlEngine *, QJSEngine *)
{
    return ginstance;
}

int main(int argc, char *argv[])
{
    Application::init_all(0/*INIT_NEED_NOSIGNAL*/, "default", "./zlog.conf");

    Application::instance()->initialize();

    QGuiApplication app(argc, argv);

    qSetMessagePattern(QT_MESSAGE_PATTERN);

    QQmlApplicationEngine engine;

    ginstance = new KLUIProc;

    qmlRegisterSingletonType<QObject> ("Hongjing.HMI.KL", 1, 0, "Controller", controllerProvider);

    ginstance->init(engine.rootContext());

    engine.load(QUrl(QStringLiteral("qrc:/main.qml")));

    if (engine.rootObjects().isEmpty())
    {
        return -1;
    }

    int res = app.exec();

    Application::uninit_all();
    return res;
}
