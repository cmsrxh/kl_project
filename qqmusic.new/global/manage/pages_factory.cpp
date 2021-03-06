#include <events/common_log.h>
#include <QQmlComponent>
#include <QQuickItem>
#include "pages_factory.h"

PagesFactory::PagesFactory()
    : mCurrent(WIN_TYPE_INVALID)
    , mOrigin {
          Node("qrc:/qml/playList/CatePlayList.qml"),
          Node("qrc:/qml/singerList/SingerList.qml"),
          Node(""),
          }
{

}

PagesFactory::Node &PagesFactory::getNode(int type)
{
    SF_ASSERT(type > WIN_TYPE_INVALID && type < WIN_TYPE_MAX );

    return mOrigin[type];
}

void PagesFactory::windowSwitch(WinSwitchInfo const &info)
{
    PageQueue::WinNode cur(info.type, info.arg);

    if (mCurrent == cur)
    {
        GEN_Printf(LOG_WARN, "cur:%d equal.", cur.winId);
        return;
    }

    // 如果当前显示界面是主页,表示清除'show'队列
    if (isHome(cur))
    {
        mShowList.clean();
    }
    // 显示队列存放当前前进的显示窗口,队列中的窗口,都是隐藏的
    // 用于后撤的时候,重新显示出来
    else if (mCurrent.isValid())
    {
        GEN_Printf(LOG_DEBUG, "push prev new: %d", mCurrent.winId);
        mShowList.push(mCurrent);
    }

    GEN_Printf(LOG_DEBUG, "current win: %d", cur.winId);
    // 最上层的显示不进队列
    mCurrent = cur;

    mShowList.profile();

    // 除去两个列表中,当前显示的窗口
    mShowList.remove(cur);
    mHideList.remove(cur);
}

void PagesFactory::backCurrent(const PageQueue::WinNode &cur)
{
    mCurrent = cur;

    // 如果当前显示界面是主页,表示清除'show'队列
    if (isHome(cur))
    {
        mShowList.clean();
    }

    mHideList.push(cur);
}

bool PagesFactory::isHome(const PageQueue::WinNode &win)
{
    int type = win.winId;

    SF_ASSERT(type > WIN_TYPE_INVALID && type < WIN_TYPE_MAX );

    return mOrigin[type].isHome;
}

WinSwitchInfo PagesFactory::getCurrent() const
{
    return WinSwitchInfo((GlobalWinType)mCurrent.winId, mCurrent.winArg);
}

WinSwitchInfo PagesFactory::getPrev() const
{
    PageQueue::WinNode cur = mShowList.front();
    return WinSwitchInfo((GlobalWinType)cur.winId, cur.winArg);
}

void PagesFactory::cleanShow()
{
    ListTable<PageQueue::WinNode> &lst = mShowList.list();
    ListTable<PageQueue::WinNode>::iterator it = lst.begin();
    for (; it != lst.end(); ++it)
    {
        int winId = it->winId;

        SF_ASSERT(winId > WIN_TYPE_INVALID && winId < WIN_TYPE_MAX );
        if (false == mOrigin[winId].isHome)
        {
            if (winId != mCurrent.winId)
            {
                GEN_Printf(LOG_DEBUG, "----remove: %d----", winId);

                //delete mOrigin[winId].comp;
                mOrigin[winId].item->deleteLater();

                mOrigin[winId].comp = Q_NULLPTR;
                mOrigin[winId].item = Q_NULLPTR;

                ListTable<PageQueue::WinNode>::iterator delIt = it;
                --it;
                mShowList.queue().move(QueueContainerInc <PageQueue::WinNode>::PREPARE, QueueContainerInc <PageQueue::WinNode>::FREE, delIt);
            }
        }
    }
}

void PagesFactory::cleanHide()
{

}
