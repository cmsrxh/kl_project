#include "state_common.h"
#include "events/common_log.h"
#include "application.h"
#include "mpv_player_proc.h"
#include "iface/media_notify.h"
#include "playing_state.h"

PlayingState::PlayingState()
{

}

int PlayingState::entry(const StateQEvt *)
{
    GEN_Printf(LOG_DUMP, "PlayingState::entry");
    return 0;
}

int PlayingState::exit(const StateQEvt *)
{
    GEN_Printf(LOG_DUMP, "PlayingState::exit");
    MediaNotify::instance()->notifyPlayerIdle();
    return 0;
}

int PlayingState::translate(const StateQEvt *evt)
{
    int ret = state::STATE_RETURN_NORMAL;
    // GEN_Printf(LOG_DUMP, "Playing state event:%d, arg:%d", evt->stateMsg, evt->wParam);

    switch (evt->stateMsg)
    {       
    case state::STATE_EVENT_START:          // 开始播放
        MPVPlayerProc::instance()->start(evt->wParam);
        break;

    case state::STATE_EVENT_PLAY:           // 播放
        MPVPlayerProc::instance()->play();
        break;

    case state::STATE_EVENT_STOP:           // 停止
        MPVPlayerProc::instance()->stop();
        break;

    case state::STATE_EVENT_PAUSE:          // 暂停
        MPVPlayerProc::instance()->pause();
        break;

    case state::STATE_EVENT_PLAY_PAUSE:     // 暂停 or 播放
        MPVPlayerProc::instance()->playSwitch();
        break;
    case state::STATE_EVENT_RESET:          // 重设
        MPVPlayerProc::instance()->stop();
        break;

    case state::STATE_EVENT_SEEK_TO:        // 播放跳转, seek毫秒，以及模式
        MPVPlayerProc::instance()->seek(evt->wParam);
        break;

    case state::STATE_EVENT_EXIT_PLAYING:   // 退出播放器
        MPVPlayerProc::instance()->stop();
        break;

    case state::STATE_EVENT_ENTER_PLAYING:  // 播放器已经启动了并播放媒体，进入播放状态
        break;

    case state::STATE_EVENT_PLAYING_IDLE:   // 媒体播放已经结束了，需要退出播放状态，进入初始状态
        ret = state::STATE_NORMAL;
        break;
    default:
        ret = NormalState::translate(evt);
        break;
    }
    return ret;
}
