#include "application.h"
#include "../klUI/image_frame.h"
#include <events/common_log.h>
#include <unistd.h>
#include <QGuiApplication>
#include <QQmlApplicationEngine>

#define QT_MESSAGE_PATTERN "[%{time hh:mm:ss.zzz}][%{file}.%{line}] %{message}"

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);

    qSetMessagePattern(QT_MESSAGE_PATTERN);

    QQmlApplicationEngine engine;

    Application::init_all(INIT_NEED_NOSIGNAL, "default", "./zlog.conf");

    Application::instance()->initialize();

    qmlRegisterType<ImageFrameCpp, 1>("ImageFrame", 1, 0, "ImageFrame");

    engine.load(QUrl(QStringLiteral("qrc:/main.qml")));

    if (engine.rootObjects().isEmpty())
    {
        return -1;
    }

    int res = app.exec();

    Application::uninit_all();
    return res;
}
