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

/**
  "User-Agent:  Mozilla/5.0 (Windows NT 10.0; Win64; x64)
                AppleWebKit/537.36 (KHTML, like Gecko)
                Chrome/76.0.3809.132
                Safari/537.36"

有时候发出的https GET/POST 消息不会收到信息，但是数据都是对的，这有时需要在http的协议头上，加上user-agent
*/
#ifndef USER_AGENT_STRING
#define USER_AGENT_STRING "Chrome/76.0.3809.132"
#endif

CurlLoadItem::CurlLoadItem(const NetUrl &url, OpCurlStatus fstate, void *priv, bool needUserAgent)
    : m_pCurl(nullptr), m_pPriv(priv)
    , m_pBuffer(nullptr)
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
    if (needUserAgent)
    {
        my_curl_easy_setopt(m_pCurl, CURLOPT_USERAGENT, USER_AGENT_STRING);
    }
    // my_curl_easy_setopt(m_pCurl, CURLOPT_HTTPGET, 0L);
    my_curl_easy_setopt(m_pCurl, CURLOPT_URL, url.genUrl().string());
    my_curl_easy_setopt(m_pCurl, CURLOPT_WRITEFUNCTION, CurlLoadItem::writeData);
    my_curl_easy_setopt(m_pCurl, CURLOPT_HEADER, 0L);
    my_curl_easy_setopt(m_pCurl, CURLOPT_PRIVATE, this);
    my_curl_easy_setopt(m_pCurl, CURLOPT_WRITEDATA, this);
    my_curl_easy_setopt(m_pCurl, CURLOPT_VERBOSE, 0L);
    my_curl_easy_setopt(m_pCurl, CURLOPT_SSL_VERIFYPEER, 0L);
    my_curl_easy_setopt(m_pCurl, CURLOPT_SSL_VERIFYHOST, 0L);
    /**
     * @note 下面两个属性，时间不能设置太小(eg: 设置15时)，就会导致退出时调用curl_multi_remove_handle
     *  或curl_easy_cleanup 阻塞一段时间（发生在系统没有网络时）
     */
    my_curl_easy_setopt(m_pCurl, CURLOPT_TIMEOUT, 20L); //接收数据时超时设置，如果10秒内数据未接收完，直接退出
    my_curl_easy_setopt(m_pCurl, CURLOPT_CONNECTTIMEOUT, 20L); //连接超时，这个数值如果设置太短可能导致数据请求不到就断开了

    return;
err:
    GEN_Printf(LOG_ERROR, "throw curl contruct failed !");
    curl_easy_cleanup(m_pCurl);
    m_pCurl = nullptr;
    throw -1;
}

CurlLoadItem::~CurlLoadItem()
{
    if (m_pBuffer)
    {
        NetBuffer::unref(m_pBuffer);
    }

    if (m_pCurl)
    {
        curl_easy_cleanup(m_pCurl);
    }
}

void CurlLoadItem::setBuffer()
{
    if (!m_pBuffer)
    {
        m_pBuffer = NetBuffer::newBuffer();
    }
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
    if (m_pBuffer)
    {
        m_pBuffer->append(data, size);
    } else if (m_fStatus)
    {
        char tmp[sizeof(NetBuffer)];
        m_fStatus(OP_CURL_STATUS_LOAD_ONE_FRAME, NetBuffer::packBuffer(tmp, data, size), m_pPriv);
    }
}

/**
 * @brief CurlLoadItem::tryCallBack
 * @note 尝试回调数据或状态，根据当前加载数据的方式
 */
void CurlLoadItem::tryCallBack()
{
    m_fStatus(m_pBuffer ? OP_CURL_STATUS_LOAD_ALL_OVER
                        : OP_CURL_STATUS_LOAD_OVER, m_pBuffer, m_pPriv);
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
