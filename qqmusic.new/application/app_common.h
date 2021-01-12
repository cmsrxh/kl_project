#ifndef _APP_COMMON_H
#define _APP_COMMON_H

#include "events/event_def.h"

enum {
    // SIG_USER_UNUSED = SIG_INNER_MAX,
    SIG_USER_UNUSED = 1000,     /* 前面999个信号枚举交给系统使用 */

    SIG_RELOAD_ERR_OBJECT,   // 重新加载错误的对象
    SIG_COLLECT_ERR_OBJECT,  // 收集加载错误的对象

};

#define NO_RETUEN __attribute__((noreturn))

#endif // _APP_COMMON_H
