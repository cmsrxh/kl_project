#include <events/common_log.h>
#include "kl_url/kl_chip_audio_list.h"
#include "kl_url/kl_chip_radio_list.h"
#include "kl_url/kl_broadcast_item_programlist.h"
#include "chip_item_model.h"
#include "chip_item_union.h"
#include "chip_item_play_manage.h"

ChipItemUnion::ChipItemUnion(int type)
    : mLoadAction(0), mChipType(0)
    , m_pChip(nullptr)
{
    if (0 == type)
    {
        mChipType = CHIP_ITEM_AUDIO;
    } else if (3 == type)
    {
        mChipType = CHIP_ITEM_RADIO;
    } else if (11 == type)
    {
        mChipType = CHIP_ITEM_BDC_PROGRAM;
    } else
    {
        GEN_Printf(LOG_ERROR, "Media Type = %d is not invalid.", type);
        assert(0);
    }
}

void ChipItemUnion::loadChipList(const ByteString &id, bool sorttype, int loadAction)
{
    mLoadAction = loadAction;

    switch (mChipType)
    {
    case CHIP_ITEM_AUDIO:
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
    case CHIP_ITEM_RADIO:
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
    case CHIP_ITEM_BDC_PROGRAM:
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
        break;
    }
}

void ChipItemUnion::dataPrepare()
{
    bool isEmpty = true;
    switch (mChipType)
    {
    case CHIP_ITEM_AUDIO:
        isEmpty = ((kl::ChipAudioList *)m_pChip)->nodes().empty();
        break;
    case CHIP_ITEM_RADIO:
        isEmpty = ((kl::ChipRadioList *)m_pChip)->nodes().empty();
        break;
    case CHIP_ITEM_BDC_PROGRAM:
        isEmpty = ((kl::BroadcastItemProgramlist *)m_pChip)->nodes().empty();
        break;
    default:
        break;
    }
    if (isEmpty)
    {
        GEN_Printf(LOG_DEBUG, "Chip Item List is empty.");
        gPlayInstance->loadError(mLoadAction, 0, "Chip Item List is empty.");
    } else
    {
        gPlayInstance->dataLoadOver((long)this, mLoadAction);
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
    case CHIP_ITEM_AUDIO:
        genCatesByAudioItem(((kl::ChipAudioList *)m_pChip)->nodes(), parent->vec());
        break;
    case CHIP_ITEM_RADIO:
        genCatesByRadioItem(((kl::ChipRadioList *)m_pChip)->nodes(), parent->vec());
        break;
    case CHIP_ITEM_BDC_PROGRAM:
        genCatesByBDCProgramItem(((kl::BroadcastItemProgramlist *)m_pChip)->nodes(), parent->vec());
        break;
    default:
        break;
    }
}

int ChipItemUnion::itemCount()
{
    int count = 0;
    switch (mChipType)
    {
    case CHIP_ITEM_AUDIO:
        count = ((kl::ChipAudioList *)m_pChip)->getCount();
        break;
    case CHIP_ITEM_RADIO:
        count = ((kl::ChipRadioList *)m_pChip)->nodes().size();
        break;
    case CHIP_ITEM_BDC_PROGRAM:
        count = ((kl::BroadcastItemProgramlist *)m_pChip)->nodes().size();
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
    case CHIP_ITEM_AUDIO:
        return ((kl::ChipAudioList *)m_pChip)->loadNextPage();
    case CHIP_ITEM_RADIO:
        return ((kl::ChipRadioList *)m_pChip)->loadNextPage();
    case CHIP_ITEM_BDC_PROGRAM:
    default:
        break;
    }
    return false;
}

bool ChipItemUnion::getUnionInfoByIndex(MusicChipItemUnion &info, int index)
{
    if (index < 0) return false;

    switch (mChipType)
    {
    case CHIP_ITEM_AUDIO:
    {
        GEN_Printf(LOG_DEBUG, "Get Info by index=%d, %p", index, this);
        ListTable<kl::AudioItem> &nodes = ((kl::ChipAudioList *)m_pChip)->nodes();
        ListTable<kl::AudioItem>::iterator it = nodes.begin();
        for ( ; it != nodes.end() && index; ++it, --index);
        if (it != nodes.end())
        {
            info.chipId  = it->audioId;
            info.name    = it->audioName;
            info.image   = it->audioPic;
            info.playUrl = it->mp3PlayUrl64;
            info.desc    = it->audioDes;
            return true;
        }
        GEN_Printf(LOG_DEBUG, "Get Info by index=%d", index);
        return false;
    }
    case CHIP_ITEM_RADIO:
    {
        ListTable<kl::RadioItem> &nodes = ((kl::ChipRadioList *)m_pChip)->nodes();
        ListTable<kl::RadioItem>::iterator it = nodes.begin();
        for ( ; it != nodes.end() && index; ++it, --index);
        if (it != nodes.end())
        {
            info.chipId  = it->audioId;
            info.name    = it->audioName;
            info.image   = it->audioPic;
            info.playUrl = it->mp3PlayUrl64;
            info.desc    = it->audioDes;
            return true;
        }
        return false;
    }
    case CHIP_ITEM_BDC_PROGRAM:
    {
        ListTable<kl::BDCastProgramItem> &nodes = ((kl::BroadcastItemProgramlist *)m_pChip)->nodes();
        ListTable<kl::BDCastProgramItem>::iterator it = nodes.begin();
        for ( ; it != nodes.end() && index; ++it, --index);
        if (it != nodes.end())
        {
            info.chipId  = it->programId;
            info.name    = it->title;
            info.image   = it->broadcastImg;
            info.playUrl = it->playUrl;
            info.desc    = it->desc;
            return true;
        }
        return false;
    }
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

        tmp->chipId  = it->audioId;
        tmp->name    = it->audioName;
        tmp->image   = it->audioPic;
        tmp->playUrl = it->mp3PlayUrl64;
        tmp->desc    = it->audioDes;

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

        tmp->chipId  = it->audioId;
        tmp->name    = it->audioName;
        tmp->image   = it->audioPic;
        tmp->playUrl = it->mp3PlayUrl64;
        tmp->desc    = it->audioDes;

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

        tmp->chipId  = it->programId;
        tmp->name    = it->title;
        tmp->image   = it->broadcastImg;
        tmp->playUrl = it->playUrl;
        tmp->desc    = it->desc;

        vec.push_back(tmp);
    }
}
