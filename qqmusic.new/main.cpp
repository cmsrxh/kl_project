#include <events/common_log.h>
#include "application/application.h"
#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QtQml>
#include <QTextCodec>
#include <QQuickView>
#include "common/image_frame.h"
#include "global/ctrl/global_controller.h"
#include "global/ctrl/layer_manage_control.h"
#include "global/manage/pages_manager.h"
#include "pages/home/home_controller.h"
#include "pages/playlist/category_playlist.h"

static QObject *globalControllerProvider(QQmlEngine *, QJSEngine *)
{
    return GlobalControls::instance();
}

static QObject *layerManagerProvider(QQmlEngine *, QJSEngine *)
{
    return LayerManageControl::instance();
}
static QObject *pagesManagerControllerProvider(QQmlEngine *, QJSEngine *)
{
    return PagesManager::instance();
}
static QObject *homeManagerControllerProvider(QQmlEngine *, QJSEngine *)
{
    return HomeController::instance();
}
static QObject *catePlayListControllerProvider(QQmlEngine *, QJSEngine *)
{
    return CategoryPlaylistProc::instance();
}

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

int main(int argc, char *argv[])
{
    // 重新定向打印,使用sflog打印
    qInstallMessageHandler(qtMessageHandler);

    Application::init_all(INIT_NEED_NOSIGNAL, "default", "/etc/zlog.conf");

    Application::instance()->initialize();

    QGuiApplication app(argc, argv);
    QQuickView      view;

    QTextCodec *codec = QTextCodec::codecForName("UTF-8");
    QTextCodec::setCodecForLocale(codec);

    PagesManager::instance()->init(&view);

    qmlRegisterSingletonType<QObject>("com.hongjing.globalcontroller", 1, 0, "Api", globalControllerProvider);
    // 主页面之上的弹框,层级管理
    qmlRegisterSingletonType<QObject>("com.hongjing.layers", 1, 0, "Api", layerManagerProvider);
    qmlRegisterSingletonType<QObject>("com.hongjing.pagesManager", 1, 0, "Api", pagesManagerControllerProvider);
    qmlRegisterSingletonType<QObject>("com.hongjing.home", 1, 0, "Api", homeManagerControllerProvider);
    qmlRegisterSingletonType<QObject>("com.hongjing.catePlaylist", 1, 0, "Api", catePlayListControllerProvider);

    qmlRegisterType<ImageFrameCpp, 1>("ImageFrame", 1, 0, "ImageFrame");


    view.setResizeMode(QQuickView::SizeRootObjectToView);
    view.setFlags(Qt::FramelessWindowHint); //设置无窗口

#ifdef PLATFORM_PC
    view.setColor (QColor(Qt::green));
#else
    view.setColor (QColor(Qt::transparent));
#endif
    // 系统UI显示裁定, 表示启动的时候,需要提前设置需要显示的UI和显示层
    LayerManageControl::instance()->viewJudge();

    view.setSource(QUrl(QStringLiteral("qrc:/qml/main.qml")));

    view.show ();

    CategoryPlaylistProc::instance()->init(view.rootContext());

    // 通知显示层,加载预加载控件
    LayerManageControl::instance()->viewShowOver();

    int ret = app.exec();

    Application::uninit_all();

    return ret;
}
