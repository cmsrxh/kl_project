#ifndef PAGES_FACTORY_H
#define PAGES_FACTORY_H

#include <QString>
#include <global/win_global_common.h>
#include "page_queue.h"

class QQuickItem;
class QQuickView;
class QQmlEngine;
class QQmlComponent;
class PageQmlItem;
class PageQueue;
class PagesFactory
{
public:
    class Node
    {
    public:
        Node (QString const &url)
            : url(url)
            , comp(Q_NULLPTR)
            , item(Q_NULLPTR)
            , isHome(false)
        {}

        QString         url;
        QQmlComponent  *comp;
        QQuickItem     *item;
        bool            isHome;
    };

    PagesFactory();

    /**
     * @brief getNode
     * @param type [in] 当前需要的界面id
     * @return 返回界面工厂数组
     */
    Node &getNode(int type);

    /**
     * @brief getCurrent
     * @return 获取当前界面类型
     */
    WinSwitchInfo getCurrent() const;

    /**
     * @brief getPrev
     * @return 获取上一个界面类型
     */
    WinSwitchInfo getPrev() const;

private:
    /**
     * @brief windowSwitch
     * @param info [in] 当前窗口
     * @note 界面前进时调用
     * @warning 只是在主进程的调用(即槽函数中调用)
     */
    void windowSwitch(WinSwitchInfo const &info);

    /**
     * @brief setCurrent
     * @param win [in] 当前窗口
     * @note 当前界面后撤时调用,设置从显示队列中弹出的窗口
     * @warning 只是在主进程的调用(即槽函数中调用)
     */
    void backCurrent(PageQueue::WinNode const &win);

    /**
     * @brief isHome
     * @param win []
     * @return true: 表示当前界面是主页home界面,是所有界面的入口
     * @details 判断当前界面是否是所有界面的入口
     *  可以有多个
     */
    bool isHome(PageQueue::WinNode const &win);

    PageQueue::WinNode   mCurrent;

    Node            mOrigin[WIN_TYPE_MAX];

    PageQueue       mShowList;
    PageQueue       mHideList;

    friend class PageQmlItem;
};

#endif // PAGES_FACTORY_H
