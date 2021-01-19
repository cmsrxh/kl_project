#include <QTimer>
#include <QFile>
#include <QDebug>
#include <QQuickView>
#include <QQmlEngine>
#include <QQuickItem>
#include <QGuiApplication>
#include <global/manage/pages_manager.h>
#include "global_controller.h"


GlobalControls::GlobalControls(QObject *parent)
    : QObject(parent)
    , mViewHandler(Q_NULLPTR)
{
    connect(PagesManager::instance(), SIGNAL(pageChanged(WinSwitchInfo, WinSwitchInfo)), this, SLOT(onPageChanaged(WinSwitchInfo, WinSwitchInfo)));
}

GlobalControls::~GlobalControls()
{
}

void GlobalControls::init(QQuickView *view)
{
    mViewHandler = view;
}

void GlobalControls::windowMinimize()
{
    mViewHandler->setWindowState(Qt::WindowMinimized);
}

void GlobalControls::programExit()
{
    qApp->exit();
}


// #include <QDebug>
// QDebug operator<<(QDebug, const WinSwitchInfo &);

void GlobalControls::onPageChanaged(WinSwitchInfo const &prevPage, WinSwitchInfo const &newPage)
{
    qDebug() << "window page change: " << prevPage.type << newPage.type;
}

