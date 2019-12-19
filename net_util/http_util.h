#ifndef HTTP_UTIL_H
#define HTTP_UTIL_H

#include <util/byte_string.h>
#include <util/list_table.h>

class HttpUtil
{
public:
    HttpUtil();

    static void clearBytes (const ByteString &it);

    ByteString formatEmpty(const ByteString &query);

    void urlDecoding(ByteString &query, char percent = '%');
    void urlEncoding(const ByteString &query, ByteString &result, char percent = '%');

    void replace(ByteString &str, const char *src, const char *dest);

    void remove(ByteString &str, const char ch);

    void split(ByteString const &str, char rep, ListTable<ByteString> &root);
};

#endif // HTTP_UTIL_H
