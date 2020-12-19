#ifndef _APP_COMMON_H
#define _APP_COMMON_H

#include "events/event_def.h"

enum {
    // SIG_USER_UNUSED = SIG_INNER_MAX,
    SIG_USER_UNUSED = 1000,     /* 前面999个信号枚举交给系统使用 */

    SIG_TRAVEL_MEDIA_FILE,
    SIG_USB_DEVICE_REMOVE,

    SIG_MEDIA_CTRL_playFile,
    SIG_MEDIA_CTRL_autoPlayNext,
    SIG_MEDIA_CTRL_showPlayingInfo,
    SIG_MEDIA_CTRL_prev,
    SIG_MEDIA_CTRL_next,
    SIG_MEDIA_CTRL_modelSwitch,
    SIG_MEDIA_CTRL_pictureWallpaper,
    SIG_MEDIA_CTRL_pictureScaleModel,
    SIG_MEDIA_CTRL_pictureRotateModel,

};

#define NO_RETUEN __attribute__((noreturn))

#endif // _APP_COMMON_H
