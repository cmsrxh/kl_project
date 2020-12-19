#include <events/common_log.h>
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
    }

void ChipItemUnion::loadChipList(const ByteString &id, bool sorttype, int loadAction)
{
    int ret = false;
    mLoadAction = loadAction;


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
}

bool ChipItemUnion::isEmpty()
{
    bool res = true;
    switch (mChipType)
    {

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
//        ret = ((kl::ChipAudioList *)m_pChip)->loadNextPage();
        break;
    case PLAY_CHIP_TYPE_RADIO_CHIP:
//        ret = ((kl::ChipRadioList *)m_pChip)->loadNextPage();
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

bool ChipItemUnion::getUnionInfo(PlayingChipnion &info, int &index)
{
    int newIndex;
    return false;
}

bool ChipItemUnion::getUnionIndex(ByteString const &id, int &index)
{
    return false;
}

void ChipItemUnion::getUnionSlideBase(int &cur, int &dur, int index)
{
    GEN_Printf(LOG_DEBUG, "chipType=%d, index=%d", mChipType, index);
    if (mChipType == PLAY_CHIP_TYPE_BDC_PROGRAM_CHIP)
    {

    }
}
