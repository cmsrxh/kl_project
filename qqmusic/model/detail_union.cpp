#include "events/common_log.h"
#include "detail_union.h"
#include "detail_qobject.h"
#include "data_proc.h"
#include "ui_proc.h"

extern UIProc *gInstance;

DetailUnion::DetailUnion(int type)
    : mDetailType(type)
    , mLoadAction(-1)
    , m_pDetail(NULL)
{    
}

DetailUnion::~DetailUnion()
{
    if (!m_pDetail) return;

}

void DetailUnion::loadDetail(const ByteString &id, int loadAction)
{
    int ret = false;
    mLoadAction = loadAction;

    
    PopTipManage::instance()->klObjectObtainStart(ret, mDetailType, mLoadAction);
}

void DetailUnion::dataPrepare()
{
    // GEN_Printf(LOG_DEBUG, "data prepared, mDetailType=%d", mDetailType);
    PopTipManage::instance()->klObjectObtainOver(mDetailType, mLoadAction);

    Q_EMIT DetailQobject::instance()->dataLoadOver((long)this);
}

void DetailUnion::errorInfo(int type, const ByteString &err_str)
{
    switch (type)
    {
    case LOAD_EMPTY_DATA:        // 分析数据正确，但是得到的数据是空
        PopTipManage::instance()->klLoadDataExportEmpty(mDetailType, mLoadAction, err_str);
        break;
    case LOAD_PRISER_JSOC_ERROR: // 不能正确解析json数据
        PopTipManage::instance()->klLoadDataPriserExcept(mDetailType, mLoadAction, err_str);
        break;
    case LOAD_SYS_API_FAILED:    // libcurl下载反馈的错误信息
        PopTipManage::instance()->sysNetLoadApiExcept(mDetailType, mLoadAction, err_str);
        break;
    default :
        assert(0);
        break;
    }
}

void DetailUnion::getDetail(MusicDetail &detail)
{
    GEN_Printf(LOG_DEBUG, "Get Detail info: %d", mDetailType);
    }

/*
    switch (mDetailType)
    {
    case PLAY_CHIP_TYPE_ALBUM:
        ((kl::AlbumDetail *)m_pDetail);
        break;
    case PLAY_CHIP_TYPE_BROADCAST:
        ((kl::BroadcastItemDetail *)m_pDetail);
        break;
    case PLAY_CHIP_TYPE_AUDIO_CHIP:
        ((kl::ChipAudioDetail *)m_pDetail);
        break;
    case PLAY_CHIP_TYPE_RADIO_CHIP:
        ((kl::ChipRadioDetail *)m_pDetail);
        break;
    default:
        break;
    }
*/
