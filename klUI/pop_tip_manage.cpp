#include <events/common_log.h>
#include "model/kl_data_proc.h"
#include "model/kl_local_data_proc.h"
#include "model/chip_item_play_manage.h"
#include "kl_ui_proc.h"
#include "events/app_timer.h"
#include "application.h"
#include "pop_tip_manage.h"

extern KLUIProc *gInstance;

PopTipManage::PopTipManage()
    : mCurrenObjectName(-1)
    , m_pMsgPopDelayTimer(new ATimer)
{
    m_pMsgPopDelayTimer->setHandler(PopTipManage::msgTipTimer, this);
    m_pMsgPopDelayTimer->setRepeat(false);
    m_pMsgPopDelayTimer->setInternal(0, 800);
}

PopTipManage::~PopTipManage()
{

}

void PopTipManage::klObjectObtainStart(bool state, int objectName, int loadAction)
{
    GEN_Printf(LOG_DEBUG, "klObjectObtainStart: mCurrenObjectName=%d, state=%d, objectName=%d, loadAction=%d", mCurrenObjectName, state, objectName, loadAction);
    if (state)
    {
        if (-1 != mCurrenObjectName)
        {
            closeMsgBox(mCurrenObjectName);
            mCurrenObjectName = -1;
        }
        bool needPop = false;
        switch (objectName)
        {
        case kl::OBJECT_ACTIVE_MANAGE:
        case kl::OBJECT_INIT_MANAGE:
        case kl::OBJECT_SUGGESTION_WORD:
        case kl::OBJECT_VOICE_SEARCH_ALL:
            // NOT PORCESS
            break;
        case kl::OBJECT_ALBUM_LIST:
        case kl::OBJECT_BDC_AREA_LIST:
        case kl::OBJECT_BDC_ITEM_DETAIL:
        case kl::OBJECT_BDC_ITEM_LIST:
        case kl::OBJECT_BDC_ITEM_PROGRAM:
        case kl::OBJECT_CATEGORY_ALL:
        case kl::OBJECT_CATEGORY_BDC:
        case kl::OBJECT_CATEGORY_SUB_LIST:
        case kl::OBJECT_CHIP_AUDIO_DETAIL:
        case kl::OBJECT_CHIP_RADIO_DETAIL:
        case kl::OBJECT_CHIP_RADIO_LIST:
        case kl::OBJECT_OPERATE_LIST:
        case kl::OBJECT_TYPERADIO_LIST:
        case kl::OBJECT_ALBUM_DETAIL:
        case kl::OBJECT_CHIP_AUDIO_LIST:
            // 现在只有这两种下载在情况下，才进行弹框显示，往后可以进一步添加
            needPop = (loadAction == LOAD_MAIN_PAGE
                        || loadAction == LOAD_ALBUM_NORMAL_SHOW) ? true : false;
            break;
        default:
            GEN_Printf(LOG_ERROR, "Invalid objectName: %d", mCurrenObjectName);
            break;
        }

        if (needPop)
        {
            mCurrenObjectName = objectName;
            mBackLoadAction = loadAction;
            m_pMsgPopDelayTimer->restart(700);
        }
    } else
    {
        GEN_Printf(LOG_ERROR, "System Load start failed.");
    }
}

void PopTipManage::klObjectObtainOver(int objectName, int loadAction)
{
    GEN_Printf(LOG_DEBUG, "objectName=%d, loadAction=%d", objectName, loadAction);

    if (m_pMsgPopDelayTimer->isRunning())
    {
        GEN_Printf(LOG_DEBUG, "Timer is running.");
        m_pMsgPopDelayTimer->stop();
    } else
    {
        // 表示谁发起的弹框，就谁负责关闭
        if (mCurrenObjectName == objectName)
        {
            closeMsgBox(objectName);
            // 表示当前弹框已经关闭了
            mCurrenObjectName = -1;
        }
    }
}

