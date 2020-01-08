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

    for (; it != mNodes.end(); ++it)
    {
        if (it->id == item->id
                && it->parentId == item->parentId)
        {
            break;
        }
    }
    // 找到了需要删除， 同时通知UI
    if (it != mNodes.end())
    {

        it->clear();
        mNodes.remove(it);

        item->clear();

        execStatus(LOCAL_RECORD_OP_STATUS_BDC_COLLECT_REMOVE_IN_LIST, ptr);

        delete item;
    }
    // 未找到需要添加到列表中， 同时通知UI
    else
    {
        mNodes.push_front(*item);
        execStatus(LOCAL_RECORD_OP_STATUS_BDC_COLLECT_ADD_IN_LIST, ptr);
        delete item;
    }
}
