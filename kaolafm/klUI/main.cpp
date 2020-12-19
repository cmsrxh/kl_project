#include <events/common_log.h>
#include "kl_ui_proc.h"
#include "image_frame.h"
#include "application.h"
#include <unistd.h>
#include <QGuiApplication>
#include <QQmlApplicationEngine>

#define QT_MESSAGE_PATTERN "[%{time hh:mm:ss.zzz}][%{file}.%{line}] %{message}"

KLUIProc *gInstance;

static QObject *controllerProvider (QQmlEngine *, QJSEngine *)
{
    return gInstance;
}
#if 0
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
#else
static void qtMessageHandler(QtMsgType type, const QMessageLogContext &ctx, const QString &str)
{
    int level        = 255;
    const char *file = ctx.file ? ctx.file : "";
    int file_len     = ctx.file ? strlen(ctx.file) : 0;

    const char *function = ctx.function ? ctx.function : "";
    int func_len         = ctx.function ? strlen(ctx.function) : 0;

    switch (type) {
    case QtDebugMsg:
        level = LOG_DEBUG;
        break;
    case QtWarningMsg:
        level = LOG_WARN;
        break;
    case QtCriticalMsg:
//    case QtSystemMsg:
    case QtFatalMsg:
        level = LOG_ERROR;
        break;
    case QtInfoMsg:
        level = LOG_INFO;
        break;
    default:
        break;
    }

    __logprintf(file, file_len, function, func_len, ctx.line, level, str.toStdString().c_str());
}
#endif
int main(int argc, char *argv[])
{
    qInstallMessageHandler(qtMessageHandler);
    // qSetMessagePattern(QT_MESSAGE_PATTERN);

    // 重新定向打印,使用Qt打印
    // __logprintf = _qt_log_out;
    Application::init_all(INIT_NEED_NOSIGNAL, "default", "./zlog.conf");

    QGuiApplication app(argc, argv);

    QQmlApplicationEngine engine;

    gInstance = new KLUIProc;

    Application::instance()->initialize();

    qmlRegisterSingletonType<QObject> ("Hongjing.HMI.KL", 1, 0, "Controller", controllerProvider);
    qmlRegisterType<ImageFrameCpp, 1>("ImageFrameCpp", 1, 0, "ImageFrameCpp");

    gInstance->init(engine.rootContext());

    engine.load(QUrl(QStringLiteral("qrc:/main.qml")));

    if (engine.rootObjects().isEmpty())
    {
        return -1;
    }

    int res = app.exec();

    Application::uninit_all();

    qDebug() << "Main Thread exit.";
    Application::instance()->poweroff();
    return res;
}
