#include <events/common_log.h>
#include "kl_iface_common.h"
#include "ipc/client_iface_base.h"
#include "ipc/ipc_data_package.h"
#include "kl_client_call.h"
#include "cJSON.h"

#define JSON_VALUETRING_SWAP_BYTESTRING(parent, name, proc) \
    do { \
        cJSON *obj = cJSON_GetObjectItem(parent, #name); \
        if(obj) \
        { \
            proc = obj->valuestring; \
        } \
    } while(0)

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
            std::list<client::SearchItem> lst;

            Data::Unpack un(pack->packet());
            char *id = pack->dataGetString(un);

            if (!KLClientCall::instance()->checkId(id))
            {
                GEN_Printf(LOG_ERROR, "current id invalid, %s != %s", id, KLClientCall::instance()->mID);
                break;
            }
            if (pack->dataGetData(un, data, len))
            {
                // GEN_Printf(LOG_DEBUG, "id[%s] json: %s", id, data);
                cJSON *root = cJSON_Parse((char *)data, len);
                cJSON *result = cJSON_GetObjectItem(root, "result");
                if (result)
                {
                    cJSON *dataList;

                    if ((dataList = cJSON_GetObjectItem(result, "dataList")))
                    {
                        for (cJSON *item = dataList->child; NULL != item; item = item->next )
                        {
                            client::SearchItem tmp;

//                            JSON_VALUETRING_SWAP_BYTESTRING(item, id, tmp.id);
                            JSON_VALUETRING_SWAP_BYTESTRING(item, name, tmp.name);
//                            JSON_VALUETRING_SWAP_BYTESTRING(item, img, tmp.img);
//                            JSON_VALUETRING_SWAP_BYTESTRING(item, type, tmp.type);
//                            JSON_VALUETRING_SWAP_BYTESTRING(item, albumName, tmp.albumName);
//                            JSON_VALUETRING_SWAP_BYTESTRING(item, source, tmp.source);
//                            JSON_VALUETRING_SWAP_BYTESTRING(item, playUrl, tmp.playUrl);
//                            JSON_VALUETRING_SWAP_BYTESTRING(item, sourceName, tmp.sourceName);
//                            JSON_VALUETRING_SWAP_BYTESTRING(item, oldId, tmp.oldId);
//                            JSON_VALUETRING_SWAP_BYTESTRING(item, callback, tmp.callback);
//                            JSON_VALUETRING_SWAP_BYTESTRING(item, listenNum, tmp.listenNum);
//                            JSON_VALUETRING_SWAP_BYTESTRING(item, isShowRed, tmp.isShowRed);
//                            JSON_VALUETRING_SWAP_BYTESTRING(item, isRequest, tmp.isRequest);
//                            JSON_VALUETRING_SWAP_BYTESTRING(item, originalDuration, tmp.originalDuration);

                            lst.push_back(tmp);
                        }
                    }
                }
            } else
            {
                GEN_Printf(LOG_WARN, "data priser failed.");
            }

            KLClientCall::instance()->m_pResult->searchResult(lst);

            break;
        }
        default:
        {
            GEN_Printf(LOG_INFO, "CMD: %d, %d, %d, %d", pack->getCmd(), pack->getHeadDataSize(), pack->bufferLen(), pack->dataLen());
            break;
        }
        }
    }
};

static MsgPriser gMsgInstance;
bool KLClientCall::initClientResult(client::ClientResult *res)
{
    m_pResult = res;
    int me[] =
    {
        SERVICE_NOTIFY_SEARCH_RESULT,
    };

    return m_pHandler->registMessage(me, ARRAY_SIZE(me));
}

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

bool KLClientCall::searchKeyWord(const char *str)
{
    return commonOneString(CLIENT_CALL_SEARCH_KEY_WORD, str);
}

bool KLClientCall::playSearchWord(const char *str)
{
    return commonOneString(CLIENT_CALL_PLAY_SEARCH_NAME, str);
}

bool KLClientCall::playSearchIndex(int index)
{
    IPCDataPackage in;
    IPCDataPackage out;

    in.setCmdType(CLIENT_CALL_PLAY_SEARCH_INDEX);

    in.allocSetString(mID);
    in.allocSetInt(index);

    m_pHandler->cmdCall(&in, &out);

    //TODO: 'out' process
    if (out.getCmd() == ipc::SERVICE_REPLY_OK)
    {
        // GEN_Printf(LOG_DEBUG, "Set common cmd OK !!!");
        return true;
    } else
    {
        GEN_Printf(LOG_ERROR, "Set common cmd failed: %d !!!", out.getCmd());
        return false;
    }
}

bool KLClientCall::playPause()
{
    return commonNoArg(CLIENT_CALL_PLAY_PAUSE);
}

bool KLClientCall::playPlaying()
{
    return commonNoArg(CLIENT_CALL_PLAY_PLAYING);
}

bool KLClientCall::playNext()
{
    return commonNoArg(CLIENT_CALL_PLAY_NEXT);
}

bool KLClientCall::playPrev()
{
    return commonNoArg(CLIENT_CALL_PLAY_PREV);
}

void KLClientCall::setIdentity(char id[])
{
    strncpy(mID, id, 31);
}

bool KLClientCall::checkId(const char *id)
{
    if (!id) return false;
    return 0 == strncmp(mID, id, 31);
}

KLClientCall::KLClientCall()
    : m_pHandler(new ClientIfaceBase)
    , mID{"test"}
{
    m_pHandler->setMsgPriser(80081, &gMsgInstance);
}

bool KLClientCall::commonOneString(int cmd, const char *str)
{
    IPCDataPackage in;
    IPCDataPackage out;

    in.setCmdType(cmd);

    in.allocSetString(mID);
    in.allocSetString(str);

    m_pHandler->cmdCall(&in, &out);

    //TODO: 'out' process
    if (out.getCmd() == ipc::SERVICE_REPLY_OK)
    {
        // GEN_Printf(LOG_DEBUG, "Set common cmd OK !!!");
        return true;
    } else
    {
        GEN_Printf(LOG_ERROR, "Set common cmd failed: %d !!!", out.getCmd());
        return false;
    }
}

bool KLClientCall::commonNoArg(int cmd)
{
    IPCDataPackage in;
    IPCDataPackage out;

    in.setCmdType(cmd);

    in.allocSetString(mID);

    m_pHandler->cmdCall(&in, &out);

    //TODO: 'out' process
    if (out.getCmd() == ipc::SERVICE_REPLY_OK)
    {
        // GEN_Printf(LOG_DEBUG, "Set common cmd OK !!!");
        return true;
    } else
    {
        GEN_Printf(LOG_ERROR, "Set common cmd failed: %d !!!", out.getCmd());
        return false;
    }
}
