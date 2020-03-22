#include <events/common_log.h>
#include "curl_load_item.h"
#include "curl_load_proc.h"
#include "load_item.h"

LoadItem::LoadItem(bool isAll)
    : m_pLoad(nullptr)
    , mIsLoadAll(isAll)
{    
}

LoadItem::~LoadItem()
{
}

bool LoadItem::setLoad(const NetUrl &url, OpCurlStatus fstate, void *ptr)
{
    try
    {
        if (m_pLoad)
        {
            CurlGlobal::instance()->removeHandler(m_pLoad);
        }

        m_pLoad = new CurlLoadItem(url, fstate, ptr);

        if (mIsLoadAll)
        {
            m_pLoad->setBuffer();
        }

        CurlLoadProc::instance()->addItem(m_pLoad);

        return CurlLoadProc::instance()->start();
    } catch (...)
    {
        GEN_Printf(LOG_ERROR, "Set Load failed !");
        return false;
    }
}

bool LoadItem::cancel()
{
    if (!m_pLoad)
    {
        return true;
    }
    bool ret = CurlGlobal::instance()->removeHandler(m_pLoad);

    CurlLoadProc::instance()->removeItem(m_pLoad);
    m_pLoad = nullptr;

    return ret;
}

void LoadItem::loadStatus(int status, void *data, void *arg)
{
    switch (status)
    {
    case OP_CURL_STATUS_LOAD_ALL_OVER:      // 表示数据下载全部一次性返回了
        static_cast<LoadItem *>(arg)->loadOver();
        static_cast<LoadItem *>(arg)->allDataObtain(static_cast<NetBuffer *>(data));
        break;
    case OP_CURL_STATUS_LOAD_ONE_FRAME:     // 表示数据下载方式按照块来计算，有多少数据就来多少
        static_cast<LoadItem *>(arg)->oneFrameObtain(static_cast<NetBuffer *>(data));
        break;
    case OP_CURL_STATUS_LOAD_OVER:          // 表示数据下载完成了，与上一个枚举配合，表示下载已经完成，可以关闭文件。
        static_cast<LoadItem *>(arg)->loadOver();
        static_cast<LoadItem *>(arg)->oneFrameObtainOver();        
        break;
    case OP_CURL_STATUS_ERROR_TYPE:         // 往下的枚举表示数据错误的定义
    default:
        static_cast<LoadItem *>(arg)->loadOver();
        static_cast<LoadItem *>(arg)->errorInfo(status - OP_CURL_STATUS_ERROR_TYPE, (char *)data);        
        break;
    }
}

bool LoadItem::obtain(const NetUrl &url)
{
    if (isLoading())
    {
        GEN_Printf(LOG_WARN, "is loading, need cancel loading.");
        cancel();
    }

    return setLoad(url, LoadItem::loadStatus, (void *)this);
}
