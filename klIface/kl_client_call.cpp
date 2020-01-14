#include <events/common_log.h>
#include "kl_iface_common.h"
#include "ipc/client_iface_base.h"
#include "ipc/ipc_data_package.h"
#include "kl_client_call.h"

/**
 * @brief The MsgPriser class
 * @details 用于接收kl发来的数据，并进行分析
 */
class MsgPriser : public ClientMsgPriserProc
{
public:
    void msgPriser(IPCDataPackage *pack)
    {
        switch (pack->getCmd())
        {
        case SERVICE_NOTIFY_SEARCH_RESULT:
        {
            char *data;
            int   len;
            Data::Unpack un(pack->packet());
            if (pack->dataGetData(un, data, len))
            {
                GEN_Printf(LOG_DEBUG, "json: %s", data);
            } else
            {
                GEN_Printf(LOG_WARN, "data priser failed.");
            }
            break;
        }
        default:
            GEN_Printf(LOG_INFO, "CMD: %d, %d, %d, %d", pack->getCmd(), pack->getHeadDataSize(), pack->bufferLen(), pack->dataLen());
            break;
        }
    }
};

static MsgPriser gMsgInstance;
bool KLClientCall::initClientIface(ClientMsgPriserProc *priser)
{
    if (priser)
    {
        m_pHandler->setMsgPriser(80081, priser);
    }

    int me[] =
    {
        SERVICE_NOTIFY_SEARCH_RESULT,
    };

    return m_pHandler->registMessage(me, ARRAY_SIZE(me));
}

void KLClientCall::searchKeyWord(const char *str)
{
    IPCDataPackage in;
    IPCDataPackage out;

    in.setCmdType(CLIENT_CALL_SEARCH_KEY_WORD);

    in.allocSetString(str);

    m_pHandler->cmdCall(&in, &out);

    //TODO: 'out' process
    if (out.getCmd() == ipc::SERVICE_REPLY_OK)
    {
        // GEN_Printf(LOG_DEBUG, "Set common cmd OK !!!");
        return ;
    } else
    {
        GEN_Printf(LOG_ERROR, "Set common cmd failed: %d !!!", out.getCmd());
        return ;
    }
}

KLClientCall::KLClientCall()
    : m_pHandler(new ClientIfaceBase)
{
    m_pHandler->setMsgPriser(80081, &gMsgInstance);
}
