#include "events/common_log.h"
#include "kl_client_result_common.h"

client::ClientResult::ClientResult()
{

}

void client::ClientResult::searchResult(const std::list<client::SearchItem> &lst)
{
    std::list<client::SearchItem>::const_iterator it = lst.begin();

    for (; it != lst.end(); ++it)
    {
        GEN_Printf(LOG_DEBUG, "%s", it->name.c_str());
    }
}
