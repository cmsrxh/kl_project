#include "normal_state.h"
#include "state_common.h"
#include "events/common_log.h"
#include "mpv_player_proc.h"
#include "application.h"

NormalState::NormalState()
{

}

int NormalState::entry(const StateQEvt *)
{
    GEN_Printf(LOG_DUMP, "NormalState::entry");
    return 0;
}

int NormalState::exit(const StateQEvt *)
{
    GEN_Printf(LOG_DUMP, "NormalState::exit");
    return 0;
}

int NormalState::translate(const StateQEvt *evt)
{
    int ret = state::STATE_RETURN_NORMAL;
    GEN_Printf(LOG_DUMP, "Normal state event:%d, arg:%d", evt->stateMsg, evt->wParam);
    switch (evt->stateMsg)
    {
    case state::STATE_EVENT_SET_FILE:       // 设置播放文件, 参数时文件名字
        if(NULL == evt->pHander)
        {
            GEN_Printf(LOG_ERROR, "Set Play file is null.");
        } else
        {
            MPVPlayerProc::instance()->setFile((char *)evt->pHander);
            Application::instance()->postCmd(SIG_SET_SOURCE_PREPARE);
        }
        break;

    case state::STATE_EVENT_START:          // 开始播放
        MPVPlayerProc::instance()->start();
        break;

    case state::STATE_EVENT_ENTER_PLAYING:  // 播放器已经启动了并播放媒体，进入播放状态
        ret = state::STATE_PLAYING;
        break;

    case state::STATE_EVENT_PLAYING_IDLE:   // 媒体播放已经结束了，需要退出播放状态，进入初始状态
    case state::STATE_EVENT_PLAY:           // 播放
    case state::STATE_EVENT_STOP:           // 停止
    case state::STATE_EVENT_PAUSE:          // 暂停
    case state::STATE_EVENT_PLAY_PAUSE:     // 暂停 or 播放
    case state::STATE_EVENT_RESET:          // 重设
    case state::STATE_EVENT_SEEK_TO:        // 播放跳转, seek毫秒，以及模式
    case state::STATE_EVENT_EXIT_PLAYING:   // 退出播放器
        GEN_Printf(LOG_INFO, "Current is not in playing status.");
        break;
    default:
        GEN_Printf(LOG_WARN, "Unkown state: %d", evt->stateMsg);
        break;
    }
    return ret;
}
