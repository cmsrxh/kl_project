/**
  \page index.html
  \title QQMusic使用C++源码实现其完整的功能

  \section1 Description
  这个程序主要根据网络vue开源代码,进行重新的整合，
  使用QT+MPV这两部分进行搭建

  \section1 Class
  \list
  \o \l Applicant
  \o UIProc
  \o
  \endlist

  */
/**
@file	档案的批注说明。
@author	作者的信息
@brief	用于class 或function的简易说明eg：@brief 本函数负责打印错误信息串
@param	主要用于函数说明中，后面接参数的名字，然后再接关于该参数的说明
@return	描述该函数的返回值情况eg:@return 本函数返回执行结果，若成功则返回TRUE，否则返回FLASE
@retval	描述返回值类型，eg:@retval NULL 空字符串。
@note	注解
@attention	注意
@warning	警告信息
@enum	引用了某个枚举，Doxygen会在该枚举处产生一个链接，eg：@enum CTest::MyEnum
@var	引用了某个变量，Doxygen会在该枚举处产生一个链接，eg：@var CTest::m_FileKey
@class	引用某个类，格式：@class <name> [<header-file>] [<header-name> ]，eg:@class CTest "inc/class.h"
@exception	可能产生的异常描述,eg:@exception 本函数执行可能会产生超出范围的异常


*/
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

    gInstance = new UIProc;

    Application::instance()->initialize();

    qmlRegisterSingletonType<QObject> ("Hongjing.HMI.KL", 1, 0, "Controller", controllerProvider);
    qmlRegisterType<ImageFrameCpp, 1>("ImageFrame", 1, 0, "ImageFrame");

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
