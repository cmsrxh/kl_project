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
    i--;
    urlStr[i] = '\0';

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


