#include <events/common_log.h>
#include "kl_url/kl_chip_audio_list.h"
#include "kl_url/kl_chip_radio_list.h"
#include "kl_url/kl_broadcast_item_programlist.h"
#include "kl_url/kl_res_voice_search_all.h"
#include "kl_collect_manage.h"
#include "kl_download_manage.h"
#include "kl_record_manage.h"
#include "chip_item_model.h"
#include "chip_item_union.h"
#include "chip_item_play_manage.h"

ChipItemUnion::ChipItemUnion(int type)
    : mLoadAction(0), mChipType(0)
    , m_pChip(nullptr)
{
    switch (type) {
    case PLAY_CHIP_TYPE_ALBUM:
        mChipType = PLAY_CHIP_TYPE_AUDIO_CHIP;
        break;
    case PLAY_CHIP_TYPE_TYPE_RADIO:
        mChipType = PLAY_CHIP_TYPE_RADIO_CHIP;
        break;
    case PLAY_CHIP_TYPE_BROADCAST:
        mChipType = PLAY_CHIP_TYPE_BDC_PROGRAM_CHIP;
        break;
    case PLAY_CHIP_TYPE_LOCAL_LOAD:
    case PLAY_CHIP_TYPE_COLLECT_RECORD:
    case PLAY_CHIP_TYPE_HISTROY_RECORD:
    case PLAY_CHIP_TYPE_SEARCH_LOAD:
        mChipType = type;
        break;
    default:
        GEN_Printf(LOG_ERROR, "Media Type = %d is not invalid.", type);
        assert(0);
        break;
    }
}

ChipItemUnion::~ChipItemUnion()
{
    if (!m_pChip) return;
    switch (mChipType)
    {
    case PLAY_CHIP_TYPE_AUDIO_CHIP:
        delete ((kl::ChipAudioList *)m_pChip);
        break;
    case PLAY_CHIP_TYPE_RADIO_CHIP:
        delete ((kl::ChipRadioList *)m_pChip);
        break;
    case PLAY_CHIP_TYPE_BDC_PROGRAM_CHIP:
        delete ((kl::BroadcastItemProgramlist *)m_pChip);
        break;
    default:
        break;
    }
}

void ChipItemUnion::loadChipList(const ByteString &id, bool sorttype, int loadAction)
{
    mLoadAction = loadAction;

    switch (mChipType)
    {
    case PLAY_CHIP_TYPE_AUDIO_CHIP:
    {
        kl::ChipAudioList *audioList;
        if (m_pChip)
        {
            audioList = ((kl::ChipAudioList *)m_pChip);
        } else
        {
            audioList = new kl::ChipAudioList(id, sorttype);            
            m_pChip = audioList;
        }
        audioList->setUINotify(this);
        audioList->obtain();
        break;
    }
    case PLAY_CHIP_TYPE_RADIO_CHIP:
    {
        kl::ChipRadioList *radioList;
        if (m_pChip)
        {
            radioList = ((kl::ChipRadioList *)m_pChip);
        } else
        {
            radioList = new kl::ChipRadioList(id);

            m_pChip = radioList;
        }
        radioList->setUINotify(this);
        radioList->obtain();
        break;
    }
    case PLAY_CHIP_TYPE_BDC_PROGRAM_CHIP:
    {
        kl::BroadcastItemProgramlist *bdcProgram;
        if (m_pChip)
        {
            bdcProgram = ((kl::BroadcastItemProgramlist *)m_pChip);
        } else
        {
            bdcProgram = new kl::BroadcastItemProgramlist(id);

            m_pChip = bdcProgram;
        }
        bdcProgram->setUINotify(this);
        bdcProgram->obtain();
        break;
    }
    default:
        GEN_Printf(LOG_WARN, "Not exist type=%d", mChipType);
        assert(0);
        break;
    }
}

