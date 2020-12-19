#include <events/common_log.h>
#include "ipc/ipc_data_package.h"
#include "ipc/service_iface_base.h"
#include "kl_iface_common.h"
#include "kl_service_notify.h"

// operate UI
/**
 * @brief The KlCtrlPriser class
 * @details 客户端发来的命令，用操作kl应用，进行各种处理
 */
class KlCtrlPriser : public ServiceCmdPriserProc
{
public:
    void cmdPriser(IPCDataPackage *in, IPCDataPackage *out)
    {
        out->setCmdType(ipc::SERVICE_REPLY_OK);
        GEN_Printf(LOG_DEBUG, "Test Cmd: %d not process.", in->getCmd());
    }
};

static KlCtrlPriser gPriserInstance;
void KLServiceNotify::initPriser(ServiceCmdPriserProc *priser)
{
    m_pHandler->setCmdPriser(priser);
}

void KLServiceNotify::notifySearchResult(const char *id, const char *data, int size)
{
    IPCDataPackage in;

    /* 添加此函数的命令标志 */
    in.setCmdType(SERVICE_NOTIFY_SEARCH_RESULT);

    /* 添加参数 */
    in.allocSetString(id);
    in.allocSetData(data, size);

    m_pHandler->msgNotify(&in);
}

KLServiceNotify::KLServiceNotify()
    : m_pHandler(new ServiceIfaceBase(80081, 256))
{
    m_pHandler->setCmdPriser(&gPriserInstance);
    m_pHandler->init();
}
