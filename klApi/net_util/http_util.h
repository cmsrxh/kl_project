#ifndef HTTP_UTIL_H
#define HTTP_UTIL_H

#include <util/byte_string.h>
#include <util/list_table.h>

class HttpUtil
{
public:
    HttpUtil();

    static void clearBytes (const ByteString &it);

    static ByteString formatEmpty(const ByteString &query);

    static void urlDecoding(ByteString &query, char percent = '%');
    static void urlEncoding(const ByteString &query, ByteString &result, char percent = '%');

    static void replace(ByteString &str, const char *src, const char *dest);

    static void remove(ByteString &str, const char ch);

    static void split(ByteString const &str, char rep, ListTable<ByteString> &root);
};

#endif // HTTP_UTIL_H
