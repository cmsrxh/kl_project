#ifndef NET_URL_H
#define NET_URL_H

#include <util/byte_string.h>
#include <util/list_table.h>

class CurlLoadItem;
class NetUrl
{
public:
    struct QueryKV
    {
        QueryKV(const ByteString &key, const ByteString &value)
            : key(key), value(value)
        {}
        QueryKV()
        {}

        ByteString key;
        ByteString value;
    };

    static void delQueryKV(QueryKV &kv)
    {
        kv.key.clear();
        kv.value.clear();
    }
    enum
    {
        NET_HTTP_METHOD_GET,
        NET_HTTP_METHOD_POST,
    };

    NetUrl(const ByteString &url = ByteString(), int methodType = NET_HTTP_METHOD_GET);
    ~NetUrl();

    bool empty() const
    {
        return mBaseUrl.empty();
    }

    void append(const ByteString &key, const ByteString &value)
    {
        mList.push_back(QueryKV(key, value));
    }
    void appendChange(const ByteString &key, const ByteString &value);

    void appendContent(const ByteString &key, const ByteString &value)
    {
        mContentList.push_back(QueryKV(key, value));
    }
    void clean()
    {
        mList.removeAll(delQueryKV);
    }
    ByteString baseUrl() const
    {
        return mBaseUrl;
    }

    ByteString genUrl();
    ByteString genUrl() const
    {
        return const_cast<NetUrl *>(this)->genUrl();
    }

    ByteString genTable()
    {
        return tar2String(mList);
    }
    ByteString genTable() const
    {
        return const_cast<NetUrl *>(this)->genTable();
    }

    ByteString genContent()
    {
        return tar2String(mContentList);
    }
    ByteString genContent() const
    {
        return const_cast<NetUrl *>(this)->genContent();
    }

    ByteString genConcat(const ByteString &first, const char *str0, ...);

    ByteString genKLSign(const ByteString &appid, const ByteString &secretkey);
private:
    ByteString tar2String(ListTable<QueryKV> &list);
    char *allocAddr(int len);

    int     mMethodType;
    int     mTmpAllocLen;
    char   *mTmpAlloc;
    ByteString mBaseUrl;

    ListTable<QueryKV> mList;
    ListTable<QueryKV> mContentList;

    friend class CurlLoadItem;
};

#endif // NET_URL_H
