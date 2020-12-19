#include <events/common_log.h>
#include "kl_url/kl_chip_audio_list.h"
#include "kl_url/kl_chip_radio_list.h"
#include "kl_url/kl_broadcast_item_programlist.h"
#include "kl_url/kl_res_voice_search_all.h"
#include "kl_collect_manage.h"
#include "kl_download_manage.h"
#include "kl_record_manage.h"
#include "pop_tip_manage.h"
#include "chip_item_model.h"
#include "chip_item_union.h"
#include "current_backup.h"
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
    case PLAY_CHIP_TYPE_PREV_PLAYING_RECORD:
        mChipType = type;
        break;
    default:
        GEN_Printf(LOG_ERROR, "Media Type = %d is not invalid.", type);
        SF_ASSERT(0);
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
    int ret = false;
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
        ret = audioList->obtain();
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
        ret = radioList->obtain();
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
        ret = bdcProgram->obtain();
        break;
    }
    default:
        GEN_Printf(LOG_WARN, "Not exist type=%d", mChipType);
        SF_ASSERT(0);
        break;
    }

    PopTipManage::instance()->klObjectObtainStart(ret, mChipType, mLoadAction);
}

void ChipItemUnion::dataPrepare()
{
    PopTipManage::instance()->klObjectObtainOver(mChipType, mLoadAction);
    Q_EMIT gPlayInstance->dataLoadOver((long)this, mLoadAction);
    
}

void ChipItemUnion::errorInfo(int type, const ByteString &err_str)
{
    switch (type)
    {
    case LOAD_EMPTY_DATA:        // 分析数据正确，但是得到的数据是空
        PopTipManage::instance()->klLoadDataExportEmpty(mChipType, mLoadAction, err_str);
        break;
    case LOAD_PRISER_JSOC_ERROR: // 不能正确解析json数据
        PopTipManage::instance()->klLoadDataPriserExcept(mChipType, mLoadAction, err_str);
        break;
    case LOAD_SYS_API_FAILED:    // libcurl下载反馈的错误信息
        PopTipManage::instance()->sysNetLoadApiExcept(mChipType, mLoadAction, err_str);
        break;
    default :
        SF_ASSERT(0);
        break;
    }
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
    case PLAY_CHIP_TYPE_PREV_PLAYING_RECORD:
        CurrentBackup::instance()->pushRecPlayInto(parent->vec());
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
    case PLAY_CHIP_TYPE_PREV_PLAYING_RECORD:
        res = ((CurrentBackup *)m_pChip)->empty();
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
    case PLAY_CHIP_TYPE_PREV_PLAYING_RECORD:
        count = 1;
    default:
        break;
    }
    // GEN_Printf(LOG_DEBUG, "Count = %d", count);
    return count;
}

bool ChipItemUnion::loadNextPage(int loadAction)
{
    bool ret = false;
    mLoadAction = loadAction;

    switch (mChipType)
    {
    case PLAY_CHIP_TYPE_AUDIO_CHIP:
        ret = ((kl::ChipAudioList *)m_pChip)->loadNextPage();
        break;
    case PLAY_CHIP_TYPE_RADIO_CHIP:
        ret = ((kl::ChipRadioList *)m_pChip)->loadNextPage();
        break;
    default:
        break;
    }
    if (ret)
    {
        PopTipManage::instance()->klObjectObtainStart(true, mChipType, mLoadAction);
    }

    return ret;
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
            newIndex = ++index;
            for (; it != nodes.end() && newIndex; ++it, newIndex--);
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
            newIndex = ++index;
            for (; it != nodes.end() && newIndex; ++it, newIndex--);
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
            index = --newIndex;
        } else
        {
            newIndex = ++index;
            for (; it != nodes.end() && newIndex; ++it, newIndex--);            
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
            newIndex = ++index;
            for (; it != nodes.end() && newIndex; ++it, newIndex--);
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
        SF_ASSERT(0);
        break;
    }
    return false;
}

bool ChipItemUnion::getUnionIndex(ByteString const &id, int &index)
{
    switch (mChipType)
    {
    case PLAY_CHIP_TYPE_AUDIO_CHIP:
    {
        ListTable<kl::AudioItem> &nodes = ((kl::ChipAudioList *)m_pChip)->nodes();
        ListTable<kl::AudioItem>::iterator it = nodes.begin();
        for (; it != nodes.end(); ++it, index++)
        {
            if (id == it->audioId) return true;
        }
    }
    case PLAY_CHIP_TYPE_RADIO_CHIP:
    {
        ListTable<kl::RadioItem> &nodes = ((kl::ChipRadioList *)m_pChip)->nodes();
        ListTable<kl::RadioItem>::iterator it = nodes.begin();
        for (; it != nodes.end(); ++it, index++)
        {
            if (id == it->audioId) return true;
        }
    }
    case PLAY_CHIP_TYPE_BDC_PROGRAM_CHIP:
    {
        ListTable<kl::BDCastProgramItem> &nodes = ((kl::BroadcastItemProgramlist *)m_pChip)->nodes();
        ListTable<kl::BDCastProgramItem>::iterator it = nodes.begin();
        for (; it != nodes.end(); ++it, index++)
        {
            if (id == it->programId) return true;
        }
    }
    case PLAY_CHIP_TYPE_LOCAL_LOAD:
    {
        ListTable<kl::RecordItem> &nodes = ((kl::DownloadManage *)m_pChip)->nodes();
        ListTable<kl::RecordItem>::iterator it = nodes.begin();
        for (; it != nodes.end(); ++it, index++)
        {
            if (id == it->id) return true;
        }
    }
    default:
        GEN_Printf(LOG_ERROR, "chip type=%d is out of range.", mChipType);
        SF_ASSERT(0);
        break;
    }
    return false;
}

void ChipItemUnion::getUnionSlideBase(int &cur, int &dur, int index)
{
    GEN_Printf(LOG_DEBUG, "chipType=%d, index=%d", mChipType, index);
    if (mChipType == PLAY_CHIP_TYPE_BDC_PROGRAM_CHIP)
    {
        ListTable<kl::BDCastProgramItem> &nodes = ((kl::BroadcastItemProgramlist *)m_pChip)->nodes();
        ListTable<kl::BDCastProgramItem>::iterator it = nodes.begin();
        for (; it != nodes.end() && index; ++it, --index);
//        {
//            GEN_Printf(LOG_DEBUG, "[%d]startTime=%s, finishTime=%s", index, it->startTime.string(), it->finishTime.string());
//        }
        if (it != nodes.end())
        {
            GEN_Printf(LOG_DEBUG, "startTime=%s, finishTime=%s", it->startTime.string(), it->finishTime.string());
            uint64_t start  = strtoull(it->startTime.string(), NULL, 10);
            uint64_t finish = strtoull(it->finishTime.string(), NULL, 10);

            cur  = (start  / 1000) % (24 * 60 * 60);
            dur  = (finish / 1000) % (24 * 60 * 60);
        }
    }
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
