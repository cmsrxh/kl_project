#include "events/common_log.h"
#include "app_common.h"
#include "net_util/net_common.h"
#include "kl_download_manage.h"

kl::DownloadManage::DownloadManage()
{
    setSaveFile(LOCAL_KL_DOWNLOAD_FILE);

    loadNodesFile();
}