void PopTipManage::klLoadDataExportEmpty(int objectName, int loadAction, const ByteString &)
{
    GEN_Printf(LOG_DEBUG, "objectName=%d, loadAction=%d", objectName, loadAction);

    if (m_pMsgPopDelayTimer->isRunning())
    {
        m_pMsgPopDelayTimer->stop();
    }

    switch (objectName)
    {
    case kl::OBJECT_ACTIVE_MANAGE:
    case kl::OBJECT_INIT_MANAGE:
        Q_EMIT gInstance->msgTipGlobal(KLUIProc::emptyData, "Load Empty Data.");
        break;
    case kl::OBJECT_CATEGORY_BDC:
    case kl::OBJECT_CATEGORY_ALL:
    case kl::OBJECT_BDC_AREA_LIST:
        Q_EMIT gInstance->msgTipGlobal(KLUIProc::emptyData, "Load Empty Data.");
        break;
    case kl::OBJECT_BDC_ITEM_PROGRAM:
        Q_EMIT gInstance->msgTipGlobal(KLUIProc::generalBox, "当前电台还没有节目");
        break;
    case kl::OBJECT_BDC_ITEM_DETAIL:    
    case kl::OBJECT_CATEGORY_SUB_LIST:
    case kl::OBJECT_CHIP_AUDIO_DETAIL:
    case kl::OBJECT_CHIP_RADIO_DETAIL:
    case kl::OBJECT_CHIP_RADIO_LIST: // 智能电台节目列表
    case kl::OBJECT_ALBUM_DETAIL:
//    case kl::OBJECT_SUGGESTION_WORD:
//    case kl::OBJECT_VOICE_SEARCH_ALL:
        break;
    case kl::OBJECT_BDC_ITEM_LIST:
    case kl::OBJECT_TYPERADIO_LIST:
        if (loadAction == LOAD_MAIN_PAGE)
        {
            Q_EMIT gInstance->msgTipBroadcast(KLUIProc::emptyData, "Load Empty Data.");
        }
        break;
    case kl::OBJECT_OPERATE_LIST:
    case kl::OBJECT_ALBUM_LIST:
        if (loadAction == LOAD_MAIN_PAGE)
        {
            Q_EMIT gInstance->msgTipCateItem(KLUIProc::emptyData, "Load Empty Data.");
        }
        break;
    case kl::OBJECT_CHIP_AUDIO_LIST:
        if (loadAction == LOAD_ALBUM_NORMAL_SHOW)
        {
            Q_EMIT gInstance->msgTipAudioList(KLUIProc::emptyData, "Load Empty Data.");
        }
        break;
    default:
        GEN_Printf(LOG_ERROR, "Invalid objectName: %d", mCurrenObjectName);
        break;
    }
}

void PopTipManage::klLoadDataPriserExcept(int objectName, int loadAction, const ByteString &err_str)
{
    GEN_Printf(LOG_DEBUG, "klLoadDataPriserExcept: objectName=%d, loadAction=%d", objectName, loadAction);
    if (m_pMsgPopDelayTimer->isRunning())
    {
        m_pMsgPopDelayTimer->stop();
    }

    switch (objectName)
    {
    case kl::OBJECT_ACTIVE_MANAGE:
    case kl::OBJECT_INIT_MANAGE:
        Q_EMIT gInstance->msgTipGlobal(KLUIProc::failTip, QStringFromByteString(err_str));
        break;
    case kl::OBJECT_CATEGORY_BDC:
    case kl::OBJECT_CATEGORY_ALL:
    case kl::OBJECT_BDC_AREA_LIST:
        Q_EMIT gInstance->msgTipGlobal(KLUIProc::failTip, QStringFromByteString(err_str));
        break;
    case kl::OBJECT_BDC_ITEM_DETAIL:
    case kl::OBJECT_BDC_ITEM_PROGRAM:
    case kl::OBJECT_CATEGORY_SUB_LIST:
    case kl::OBJECT_CHIP_AUDIO_DETAIL:
    case kl::OBJECT_CHIP_RADIO_DETAIL:
    case kl::OBJECT_CHIP_RADIO_LIST: // 智能电台节目列表
    case kl::OBJECT_ALBUM_DETAIL:
//    case kl::OBJECT_SUGGESTION_WORD:
//    case kl::OBJECT_VOICE_SEARCH_ALL:
        break;
    case kl::OBJECT_BDC_ITEM_LIST:
    case kl::OBJECT_TYPERADIO_LIST:
        if (loadAction == LOAD_MAIN_PAGE)
        {
            Q_EMIT gInstance->msgTipBroadcast(KLUIProc::failTip, QStringFromByteString(err_str));
        }
        break;
    case kl::OBJECT_OPERATE_LIST:
    case kl::OBJECT_ALBUM_LIST:
        if (loadAction == LOAD_MAIN_PAGE)
        {
            Q_EMIT gInstance->msgTipCateItem(KLUIProc::failTip, QStringFromByteString(err_str));
        }
        break;
    case kl::OBJECT_CHIP_AUDIO_LIST:
        if (loadAction == LOAD_ALBUM_NORMAL_SHOW)
        {
            Q_EMIT gInstance->msgTipAudioList(KLUIProc::failTip, QStringFromByteString(err_str));
        }
        break;
    default:
        GEN_Printf(LOG_ERROR, "Invalid objectName: %d", mCurrenObjectName);
        break;
    }
}

