#include <events/common_log.h>
#include "curl_load_item.h"
#include "curl_load_proc.h"
#include "load_item.h"

LoadItem::LoadItem(bool isAll)
    : m_pLoad(nullptr)
    , mIsLoadAll(isAll)
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
    bool ret = CurlGlobal::instance()->removeHandler(m_pLoad);

    CurlLoadProc::instance()->removeItem(m_pLoad);
    m_pLoad = nullptr;

    return ret;
}
