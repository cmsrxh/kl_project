#ifndef _APP_COMMON_H
#define _APP_COMMON_H

#include "events/event_def.h"

enum {
    // SIG_USER_UNUSED = SIG_INNER_MAX,
    SIG_USER_UNUSED = 1000,     /* 前面999个信号枚举交给系统使用 */

};

#define NO_RETUEN __attribute__((noreturn))

typedef void (*CurlLoadData) (uint8_t *data, size_t len, void *arg);
typedef void (*CurlLoadState) (bool state, const char *desc, void *arg);


#endif // _APP_COMMON_H
