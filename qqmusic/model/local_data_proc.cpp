#include <events/common_log.h>
//#include "kl_collect_manage.h"
//#include "kl_download_manage.h"
//#include "kl_record_manage.h"
#include "collect_model.h"
#include "ui_data_union.h"
#include "data_proc.h"
#include "application.h"
#include "ui_proc.h"
#include "model/chip_item_union.h"
#include "local_data_proc.h"

extern UIProc *gInstance;

//static void collectDataStatus(int status, long ptr, long nodePtr)
//{
//    Q_EMIT LocalDataProc::instance()->collectStatus(status, (long)ptr, nodePtr);
//}

//static void downloadDataStatus(int status, long ptr, long nodePtr)
//{
//    Q_EMIT LocalDataProc::instance()->downloadStatus(status, (long)ptr, nodePtr);
//}

//static void historyDataStatus(int status, long ptr, long nodePtr)
//{
//    Q_EMIT LocalDataProc::instance()->historyStatus(status, (long)ptr, nodePtr);
//}

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

}

int LocalDataProc::getLocalPlayIndex(ListTable<CollectNode>::vector &vec)
{

    return -1;
}

void LocalDataProc::checkCurIsCollect(CollectNode *info)
{

}

void LocalDataProc::opCurCollect(CollectNode *info)
{

}

void LocalDataProc::bdcTypeRadioCollect(int bdcItemIndeex, CollectNode *info)
{
    if (!Application::instance()->postEvent(SIG_KL_BDC_OP_COLLECT, (long)info, bdcItemIndeex))
    {
        GEN_Printf(LOG_ERROR, "Post Msg SIG_KL_BDC_OP_COLLECT failed.");
        info->clear();
        delete info;
    }
}

bool LocalDataProc::checkBDCItemIsCollect(int , const ByteString &id)
{

    return false;
}

void LocalDataProc::recordCurrentPlayItem(CollectNode *info)
{

}

void LocalDataProc::collectItemPlay(int index)
{

    DataProc::instance()->localItemPlay(CURREN_PLAY_SOURCE_COLLECT_LIST, index, m_pCollectChip);
}

void LocalDataProc::collectItemEnable(int index)
{
    GEN_Printf(LOG_INFO, "Func not used, index=%d", index);
}

void LocalDataProc::downLoadItemPlay(int index)
{


    DataProc::instance()->localItemPlay(CURREN_PLAY_SOURCE_DOWNLOAD_LIST, index, m_pDownLoadChip);
}

void LocalDataProc::historyItemPlay(int index)
{


    DataProc::instance()->localItemPlay(CURREN_PLAY_SOURCE_HISTORY_RECORD_LIST, index, m_pHistoryChip);
}

void LocalDataProc::historyClearAll()
{
    Application::instance()->postEvent(SIG_KL_HISTORY_CLEAR_APP);
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

    Q_EMIT m_pCollect->collectIdChanged();
    if (nodePtr)
    {
        reinterpret_cast<CollectNode *>(nodePtr)->clear();
        delete reinterpret_cast<CollectNode *>(nodePtr);
    }
}

void LocalDataProc::onDownload(int st, long /*ptr*/, long nodePtr)
{

    Q_EMIT m_pDownload->collectIdChanged();
    if (nodePtr)
    {
        reinterpret_cast<CollectNode *>(nodePtr)->clear();
        delete reinterpret_cast<CollectNode *>(nodePtr);
    }
}

void LocalDataProc::onRecord(int st, long , long nodePtr)
{

    Q_EMIT m_pHistory->itemsChanged();
    Q_EMIT m_pHistory->collectIdChanged();
    if (nodePtr)
    {
        reinterpret_cast<CollectNode *>(nodePtr)->clear();
        delete reinterpret_cast<CollectNode *>(nodePtr);
    }
}
