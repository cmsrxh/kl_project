#ifndef _APP_COMMON_H
#define _APP_COMMON_H

#include "events/event_def.h"

enum {
    // SIG_USER_UNUSED = SIG_INNER_MAX,
    SIG_USER_UNUSED = 1000,     /* 前面999个信号枚举交给系统使用 */

    SIG_U_DISK_MOUNT,           //U盘添加
    SIG_U_DISK_REMOVE,          //U盘移除
    SIG_TEST_EXE_CMD,
};

#define NO_RETUEN __attribute__((noreturn))

#endif // _APP_COMMON_H
