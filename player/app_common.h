#ifndef _APP_COMMON_H
#define _APP_COMMON_H

#include "events/event_def.h"

enum {
    SIG_USER_UNUSED = 1000,     /* 前面999个信号枚举交给系统使用 */

    SIG_SURFACE_CREATED,
    SIG_SURFACE_CHANGED,
    SIG_SURFACE_DESTROYED,

    SIG_MVP_NEED_RECREARE,
    SIG_SET_SOURCE_PREPARE,    /* 设置播放源信息完成 */

    SIG_ON_MPV_EVENT,

    SIG_STATE_EVENT_PROC,   // 状态机处理

    SIG_SOCKET_SERVICE_EXIT,
};

#define NO_RETUEN __attribute__((noreturn))

#endif // _APP_COMMON_H
