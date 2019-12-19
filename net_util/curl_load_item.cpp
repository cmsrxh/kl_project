#include <events/common_log.h>
#include <stdexcept>
#include "curl_load_item.h"

#define my_curl_easy_setopt(A, B, C) \
    do { \
        CURLcode res = curl_easy_setopt((A), (B), (C)); \
        if(res != CURLE_OK) \
        { \
            GEN_Printf(LOG_ERROR, "curl_easy_setopt(%s, %s, %s) failed: %s", #A, #B, #C, curl_easy_strerror(res)); \
            goto err; \
        } \
    }while(0)


CurlLoadItem::CurlLoadItem(const char *url, CurlLoadData fdata, CurlLoadState fstate, void *priv)
    : m_pCurl(nullptr), m_pPriv(priv)
    , mMemory(nullptr), mSize(0)
    , m_fData(fdata), m_fState(fstate)
{
    if (!url)
    {
        GEN_Printf(LOG_ERROR, "Url addr is null !!!");
        throw -1;
    }
    m_pCurl = curl_easy_init();
    if (!m_pCurl)
    {
        GEN_Printf(LOG_ERROR, "Alloc curl memery failed.");
        throw -1;
    }

    my_curl_easy_setopt(m_pCurl, CURLOPT_WRITEFUNCTION, CurlLoadItem::writeData);
    my_curl_easy_setopt(m_pCurl, CURLOPT_HEADER, 0L);
    my_curl_easy_setopt(m_pCurl, CURLOPT_URL, url);
    my_curl_easy_setopt(m_pCurl, CURLOPT_PRIVATE, this);
    my_curl_easy_setopt(m_pCurl, CURLOPT_WRITEDATA, this);
    my_curl_easy_setopt(m_pCurl, CURLOPT_VERBOSE, 0L);
    my_curl_easy_setopt(m_pCurl, CURLOPT_SSL_VERIFYPEER, 0L);
    my_curl_easy_setopt(m_pCurl, CURLOPT_SSL_VERIFYHOST, 0L);
    my_curl_easy_setopt(m_pCurl, CURLOPT_TIMEOUT, 15L);
    my_curl_easy_setopt(m_pCurl, CURLOPT_CONNECTTIMEOUT, 10L);

    return;
err:
    GEN_Printf(LOG_ERROR, "throw curl contruct failed !");
    curl_easy_cleanup(m_pCurl);
    m_pCurl = nullptr;
    throw -1;
}

CurlLoadItem::~CurlLoadItem()
{
    if (mMemory)
    {
        free(mMemory);
    }

    if (m_pCurl)
    {
        curl_easy_cleanup(m_pCurl);
    }
}

void CurlLoadItem::setBuffer()
{
    if (!mMemory)
    {
        mMemory = (uint8_t *)malloc(1);
        if (!mMemory)
        {
            GEN_Printf(LOG_ERROR, "no memery");
            throw std::runtime_error("no memery");
        }
    }
    mSize = 0;
}

size_t CurlLoadItem::writeData(void *ptr, size_t size, size_t nmemb, void *stream)
{
    size_t realsize = size * nmemb;
    if (stream)
    {
        static_cast<CurlLoadItem *>(stream)->callbackData(static_cast<uint8_t *>(ptr), realsize);
    } else
    {
        GEN_Printf(LOG_ERROR, "Stream point is nullptr");
    }

    return realsize;
}

void CurlLoadItem::callbackData(uint8_t *data, size_t size)
{
    if (mMemory)
    {
        mMemory = (uint8_t *)realloc(mMemory, mSize + size + 1);
        if (mMemory == nullptr)
        {
            GEN_Printf(LOG_WARN, "not enough memory !");
            return ;
        }

        memcpy(&(mMemory[mSize]), data, size);
        mSize += size;
        mMemory[mSize] = 0;
    } else if (m_fData)
    {
        m_fData(data, size, m_pPriv);
    }
}

/**
 * @brief CurlLoadItem::tryCallBack
 * @note 尝试回调数据或状态，根据当前加载数据的方式
 */
void CurlLoadItem::tryCallBack()
{
    if (mMemory && m_fData)
    {
        m_fData(mMemory, mSize, m_pPriv);
    } else if (m_fState)
    {
        m_fState(true, nullptr, m_pPriv);
    }
}

CurlGlobal::CurlGlobal()
{
    CURLcode ret = curl_global_init(CURL_GLOBAL_ALL);
    if (ret != CURLE_OK)
    {
        GEN_Printf(LOG_ERROR, "curl_global_init failed, %s.", curl_easy_strerror(ret));
        throw -1;
    }

    m_pMulti = curl_multi_init();
    if( nullptr == m_pMulti )
    {
        GEN_Printf(LOG_ERROR, "curl_multi_init failed.");
        throw -1;
    }
}

CurlGlobal::~CurlGlobal()
{
    if(m_pMulti)
    {
        curl_multi_cleanup(m_pMulti);
    }
}

bool CurlGlobal::removeHandler(CurlLoadItem *item)
{
    CURLMcode ret;
    if (!item)
    {
        GEN_Printf(LOG_ERROR, "remove handler is nullptr.");
        return false;
    }

    ret = curl_multi_remove_handle(m_pMulti, item->m_pCurl);
    if (CURLM_OK != ret)
    {
        GEN_Printf(LOG_ERROR, "curl_multi_remove_handle failed, %s", curl_multi_strerror(ret));
        return false;
    }

    return true;
}
