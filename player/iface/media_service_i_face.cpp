#include <events/common_log.h>
#include "media_service_i_face.h"
#include "media_service_call_back.h"

static MediaServiceCallback *gClientCallback = nullptr;

#ifdef USE_MPV_API_INTERFACE

#include "mpv_player_proc.h"
#include "application.h"
#include "fsm/sf_state_mache.h"

bool MediaServiceIFace::initClientIface(MediaServiceCallback *callback)
{
    gClientCallback = callback;
    return true;
}

void MediaServiceIFace::start()
{
    stateMache->postEvent(state::STATE_EVENT_START);
}

void MediaServiceIFace::play()
{
    stateMache->postEvent(state::STATE_EVENT_PLAY);
}

void MediaServiceIFace::stop()
{
    stateMache->postEvent(state::STATE_EVENT_STOP);
}

void MediaServiceIFace::pause()
{
    stateMache->postEvent(state::STATE_EVENT_PAUSE);
}

void MediaServiceIFace::reset()
{
    stateMache->postEvent(state::STATE_EVENT_RESET);
}

void MediaServiceIFace::playPause()
{
    stateMache->postEvent(state::STATE_EVENT_PLAY_PAUSE);
}

void MediaServiceIFace::setFile(const char *file)
{
    stateMache->postEvent(state::STATE_EVENT_SET_FILE, 0, 0, file);
}

bool MediaServiceIFace::isPlaying()
{
    if (stateMache->checkState(state::STATE_PLAYING))
    {
        return !MPVPlayerProc::instance()->isPause();
    } else
    {
        // GEN_Printf(LOG_WARN, "Not in playing status.");
        return false;
    }
}

void MediaServiceIFace::seekTo(long msec, int mode)
{
    stateMache->postEvent(state::STATE_EVENT_SEEK_TO, msec, mode);
}

int MediaServiceIFace::getCurrentPosition()
{
    if (stateMache->checkState(state::STATE_PLAYING))
    {
        return MPVPlayerProc::instance()->getCurrentPos();
    } else
    {
        // GEN_Printf(LOG_WARN, "Not in playing status.");
        return 0;
    }
}

int MediaServiceIFace::getDuration()
{
    if (stateMache->checkState(state::STATE_PLAYING))
    {
        return MPVPlayerProc::instance()->getDuration();
    } else
    {
        GEN_Printf(LOG_WARN, "Not in playing status.");
        return 0;
    }
}
#else
#include "media_iface_common.h"
#include "ipc/client_iface_base.h"
#include "ipc/ipc_data_package.h"

class MsgPriser : public ClientMsgPriserProc
{
public:

    void msgPriser(IPCDataPackage *pack)
    {
        GEN_Printf(LOG_INFO, "CMD: %d, %d, %d, %d", pack->getCmd(), pack->getHeadDataSize(), pack->bufferLen(), pack->dataLen());

        int cmd = pack->getCmd() - ipc::SERVICE_NOTIFY_Method_Base;
        if (gClientCallback)
        {
            Data::Unpack un(pack->packet());
            // pack->packet().showBuffer();
            int ext1    = pack->dataGetInt(un);
            int ext2    = pack->dataGetInt(un);
            char * str  = pack->dataGetString(un);


//            GEN_Printf(LOG_INFO, "%d", ext1);
//            GEN_Printf(LOG_INFO, "%d", ext2);
//            GEN_Printf(LOG_INFO, "%s", str);

            gClientCallback->mediaNotify(cmd, ext1, ext2, str);
        }
#if 0
        switch (cmd)
        {
        case MEDIA_PREPARED:
            GEN_Printf(LOG_DEBUG, "MEDIA_PREPARED");
            break;
        case MEDIA_PLAYBACK_COMPLETE:
            GEN_Printf(LOG_DEBUG, "MEDIA_PLAYBACK_COMPLETE");
            break;
        case MEDIA_BUFFERING_UPDATE:
            GEN_Printf(LOG_DEBUG, "MEDIA_BUFFERING_UPDATE");
            break;
        case MEDIA_SEEK_COMPLETE:
            GEN_Printf(LOG_DEBUG, "MEDIA_SEEK_COMPLETE");
            break;
        case MEDIA_SET_VIDEO_SIZE:
            GEN_Printf(LOG_DEBUG, "MEDIA_SET_VIDEO_SIZE");
            break;
        case MEDIA_STARTED:
            GEN_Printf(LOG_DEBUG, "MEDIA_STARTED");
            break;
        case MEDIA_PAUSED:
            GEN_Printf(LOG_DEBUG, "MEDIA_PAUSED");
            break;
        case MEDIA_STOPPED:
            GEN_Printf(LOG_DEBUG, "MEDIA_STOPPED");
            break;
        case MEDIA_SKIPPED:
            GEN_Printf(LOG_DEBUG, "MEDIA_SKIPPED");
            break;
        case MEDIA_NOTIFY_TIME:
            GEN_Printf(LOG_DEBUG, "MEDIA_NOTIFY_TIME");
            break;
        case MEDIA_TIMED_TEXT:
            GEN_Printf(LOG_DEBUG, "MEDIA_TIMED_TEXT");
            break;
        case MEDIA_ERROR:
            GEN_Printf(LOG_DEBUG, "MEDIA_ERROR");
            break;
        case MEDIA_INFO:
            GEN_Printf(LOG_DEBUG, "MEDIA_INFO");
            break;
        case MEDIA_SUBTITLE_DATA:
            GEN_Printf(LOG_DEBUG, "MEDIA_SUBTITLE_DATA");
            break;
        case MEDIA_META_DATA:
            GEN_Printf(LOG_DEBUG, "MEDIA_META_DATA");
            break;
        case MEDIA_DRM_INFO:
            GEN_Printf(LOG_DEBUG, "MEDIA_DRM_INFO");
            break;
        case MEDIA_TIME_DISCONTINUITY:
            GEN_Printf(LOG_DEBUG, "MEDIA_TIME_DISCONTINUITY");
            break;
        default:
            GEN_Printf(LOG_ERROR, "UNKown Cmd: %d", cmd);
            break;
        }
#endif
    }
};

static MsgPriser gMsgInstance;

bool MediaServiceIFace::initClientIface(MediaServiceCallback *callback)
{
    gClientCallback = callback;

    int me[] =
    {
        MEDIA_PREPARED          + ipc::SERVICE_NOTIFY_Method_Base,
        MEDIA_PLAYBACK_COMPLETE + ipc::SERVICE_NOTIFY_Method_Base,
        MEDIA_BUFFERING_UPDATE  + ipc::SERVICE_NOTIFY_Method_Base,
        MEDIA_SEEK_COMPLETE     + ipc::SERVICE_NOTIFY_Method_Base,
        MEDIA_SET_VIDEO_SIZE    + ipc::SERVICE_NOTIFY_Method_Base,
        MEDIA_STARTED           + ipc::SERVICE_NOTIFY_Method_Base,
        MEDIA_PAUSED            + ipc::SERVICE_NOTIFY_Method_Base,
        MEDIA_STOPPED           + ipc::SERVICE_NOTIFY_Method_Base,
        MEDIA_SKIPPED           + ipc::SERVICE_NOTIFY_Method_Base,
        MEDIA_CACHE_TIME        + ipc::SERVICE_NOTIFY_Method_Base,
        MEDIA_NOTIFY_TIME       + ipc::SERVICE_NOTIFY_Method_Base,
        MEDIA_TIMED_TEXT        + ipc::SERVICE_NOTIFY_Method_Base,
        MEDIA_ERROR             + ipc::SERVICE_NOTIFY_Method_Base,
        MEDIA_INFO              + ipc::SERVICE_NOTIFY_Method_Base,
        MEDIA_SUBTITLE_DATA     + ipc::SERVICE_NOTIFY_Method_Base,
        MEDIA_META_DATA         + ipc::SERVICE_NOTIFY_Method_Base,
        MEDIA_DRM_INFO          + ipc::SERVICE_NOTIFY_Method_Base,
        MEDIA_TIME_DISCONTINUITY+ ipc::SERVICE_NOTIFY_Method_Base
    };

    return m_pHandler->registMessage(me, ARRAY_SIZE(me));
}