void ChipItemUnion::dataPrepare()
{
    bool isEmpty = true;
    switch (mChipType)
    {
    case PLAY_CHIP_TYPE_AUDIO_CHIP:
        isEmpty = ((kl::ChipAudioList *)m_pChip)->nodes().empty();
        break;
    case PLAY_CHIP_TYPE_RADIO_CHIP:
        isEmpty = ((kl::ChipRadioList *)m_pChip)->nodes().empty();
        break;
    case PLAY_CHIP_TYPE_BDC_PROGRAM_CHIP:
        isEmpty = ((kl::BroadcastItemProgramlist *)m_pChip)->nodes().empty();
        break;    
    default:
        GEN_Printf(LOG_WARN, "Not exist type=%d", mChipType);
        assert(0);
        return;
    }
    if (isEmpty)
    {
        GEN_Printf(LOG_DEBUG, "Chip Item List is empty.");
        gPlayInstance->loadError(mLoadAction, 0, "Chip Item List is empty.");
    } else
    {
        Q_EMIT gPlayInstance->dataLoadOver((long)this, mLoadAction);
    }
}

void ChipItemUnion::errorInfo(int type, const char *err_str)
{
    GEN_Printf(LOG_DEBUG, "Chip Item List Error, %s", err_str);
    gPlayInstance->loadError(mLoadAction, type, QStringFromCString(err_str));
}

void ChipItemUnion::onLoadOver(ChipItemModel *parent)
{
    switch (mChipType)
    {
    case PLAY_CHIP_TYPE_AUDIO_CHIP:
        genCatesByAudioItem(((kl::ChipAudioList *)m_pChip)->nodes(), parent->vec());
        break;
    case PLAY_CHIP_TYPE_RADIO_CHIP:
        genCatesByRadioItem(((kl::ChipRadioList *)m_pChip)->nodes(), parent->vec());
        break;
    case PLAY_CHIP_TYPE_BDC_PROGRAM_CHIP:
        genCatesByBDCProgramItem(((kl::BroadcastItemProgramlist *)m_pChip)->nodes(), parent->vec());
        break;
    case PLAY_CHIP_TYPE_LOCAL_LOAD:
        genCatesByLocalLoadItem(((kl::DownloadManage *)m_pChip)->nodes(), parent->vec());
        break;
    case PLAY_CHIP_TYPE_COLLECT_RECORD:
        genCatesByCollectItem(((kl::CollectManage *)m_pChip)->nodes(), parent->vec());
        break;
    case PLAY_CHIP_TYPE_HISTROY_RECORD:
        genCatesByHistroyItem(((kl::RecordManage *)m_pChip)->nodes(), parent->vec());
        break;
    case PLAY_CHIP_TYPE_SEARCH_LOAD:
        genCatesBySearchItem(((kl::VoiceSearchAll *)m_pChip)->nodes(), parent->vec());
        break;
    default:
        break;
    }
}

bool ChipItemUnion::isEmpty()
{
    bool res = true;
    switch (mChipType)
    {
    case PLAY_CHIP_TYPE_AUDIO_CHIP:
        res = ((kl::ChipAudioList *)m_pChip)->nodes().empty();
        break;
    case PLAY_CHIP_TYPE_RADIO_CHIP:
        res = ((kl::ChipRadioList *)m_pChip)->nodes().empty();
        break;
    case PLAY_CHIP_TYPE_BDC_PROGRAM_CHIP:
        res = ((kl::BroadcastItemProgramlist *)m_pChip)->nodes().empty();
        break;
    case PLAY_CHIP_TYPE_LOCAL_LOAD:
        res = ((kl::DownloadManage *)m_pChip)->nodes().empty();
        break;
    case PLAY_CHIP_TYPE_COLLECT_RECORD:
        res = ((kl::CollectManage *)m_pChip)->nodes().empty();
        break;
    case PLAY_CHIP_TYPE_HISTROY_RECORD:
        res = ((kl::RecordManage *)m_pChip)->nodes().empty();
        break;
    case PLAY_CHIP_TYPE_SEARCH_LOAD:
        res = ((kl::VoiceSearchAll *)m_pChip)->nodes().empty();
        break;
    default:
        break;
    }

    return res;
}

