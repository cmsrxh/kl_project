#ifndef PAGE_QUEUE_H
#define PAGE_QUEUE_H

#include <global/win_global_common.h>
#include <util/queue_container_inc.h>

/**
 * @brief The PageSimple class
 * @note 窗口队列处理
 */
class PageQueue
{
public:
    class WinNode
    {
    public:
        WinNode()
            : winId(WIN_TYPE_INVALID)
            , winArg(0)
        {}
        WinNode(int id)
            : winId(id)
            , winArg(0)
        {}
        WinNode(int id, long arg)
            : winId(id)
            , winArg(arg)
        {}

        bool operator==(const WinNode &n)
        {
            return (winId == n.winId &&
                    winArg == n.winArg) ? true
                                        : false;
        }

        bool isValid() const
        {
            return winId != WIN_TYPE_INVALID;
        }

        // 窗口id, 对因数组下标 PagesFactory::mOrigin
        int  winId;
        // 窗口下需要传递的参数,一般作为主窗口下的子窗口,也可以是指针地址
        // 即使winId一样,只要winArg不一样,也会参与调度
        long winArg;
    };

    PageQueue();

    WinNode  front() const;
    WinNode  front()
    {
        return const_cast <PageQueue *>(this)->front();
    }

    void push(WinNode const &n);

    WinNode pop();

    void remove(WinNode const &n);

    void clean();

    void profile();

    // clean operate.
    ListTable<WinNode> &list()
    {
        return mList;
    }
    QueueContainerInc <WinNode> &queue()
    {
        return mQueue;
    }

private:
    QueueContainerInc <WinNode> mQueue;
    ListTable<WinNode>         &mList;
};

#endif // PAGE_QUEUE_H
