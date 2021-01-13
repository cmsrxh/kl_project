#include <events/common_log.h>
#include <QQmlComponent>
#include <QQmlEngine>
#include <QQmlContext>
#include <QQuickItem>
#include <QQuickView>
#include "pages_manager.h"
#include "page_qml_item.h"

PageQmlItem::PageQmlItem()
    : mCurrentItem(Q_NULLPTR), mNeedShowItem(Q_NULLPTR)
    , mFactory(new PagesFactory)
{
    connect(PagesManager::instance(), SIGNAL(notifyPageId(WinSwitchInfo)), this, SLOT(onPageChanged(WinSwitchInfo)));
    connect(PagesManager::instance(), SIGNAL(notifyBack()), this, SLOT(onBack()));

    connect(this, SIGNAL(createFailed(int,QString)), PagesManager::instance(), SLOT(onCreateFailed(int,QString)));
}

QObject *PageQmlItem::getNeedShowItem()
{
    return mNeedShowItem;
}

void PageQmlItem::onPageChanged(WinSwitchInfo const &info)
{
    qDebug() << "create page:" << info.type;

    createPage(info);
}

void PageQmlItem::onBack()
{
    PageQueue::WinNode win  = mFactory->mShowList.pop();

    if (win.winId > WIN_TYPE_INVALID)
    {
        const PagesFactory::Node &node = mFactory->getNode(win.winId);

        mFactory->backCurrent(win);

        show(node);
    } else
    {
        GEN_Printf(LOG_DEBUG, "show is empty.");
    }
}

void PageQmlItem::qmlItemOver()
{
    qDebug() << "construct over, show first";

    createPage(WinSwitchInfo(WIN_TYPE_TWO));
}

void PageQmlItem::setCurrentCreateResult(QObject *comp, QObject *item)
{
    mCurComp = comp;
    mCurItem = item;
}

void PageQmlItem::setCurrentErrorString(const QString &errStr)
{
    mErrorString = errStr;
}

void PageQmlItem::createPage(WinSwitchInfo const &info)
{
#if 1
    PagesFactory::Node &node = mFactory->getNode(info.type);

    if (node.item)
    {
        mFactory->windowSwitch(info);

        show(node);
    } else
    {
        qDebug() << "c++" << node.url;
        QVariant ret;

        mCurComp = NULL;
        mCurItem = NULL;
        QMetaObject::invokeMethod(this, "createItem", Q_RETURN_ARG(QVariant, ret), Q_ARG(QVariant, node.url));

        node.comp = qobject_cast<QQmlComponent *> (mCurComp);
        node.item = qobject_cast<QQuickItem *> (mCurItem);

        GEN_Printf(LOG_DEBUG, "componnect: %p, item: %p", mCurComp, mCurItem);
        if (ret.toBool())
        {
            qDebug () << "create success.";

            mFactory->windowSwitch(info);

            show(node);
        } else
        {
            qCritical() << "create '" << node.url << "' failed.";
            Q_EMIT createFailed(info.type, mErrorString);
        }
    }
#else
    QObject *item = PagesManager::instance()->createItem(this, 0);
    if (item)
    {
        item->setProperty("width", this->width());
        item->setProperty("height", this->height());
        item->setProperty("visible", true);
        item->setParent(this);
        qDebug () << this->width() << this->height();
    }

    GEN_Printf(LOG_DEBUG, "item: %p", item);
#endif
}

void PageQmlItem::show(const PagesFactory::Node &node)
{
    if (node.item == mCurrentItem)
    {
        qWarning() << "current item equal";
        return;
    }
#ifdef NORMAL_SHOW
    if (mCurrentItem)
    {
        mCurrentItem->setVisible(false);
    }

    node.item->setVisible(true);

    mCurrentItem = node.item;
#else
    if (mCurrentItem)
    {
        mNeedShowItem = node.item;
        QMetaObject::invokeMethod(this, "animateShow", Q_ARG(QVariant, true));
        // 被qml中的属性赋值
        // qDebug() << "animal show.";
    } else
    {
        node.item->setVisible(true);
        mCurrentItem = node.item;
    }
#endif

    Q_EMIT PagesManager::instance()->pageChanged(mFactory->getPrev(), mFactory->getCurrent());
}