int ChipItemUnion::itemCount()
{
    int count = 0;
    switch (mChipType)
    {
    case PLAY_CHIP_TYPE_AUDIO_CHIP:
        count = ((kl::ChipAudioList *)m_pChip)->getCount();
        break;
    case PLAY_CHIP_TYPE_RADIO_CHIP:
        count = ((kl::ChipRadioList *)m_pChip)->nodes().size();
        break;
    case PLAY_CHIP_TYPE_BDC_PROGRAM_CHIP:
        count = ((kl::BroadcastItemProgramlist *)m_pChip)->nodes().size();
        break;
    case PLAY_CHIP_TYPE_LOCAL_LOAD:
        count = ((kl::DownloadManage *)m_pChip)->nodes().size();
        break;
    case PLAY_CHIP_TYPE_COLLECT_RECORD:
        count = ((kl::CollectManage *)m_pChip)->nodes().size();
        break;
    case PLAY_CHIP_TYPE_HISTROY_RECORD:
        count = ((kl::RecordManage *)m_pChip)->nodes().size();
        break;
    case PLAY_CHIP_TYPE_SEARCH_LOAD:
        count = ((kl::VoiceSearchAll *)m_pChip)->nodes().size();
        break;
    default:
        break;
    }
    // GEN_Printf(LOG_DEBUG, "Count = %d", count);
    return count;
}

bool ChipItemUnion::loadNextPage(int loadAction)
{
    mLoadAction = loadAction;
    switch (mChipType)
    {
    case PLAY_CHIP_TYPE_AUDIO_CHIP:
        return ((kl::ChipAudioList *)m_pChip)->loadNextPage();
    case PLAY_CHIP_TYPE_RADIO_CHIP:
        return ((kl::ChipRadioList *)m_pChip)->loadNextPage();
    default:
        break;
    }
    return false;
}

bool ChipItemUnion::getUnionInfo(MusicChipItemUnion &info, int &index)
{
    int newIndex;
    switch (mChipType)
    {
    case PLAY_CHIP_TYPE_AUDIO_CHIP:
    {
        ListTable<kl::AudioItem> &nodes = ((kl::ChipAudioList *)m_pChip)->nodes();
        ListTable<kl::AudioItem>::iterator it = nodes.begin();
        if (-1 == index)
        {
            index = 0;
        } else
        {
            newIndex = index;
            for (; it != nodes.end() && newIndex; ++it, newIndex--);
            ++index;
        }
        if (it != nodes.end())
        {
            info.type       = PLAY_CHIP_TYPE_AUDIO_CHIP;
            info.parentId   = it->albumId;
            info.chipId     = it->audioId;
            info.parentName = it->albumName;
            info.name       = it->audioName;
            info.image      = it->audioPic;
            info.playUrl    = it->mp3PlayUrl64;
            info.desc       = it->audioDes;
            return true;
        } else
        {
            return false;
        }
    }
    case PLAY_CHIP_TYPE_RADIO_CHIP:
    {
        ListTable<kl::RadioItem> &nodes = ((kl::ChipRadioList *)m_pChip)->nodes();
        ListTable<kl::RadioItem>::iterator it = nodes.begin();
        if (-1 == index)
        {
            index = 0;
        } else
        {
            newIndex = index;
            for (; it != nodes.end() && newIndex; ++it, newIndex--);
            ++index;
        }
        if (it != nodes.end())
        {
            info.type       = PLAY_CHIP_TYPE_RADIO_CHIP;
            info.parentId   = it->albumId;
            info.chipId     = it->audioId;
            info.parentName = it->albumName;
            info.image      = it->audioPic;
            info.playUrl    = it->mp3PlayUrl64;
            info.desc       = it->audioDes;
            return true;
        } else
        {
            return false;
        }
    }
    case PLAY_CHIP_TYPE_BDC_PROGRAM_CHIP:
    {
        ListTable<kl::BDCastProgramItem> &nodes = ((kl::BroadcastItemProgramlist *)m_pChip)->nodes();
        ListTable<kl::BDCastProgramItem>::iterator it = nodes.begin();
        if (-1 == index)
        {
            newIndex = 0;
            for (; it != nodes.end(); ++it, ++newIndex);
            --it;
            index = newIndex;
        } else
        {
            newIndex = index;
            for (; it != nodes.end() && newIndex; ++it, newIndex--);
            if (it == nodes.end())
            {
                it    = nodes.begin();
                index = 0;
            } else
            {
                ++index;
            }
        }
        if (it != nodes.end())
        {
            info.type       = PLAY_CHIP_TYPE_BDC_PROGRAM_CHIP;
            info.parentId   = it->broadcastId;
            info.chipId     = it->programId;
            info.parentName = it->broadcastName;
            info.name       = it->title;
            info.image      = it->broadcastImg;
            info.playUrl    = it->playUrl;
            info.desc       = it->desc;
            return true;
        } else
        {
            return false;
        }
    }
    case PLAY_CHIP_TYPE_LOCAL_LOAD:
    {
        ListTable<kl::RecordItem> &nodes = ((kl::DownloadManage *)m_pChip)->nodes();
        ListTable<kl::RecordItem>::iterator it = nodes.begin();
        if (-1 == index)
        {
            index = 0;
        } else
        {
            newIndex = index;
            for (; it != nodes.end() && newIndex; ++it, newIndex--);
            ++index;
        }
        if (it != nodes.end())
        {
            info.type       = PLAY_CHIP_TYPE_LOCAL_LOAD;
            info.parentId   = it->parentId;
            info.chipId     = it->id;
            info.parentName = it->parentName;
            info.name       = it->name;
            info.image      = it->image;
            info.playUrl    = it->playUrl;
            return true;
        } else
        {
            return false;
        }
    }
    default:
        GEN_Printf(LOG_ERROR, "chip type=%d is out of range.", mChipType);
        assert(0);
        break;
    }
    return false;
}

