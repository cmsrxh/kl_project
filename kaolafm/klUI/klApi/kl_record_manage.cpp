#include "events/common_log.h"
#include "net_util/net_common.h"
#include "app_common.h"
#include "kl_record_manage.h"

kl::RecordManage::RecordManage()
{
    setSaveFile(LOCAL_KL_SAVE_RECORD_FILE);

    loadNodesFile();
}

void kl::RecordManage::addHistoryItem(kl::RecordItem *item)
{
    int op = LOCAL_RECORD_OP_STATUS_HISTORY_APPEND;

    ListTable<RecordItem>::iterator it = mNodes.begin();

    GEN_Printf(LOG_DEBUG, "history update: %s", item->name.string());

    for (; it != mNodes.end(); ++it)
    {
        if (it->id == item->id
                && it->parentId == item->parentId)
        {
            break;
        }
    }
    // 当前项已经在历史记录中，移动到最前面, 同时通知UI
    if (it != mNodes.end())
    {
        it->clear();
        mNodes.remove(it);

        op = LOCAL_RECORD_OP_STATUS_HISTORY_CHANGE;
    }

    // 当前项是新增项, 同时通知UI
    kl::RecordItem tmp;
    item->copy(&tmp);

    mNodes.push_front(tmp);
    execStatus(op, 0, (long)item);
}

void kl::RecordManage::historyClear()
{
    clearData();
    execStatus(LOCAL_RECORD_OP_STATUS_HISTORY_CLEARALL, 0, 0);
}


