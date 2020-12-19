#include <locale.h>
#include <stdarg.h>
#include "events/common_log.h"
#include "fsm/sf_state_mache.h"
#include "mpv_player_proc.h"
#include "iface/media_notify.h"
#include "application.h"

Application::Application()
{
    setlocale(LC_NUMERIC, "C");
}

void Application::initialize()
{    
    stateMache->initialize();

    MediaNotify::instance()->init_notify();
}

void Application::runLoop()
{
    const QEvt *evt;

    initThreadPrivate();

    GEN_Printf(LOG_DEBUG, "event loop is started");
    while(getMessage(evt))
    {
        switch (evt->sig)
        {        
        case SIG_ON_MPV_EVENT:
            MPVPlayerProc::instance()->onMpvEvents();
            break;

        case SIG_MVP_NEED_RECREARE:
            MPVPlayerProc::instance()->reCreatePlayer();
            break;

        case SIG_SET_SOURCE_PREPARE:
            MediaNotify::instance()->notifyPrepare();
            break;

        case SIG_STATE_EVENT_PROC:
            stateMache->procEvt(evt);
            break;

        case SIG_MPV_PLAY_STOP:       /* mpv播放状态停止了，有可能有新的媒体文件要播放，所以上一个播放被停止了 */
            GEN_Printf(LOG_DEBUG, "player stop.");
            //MPVPlayerProc::instance()->checkSurface(0);
            break;

        case SIG_USER_UNUSED:
        default:
            GEN_Printf(LOG_WARN, "[%d] is UNKOWN.", evt->sig);
            break;
        }

        recycleEvent(evt);
    }

    GEN_Printf(LOG_WARN, "Event Loop is Exit.");
    exit(EXIT_SUCCESS);
}


