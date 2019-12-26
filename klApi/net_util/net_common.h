#ifndef NET_COMMON_H
#define NET_COMMON_H

enum
{
    OP_CURL_STATUS_LOAD_ALL_OVER,      // 表示数据下载全部一次性返回了
    OP_CURL_STATUS_LOAD_ONE_FRAME,     // 表示数据下载方式按照块来计算，有多少数据就来多少
    OP_CURL_STATUS_LOAD_OVER,          // 表示数据下载完成了，与上一个枚举配合，表示下载已经完成，可以关闭文件。

    OP_CURL_STATUS_ERROR_TYPE,         // 往下的枚举表示数据下载过程中curl的错误定义值

};
typedef void (*OpCurlStatus) (int status, uint8_t *data, size_t len, void *arg);


#include "cJSON.h"
#include <stdarg.h>

inline static cJSON *json_items_proc(cJSON *item, const char *name, ...)
{
    cJSON *obj = item;
    va_list args;

    va_start (args, name);
    for (const char *arg = name; arg; arg = va_arg (args, const char *))
    {
        obj = cJSON_GetObjectItem(obj, arg);
        if (!obj)
        {
            // GEN_Printf(LOG_WARN, "json section: %s is null", arg);
        }
    }

    va_end (args);
    return obj;
}

#define JSON_VALUETRING_SWAP_BYTESTRING(parent, name, proc) \
    do { \
        cJSON *obj = cJSON_GetObjectItem(parent, #name); \
        if(obj) \
        { \
            proc = obj->valuestring; \
            obj->valuestring = NULL; \
        } else \
        { \
            proc = ByteString(); \
        } \
    } while(0)

#define JSON_STRING_POINTER_SWAP(json, parent, proc) \
    do { \
        if(json != parent) \
        { \
            proc = json->valuestring; \
            json->valuestring = NULL; \
        } else \
        { \
            proc = ByteString(); \
        } \
    } while(0)
#endif // NET_COMMON_H
