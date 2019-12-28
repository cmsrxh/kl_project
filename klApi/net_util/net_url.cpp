#include <events/common_log.h>
#include <stdarg.h>
#include "net_url.h"

NetUrl::NetUrl(const ByteString &url, int method)
    : mMethodType(method), mTmpAllocLen(0), mTmpAlloc(nullptr)
    , mBaseUrl(url)
{}

NetUrl::~NetUrl()
{
//    mList.removeAll(delQueryKV);
//    mBaseUrl.clear();
    mList.clear();
    mContentList.clear();
    if (mTmpAlloc)
    {
        delete [] mTmpAlloc;
    }
}

void NetUrl::appendChange(const ByteString &key, const ByteString &value)
{
    ListTable<QueryKV>::iterator it = mList.begin();
    for ( ; it != mList.end(); ++it)
    {
        if (it->key == key)
        {
            it->value = value;
            return;
        }
    }

    mList.push_back(QueryKV(key, value));
}

ByteString NetUrl::genUrl()
{
    if (mBaseUrl.empty() || mList.empty())
    {
        return mBaseUrl;
    }

    ByteString tmp = tar2String(mList);

    memcpy(mTmpAlloc, mBaseUrl.string(), mBaseUrl.size());
    mTmpAlloc[mBaseUrl.size()] = '?';

    return ByteString(mTmpAlloc, mBaseUrl.size() + 1 + tmp.size());
}

static size_t _sf_strlcpy (char *dst, const char *src, size_t size)
{
    const char *old = src;

    /* Copy as many bytes as will fit */
    if (size)
    {
        while (--size)
        {
            if (!(*dst++ = *src++))
            {
                return src - old - 1;
            }
        }

        *dst = 0;
    }

    while (*src++);
    return src - old - 1;
}

/**
 * @brief NetUrl::genConcat
 * @param first
 * @param str0
 * @return
 * @details 拼接字符串，同ByteString::concatStrings
 */
ByteString NetUrl::genConcat(const ByteString &first, const char *str0, ...)
{
    va_list args;
    const char *arg;
    int length = first.size(), pos = first.size();
    char *s;
    if(!str0 || first.empty()) return first;

    va_start (args, str0);
    for (arg = str0; arg; arg = va_arg (args, const char *))
    {
        length += strlen(arg);
    }
    va_end (args);

    s = allocAddr(length);

    //GEN_Printf(LOG_DEBUG, "[%s], size: %d, length: %d", first.string(), first.size(), length);
    strncpy(s, first.string(), first.size());

    va_start (args, str0);
    for (arg = str0; arg; arg = va_arg (args, const char *))
    {
        pos += _sf_strlcpy(s + pos, arg, length - pos + 1);
    }
    va_end (args);

    return ByteString(s, length);
}

/**
 * @brief NetUrl::genKLSign
 * @param appid
 * @param secretkey
 * @return
 * @details kl 签名算法的字串生成
 */
ByteString NetUrl::genKLSign(const ByteString &appid, const ByteString &secretkey)
{
    const char *method = "get";
    if (mMethodType == NET_HTTP_METHOD_POST)
    {
        method = "post";
    }
    int urlLen = 5 + mBaseUrl.size() + appid.size() + secretkey.size();

    int   i = 0;
    char *urlStr = allocAddr(urlLen);

    strcpy(urlStr, method);
    i += strlen(method);

    memcpy(urlStr + i, mBaseUrl.string(), mBaseUrl.size());
    i += mBaseUrl.size();

    memcpy(urlStr + i, appid.string(), appid.size());
    i += appid.size();

    memcpy(urlStr + i, secretkey.string(), secretkey.size());
    i += secretkey.size();

    urlStr[i] = '\0';

    return ByteString(urlStr, i);
}

ByteString NetUrl::tar2String(ListTable<NetUrl::QueryKV> &list)
{
    int urlLen = mBaseUrl.size() + 2; //uri + ?
    ListTable<QueryKV>::iterator it = list.begin();

    for (; it != list.end(); ++it)
    {
        urlLen += it->key.size() + it->value.size() + 2;  // &key=value
    }

    int   i = 0;
    char *urlStr = allocAddr(urlLen);

    // 跳过基本地址的长度，这部分的位置
    // memcpy(urlStr, mBaseUrl.string(), mBaseUrl.size());
    // int i = mBaseUrl.size();
    // urlStr[i++] = '?';
    urlStr += mBaseUrl.size() + 1;

    for (it = list.begin(); it != list.end(); ++it)
    {
        if (!it->key.empty())
        {
            memcpy(urlStr + i, it->key.string(), it->key.size());
            i += it->key.size();
            urlStr[i++] = '=';
            if (!it->value.empty())
            {
                memcpy(urlStr + i, it->value.string(), it->value.size());
                i += it->value.size();
            }
            urlStr[i++] = '&';
        }
    }
    urlStr[i - 1] = '\0';

    return ByteString(urlStr, i);
}

char *NetUrl::allocAddr(int len)
{
    if (mTmpAlloc || (mTmpAllocLen < len))
    {
        delete [] mTmpAlloc;
        mTmpAlloc = new char[len + 1];
        mTmpAllocLen = len;
    }
    return  mTmpAlloc;
}