void PopTipManage::sysNetLoadApiExcept(int objectName, int loadAction, const ByteString &err_str)
{
    GEN_Printf(LOG_DEBUG, "objectName=%d, loadAction=%d", objectName, loadAction);
    if (m_pMsgPopDelayTimer->isRunning())
    {
        m_pMsgPopDelayTimer->stop();
    }

    switch (objectName)
    {
    case kl::OBJECT_ACTIVE_MANAGE:
    case kl::OBJECT_INIT_MANAGE:
        Q_EMIT gInstance->msgTipGlobal(KLUIProc::failTip, QStringFromByteString(err_str));
        break;
    case kl::OBJECT_CATEGORY_BDC:
    case kl::OBJECT_CATEGORY_ALL:
    case kl::OBJECT_BDC_AREA_LIST:
        Q_EMIT gInstance->msgTipGlobal(KLUIProc::failTip, QStringFromByteString(err_str));
        break;
    case kl::OBJECT_BDC_ITEM_DETAIL:
    case kl::OBJECT_BDC_ITEM_PROGRAM:
    case kl::OBJECT_CATEGORY_SUB_LIST:
    case kl::OBJECT_CHIP_AUDIO_DETAIL:
    case kl::OBJECT_CHIP_RADIO_DETAIL:
    case kl::OBJECT_CHIP_RADIO_LIST: // 智能电台节目列表
    case kl::OBJECT_ALBUM_DETAIL:
//    case kl::OBJECT_SUGGESTION_WORD:
//    case kl::OBJECT_VOICE_SEARCH_ALL:
        break;
    case kl::OBJECT_BDC_ITEM_LIST:
    case kl::OBJECT_TYPERADIO_LIST:
        if (loadAction == LOAD_MAIN_PAGE)
        {
            Q_EMIT gInstance->msgTipBroadcast(KLUIProc::failTip, QStringFromByteString(err_str));
        }
        break;
    case kl::OBJECT_OPERATE_LIST:
    case kl::OBJECT_ALBUM_LIST:
        if (loadAction == LOAD_MAIN_PAGE)
        {
            Q_EMIT gInstance->msgTipCateItem(KLUIProc::failTip, QStringFromByteString(err_str));
        }
        break;
    case kl::OBJECT_CHIP_AUDIO_LIST:
        if (loadAction == LOAD_ALBUM_NORMAL_SHOW)
        {
            Q_EMIT gInstance->msgTipAudioList(KLUIProc::failTip, QStringFromByteString(err_str));
        }
        break;
    default:
        GEN_Printf(LOG_ERROR, "Invalid objectName: %d", mCurrenObjectName);
        break;
    }
}

void PopTipManage::msgTipTimer(ATimer *that, void *ptr)
{
    that->setRunning(false);
    ((PopTipManage *)ptr)->showDelayMsgBox();
}

