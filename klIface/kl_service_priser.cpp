#include <events/common_log.h>
#include "ipc/ipc_data_package.h"
#include "ipc/service_iface_base.h"
#include "../klApi/kl_search_manage.h"

#include "kl_service_priser.h"

KLServicePriser::KLServicePriser()
{

}

void KLServicePriser::cmdPriser(IPCDataPackage *in, IPCDataPackage *out)
{
    out->setCmdType(ipc::SERVICE_REPLY_OK);

    switch (in->getCmd())
    {
    case CLIENT_CALL_SEARCH_KEY_WORD:
    {
        Data::Unpack un(in->packet());
        char *id      = in->dataGetString(un);
        char *keyword = in->dataGetString(un);
        kl::SearchManage::instance()->searchKeyword(id, keyword);
        GEN_Printf(LOG_DEBUG, "Search[%s] keyword: %s", id, keyword);
        break;
    }
    case CLIENT_CALL_PLAY_SEARCH_NAME:
    {
        Data::Unpack un(in->packet());
        char *id   = in->dataGetString(un);
        char *name = in->dataGetString(un);
        kl::SearchManage::instance()->playSearchName(id, name);
        break;
    }
    case CLIENT_CALL_PLAY_SEARCH_INDEX:
    {
        Data::Unpack un(in->packet());
        char *id   = in->dataGetString(un);
        int index  = in->dataGetInt(un);
        kl::SearchManage::instance()->playSearchIndex(id, index);
        break;
    }
    case CLIENT_CALL_PLAY_PAUSE:
    {
        Data::Unpack un(in->packet());
        char *id   = in->dataGetString(un);
        kl::SearchManage::instance()->playPause(id);
        break;
    }
    case CLIENT_CALL_PLAY_PLAYING:
    {
        Data::Unpack un(in->packet());
        char *id   = in->dataGetString(un);
        kl::SearchManage::instance()->playPlaying(id);
        break;
    }
    case CLIENT_CALL_PLAY_NEXT:
    {
        Data::Unpack un(in->packet());
        char *id   = in->dataGetString(un);
        kl::SearchManage::instance()->playNext(id);
        break;
    }
    case CLIENT_CALL_PLAY_PREV:
    {
        Data::Unpack un(in->packet());
        char *id   = in->dataGetString(un);
        kl::SearchManage::instance()->playPrev(id);
        break;
    }
    default:
    {
        GEN_Printf(LOG_DEBUG, "UNKown Cmd: %d", in->getCmd());
        break;
    }
    }
}
