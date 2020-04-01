#ifndef NET_COMMON_H
#define NET_COMMON_H

#include <util/net_buffer.h>

enum
{
    OP_CURL_STATUS_LOAD_ALL_OVER,      // 表示数据下载全部一次性返回了, data = (NetBuffer *)
    OP_CURL_STATUS_LOAD_ONE_FRAME,     // 表示数据下载方式按照块来计算，有多少数据就来多少 data = (NetBuffer *)
    OP_CURL_STATUS_LOAD_OVER,          // 表示数据下载完成了，与上一个枚举配合，表示下载已经完成，可以关闭文件。 data = not used

    OP_CURL_STATUS_ERROR_TYPE,         // 往下的枚举表示数据下载过程中curl的错误定义值, data = (char *)

};
typedef void (*OpCurlStatus) (int status, void *data, void *arg);

/**
  "User-Agent:  Mozilla/5.0 (Windows NT 10.0; Win64; x64)
                AppleWebKit/537.36 (KHTML, like Gecko)
                Chrome/76.0.3809.132
                Safari/537.36"

有时候发出的https GET/POST 消息不会收到信息，但是数据都是对的，这有时需要在http的协议头上，加上user-agent
*/
#ifndef USER_AGENT_STRING
//#define USER_AGENT_STRING "Chrome/76.0.3809.132"
#define USER_AGENT_STRING "Mozilla/5.0 (X11; Linux x86_64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/75.0.3770.100 Safari/537.36"
#endif

class CurlLoadItem;
class CurlLoadArg
{
public:
    CurlLoadArg()
        : mUserAgent(0)
        , mReferer(0)
    {}
    CurlLoadArg(const char *agent, const char *ref)
        : mUserAgent(agent)
        , mReferer(ref)
    {}

    const char *userAgent()
    {
        return  mUserAgent;
    }
    void setUserAgent(const char *userAgent)
    {
        mUserAgent = userAgent;
    }

    const char *referer()
    {
        return mReferer;
    }
    void setReferer(const char *ref)
    {
        mReferer = ref;
    }

private:
    const char *mUserAgent;
    const char *mReferer;
};

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
