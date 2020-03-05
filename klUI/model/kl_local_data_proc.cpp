#include <events/common_log.h>
#include "kl_collect_manage.h"
#include "kl_download_manage.h"
#include "kl_record_manage.h"
#include "collect_model.h"
#include "kl_ui_data_union.h"
#include "kl_data_proc.h"
#include "application.h"
#include "kl_ui_proc.h"
#include "model/chip_item_union.h"
#include "kl_local_data_proc.h"

extern KLUIProc *gInstance;

static void collectDataStatus(int status, long ptr, long nodePtr)
{
    Q_EMIT LocalDataProc::instance()->collectStatus(status, (long)ptr, nodePtr);
}

static void downloadDataStatus(int status, long ptr, long nodePtr)
{
    Q_EMIT LocalDataProc::instance()->downloadStatus(status, (long)ptr, nodePtr);
}

static void historyDataStatus(int status, long ptr, long nodePtr)
{
    Q_EMIT LocalDataProc::instance()->historyStatus(status, (long)ptr, nodePtr);
}

LocalDataProc::LocalDataProc()
    : m_pDownLoadChip(new ChipItemUnion(PLAY_CHIP_TYPE_LOCAL_LOAD))
    , m_pCollectChip(new ChipItemUnion(PLAY_CHIP_TYPE_COLLECT_RECORD))
    , m_pHistoryChip(new ChipItemUnion(PLAY_CHIP_TYPE_HISTROY_RECORD))
{
    connect(this, SIGNAL(collectStatus(int, long, long)),  this, SLOT(onCollect(int, long, long)));
    connect(this, SIGNAL(downloadStatus(int, long, long)), this, SLOT(onDownload(int, long, long)));
    connect(this, SIGNAL(historyStatus(int, long, long)),  this, SLOT(onRecord(int, long, long)));
}

LocalDataProc::~LocalDataProc()
{
    delete m_pDownLoadChip;
    delete m_pCollectChip;
    delete m_pHistoryChip;
}

void LocalDataProc::initLocal(CollectModel *collect, CollectModel *load, CollectModel *history)
{
    m_pCollect  = collect;
    m_pDownload = load;
    m_pHistory  = history;

    m_pDownLoadChip->setChipHandler(kl::DownloadManage::instance());
    m_pCollectChip->setChipHandler(kl::CollectManage::instance());
    m_pHistoryChip->setChipHandler(kl::RecordManage::instance());

    kl::CollectManage::instance()->setCallBack(collectDataStatus);
    kl::DownloadManage::instance()->setCallBack(downloadDataStatus);
    kl::RecordManage::instance()->setCallBack(historyDataStatus);

    if (!kl::CollectManage::instance()->isEmpty())
    {
        kl::CollectManage::instance()->nodes().genVector(m_pCollect->vec());
        m_pCollect->resetAll();
    }
    if (!kl::DownloadManage::instance()->isEmpty())
    {
        kl::DownloadManage::instance()->nodes().genVector(m_pDownload->vec());
        m_pDownload->resetAll();
    }
    if (!kl::RecordManage::instance()->isEmpty())
    {
        kl::RecordManage::instance()->nodes().genVector(m_pHistory->vec());
        m_pHistory->resetAll();
    }
}

int LocalDataProc::getLocalPlayIndex(ListTable<kl::RecordItem>::vector &vec)
{
    ByteString parentId;
    ByteString id;

    if (KLDataProc::instance()->getCurrentPlayInfo(parentId, id))
    {
        //GEN_Printf(LOG_DEBUG, "Check,id: %s, parentId: %s", id.string(), parentId.string());
        for (int i = 0; i < vec.size(); ++i)
        {
            //GEN_Printf(LOG_DEBUG, "id: %s, parentId: %s", vec[i].id.string(), vec[i].parentId.string());
            if (vec[i].id == id && vec[i].parentId == parentId)
            {
                //GEN_Printf(LOG_DEBUG, "correct id: %d", i);
                return i;
            }
        }
    }
    return -1;
}

void LocalDataProc::checkCurIsCollect(CollectNode *info)
{
    kl::RecordItem *newItem = new kl::RecordItem;

    info->copy(newItem);

    if (!Application::instance()->postKlEvent(SIG_KL_CURRENT_IS_COLLECT, (long)newItem))
    {
        GEN_Printf(LOG_ERROR, "Post Msg SIG_KL_CURRENT_IS_COLLECT failed.");
        newItem->clear();
        delete newItem;
    }
}

void LocalDataProc::opCurCollect(CollectNode *info)
{
    kl::RecordItem *newItem = new kl::RecordItem;

    info->copy(newItem);

    if (!Application::instance()->postKlEvent(SIG_KL_CURRENT_OP_COLLECT, (long)newItem))
    {
        GEN_Printf(LOG_ERROR, "Post Msg SIG_KL_CURRENT_OP_COLLECT failed.");
        newItem->clear();
        delete newItem;
    }
}

