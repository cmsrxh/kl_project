#include <events/common_log.h>
#include "kl_collect_manage.h"
#include "kl_download_manage.h"
#include "kl_record_manage.h"
#include "collect_model.h"
#include "kl_ui_data_union.h"
#include "kl_local_data_proc.h"
#include "kl_data_proc.h"
#include "application.h"

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
{
    connect(this, SIGNAL(collectStatus(int, long)),  this, SLOT(onCollect(int, long)));
    connect(this, SIGNAL(downloadStatus(int, long)), this, SLOT(onDownload(int, long)));
    connect(this, SIGNAL(historyStatus(int, long)),  this, SLOT(onRecord(int, long)));
}

LocalDataProc::~LocalDataProc()
{

}

void LocalDataProc::initLocal(CollectModel *collect, CollectModel *load, CollectModel *history)
{
    m_pCollect  = collect;
    m_pDownload = load;
    m_pHistory  = history;

    kl::CollectManage::instance()->setCallBack(collectDataStatus);
    kl::DownloadManage::instance()->setCallBack(downloadDataStatus);
    kl::RecordManage::instance()->setCallBack(historyDataStatus);
}

int LocalDataProc::getLocalPlayIndex(CollectModel *ptr)
{
    CollectNode &info = KLDataProc::instance()->getCurrentPlayInfo();
    ListTable<kl::RecordItem>::vector &vec = ptr->vec();

    for (int i = 0; i < vec.size(); ++i)
    {
        if (vec[i].type == info.type
                && vec[i].parentId == info.parentId)
        {
            if (info.type > PLAY_CHIP_TYPE_TYPE_RADIO)
            { // 表示第三级列表必须比对子ID
                if (vec[i].id == info.id)
                {
                    return i;
                }
            } else
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

}

void LocalDataProc::collectItemEnable(int index)
{

}

void LocalDataProc::downLoadItemPlay(int index)
{

}

void LocalDataProc::historyItemPlay(int index)
{

}

void LocalDataProc::historyClearAll()
{

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
