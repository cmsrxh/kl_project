#include "events/common_log.h"
#include "net_util/net_common.h"
#include "app_common.h"
#include "kl_record_manage.h"

kl::RecordManage::RecordManage()
{
    setSaveFile(LOCAL_KL_SAVE_RECORD_FILE);

    loadNodesFile();
}
