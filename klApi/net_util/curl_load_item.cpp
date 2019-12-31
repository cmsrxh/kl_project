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


CurlLoadItem::CurlLoadItem(const NetUrl &url, OpCurlStatus fstate, void *priv)
    : m_pCurl(nullptr), m_pPriv(priv)
    , mMemory(nullptr), mSize(0)
    , m_fStatus(fstate)
{
    CurlGlobal::instance();

    if (url.empty() || !fstate)
    {
        GEN_Printf(LOG_ERROR, "Url addr is null, fstate: %p !!!", fstate);
        throw -1;
    }
    m_pCurl = curl_easy_init();
    if (!m_pCurl)
    {
        GEN_Printf(LOG_ERROR, "Alloc curl memery failed.");
        throw -1;
    }

    switch (url.mMethodType)
    {
    case NetUrl::NET_HTTP_METHOD_POST:
    {
        ByteString tab = url.genContent();
        my_curl_easy_setopt(m_pCurl, CURLOPT_POST, 1L);
        my_curl_easy_setopt(m_pCurl, CURLOPT_POSTFIELDS, tab.string());
        my_curl_easy_setopt(m_pCurl, CURLOPT_POSTFIELDSIZE, tab.size());
        GEN_Printf(LOG_INFO, "string: %s", tab.string());
        GEN_Printf(LOG_INFO, "size: %d", tab.size());
        break;
    }
    case NetUrl::NET_HTTP_METHOD_GET:        
    default:
        break;
    }

    GEN_Printf(LOG_DEBUG, "string: %s", url.genUrl().string());

    // my_curl_easy_setopt(m_pCurl, CURLOPT_HTTPGET, 0L);
    my_curl_easy_setopt(m_pCurl, CURLOPT_URL, url.genUrl().string());
    my_curl_easy_setopt(m_pCurl, CURLOPT_WRITEFUNCTION, CurlLoadItem::writeData);
    my_curl_easy_setopt(m_pCurl, CURLOPT_HEADER, 0L);
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
    // GEN_Printf(LOG_DUMP, "Load Data: %ld", realsize);
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
    } else if (m_fStatus)
    {
        m_fStatus(OP_CURL_STATUS_LOAD_ONE_FRAME, data, size, m_pPriv);
    }
}

/**
 * @brief CurlLoadItem::tryCallBack
 * @note 尝试回调数据或状态，根据当前加载数据的方式
 */
void CurlLoadItem::tryCallBack()
{
    m_fStatus(mMemory ? OP_CURL_STATUS_LOAD_ALL_OVER
                      : OP_CURL_STATUS_LOAD_OVER, mMemory, mSize, m_pPriv);
}

CurlGlobal::CurlGlobal()
{
    GEN_Printf(LOG_DEBUG, "Curl Init.");
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
