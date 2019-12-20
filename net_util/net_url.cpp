#include <events/common_log.h>
#include <stdarg.h>
#include "net_url.h"

NetUrl::NetUrl(const ByteString &url, int method)
    : mMethodType(method), mTmpAllocLen(0)
    , mBaseUrl(url)
{}

NetUrl::~NetUrl()
{
//    mList.removeAll(delQueryKV);
//    mBaseUrl.clear();
}

ByteString NetUrl::genUrl()
{
    if (mBaseUrl.empty() || mList.empty())
    {
        return mBaseUrl;
    }

    int urlLen = mBaseUrl.size() + 2; //uri + ?
    ListTable<QueryKV>::iterator it = mList.begin();

    for (; it != mList.end(); ++it)
    {
        urlLen += it->key.size() + it->value.size() + 2;  // &key=value
    }

    char *urlStr;
    int i = 0;
    if (mTmp.empty() || (mTmpAllocLen < urlLen))
    {
        mTmp.clear();
        urlStr = new char[urlLen + 1];
        mTmpAllocLen = urlLen;
    } else
    {
        urlStr = mTmp.string();
    }

    memcpy(urlStr, mBaseUrl.string(), mBaseUrl.size());
    i += mBaseUrl.size();
    urlStr[i++] = '?';

    for (it = mList.begin(); it != mList.end(); ++it)
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
    mTmp = ByteString(urlStr, i);

    return mTmp;
}

ByteString NetUrl::genTable()
{
    ByteString tab = genUrl();

    return ByteString(tab.string() + mBaseUrl.size() + 1, tab.size() - mBaseUrl.size() - 1);
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
    size_t length = first.size(), pos = first.size();
    char *s;
    if(!str0 || first.empty()) return first;

    va_start (args, str0);
    for (arg = str0; arg; arg = va_arg (args, const char *))
    {
        length += strlen(arg);
    }
    va_end (args);

    if (mTmp.empty() || (mTmpAllocLen < (int)length))
    {
        mTmp.clear();
        s = new char[length + 1];
        mTmpAllocLen = length;
    } else
    {
        s = mTmp.string();
    }

    //GEN_Printf(LOG_DEBUG, "[%s], size: %d, length: %d", first.string(), first.size(), length);
    strncpy(s, first.string(), first.size());

    va_start (args, str0);
    for (arg = str0; arg; arg = va_arg (args, const char *))
    {
        pos += _sf_strlcpy(s + pos, arg, length - pos + 1);
    }
    va_end (args);

    mTmp = ByteString(s, length);

    return mTmp;
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

    char *urlStr;
    int i = 0;
    if (mTmp.empty() || (mTmpAllocLen < urlLen))
    {
        mTmp.clear();
        urlStr = new char[urlLen + 1];
        mTmpAllocLen = urlLen;
    } else
    {
        urlStr = mTmp.string();
    }

    strcpy(urlStr, method);
    i += strlen(method);

    memcpy(urlStr + i, mBaseUrl.string(), mBaseUrl.size());
    i += mBaseUrl.size();

    memcpy(urlStr + i, appid.string(), appid.size());
    i += appid.size();

    memcpy(urlStr + i, secretkey.string(), secretkey.size());
    i += secretkey.size();

    urlStr[i] = '\0';
    mTmp = ByteString(urlStr, i);

    return mTmp;
}


