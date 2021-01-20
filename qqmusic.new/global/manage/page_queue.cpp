#include <events/common_log.h>
#include "page_queue.h"

PageQueue::PageQueue()
    : mList(mQueue.refQueue(QueueContainerInc <WinNode>::PREPARE))
{
}

PageQueue::WinNode PageQueue::front() const
{
    return mList.empty() ? WIN_TYPE_INVALID : mList.back();
}

void PageQueue::push(const PageQueue::WinNode &cur)
{
    ListTable<WinNode>::iterator it = mList.begin();

    for (; it != mList.end(); ++it)
    {
        if (*it == cur)
        {
            mList.move(it);
            break;
        }
    }

    if (it == mList.end())
    {
        WinNode *n = mQueue.getFreeNode();

        *n = cur;

        mQueue.putNode(QueueContainerInc <WinNode>::PREPARE, n);
        // mList.push_back();
    }
}

PageQueue::WinNode PageQueue::pop()
{
    WinNode id = WIN_TYPE_INVALID;

    if (!mList.empty())
    {
        WinNode &it = mList.back();

        id = it;

        mQueue.move(QueueContainerInc <WinNode>::PREPARE, QueueContainerInc <WinNode>::FREE, &it);
    }

    return id;
}

void PageQueue::remove(const PageQueue::WinNode &cur)
{
//    GEN_Printf(LOG_DEBUG, "remove: %d", cur.winId);
    ListTable<WinNode>::iterator it = mList.begin();

    for (; it != mList.end(); ++it)
    {
//        GEN_Printf(LOG_DEBUG, "winId: %d", it->winId);
        if (*it == cur)
        {
            mQueue.move(QueueContainerInc <WinNode>::PREPARE, QueueContainerInc <WinNode>::FREE, it);
            return;
        }
    }
}

void PageQueue::clean()
{
    mQueue.clearPrepare();
}

void PageQueue::profile()
{
    ListTable<WinNode>::iterator it = mList.begin();

    for (; it != mList.end(); ++it)
    {
        GEN_Printf(LOG_DEBUG, "winId: %d", it->winId);
    }
}