void LocalDataProc::bdcTypeRadioCollect(int bdcItemIndeex, CollectNode *info)
{
    if (!Application::instance()->postKlEvent(SIG_KL_BDC_OP_COLLECT, (long)info, bdcItemIndeex))
    {
        GEN_Printf(LOG_ERROR, "Post Msg SIG_KL_BDC_OP_COLLECT failed.");
        info->clear();
        delete info;
    }
}

bool LocalDataProc::checkBDCItemIsCollect(int , const ByteString &id)
{
    // GEN_Printf(LOG_DEBUG, "------check, type=%d, id=%s------", type, id.string());
    ListTable<kl::RecordItem>::vector &vec = m_pCollect->vec();
    for (int i = 0; i < vec.size(); ++i)
    {
        if (vec[i].parentId == id)
        {
            return true;
        }
    }
    return false;
}

void LocalDataProc::recordCurrentPlayItem(CollectNode *info)
{
    kl::RecordItem *newItem = new kl::RecordItem;

    info->copy(newItem);

    if (!Application::instance()->postKlEvent(SIG_KL_RECORD_CURRENT_PLAY, (long)newItem))
    {
        GEN_Printf(LOG_ERROR, "Post Msg SIG_KL_RECORD_CURRENT_PLAY failed.");
        newItem->clear();
        delete newItem;
    }
}

void LocalDataProc::collectItemPlay(int index)
{
    ListTable<kl::RecordItem>::vector &vec = m_pCollect->vec();

    if (index < 0 || index > vec.size())
    {
        GEN_Printf(LOG_WARN, "Collect List, index=%d, size=%d out of range", index, vec.size());
        return;
    }
    KLDataProc::instance()->localItemPlay(CURREN_PLAY_SOURCE_COLLECT_LIST, index, m_pCollectChip);
}

void LocalDataProc::collectItemEnable(int index)
{
    GEN_Printf(LOG_INFO, "Func not used, index=%d", index);
}

void LocalDataProc::downLoadItemPlay(int index)
{
    ListTable<kl::RecordItem>::vector &vec = m_pCollect->vec();

    if (index < 0 || index > vec.size())
    {
        GEN_Printf(LOG_WARN, "Collect List, index=%d, size=%d out of range", index, vec.size());
        return;
    }

    KLDataProc::instance()->localItemPlay(CURREN_PLAY_SOURCE_DOWNLOAD_LIST, index, m_pDownLoadChip);
}

void LocalDataProc::historyItemPlay(int index)
{
    ListTable<kl::RecordItem>::vector &vec = m_pHistory->vec();

    if (index < 0 || index > vec.size())
    {
        GEN_Printf(LOG_WARN, "Collect List, index=%d, size=%d out of range", index, vec.size());
        return;
    }

    KLDataProc::instance()->localItemPlay(CURREN_PLAY_SOURCE_HISTORY_RECORD_LIST, index, m_pHistoryChip);
}

void LocalDataProc::historyClearAll()
{
    Application::instance()->postKlEvent(SIG_KL_HISTORY_CLEAR_APP);
}

void LocalDataProc::showCollectIndex()
{
    Q_EMIT m_pCollect->collectIdChanged();
}

void LocalDataProc::showDownloadIndex()
{
    Q_EMIT m_pDownload->collectIdChanged();
}

void LocalDataProc::showHistoryIndex()
{
    Q_EMIT m_pHistory->collectIdChanged();
}

