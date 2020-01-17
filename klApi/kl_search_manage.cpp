#include <events/common_log.h>
#include "kl_url/kl_res_voice_search_all.h"
#include "kl_url/kl_res_suggestion_word.h"
#include "application.h"
#include "kl_search_manage.h"
#include "model/kl_data_proc.h"
#include "model/kl_local_data_proc.h"
#include "kl_ui_proc.h"
#include "model/chip_item_union.h"
#include "../klIface/kl_service_notify.h"

extern KLUIProc *gInstance;

static void searchNotify(kl::VoiceSearchAll *obj, bool res)
{
    GeneralQEvt *evt = static_cast<GeneralQEvt *> (Application::newEvt(SIG_KL_SEARCH_RESULT_PROC, sizeof(GeneralQEvt)));
    if (evt)
    {
        evt->wParam  = res;
        evt->pHander = obj;
        Application::instance()->post(evt);
    }
}

kl::SearchManage::SearchManage()
    : m_pDeleteSearch(nullptr)
    , m_pSearchChip(new ChipItemUnion(PLAY_CHIP_TYPE_SEARCH_LOAD))
{
}

kl::SearchManage::SearchStatus *kl::SearchManage::getSearchNode(const ByteString &id)
{
    ListTable<SearchStatus>::iterator it = mList.begin();

    for (; it != mList.end(); ++it)
    {
        if (it->id == id)
        {
            return &(*it);
        }
    }
    return nullptr;
}

void kl::SearchManage::playSearchItem(const char *id, kl::SearchManage::SearchStatus *item, int index)
{
    int start = index;
    ListTable<SearchItem> &nodes = item->search->nodes();
    ListTable<SearchItem>::iterator it = nodes.begin();
    for (; it != nodes.end(); ++it)
    {
        if (index-- <= 0)
        {
            item->playState    = 1;
            item->currentIndex = start;
            /* 0：专辑, 1：碎片, 3：智能电台, 11：传统电台 */
            GEN_Printf(LOG_DEBUG, "Search by index: %d", start);
            if (item->search == m_pDeleteSearch)
            {
                Q_EMIT gInstance->searchProc(4, start, 0);
            } else
            {
                Q_EMIT gInstance->searchProc(1, start, (long)item->search);
            }
            mPlaySearch = *item;
            strncpy(mPlayId, id, 31);
            return;
        }
    }
}

kl::SearchManage::~SearchManage()
{
    ListTable<SearchStatus>::iterator it = mList.begin();

    for (; it != mList.end(); ++it)
    {
        it->id.clear();
        delete it->search;
    }

    delete m_pSearchChip;
}

void kl::SearchManage::searchKeyword(const char *id, const char *keyWord)
{
    VoiceSearchAll *search = nullptr;
    ListTable<SearchStatus>::iterator it = mList.begin();

    for (; it != mList.end(); ++it)
    {
        if (it->id == id)
        {
            search = it->search;
            break;
        }
    }

    if (search)
    {
        if (mPlaySearch.search != search)
        {
            search->search(keyWord);
            return;
        } else
        {
            mList.remove(it);
        }
    }

    search = new VoiceSearchAll(keyWord);
    search->setNofity(searchNotify);
    search->obtain();

    SearchStatus node;

    node.search    = search;
    node.playState = 0;
    node.id        = ByteString::allocString(id);

    mList.push_back(node);

}

void kl::SearchManage::onSearchResult(kl::VoiceSearchAll *search, bool )
{
    char *id = nullptr;
    NetBuffer *data = search->pData();
    ListTable<SearchStatus>::iterator it = mList.begin();

    for (; it != mList.end(); ++it)
    {
        if (it->search == search)
        {
            id = it->id.string();
            break;
        }
    }

    if (data)
    {
        KLServiceNotify::instance()->notifySearchResult(id, (char *)data->buffer(), data->size());
    } else
    {
        KLServiceNotify::instance()->notifySearchResult(id, "{\"result\": {\"dataList\":[]}}",
                                                        sizeof("{\"result\": {\"dataList\":[]}}") - 1);
    }
}

void kl::SearchManage::playSearchName(const char *id, const char *name)
{
    SearchStatus *state = getSearchNode(id);
    if (state)
    {
        int index = 0;
        ListTable<SearchItem> &nodes = state->search->nodes();
        ListTable<SearchItem>::iterator it = nodes.begin();
        for (; it != nodes.end(); ++it, ++index)
        {
            if (it->name == name)
            {
                state->playState    = 1;
                state->currentIndex = index;
                /* 0：专辑, 1：碎片, 3：智能电台, 11：传统电台 */
                GEN_Printf(LOG_DEBUG, "Search by name: %s, index: %d", name, index);
                if (state->search == m_pDeleteSearch)
                {
                    Q_EMIT gInstance->searchProc(4, index, 0);
                } else
                {
                    Q_EMIT gInstance->searchProc(1, index, (long)state->search);
                }
                mPlaySearch = *state;
                strncpy(mPlayId, id, 31);
                return;
            }
        }
    } else
    {
        GEN_Printf(LOG_ERROR, "No exist id[%s] process", id);
    }
}

void kl::SearchManage::playSearchIndex(const char *id, int index)
{
    SearchStatus *state = getSearchNode(id);
    if (state)
    {
        playSearchItem(id, state, index);
    } else
    {
        GEN_Printf(LOG_ERROR, "No exist id[%s] process", id);
    }
}

void kl::SearchManage::playPause(const char *id)
{
    if (0 == strncmp(mPlayId, id, 31))
    {
        if (mPlaySearch.playState)
        {
            gInstance->qmlPause();
        } else
        {
            GEN_Printf(LOG_WARN, "Id[%s] not in playing", id);
        }
    } else
    {
        GEN_Printf(LOG_ERROR, "No exist id[%s] process", id);
    }
}

void kl::SearchManage::playPlaying(const char *id)
{
    if (0 == strncmp(mPlayId, id, 31))
    {
        if (mPlaySearch.playState)
        {
            gInstance->qmlPlay();
        } else
        {
            GEN_Printf(LOG_WARN, "Id[%s] not in playing", id);
        }
    } else
    {
        GEN_Printf(LOG_ERROR, "No exist id[%s] process", id);
    }
}

void kl::SearchManage::playNext(const char *id)
{
    if (0 == strncmp(mPlayId, id, 31))
    {
        if (mPlaySearch.playState)
        {
            Q_EMIT gInstance->searchProc(2, 0, 0);
        } else
        {
            GEN_Printf(LOG_WARN, "Id[%s] not in playing", id);
        }
    } else
    {
        GEN_Printf(LOG_ERROR, "No exist id[%s] process", id);
    }
}

void kl::SearchManage::playPrev(const char *id)
{
    if (0 == strncmp(mPlayId, id, 31))
    {
        if (mPlaySearch.playState)
        {
            Q_EMIT gInstance->searchProc(3, 0, 0);
        } else
        {
            GEN_Printf(LOG_WARN, "Id[%s] not in playing", id);
        }
    } else
    {
        GEN_Printf(LOG_ERROR, "No exist id[%s] process", id);
    }
}

void kl::SearchManage::setCurSearch(kl::VoiceSearchAll *seach)
{
    if (m_pDeleteSearch)
    {
        delete m_pDeleteSearch;
    }
    m_pDeleteSearch = seach;
}

ChipItemUnion *kl::SearchManage::genUnion(kl::VoiceSearchAll *seach)
{
    m_pSearchChip->setChipHandler(seach);
    return m_pSearchChip;
}
