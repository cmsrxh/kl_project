#ifndef STATE_COMMON_H
#define STATE_COMMON_H

class SFState;
class StateQEvt;

namespace state
{
enum StateTypes
{
    STATE_NORMAL,
    STATE_PLAYING,

    STATE_MAX_DESC,
};

enum StateEvent
{
    STATE_EVENT_START,          // 开始播放
    STATE_EVENT_PLAY,           // 播放
    STATE_EVENT_STOP,           // 停止
    STATE_EVENT_PAUSE,          // 暂停
    STATE_EVENT_PLAY_PAUSE,     // 暂停 or 播放

    STATE_EVENT_RESET,          // 重设
    STATE_EVENT_SET_FILE,       // 设置播放文件, 参数时文件名字
    STATE_EVENT_SEEK_TO,        // 播放跳转, seek毫秒，以及模式

    STATE_EVENT_EXIT_PLAYING,   // 退出播放器

    // 状态切换事件
    STATE_EVENT_ENTER_PLAYING,  // 播放器已经启动了并播放媒体，进入播放状态
    STATE_EVENT_PLAYING_IDLE,   // 媒体播放已经结束了，需要退出播放状态，进入初始状态
};

}

#endif // STATE_COMMON_H
