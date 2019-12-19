#include <events/common_log.h>
#include "http_util.h"

HttpUtil::HttpUtil()
{

}

void HttpUtil::clearBytes(const ByteString &it)
{
    GEN_Printf(LOG_DEBUG, "clear: %s", it.string());
    it.clear();
}

/**
 * @details 格式化其中的空格，使之转换程‘%20’
 * @brief HttpUtil::formatEmpty
 * @param query [in] 网络地址
 * @return  返回格式化后的地址，应当主动释放 query 的内存
 */
ByteString HttpUtil::formatEmpty(const ByteString &query)
{
    int   space_num = 0;
    char *string = query.string();

    for (int i = 0; i < query.size(); ++i)
    {
        if(string[i] == ' ')
        {
            ++space_num;
        }
    }

    if (0 == space_num)
    {
        return ByteString::allocString(query);
    }
    /*  按照最大内存计算，假设传入的都是空格 */
    // char *newStr = new char[query.size() * 3];

    char *newStr =  new char[query.size() + space_num * 3 + 1];
    int   newLen = 0;

    for (int i = 0; i < query.size(); ++i)
    {
        if (string[i] == ' ')
        {
            newStr[newLen++] = '%';
            newStr[newLen++] = '2';
            newStr[newLen++] = '0';
        } else
        {
            newStr[newLen++] = string[i];
        }
    }

    return ByteString(newStr, newLen);
}

/**
 * @brief HttpUtil::urlDecoding
 * @param query [in] 网络地址, 字符指向的地址时可以操作的
 * @param percent
 * @details 把‘%’表示的字符，替换的回来
 *  例如：%e9%99%88%e7%8c%9b 转成："陈猛"
 */
void HttpUtil::urlDecoding(ByteString &query, char percent)
{
    char *data = query.string();
    const char *inputPtr = query.string();

    int i = 0;
    int len = query.size();
    int outlen = 0;
    int a, b;
    char c;

    while (i < len)
    {
        c = inputPtr[i];
        if (c == percent && i + 2 < len)
        {
            a = inputPtr[++i];
            b = inputPtr[++i];

            if (a >= '0' && a <= '9') a -= '0';
            else if (a >= 'a' && a <= 'f') a = a - 'a' + 10;
            else if (a >= 'A' && a <= 'F') a = a - 'A' + 10;

            if (b >= '0' && b <= '9') b -= '0';
            else if (b >= 'a' && b <= 'f') b  = b - 'a' + 10;
            else if (b >= 'A' && b <= 'F') b  = b - 'A' + 10;

            data[outlen] = (char)((a << 4) | b);
        } else
        {
            data[outlen] = c;
        }

        ++i;
        ++outlen;
    }

    if (outlen != len)
    {
        query.resize(outlen);
    }
}

/**
 * @brief HttpUtil::urlEncoding
 * @param query [in] 网络地址, 字符指向的地址时可以操作的
 * @param result [out] 返回的转换后的数据
 * @param percent
 * @details 把‘%’表示的字符，替换的回来
 *  例如："陈猛" 转成： %e9%99%88%e7%8c%9b
 */
void HttpUtil::urlEncoding(const ByteString &query, ByteString &result, char percent)
{
    char *data = new char[query.size() * 3]; // 按照最大长度分配
    const char *inputPtr = query.string();
    const char *pattern = "0123456789abcdef";

    int i = 0;
    int len = query.size();
    int outlen = 0;
    char c;

    while (i < len)
    {
        c = inputPtr[i];
        if (inputPtr[i] & 0x80)
        {
            data[outlen++] = percent;
            data[outlen++] = pattern[c >> 4 & 0xF];
            data[outlen++] = pattern[c  & 0xF];
        } else
        {
            data[outlen++] = c;
        }
        ++i;
    }
    data[outlen] = '\0';

    result = ByteString(data, outlen);
}

/**
 * @details 替换
 * @brief HttpUtil::replace
 * @param str [in/out] 可以操作的字符
 * @param src [in] 需要替换的字符
 * @param dest [in] 目标字符
 */
void HttpUtil::replace(ByteString &str, const char *src, const char *dest)
{
    ByteString data;
    const char *inputPtr = str.string();

    int i = 0;
    int len = str.size();

    while (i < len)
    {
        int start = i;
        const char *p = src;
        while( inputPtr[i] == *p && *p )
        {
            ++p;
            ++i;
        }
        if(0 == *p)
        {
            data.append(dest);
        } else
        {
            if( i == start) i++;
            data.append(inputPtr + start, i - start);
        }
    }

    str.swap(data);
}

/**
 * @details 移除某个字节
 * @brief HttpUtil::remove
 * @param str [in/out] 可以操作的字符
 * @param ch [in] 字符
 */
void HttpUtil::remove(ByteString &str, const char ch)
{
    char *data = str.string();
    const char *inputPtr = str.string();

    int i = 0;
    int len = str.size();
    int outlen = 0;
    char c;

    while (i < len)
    {
        c = inputPtr[i];
        if(c != ch)
        {
            data[outlen] = c;
            ++outlen;
        }

        ++i;
    }

    if(outlen != len)
    {
        data[outlen] = '\0';

        str.resize(outlen);
    }
}

/**
 * @details 分割字符，然后加到list列表中
 * @brief HttpUtil::split
 * @param str [in] 需要分割的字符串
 * @param rep [in] 分割符
 * @param root [out] 得到的列表容器
 */
void HttpUtil::split(const ByteString &str, char rep, ListTable<ByteString> &root)
{
    int start = 0, end = 0;
    const char *data = str.string();

    while (end < (int)str.size())
    {
        if(data[end] == rep)
        {
            if (end > start)
            {
                ByteString str = ByteString::allocString(ByteString(data + start, end - start));
                root.push_back(str);
            }

            start = ++end;
        } else
        {
            ++end;
        }
    }
}
