#include <events/common_log.h>
#include "kl_collect_manage.h"
#include "kl_download_manage.h"
#include "kl_record_manage.h"
#include "collect_model.h"
#include "kl_ui_data_union.h"
#include "kl_data_proc.h"
#include "application.h"
#include "model/chip_item_union.h"
#include "kl_local_data_proc.h"

static void collectDataStatus(int status, long ptr)
{
    Q_EMIT LocalDataProc::instance()->collectStatus(status, (long)ptr);
}

static void downloadDataStatus(int status, long ptr)
{
    Q_EMIT LocalDataProc::instance()->downloadStatus(status, (long)ptr);
}

static void historyDataStatus(int status, long ptr)
{
    Q_EMIT LocalDataProc::instance()->historyStatus(status, (long)ptr);
}

LocalDataProc::LocalDataProc()
    : m_pDownLoadChip(new ChipItemUnion(PLAY_CHIP_TYPE_LOCAL_LOAD))
    , m_pCollectChip(new ChipItemUnion(PLAY_CHIP_TYPE_COLLECT_RECORD))
    , m_pHistoryChip(new ChipItemUnion(PLAY_CHIP_TYPE_HISTROY_RECORD))
{
    connect(this, SIGNAL(collectStatus(int, long)),  this, SLOT(onCollect(int, long)));
    connect(this, SIGNAL(downloadStatus(int, long)), this, SLOT(onDownload(int, long)));
    connect(this, SIGNAL(historyStatus(int, long)),  this, SLOT(onRecord(int, long)));
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
}

int LocalDataProc::getLocalPlayIndex(ListTable<kl::RecordItem>::vector &vec)
{
    ByteString parentId;
    ByteString id;

    if (KLDataProc::instance()->getCurrentPlayInfo(parentId, id))
    {
        for (int i = 0; i < vec.size(); ++i)
        {
            if (vec[i].id == id && vec[i].parentId == parentId)
            {
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

bool LocalDataProc::checkBDCItemIsCollect(int type, const ByteString &id)
{
    ListTable<kl::RecordItem>::vector &vec = m_pCollect->vec();
    for (int i = 0; i < vec.size(); ++i)
    {
        if (vec[i].type == type
                && vec[i].parentId == id)
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

void LocalDataProc::onCollect(int st, long ptr)
{
    GEN_Printf(LOG_DEBUG, "Update Collect, %d, %ld", st, ptr);
//    kl::CollectManage::instance()->profile();
    switch (st)
    {
    case LOCAL_RECORD_OP_STATUS_DATA_LOAD_OVER:
        kl::CollectManage::instance()->nodes().genVector(m_pCollect->vec());
        m_pCollect->resetAll();
        break;
    case LOCAL_RECORD_OP_STATUS_CHECK_IN_LIST:
        KLDataProc::instance()->notifyCurIsCollect(true);
        break;
    case LOCAL_RECORD_OP_STATUS_CHECK_NOT_IN_LIST:
        KLDataProc::instance()->notifyCurIsCollect(false);
        break;
    case LOCAL_RECORD_OP_STATUS_ADD_IN_LIST:
        KLDataProc::instance()->notifyCurIsCollect(true);
        kl::CollectManage::instance()->nodes().genVector(m_pCollect->vec());
        m_pCollect->resetAll();
        break;
    case LOCAL_RECORD_OP_STATUS_REMOVE_IN_LIST:
        KLDataProc::instance()->notifyCurIsCollect(false);
        kl::CollectManage::instance()->nodes().genVector(m_pCollect->vec());
        m_pCollect->resetAll();
        break;
    case LOCAL_RECORD_OP_STATUS_BDC_COLLECT_REMOVE_IN_LIST:
        KLDataProc::instance()->notifyBDCCollectChange(ptr, false);
        kl::CollectManage::instance()->nodes().genVector(m_pCollect->vec());
        m_pCollect->resetAll();
        break;
    case LOCAL_RECORD_OP_STATUS_BDC_COLLECT_ADD_IN_LIST:
        KLDataProc::instance()->notifyBDCCollectChange(ptr, true);
        kl::CollectManage::instance()->nodes().genVector(m_pCollect->vec());
        m_pCollect->resetAll();
        break;
    default:
        GEN_Printf(LOG_WARN, "invalid cmd: %d", st);
        break;
    }
}

void LocalDataProc::onDownload(int st, long /*ptr*/)
{
    switch (st) {
    case LOCAL_RECORD_OP_STATUS_DATA_LOAD_OVER:
        kl::DownloadManage::instance()->nodes().genVector(m_pDownload->vec());
        m_pDownload->resetAll();
        break;
    default:
        GEN_Printf(LOG_WARN, "invalid cmd: %d", st);
        break;
    }
}

void LocalDataProc::onRecord(int st, long )
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
        break;
    default:
        GEN_Printf(LOG_WARN, "invalid cmd: %d", st);
        break;
    }

    Q_EMIT m_pHistory->itemsChanged();
}
