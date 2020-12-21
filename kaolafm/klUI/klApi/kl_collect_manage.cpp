#include "events/common_log.h"
#include "app_common.h"
#include "net_util/net_common.h"
#include "kl_collect_manage.h"

kl::CollectManage::CollectManage()
{
    setSaveFile(LOCAL_KL_SAVE_COLLECT_FILE);

    loadNodesFile();
}

void kl::CollectManage::opBDCItem(kl::RecordItem *item, long ptr)
{
    ListTable<RecordItem>::iterator it = mNodes.begin();
    GEN_Printf(LOG_DEBUG, "type=%d, %s,%s %s", item->type, item->id.string(), item->parentId.string(), item->name);

    for (; it != mNodes.end(); ++it)
    {
        // GEN_Printf(LOG_DEBUG, "type=%d, %s,%s %s", it->type, it->id.string(), it->parentId.string(), it->name);
        if ((it->parentId == item->parentId) &&
                (item->type == kl::OBJECT_BDC_ITEM_LIST
                 || item->type == kl::OBJECT_TYPERADIO_LIST
                 || item->type == kl::OBJECT_CHIP_RADIO_LIST
                 || item->type == kl::OBJECT_BDC_ITEM_PROGRAM
                 || item->type == kl::OBJECT_ALBUM_LIST
                 || item->id == it->id))
        {
            break;
        }
    }
    // 找到了需要删除， 同时通知UI
    if (it != mNodes.end())
    {
        GEN_Printf(LOG_DEBUG, "Need Detele.");
        execStatus(LOCAL_RECORD_OP_STATUS_BDC_COLLECT_REMOVE_IN_LIST, ptr, (long)item);

        it->clear();
        mNodes.remove(it);
    }
    // 未找到需要添加到列表中， 同时通知UI
    else
    {
        kl::RecordItem tmp;
        item->copy(&tmp);
        GEN_Printf(LOG_DEBUG, "Need Add.");
        mNodes.push_front(tmp);
        execStatus(LOCAL_RECORD_OP_STATUS_BDC_COLLECT_ADD_IN_LIST, ptr, (long)item);
    }
}

/**
 * @brief kl::SaveRecord::checkCurrentItem
 * @param item [in]  item需要clear 和释放
 * @details 核对当前播放项，是否收藏了，异步遍历，防止收藏列表过多，遍历时拖慢主线程
 */
void kl::CollectManage::checkCurrentItem(kl::RecordItem *item)
{
    ListTable<RecordItem>::iterator it = mNodes.begin();

    for (; it != mNodes.end(); ++it)
    {
        //GEN_Printf(LOG_DEBUG, "type=%d, %s,%s %s", it->type, it->id.string(), it->parentId.string(), it->name);

        if ((it->parentId == item->parentId) &&
                (item->type == kl::OBJECT_BDC_ITEM_LIST
                 || item->type == kl::OBJECT_TYPERADIO_LIST
                 || item->type == kl::OBJECT_CHIP_RADIO_LIST
                 || item->type == kl::OBJECT_BDC_ITEM_PROGRAM
                 || item->type == kl::OBJECT_ALBUM_LIST
                 || item->id == it->id))
        {
            break;
        }
    }

    if (it != mNodes.end())
    {
        execStatus(LOCAL_RECORD_OP_STATUS_CHECK_IN_LIST, 0, (long)item);
    } else
    {
        execStatus(LOCAL_RECORD_OP_STATUS_CHECK_NOT_IN_LIST, 0, (long)item);
    }
}

/**
 * @brief kl::SaveRecord::opCurrentItem
 * @param item [in] item需要clear 和释放
 * @details 核对当前播放项，是否收藏了，如果当前项在收藏列表就取消收藏，反之添加, 并通知界面
 */
void kl::CollectManage::opCurrentItem(kl::RecordItem *item)
{
    GEN_Printf(LOG_DEBUG, "type=%d, %s,%s %s", item->type, item->id.string(), item->parentId.string(), item->name);
    ListTable<RecordItem>::iterator it = mNodes.begin();

    for (; it != mNodes.end(); ++it)
    {
        //GEN_Printf(LOG_DEBUG, "type=%d, %s,%s %s", it->type, it->id.string(), it->parentId.string(), it->name);

        if ((it->parentId == item->parentId) &&
                (item->type == kl::OBJECT_BDC_ITEM_LIST
                 || item->type == kl::OBJECT_TYPERADIO_LIST
                 || item->type == kl::OBJECT_CHIP_RADIO_LIST
                 || item->type == kl::OBJECT_BDC_ITEM_PROGRAM
                 || item->type == kl::OBJECT_ALBUM_LIST
                 || item->id == it->id))
        {
            break;
        }
    }
    // 找到了需要删除， 同时通知UI
    if (it != mNodes.end())
    {
        GEN_Printf(LOG_DEBUG, "Need Detele.");
        execStatus(LOCAL_RECORD_OP_STATUS_REMOVE_IN_LIST, 0, (long)item);

        it->clear();
        mNodes.remove(it);
    }
    // 未找到需要添加到列表中， 同时通知UI
    else
    {
        kl::RecordItem tmp;
        item->copy(&tmp);

        GEN_Printf(LOG_DEBUG, "Need Add.");
        mNodes.push_front(tmp);
        execStatus(LOCAL_RECORD_OP_STATUS_ADD_IN_LIST, 0, (long)item);
    }
}
