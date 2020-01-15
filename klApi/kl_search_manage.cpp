#include <events/common_log.h>
#include "kl_url/kl_res_voice_search_all.h"
#include "kl_url/kl_res_suggestion_word.h"
#include "application.h"
#include "kl_search_manage.h"
#include "model/kl_data_proc.h"
#include "model/kl_local_data_proc.h"
#include "kl_ui_proc.h"
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

void kl::SearchManage::playSearchItem(kl::SearchManage::SearchStatus *item, int index)
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
            switch (it->type.toInt())
            {
            case 0:
                KLDataProc::instance()->localItemAlbumPlay(CURREN_PLAY_SOURCE_CLIENT_SEARCH_LIST, index, it->id, ByteString());
                break;
            case 1:
                KLDataProc::instance()->localItemAlbumAudioPlay(CURREN_PLAY_SOURCE_CLIENT_SEARCH_LIST, index, it->id);
                break;
            case 3:
                KLDataProc::instance()->localItemTypeRadioPlay(CURREN_PLAY_SOURCE_CLIENT_SEARCH_LIST, index, it->id, ByteString());
                break;
            case 11:
                KLDataProc::instance()->localItemBroadcastPlay(CURREN_PLAY_SOURCE_CLIENT_SEARCH_LIST, index, it->id, ByteString());
                break;
            default:
                break;
            }
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
        search->search(keyWord);
    } else
    {
        search = new VoiceSearchAll(keyWord);
        search->setNofity(searchNotify);
        search->obtain();

        SearchStatus node;

        node.search    = search;
        node.playState = 0;
        node.id        = ByteString::allocString(id);

        mList.push_back(node);
    }
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
                switch (it->type.toInt())
                {
                case 0:
                    KLDataProc::instance()->localItemAlbumPlay(CURREN_PLAY_SOURCE_CLIENT_SEARCH_LIST, index, it->id, ByteString());
                    break;
                case 1:
                    KLDataProc::instance()->localItemAlbumAudioPlay(CURREN_PLAY_SOURCE_CLIENT_SEARCH_LIST, index, it->id);
                    break;
                case 3:
                    KLDataProc::instance()->localItemTypeRadioPlay(CURREN_PLAY_SOURCE_CLIENT_SEARCH_LIST, index, it->id, ByteString());
                    break;
                case 11:
                    KLDataProc::instance()->localItemBroadcastPlay(CURREN_PLAY_SOURCE_CLIENT_SEARCH_LIST, index, it->id, ByteString());
                    break;
                default:
                    break;
                }
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
        playSearchItem(state, index);
    } else
    {
        GEN_Printf(LOG_ERROR, "No exist id[%s] process", id);
    }
}

void kl::SearchManage::playPause(const char *id)
{
    SearchStatus *state = getSearchNode(id);
    if (state)
    {
        if (state->playState)
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
    SearchStatus *state = getSearchNode(id);
    if (state)
    {
        if (state->playState)
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
    SearchStatus *state = getSearchNode(id);
    if (state)
    {
        if (state->playState)
        {
            playSearchItem(state, state->currentIndex + 1);
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
    SearchStatus *state = getSearchNode(id);
    if (state)
    {
        if (state->playState)
        {
            playSearchItem(state, state->currentIndex - 1);
        } else
        {
            GEN_Printf(LOG_WARN, "Id[%s] not in playing", id);
        }
    } else
    {
        GEN_Printf(LOG_ERROR, "No exist id[%s] process", id);
    }
}
