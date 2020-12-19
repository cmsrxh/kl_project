#ifndef CURL_LOAD_ITEM_H
#define CURL_LOAD_ITEM_H

#include <curl/curl.h>
#include <util/byte_string.h>
#include <util/list_node.h>
#include <util/net_buffer.h>
#include "net_common.h"
#include "net_url.h"

/**
 * @brief The CurlLoadItem class
 * @note 当前加载数据有两种方式
 *  1. 使用临时buffer方式，然后把下载的数据一次性反馈给用户，适用于加载的数据量较小的时候，
 *     如下载一个网页，或在线加载图片；会占用很多的动态内存。
 *  2. 不使用间接的方式，先把数据存在buffer中，而是下载了多少数据就返还多少数据。
 */
class CurlGlobal;
class CurlLoadItem : public ListNode
{
public:
    CurlLoadItem(const NetUrl &url, OpCurlStatus fstate, void *ptr, CurlLoadArg *args = 0);
    ~CurlLoadItem();

    void setBuffer();
    void callbackData(uint8_t *data, size_t size);
    void tryCallBack();

    bool check(CURL *curl)
    {
        return curl == m_pCurl;
    }

    inline void errorState(int state, const char *str)
    {
        m_fStatus(OP_CURL_STATUS_ERROR_TYPE + state, (uint8_t *)str, m_pPriv);
    }

private:
    static size_t writeData(void *ptr, size_t size, size_t nmemb, void *stream);

    CURL            *m_pCurl;
    void            *m_pPriv;
    NetBuffer       *m_pBuffer;
    OpCurlStatus     m_fStatus;
    friend class CurlGlobal;
};

class CurlGlobal
{
    CurlGlobal();
    CurlGlobal(CurlGlobal &);
    CurlGlobal &operator=(CurlGlobal &);
public:
    static CurlGlobal *instance()
    {
        static CurlGlobal i;
        return &i;
    }
    ~CurlGlobal();

    CURLMcode perform(int *running_handles)
    {
        return curl_multi_perform(m_pMulti, running_handles);
    }

    CURLMcode fdset(fd_set *read_fd_set,
               fd_set *write_fd_set,
               fd_set *exc_fd_set,
               int *max_fd)
    {
        return curl_multi_fdset(m_pMulti, read_fd_set, write_fd_set, exc_fd_set, max_fd);
    }

    CURLMcode timeout(long *milliseconds)
    {
        return curl_multi_timeout(m_pMulti, milliseconds);
    }

    CURLMsg *infoRead(int *msgs_in_queue)
    {
        return curl_multi_info_read(m_pMulti, msgs_in_queue);
    }

    void clean(CURL *curl)
    {
        curl_multi_remove_handle(m_pMulti, curl);
    }

    CURLMcode addHandle(CurlLoadItem *item)
    {
        return curl_multi_add_handle(m_pMulti, item->m_pCurl);
    }

    bool removeHandler(CurlLoadItem *item);

public:
    CURLM   *m_pMulti;
};

#endif // CURL_LOAD_ITEM_H
