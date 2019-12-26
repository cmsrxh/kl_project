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

    //考拉播放器信息
    SIG_HAVE_OPEN_ID,       // 表示设备已经，激活成功，并获得了一个openid，只有这样考拉才能使用
    SIG_KL_INIT_ERROR,      // kaola设备激活或者初始化失败处理信息
};

// player error info
enum {
    ERROR_UNKNOWN_FORMAT    = -1,           /* Format not supported. 不支持的视频格式 */
    ERROR_LOADING_FAILED    = -2,           /* File load failed. 加载文件错误 */
    ERROR_NOTHING_TO_PLAY   = -3,           /* Nothing to play. 没有播放文件 */
    ERROR_PLAYER_SYSTEM     = -4,           /* Internal error in playback system. 播放系统内部出错 */
    ERROR_AO_INIT_FAILED    = -5,           /* Noise equipment initialization error. 出声设备错误 */
    ERROR_VO_INIT_FAILED    = -6,           /* Video device initialization error. 视频设备初始化错误 */
    ERROR_GENERIC           = -7,           /* Error generic. 通常错误 */
    ERROR_PLAYER_SHUTDOWN   = -8,           /* Play core exit. 播放核心退出 */
    ERROR_PTS_INVLAID       = -9,           /* Time schedule anomaly, 时间进度异常 */
};

#define NO_RETUEN __attribute__((noreturn))


#define APPID       "cn5290"
#define SECRETKEY   "d673b807770b49b4077cb3bfa0d6aedd"
#define STR_PATTERN "0123456789abcdef"

#define NET_LOCAL_CONFIG_FILE           "/tmp/kl.net.local.config.info"
#define NET_KL_BDC_AREA_LIST_FILE       "/tmp/kl.broadcast.area.list.info"
#define NET_KL_BDC_CATE_LIST_FILE       "/tmp/kl.broadcast.category.list.info"
#define NET_KL_CATE_ALL_LIST_FILE       "/tmp/kl.category.all.list.info"




#endif // _APP_COMMON_H