void ChipItemUnion::genCatesByRadioItem(ListTable<kl::RadioItem> &nodes, VectorTable<MusicChipItemUnion *> &vec)
{
    int count = vec.size();
    ListTable<kl::RadioItem>::iterator it = nodes.begin();

    for ( ; it != nodes.end() && count; ++it, --count);

    for ( ; it != nodes.end(); ++it)
    {
        MusicChipItemUnion *tmp = new MusicChipItemUnion;

        tmp->type       = PLAY_CHIP_TYPE_RADIO_CHIP;
        tmp->parentId   = it->albumId;
        tmp->chipId     = it->audioId;
        tmp->parentName = it->albumName;
        tmp->name       = it->audioName;
        tmp->image      = it->audioPic;
        tmp->playUrl    = it->mp3PlayUrl64;
        tmp->desc       = it->audioDes;

        vec.push_back(tmp);
    }
}

void ChipItemUnion::genCatesByAudioItem(ListTable<kl::AudioItem> &nodes, VectorTable<MusicChipItemUnion *> &vec)
{
    int count = vec.size();
    ListTable<kl::AudioItem>::iterator it = nodes.begin();
    for ( ; it != nodes.end() && count; ++it, --count);

    for ( ; it != nodes.end(); ++it)
    {
        MusicChipItemUnion *tmp = new MusicChipItemUnion;

        tmp->type       = PLAY_CHIP_TYPE_AUDIO_CHIP;
        tmp->parentId   = it->albumId;
        tmp->chipId     = it->audioId;
        tmp->parentName = it->albumName;
        tmp->name       = it->audioName;
        tmp->image      = it->audioPic;
        tmp->playUrl    = it->mp3PlayUrl64;
        tmp->desc       = it->audioDes;

        // GEN_Printf(LOG_DEBUG, "[%d] %p, %s, %d", 0, tmp->name.string(), tmp->name.string(), tmp->name.size());

        vec.push_back(tmp);
    }
}

void ChipItemUnion::genCatesByBDCProgramItem(ListTable<kl::BDCastProgramItem> &nodes, VectorTable<MusicChipItemUnion *> &vec)
{
    int count = vec.size();
    ListTable<kl::BDCastProgramItem>::iterator it = nodes.begin();
    for ( ; it != nodes.end() && count; ++it, --count);

    for ( ; it != nodes.end(); ++it)
    {
        MusicChipItemUnion *tmp = new MusicChipItemUnion;

        tmp->type       = PLAY_CHIP_TYPE_BDC_PROGRAM_CHIP;
        tmp->parentId   = it->broadcastId;
        tmp->chipId     = it->programId;
        tmp->parentName = it->broadcastName;
        tmp->name       = it->title;
        tmp->image      = it->broadcastImg;
        tmp->playUrl    = it->playUrl;
        tmp->desc       = it->desc;
        tmp->startTime  = it->startTime;
        tmp->finishTime = it->finishTime;

        vec.push_back(tmp);
    }
}

