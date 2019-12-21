#ifndef _APP_COMMON_H
#define _APP_COMMON_H

#include "events/event_def.h"

enum {
    // SIG_USER_UNUSED = SIG_INNER_MAX,
    SIG_USER_UNUSED = 1000,     /* 前面999个信号枚举交给系统使用 */

};

#define NO_RETUEN __attribute__((noreturn))

enum
{
    OP_CURL_STATUS_LOAD_ALL_OVER,      // 表示数据下载全部一次性返回了
    OP_CURL_STATUS_LOAD_ONE_FRAME,     // 表示数据下载方式按照块来计算，有多少数据就来多少
    OP_CURL_STATUS_LOAD_OVER,          // 表示数据下载完成了，与上一个枚举配合，表示下载已经完成，可以关闭文件。

    OP_CURL_STATUS_ERROR_TYPE,         // 往下的枚举表示数据下载过程中curl的错误定义值

};
typedef void (*OpCurlStatus) (int status, uint8_t *data, size_t len, void *arg);



#define APPID      "cn5290"
#define SECRETKEY  "d673b807770b49b4077cb3bfa0d6aedd"
#define STR_PATTERN "0123456789abcdef"

#endif // _APP_COMMON_H