void LocalDataProc::onCollect(int st, long ptr, long nodePtr)
{
    GEN_Printf(LOG_DEBUG, "Update Collect, %d, %ld", st, ptr);
//    kl::CollectManage::instance()->profile();
    switch (st)
    {
    case LOCAL_RECORD_OP_STATUS_DATA_LOAD_OVER:
        kl::CollectManage::instance()->nodes().genVector(m_pCollect->vec());
        m_pCollect->resetAll();
        KLDataProc::instance()->localDataRenderPlaying(m_pCollectChip);
        break;
    case LOCAL_RECORD_OP_STATUS_CHECK_IN_LIST:
        KLDataProc::instance()->notifyCurIsCollect(true);
        break;
    case LOCAL_RECORD_OP_STATUS_CHECK_NOT_IN_LIST:
        KLDataProc::instance()->notifyCurIsCollect(false);
        break;
    case LOCAL_RECORD_OP_STATUS_ADD_IN_LIST:  //  当前播放项加入收藏
        KLDataProc::instance()->notifyCurIsCollect(true);

        kl::CollectManage::instance()->nodes().genVector(m_pCollect->vec());
        m_pCollect->resetAll();

        KLDataProc::instance()->notifyCurrentCollectChange(reinterpret_cast<CollectNode *>(nodePtr), true);

        KLDataProc::instance()->localDataRenderPlaying(m_pCollectChip);
        break;
    case LOCAL_RECORD_OP_STATUS_REMOVE_IN_LIST: // 当前播放项remove收藏
        // show in main page isCollect Button
        KLDataProc::instance()->notifyCurIsCollect(false);

        // show in collect list view
        kl::CollectManage::instance()->nodes().genVector(m_pCollect->vec());
        m_pCollect->resetAll();

        // show in bdc items list view
        KLDataProc::instance()->notifyCurrentCollectChange(reinterpret_cast<CollectNode *>(nodePtr), false);

        // show in current playing list view
        KLDataProc::instance()->deleteCurrentInfo(PLAY_CHIP_TYPE_COLLECT_RECORD);
        KLDataProc::instance()->localDataRenderPlaying(m_pCollectChip);
        KLDataProc::instance()->collectItemDelete();
        break;
    case LOCAL_RECORD_OP_STATUS_BDC_COLLECT_REMOVE_IN_LIST:
        // GEN_Printf(LOG_DEBUG, "BDC Collect Remove, index: %X", ptr);
        // show in main page isCollect Button
        KLDataProc::instance()->bdcNotifyCurIsCollect(reinterpret_cast<CollectNode *>(nodePtr), false);

        // show in bdc items list view
        KLDataProc::instance()->notifyBDCCollectChange(ptr & 0xFFFF, false);

        // show in collect list view
        kl::CollectManage::instance()->nodes().genVector(m_pCollect->vec());
        m_pCollect->resetAll();

        // show in current playing list view
        if ((ptr >> 16) & 0xFFFF) // 表示操作的当前项是否正在播放中
        {
            KLDataProc::instance()->deleteCurrentInfo(PLAY_CHIP_TYPE_COLLECT_RECORD);
            KLDataProc::instance()->localDataRenderPlaying(m_pCollectChip);
        } else
        {
            KLDataProc::instance()->localDataRenderPlaying(m_pCollectChip);
            KLDataProc::instance()->collectItemDelete();
        }
        break;
    case LOCAL_RECORD_OP_STATUS_BDC_COLLECT_ADD_IN_LIST:
        // show in main page isCollect Button
        KLDataProc::instance()->bdcNotifyCurIsCollect(reinterpret_cast<CollectNode *>(nodePtr), true);

        KLDataProc::instance()->notifyBDCCollectChange(ptr & 0xFFFF, true);

        kl::CollectManage::instance()->nodes().genVector(m_pCollect->vec());
        m_pCollect->resetAll();

        KLDataProc::instance()->localDataRenderPlaying(m_pCollectChip);
        break;
    default:
        GEN_Printf(LOG_WARN, "invalid cmd: %d", st);
        break;
    }    

    Q_EMIT m_pCollect->collectIdChanged();
    if (nodePtr)
    {
        reinterpret_cast<CollectNode *>(nodePtr)->clear();
        delete reinterpret_cast<CollectNode *>(nodePtr);
    }
}

void LocalDataProc::onDownload(int st, long /*ptr*/, long nodePtr)
{
    switch (st) {
    case LOCAL_RECORD_OP_STATUS_DATA_LOAD_OVER:
        kl::DownloadManage::instance()->nodes().genVector(m_pDownload->vec());
        m_pDownload->resetAll();
        KLDataProc::instance()->localDataRenderPlaying(m_pDownLoadChip);
        break;
    default:
        GEN_Printf(LOG_WARN, "invalid cmd: %d", st);
        break;
    }

    Q_EMIT m_pDownload->collectIdChanged();
    if (nodePtr)
    {
        reinterpret_cast<CollectNode *>(nodePtr)->clear();
        delete reinterpret_cast<CollectNode *>(nodePtr);
    }
}

void LocalDataProc::onRecord(int st, long , long nodePtr)
{
    switch (st) {
    case LOCAL_RECORD_OP_STATUS_DATA_LOAD_OVER:
        kl::RecordManage::instance()->nodes().genVector(m_pHistory->vec());
        m_pHistory->resetAll();
        break;
    case LOCAL_RECORD_OP_STATUS_HISTORY_APPEND:
        kl::RecordManage::instance()->nodes().genVector(m_pHistory->vec());
        m_pHistory->resetAll();
        break;
    case LOCAL_RECORD_OP_STATUS_HISTORY_CHANGE:
        kl::RecordManage::instance()->nodes().genVector(m_pHistory->vec());
        m_pHistory->resetAll();
        break;
    case LOCAL_RECORD_OP_STATUS_HISTORY_CLEARALL:
        m_pHistory->vec().clear();
        m_pHistory->resetAll();
        if (KLDataProc::instance()->deleteCurrentInfo(PLAY_CHIP_TYPE_HISTROY_RECORD))
        {
            gInstance->qmlReset();
        }
        KLDataProc::instance()->localDataRenderPlaying(m_pHistoryChip);        
        break;
    default:
        GEN_Printf(LOG_WARN, "invalid cmd: %d", st);
        break;
    }

    Q_EMIT m_pHistory->itemsChanged();
    Q_EMIT m_pHistory->collectIdChanged();
    if (nodePtr)
    {
        reinterpret_cast<CollectNode *>(nodePtr)->clear();
        delete reinterpret_cast<CollectNode *>(nodePtr);
    }
}
