#ifndef _MEDIA_IFACE_COMMON_H
#define _MEDIA_IFACE_COMMON_H


//! 播放过程中，发生的错误信息
enum {
    ERROR_UNKNOWN_FORMAT    = -1,           //!< Format not supported. 不支持的视频格式
    ERROR_LOADING_FAILED    = -2,           //!< File load failed. 加载文件错误
    ERROR_NOTHING_TO_PLAY   = -3,           //!< Nothing to play. 没有播放文件
    ERROR_PLAYER_SYSTEM     = -4,           //!< Internal error in playback system. 播放系统内部出错
    ERROR_AO_INIT_FAILED    = -5,           //!< Noise equipment initialization error. 出声设备错误
    ERROR_VO_INIT_FAILED    = -6,           //!< Video device initialization error. 视频设备初始化错误
    ERROR_GENERIC           = -7,           //!< Error generic. 通常错误
    ERROR_PLAYER_SHUTDOWN   = -8,           //!< Play core exit. 播放核心退出
    ERROR_PTS_INVLAID       = -9,           //!< Time schedule anomaly, 时间进度异常
};

//! 播放反馈信息枚举
enum {
    MEDIA_NOP                   = 0,        //!< interface test message
    MEDIA_PREPARED              = 1,        //!< 播放媒体准备，发生在用户调用setFile之后，播放器返回
    MEDIA_PLAYBACK_COMPLETE     = 2,        //!< not used
    MEDIA_BUFFERING_UPDATE      = 4,        //!< not used
    MEDIA_SEEK_COMPLETE         = 5,        //!< 跳转完毕
    MEDIA_SET_VIDEO_SIZE        = 6,        //!< not used
    MEDIA_STARTED               = 7,        //!< 开始
    MEDIA_PAUSED                = 8,        //!< 暂停
    MEDIA_STOPPED               = 9,        //!< 停止
    MEDIA_SKIPPED               = 10,       //!< not used
    MEDIA_PLAYING               = 11,       //!< 播放
    MEDIA_CACHE_TIME            = 15,       //!< 网络下载进度
    MEDIA_NOTIFY_TIME           = 16,       //!< 播放文件的总时间
    MEDIA_TIMED_TEXT            = 17,       //!< not used
    MEDIA_ERROR                 = 18,       //!< 错误消息反馈
    MEDIA_INFO                  = 19,       //!< not used
    MEDIA_SUBTITLE_DATA         = 25,       //!< 字幕文字
    MEDIA_META_DATA             = 26,       //!< not used
    MEDIA_DRM_INFO              = 27,       //!< not used
    MEDIA_TIME_DISCONTINUITY    = 28,       //!< not used
    MEDIA_AUDIO_ROUTING_CHANGED = 29,       //!< not used
};

//! 播放器的状态
enum {
    PLAYER_IDLE   = 0,      //!< 空闲
    PLAYER_STOP   = 1,      //!< 停止
    PLAYER_PLAY   = 2,      //!< 正在播放
    PLAYER_PAUSE  = 3,      //!< 暂停
    PLAYER_WAIT   = 4,      //!< 加载中
};

#endif