void PopTipManage::showDelayMsgBox()
{
    GEN_Printf(LOG_DEBUG, "Msg Box delay show, mCurrenObjectName=%d.", mCurrenObjectName);

    switch (mCurrenObjectName)
    {
    case kl::OBJECT_ACTIVE_MANAGE:
    case kl::OBJECT_INIT_MANAGE:
         Q_EMIT gInstance->msgTipGlobal(KLUIProc::msgBufferring, "Data Loading...");
        break;
    case kl::OBJECT_BDC_ITEM_LIST:
    case kl::OBJECT_TYPERADIO_LIST:
        Q_EMIT gInstance->msgTipBroadcast(KLUIProc::msgBufferring, "Data Loading...");
        break;
    case kl::OBJECT_ALBUM_LIST:
    case kl::OBJECT_BDC_AREA_LIST:
    case kl::OBJECT_BDC_ITEM_DETAIL:
    case kl::OBJECT_BDC_ITEM_PROGRAM:
    case kl::OBJECT_CATEGORY_ALL:
    case kl::OBJECT_CATEGORY_BDC:
    case kl::OBJECT_CATEGORY_SUB_LIST:
    case kl::OBJECT_CHIP_AUDIO_DETAIL:
    case kl::OBJECT_CHIP_RADIO_DETAIL:
    case kl::OBJECT_CHIP_RADIO_LIST:
    case kl::OBJECT_OPERATE_LIST:
//    case kl::OBJECT_SUGGESTION_WORD:
//    case kl::OBJECT_VOICE_SEARCH_ALL:
        Q_EMIT gInstance->msgTipCateItem(KLUIProc::msgBufferring, "Data Loading...");
        break;
    case kl::OBJECT_ALBUM_DETAIL:
    case kl::OBJECT_CHIP_AUDIO_LIST:
        Q_EMIT gInstance->msgTipAudioList(KLUIProc::msgBufferring, "Data Loading...");
        break;
    default:
        GEN_Printf(LOG_ERROR, "Invalid objectName: %d", mCurrenObjectName);
        break;
    }
}

void PopTipManage::closeMsgBox(int objectName)
{
    if (m_pMsgPopDelayTimer->isRunning())
    {
        m_pMsgPopDelayTimer->stop();
    }

    switch (objectName)
    {
    case kl::OBJECT_ACTIVE_MANAGE:
    case kl::OBJECT_INIT_MANAGE:
        Q_EMIT gInstance->msgTipGlobal(KLUIProc::nullEmpty, "");
        break;
    case kl::OBJECT_CATEGORY_BDC:
    case kl::OBJECT_CATEGORY_ALL:
    case kl::OBJECT_BDC_AREA_LIST:
        Q_EMIT gInstance->msgTipGlobal(KLUIProc::nullEmpty, "");
        break;
    case kl::OBJECT_BDC_ITEM_DETAIL:
    case kl::OBJECT_BDC_ITEM_PROGRAM:
    case kl::OBJECT_CATEGORY_SUB_LIST:
    case kl::OBJECT_CHIP_AUDIO_DETAIL:
    case kl::OBJECT_CHIP_RADIO_DETAIL:
    case kl::OBJECT_CHIP_RADIO_LIST: // 智能电台节目列表
    case kl::OBJECT_ALBUM_DETAIL:
//    case kl::OBJECT_SUGGESTION_WORD:
//    case kl::OBJECT_VOICE_SEARCH_ALL:
        break;
    case kl::OBJECT_BDC_ITEM_LIST:
    case kl::OBJECT_TYPERADIO_LIST:
        Q_EMIT gInstance->msgTipBroadcast(KLUIProc::nullEmpty, "");
        break;
    case kl::OBJECT_OPERATE_LIST:
    case kl::OBJECT_ALBUM_LIST:
        Q_EMIT gInstance->msgTipCateItem(KLUIProc::nullEmpty, "");
        break;
    case kl::OBJECT_CHIP_AUDIO_LIST:
        Q_EMIT gInstance->msgTipAudioList(KLUIProc::nullEmpty, "");
        break;
    default:
        GEN_Printf(LOG_ERROR, "Invalid objectName: %d", mCurrenObjectName);
        break;
    }
}

