#include <events/common_log.h>
#include <QQmlComponent>
#include <QQmlEngine>
#include <QQmlContext>
#include <QQuickItem>
#include <QQuickView>
#include <QQmlError>
#include <QQmlIncubator>
#include "page_qml_item.h"
#include "pages_manager.h"

//class CreateStatus : public QQmlIncubator
//{
//  public:

//};
PagesManager::PagesManager()
    : mMainViewEnable(true)
    , mViewHandler(nullptr)
{
    qRegisterMetaType<WinSwitchInfo>("WinSwitchInfo");

    qmlRegisterType<PageQmlItem>("page.item", 1, 0, "PageItem");
}

bool PagesManager::backgroundEnable() const
{
    return true;
}

#if 0
// can't to show
QObject *PagesManager::createItem(QObject *parent, int pageId)
{
    QQmlIncubator  status(QQmlIncubator::Synchronous);
    QQmlComponent *comp = new QQmlComponent(mViewHandler->engine(), mMainUrl, parent);

    SF_ASSERT(comp);

    comp->create(status, mViewHandler->rootContext());

    if (status.status() == QQmlIncubator::Ready)
    {
        return status.object();
    } else
    {
        qDebug() << status.status() << status.errors();
    }

    return Q_NULLPTR;
}
#endif

void PagesManager::start(int win, long arg)
{
    GEN_Printf(LOG_DEBUG, "start window win: %d, arg: %ld", win, arg);

    Q_EMIT notifyPageId(WinSwitchInfo((GlobalWinType)win, arg));
}

void PagesManager::back()
{
    GEN_Printf(LOG_DEBUG, "back window");
    Q_EMIT notifyBack();
}

void PagesManager::onCreateFailed(int winType, const QString &reason)
{
    qDebug() << "create failed," << winType << reason;
}

void PagesManager::init(QQuickView *view)
{
    mViewHandler = view;
}

bool PagesManager::mainViewEnable() const
{
    return mMainViewEnable;
}

void PagesManager::setMainViewEnable(bool mainViewEnable)
{
    mMainViewEnable = mainViewEnable;
    Q_EMIT mainViewEnableChanged();
}
