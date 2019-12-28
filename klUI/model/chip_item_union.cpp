#include <events/common_log.h>
#include "kl_url/kl_chip_audio_list.h"
#include "kl_url/kl_chip_radio_list.h"
#include "chip_item_model.h"
#include "chip_item_union.h"

ChipItemUnion::ChipItemUnion(int type, ChipItemModel *parent)
    : mChipType(0)
    , m_pChip(nullptr)
    , m_pParentModel(parent)
{
    if (0 == type)
    {
        mChipType = CHIP_ITEM_AUDIO;
    } else if (3 == type)
    {
        mChipType = CHIP_ITEM_RADIO;
    } else
    {
        GEN_Printf(LOG_ERROR, "Media Type = %d is not invalid.", type);
        assert(0);
    }
}

void ChipItemUnion::loadChipList(const ByteString &id, bool sorttype)
{
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
            audioList->setUINotify(this);
            audioList->obtain();
            m_pChip = audioList;
        }

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
            radioList->setUINotify(this);
            radioList->obtain();
            m_pChip = radioList;
        }
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
    default:
        break;
    }
    if (isEmpty)
    {
        GEN_Printf(LOG_DEBUG, "Chip Item List is empty.");
        Q_EMIT m_pParentModel->loadError(0, "Chip Item List is empty.");
    } else
    {
        Q_EMIT m_pParentModel->dataLoadOver((long)this);
    }
}

void ChipItemUnion::errorInfo(int type, const char *err_str)
{
    GEN_Printf(LOG_DEBUG, "Chip Item List Error, %s", err_str);
    Q_EMIT m_pParentModel->loadError(type, err_str);
}

void ChipItemUnion::onLoadOver()
{
    switch (mChipType)
    {
    case CHIP_ITEM_AUDIO:
        genCatesByAudioItem(((kl::ChipAudioList *)m_pChip)->nodes(), m_pParentModel->vec());
        break;
    case CHIP_ITEM_RADIO:
        genCatesByRadioItem(((kl::ChipRadioList *)m_pChip)->nodes(), m_pParentModel->vec());
        break;
    default:
        break;
    }
}

int ChipItemUnion::itemCount()
{
    switch (mChipType)
    {
    case CHIP_ITEM_AUDIO:
        return ((kl::ChipAudioList *)m_pChip)->getCount();
        break;
    case CHIP_ITEM_RADIO:
        return ((kl::ChipRadioList *)m_pChip)->nodes().size();
        break;
    default:
        break;
    }
    return 0;
}

bool ChipItemUnion::loadNextPage()
{
    switch (mChipType)
    {
    case CHIP_ITEM_AUDIO:
        return ((kl::ChipAudioList *)m_pChip)->loadNextPage();
    case CHIP_ITEM_RADIO:
        return ((kl::ChipRadioList *)m_pChip)->loadNextPage();
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
        // GEN_Printf(LOG_DEBUG, "Get Info by index=%d", index);
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
    }
    return false;
}

void ChipItemUnion::genCatesByRadioItem(ListTable<kl::RadioItem> &nodes, VectorTable<MusicChipItemUnion *> &vec)
{
    ListTable<kl::RadioItem>::iterator it = nodes.begin();

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
    ListTable<kl::AudioItem>::iterator it = nodes.begin();

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
