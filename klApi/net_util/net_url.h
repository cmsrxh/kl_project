#ifndef NET_URL_H
#define NET_URL_H

#include <util/byte_string.h>
#include <util/list_table.h>

class CurlLoadItem;
/**
 * @brief The NetUrl class
 * @warning 不负责管理输入的字符串的内存，即是用户传递的string必须在
 * NetUrl对象的整个生命周期内，内存不能随意释放，即使要释放，
 * 也必须及时通知NetUrl对象.
 */
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

    /// @warning 不负责管理输入的字符串的内存
    void append(const ByteString &key, const ByteString &value)
    {
        mList.push_back(QueryKV(key, value));
    }
    /**
     * @brief appendChange
     * @param key [in]
     * @param value [in]
     * @details 互斥添加键值对
     */
    void appendChange(const ByteString &key, const ByteString &value);

    void appendContent(const ByteString &key, const ByteString &value)
    {
        mContentList.push_back(QueryKV(key, value));
    }

    ByteString baseUrl() const
    {
        return mBaseUrl;
    }

    /**
     * @brief genUrl
     * @return 返回的字串内存地址不需要用户释放
     * @details 生成完整网络访问地址，根据GET方法中参数键值对（mList）和基本网络路径
     */
    ByteString genUrl();
    ByteString genUrl() const
    {
        return const_cast<NetUrl *>(this)->genUrl();
    }

    /**
     * @brief  genTable
     * @return 返回的字串内存地址不需要用户释放
     * @details 根据（mList）生成拼接字串
     */
    ByteString genTable()
    {
        return tar2String(mList);
    }
    ByteString genTable() const
    {
        return const_cast<NetUrl *>(this)->genTable();
    }

    /**
     * @brief  genContent
     * @return 返回的字串内存地址不需要用户释放
     * @details 根据（mList）生成拼接字串
     */
    ByteString genContent()
    {
        return tar2String(mContentList);
    }
    ByteString genContent() const
    {
        return const_cast<NetUrl *>(this)->genContent();
    }

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
