#include <events/common_log.h>
#include "mpv_player_proc.h"
#include "application.h"
#include "fsm/sf_state_mache.h"
#include "ipc/ipc_data_package.h"
#include "ipc/service_iface_base.h"
#include "media_notify.h"

#ifndef USE_MPV_API_INTERFACE
class MediaPriser : public ServiceCmdPriserProc
{
public:
    void msgThreadExit()
    {
        GEN_Printf(LOG_ERROR, "Service Socket exit.");
        sleep(2);
        Application::instance()->postCmd(SIG_SOCKET_SERVICE_EXIT);
    }
    void cmdPriser(IPCDataPackage *in, IPCDataPackage *out)
    {
        out->setCmdType(ipc::SERVICE_REPLY_OK);

        switch (in->getCmd())
        {
        case CLIENT_CALL_Start:
            stateMache->postEvent(state::STATE_EVENT_START);
            break;
        case CLIENT_CALL_Play:
            stateMache->postEvent(state::STATE_EVENT_PLAY);
            break;
        case CLIENT_CALL_Stop:
            stateMache->postEvent(state::STATE_EVENT_STOP);
            break;
        case CLIENT_CALL_Pause:
            stateMache->postEvent(state::STATE_EVENT_PAUSE);
            break;
        case CLIENT_CALL_Reset:
            stateMache->postEvent(state::STATE_EVENT_RESET);
            break;
        case CLIENT_CALL_PlayPause:
            stateMache->postEvent(state::STATE_EVENT_PLAY_PAUSE);
            break;
        case CLIENT_CALL_SetFile:
        {
            Data::Unpack un(in->packet());
            char *file = in->dataGetString(un);
            GEN_Printf(LOG_DEBUG, "setFile: %s", file);
            stateMache->postEvent(state::STATE_EVENT_SET_FILE, 0, 0, file);
            break;
        }
        case CLIENT_CALL_IsPlaying:
            if (stateMache->checkState(state::STATE_PLAYING))
            {
                out->allocSetInt(!MPVPlayerProc::instance()->isPause());
            } else
            {
                out->allocSetInt(0);
            }
            break;
        case CLIENT_CALL_SeekTo:
        {
            Data::Unpack un(in->packet());
            int sec  = in->dataGetInt(un);
            int mode = in->dataGetInt(un);
            stateMache->postEvent(state::STATE_EVENT_SEEK_TO, sec, mode);
            break;
        }
        case CLIENT_CALL_Get_Current_Position:
            if (stateMache->checkState(state::STATE_PLAYING))
            {
                out->allocSetInt(MPVPlayerProc::instance()->getCurrentPos());
            } else
            {
                out->allocSetInt(0);
            }
            break;
        case CLIENT_CALL_Get_Duration:
            if (stateMache->checkState(state::STATE_PLAYING))
            {
                out->allocSetInt(MPVPlayerProc::instance()->getDuration());
            } else
            {
                out->allocSetInt(0);
            }
            break;
        default:
            GEN_Printf(LOG_DEBUG, "UNKown Cmd: %d", in->getCmd());
            break;
        }
    }
};

static MediaPriser gPriserInstance;
#endif

MediaNotify::MediaNotify()
    : m_pHandler(new ServiceIfaceBase(80080, 256))
{
#ifndef USE_MPV_API_INTERFACE
    m_pHandler->setCmdPriser(&gPriserInstance);
    m_pHandler->init();
#endif
}

void MediaNotify::init_notify()
{    

}

void MediaNotify::notifySeekEnd()
{
    notify(MEDIA_SEEK_COMPLETE);
}

void MediaNotify::notifyPlayError(int err, int sub, const char *str)
{
    notify(MEDIA_ERROR, err, sub, str);
}

void MediaNotify::notifyPlayComplete()
{
    notify(MEDIA_PLAYBACK_COMPLETE);
}

void MediaNotify::notifyPrepare()
{
    notify(MEDIA_PREPARED);
}

void MediaNotify::notifyPlayStart(const char *file)
{
    notify(MEDIA_STARTED);
    UNUSED(file);
}

void MediaNotify::notifyPlayerIdle()
{
    notify(MEDIA_STOPPED);
}

void MediaNotify::notifyPlayerPlaying()
{
    notify(MEDIA_STARTED);
}

void MediaNotify::notifyPlayerPause()
{
    notify(MEDIA_PAUSED);
}

void MediaNotify::notifyDuration(int dur)
{
    UNUSED(dur);
    notify(MEDIA_NOTIFY_TIME, dur);
}

void MediaNotify::notifySlider(int cur, int dur)
{
    UNUSED(cur);
    UNUSED(dur);
}

void MediaNotify::notifyScanStart()
{    

}

void MediaNotify::notifyScanEnd()
{

}

void MediaNotify::notifyModelSwitch(int mediaType, int mode)
{
    UNUSED(mediaType);
    UNUSED(mode);
}

void MediaNotify::notifyMusicIndex(int i)
{
    UNUSED(i);
}

void MediaNotify::notifyVideoIndex(int i)
{
    UNUSED(i);
}

void MediaNotify::notifyImageIndex(int i)
{
    UNUSED(i);
}

void MediaNotify::notifyMusicInfo(const char *author, const char *album)
{
    UNUSED(author);
    UNUSED(album);
}

void MediaNotify::notifyMusicImageInfo(uint8_t *data, int w, int h, int stride)
{
    UNUSED(data);
    UNUSED(w);
    UNUSED(h);
    UNUSED(stride);
}

void MediaNotify::notifyPictureStart(const char *name)
{
    UNUSED(name);
}

void MediaNotify::notifySubtitle(const char *sub)
{
    UNUSED(sub);
    notify(MEDIA_SUBTITLE_DATA, 0, 0, sub);
}

void MediaNotify::notifyMute(bool st)
{
    GEN_Printf(LOG_DEBUG, "MUTE: %s", st ? "true" : "false");
}

void MediaNotify::notifyVolume(int val)
{
    GEN_Printf(LOG_DEBUG, "volume value: %d", val);
}

#ifndef USE_MPV_API_INTERFACE
void MediaNotify::notify(int msg, int ext1, int ext2, const char *str)
{
    IPCDataPackage in;

    GEN_Printf(LOG_INFO, "notify: %d, %d, %d, %s", msg, ext1, ext2, str);
    /* 添加此函数的命令标志 */
    in.setCmdType(msg + ipc::SERVICE_NOTIFY_Method_Base);

    /* 添加参数 */
    in.allocSetInt(ext1);
    in.allocSetInt(ext2);
    in.allocSetString(str);

    m_pHandler->msgNotify(&in);
}
#endif
