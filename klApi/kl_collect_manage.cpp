#include "events/common_log.h"
#include "app_common.h"
#include "net_util/net_common.h"
#include "kl_collect_manage.h"

kl::CollectManage::CollectManage()
{
    setSaveFile(LOCAL_KL_SAVE_COLLECT_FILE);


    loadNodesFile();
}