static void _common_cmd(ClientIfaceBase *handler, int cmd)
{
    IPCDataPackage in;
    IPCDataPackage out;

    in.setCmdType(cmd);

    handler->cmdCall(&in, &out);

    //TODO: 'out' process
    if (out.getCmd() == ipc::SERVICE_REPLY_OK)
    {
        //GEN_Printf(LOG_DEBUG, "Set common cmd OK !!!");
    } else
    {
        GEN_Printf(LOG_ERROR, "Set common cmd failed: %d !!!", out.getCmd());
    }
}

static bool _have_arg(ClientIfaceBase *handler, int cmd, IPCDataPackage *in, IPCDataPackage *out)
{
    in->setCmdType(cmd);

    handler->cmdCall(in, out);

    //TODO: 'out' process
    if (out->getCmd() == ipc::SERVICE_REPLY_OK)
    {
        // GEN_Printf(LOG_DEBUG, "Set common cmd OK !!!");
        return true;
    } else
    {
        GEN_Printf(LOG_ERROR, "Set common cmd failed: %d !!!", out->getCmd());
        return false;
    }
}

void MediaServiceIFace::start()
{
    _common_cmd(m_pHandler, CLIENT_CALL_Start);
}

void MediaServiceIFace::play()
{
    _common_cmd(m_pHandler, CLIENT_CALL_Play);
}

void MediaServiceIFace::stop()
{
    _common_cmd(m_pHandler, CLIENT_CALL_Stop);
}

void MediaServiceIFace::pause()
{
    _common_cmd(m_pHandler, CLIENT_CALL_Pause);
}

void MediaServiceIFace::reset()
{
    _common_cmd(m_pHandler, CLIENT_CALL_Reset);
}

void MediaServiceIFace::playPause()
{
    _common_cmd(m_pHandler, CLIENT_CALL_PlayPause);
}

void MediaServiceIFace::setFile(const char *file)
{
    IPCDataPackage in;
    IPCDataPackage out;

    in.allocSetString(file);
    _have_arg(m_pHandler, CLIENT_CALL_SetFile, &in, &out);
}

bool MediaServiceIFace::isPlaying()
{
    IPCDataPackage in;
    IPCDataPackage out;

    if (_have_arg(m_pHandler, CLIENT_CALL_IsPlaying, &in, &out))
    {
        Data::Unpack un(out.packet());
        return out.dataGetInt(un);
    } else
    {
        return false;
    }
}

void MediaServiceIFace::seekTo(long msec, int mode)
{
    IPCDataPackage in;
    IPCDataPackage out;

    in.allocSetInt(msec);
    in.allocSetInt(mode);
    _have_arg(m_pHandler, CLIENT_CALL_SeekTo, &in, &out);
}

int MediaServiceIFace::getCurrentPosition()
{
    IPCDataPackage in;
    IPCDataPackage out;

   if (_have_arg(m_pHandler, CLIENT_CALL_Get_Current_Position, &in, &out))
   {
       Data::Unpack un(out.packet());
       return out.dataGetInt(un);
   } else
   {
       return 0;
   }
}

int MediaServiceIFace::getDuration()
{
    IPCDataPackage in;
    IPCDataPackage out;

    if (_have_arg(m_pHandler, CLIENT_CALL_Get_Duration, &in, &out))
    {
        Data::Unpack un(out.packet());
        return out.dataGetInt(un);
    } else
    {
        return 0;
    }
}

MediaServiceIFace::MediaServiceIFace()
    : m_pHandler(new ClientIfaceBase)
{
    m_pHandler->setMsgPriser(80080, &gMsgInstance);
}


#endif


