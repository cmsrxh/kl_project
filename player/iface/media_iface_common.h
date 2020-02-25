/*************************************************************************
	> File Name: media_iface_common.h
	> Author: 
	> Mail: 
	> Created Time: 2019年07月20日 星期六 16时00分33秒
 ************************************************************************/

#ifndef _MEDIA_IFACE_COMMON_H
#define _MEDIA_IFACE_COMMON_H

#include "ipc/ipc_sock_common.h"

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

enum {
    CLIENT_CALL_Start = ipc::CLIENT_CALL_Method_Base,
    CLIENT_CALL_Play,
    CLIENT_CALL_Stop,
    CLIENT_CALL_Pause,
    CLIENT_CALL_Reset,
    CLIENT_CALL_PlayPause,
    CLIENT_CALL_SetFile,
    CLIENT_CALL_IsPlaying,
    CLIENT_CALL_SeekTo,
    CLIENT_CALL_Get_Current_Position,
    CLIENT_CALL_Get_Duration,

};

enum {
    MEDIA_NOP = 0, // interface test message
    MEDIA_PREPARED = 1,
    MEDIA_PLAYBACK_COMPLETE = 2,
    MEDIA_BUFFERING_UPDATE = 3,
    MEDIA_SEEK_COMPLETE = 4,
    MEDIA_SET_VIDEO_SIZE = 5,
    MEDIA_STARTED = 6,
    MEDIA_PAUSED = 7,
    MEDIA_STOPPED = 8,
    MEDIA_SKIPPED = 9,
    MEDIA_CACHE_TIME = 10,  // 网络下载进度
    MEDIA_NOTIFY_TIME = 98,
    MEDIA_TIMED_TEXT = 99,
    MEDIA_ERROR = 100,
    MEDIA_INFO = 200,
    MEDIA_SUBTITLE_DATA = 201,
    MEDIA_META_DATA = 202,
    MEDIA_DRM_INFO = 210,
    MEDIA_TIME_DISCONTINUITY = 211,
    MEDIA_AUDIO_ROUTING_CHANGED = 10000,
};

namespace Media
{

//各种状态变化迁移
enum ActionStatus
{
#define PLAYER_IDLE   Media::ACTION_IDLE
    ACTION_IDLE     = 0x01,    //初始空闲状态

#define PLAYER_START  Media::ACTION_STARTED
    ACTION_STARTED  = 0x02,    //开始

#define PLAYER_PLAY   Media::ACTION_OVER
    ACTION_OVER     = 0x04,    //有两层含义，(1)结束，(2)代表某些只有三种状态的true

#define PLAYER_PAUSE  Media::ACTION_UNOVER
    ACTION_UNOVER   = 0x08,    //类似于false 表示未完成，仅仅代表某些只有三种状态的false

    ACTION_FAILED   = 0x10,    //失败


    ACTION_RUNNING  = 0x20,    //正在进行中
};

}
#endif