void ChipItemUnion::genCatesByLocalLoadItem(ListTable<kl::RecordItem> &nodes, VectorTable<MusicChipItemUnion *> &vec)
{
    ListTable<kl::RecordItem>::iterator it = nodes.begin();

    for ( ; it != nodes.end(); ++it)
    {
        MusicChipItemUnion *tmp = new MusicChipItemUnion;

        tmp->type       = PLAY_CHIP_TYPE_LOCAL_LOAD;
        tmp->sub_type   = it->type;
        tmp->parentId   = it->parentId;
        tmp->chipId     = it->id;
        tmp->parentName = it->parentName;
        tmp->name       = (!it->name.empty()) ? it->name : it->parentName;
        tmp->image      = it->image;
        tmp->playUrl    = it->playUrl;

        vec.push_back(tmp);
    }
}

void ChipItemUnion::genCatesByCollectItem(ListTable<kl::RecordItem> &nodes, VectorTable<MusicChipItemUnion *> &vec)
{
    ListTable<kl::RecordItem>::iterator it = nodes.begin();

    for ( ; it != nodes.end(); ++it)
    {
        MusicChipItemUnion *tmp = new MusicChipItemUnion;

        tmp->type       = PLAY_CHIP_TYPE_COLLECT_RECORD;
        tmp->sub_type   = it->type;
        tmp->parentId   = it->parentId;
        tmp->chipId     = it->id;
        tmp->parentName = it->parentName;
        tmp->name       = (!it->name.empty()) ? it->name : it->parentName;
        tmp->image      = it->image;
        tmp->playUrl    = it->playUrl;

        vec.push_back(tmp);
    }
}

void ChipItemUnion::genCatesByHistroyItem(ListTable<kl::RecordItem> &nodes, VectorTable<MusicChipItemUnion *> &vec)
{
    ListTable<kl::RecordItem>::iterator it = nodes.begin();

    for ( ; it != nodes.end(); ++it)
    {
        MusicChipItemUnion *tmp = new MusicChipItemUnion;

        tmp->type       = PLAY_CHIP_TYPE_HISTROY_RECORD;
        tmp->sub_type   = it->type;
        tmp->parentId   = it->parentId;
        tmp->chipId     = it->id;
        tmp->parentName = it->parentName;
        tmp->name       = (!it->name.empty()) ? it->name : it->parentName;
        tmp->image      = it->image;
        tmp->playUrl    = it->playUrl;

        vec.push_back(tmp);
    }
}

void ChipItemUnion::genCatesBySearchItem(ListTable<kl::SearchItem> &nodes, VectorTable<MusicChipItemUnion *> &vec)
{
    ListTable<kl::SearchItem>::iterator it = nodes.begin();

    for ( ; it != nodes.end(); ++it)
    {
        MusicChipItemUnion *tmp = new MusicChipItemUnion;

        /* 0：专辑, 1：碎片, 3：智能电台, 11：传统电台 */
        switch (it->type.toInt())
        {
        case 0:
            tmp->sub_type   = PLAY_CHIP_TYPE_ALBUM;
            break;
        case 1:
            tmp->sub_type   = PLAY_CHIP_TYPE_AUDIO_CHIP;
            break;
        case 3:
            tmp->sub_type   = PLAY_CHIP_TYPE_TYPE_RADIO;
            break;
        case 11:
            tmp->sub_type   = PLAY_CHIP_TYPE_BROADCAST;
            break;
        default:
            GEN_Printf(LOG_ERROR, "search item type: %s is invalid", it->type.string());
            delete tmp;
            continue;
        }
        tmp->type       = PLAY_CHIP_TYPE_SEARCH_LOAD;
        tmp->parentId   = it->id;
        tmp->chipId     = it->id;
        tmp->parentName = it->albumName;
        tmp->name       = (!it->name.empty()) ? it->name : it->albumName;
        tmp->image      = it->img;
        tmp->playUrl    = it->playUrl;

        vec.push_back(tmp);
    }
}
