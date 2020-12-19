#include "events/common_log.h"
#include "app_common.h"
#include "net_util/net_common.h"
#include "kl_download_manage.h"

kl::DownloadManage::DownloadManage()
{
    setSaveFile(LOCAL_KL_DOWNLOAD_FILE);

    loadNodesFile();
}

void kl::DownloadManage::saveItem(kl::RecordItem *item)
{
    ListTable<kl::RecordItem>::iterator it = mNodes.begin();

    for (; it != mNodes.end(); ++it)
    {
        if (item->id == it->id
                && item->parentId == it->parentId)
        {
            GEN_Printf(LOG_DEBUG, "id=%s is exist!", item->id.string());
            item->clear();
            delete item;
            return;
        }
    }

    mNodes.push_back(*item);
    delete item;
}
