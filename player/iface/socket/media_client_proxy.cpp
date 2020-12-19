#include <signal.h>
#include <events/common_log.h>
#include "media_iface_ipc_socket_priv.h"
#include "ipc/client_iface_base.h"
#include "ipc/ipc_data_package.h"
#include "media_client_proxy.h"

static MediaServiceCallback  _default_callback;
static MediaServiceCallback *_g_callback = &_default_callback;

class MediaClientMsgPriser : public ClientMsgPriserProc
{
public:
    void msgPriser(IPCDataPackage *pack)
    {
        // GEN_Printf(LOG_INFO, "CMD: %d, %d, %d, %d", pack->getCmd(), pack->getHeadDataSize(), pack->bufferLen(), pack->dataLen());

        int cmd = pack->getCmd() - ipc::SERVICE_NOTIFY_Method_Base;
        if (_g_callback)
        {
            Data::Unpack un(pack->packet());
            // pack->packet().showBuffer();
            int ext1    = pack->dataGetInt(un);
            int ext2    = pack->dataGetInt(un);
            char * str  = pack->dataGetString(un);


//            GEN_Printf(LOG_INFO, "%d", ext1);
//            GEN_Printf(LOG_INFO, "%d", ext2);
//            GEN_Printf(LOG_INFO, "%s", str);

            _g_callback->mediaNotify(cmd, ext1, ext2, str);
        }
    }
};

static MediaClientMsgPriser gMsgInstance;
static ClientIfaceBase *m_pHandler;
MediaClientProxy::MediaClientProxy()
{
    //解决socket，一方关闭，另一方连续send后，发出的管道破裂的信息，而导致系统死亡。
    signal(SIGPIPE, SIG_IGN);
    if (!m_pHandler)
    {
        m_pHandler = new ClientIfaceBase;
        m_pHandler->setMsgPriser(MEDIA_SERVICE_PORT, &gMsgInstance);
    }
}

void MediaClientProxy::registerNotifyMsg()
{
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
        MEDIA_PLAYING           + ipc::SERVICE_NOTIFY_Method_Base,
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
    GEN_Printf(LOG_DEBUG, "Regist Media Message Notify.");

    m_pHandler->registMessage(me, ARRAY_SIZE(me));
}

void MediaClientProxy::setCallback(MediaServiceCallback *value)
{
    _g_callback = value;
}

static bool _common_cmd(ClientIfaceBase *handler, int cmd)
{
    IPCDataPackage in;
    IPCDataPackage out;

    in.setCmdType(cmd);

    handler->cmdCall(&in, &out);

    //TODO: 'out' process
    if (out.getCmd() == ipc::SERVICE_REPLY_OK)
    {
        //GEN_Printf(LOG_DEBUG, "Set common cmd OK !!!");
        return true;
    } else
    {
        GEN_Printf(LOG_ERROR, "Set common cmd failed: %d !!!", out.getCmd());
        return false;
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

bool MediaClientProxy::start(int start_pos)
{
    IPCDataPackage in;
    IPCDataPackage out;

    in.allocSetInt(start_pos);
    return _have_arg(m_pHandler, CLIENT_CALL_Start, &in, &out);
}

bool MediaClientProxy::play()
{
    return _common_cmd(m_pHandler, CLIENT_CALL_Play);
}

bool MediaClientProxy::stop()
{
    return _common_cmd(m_pHandler, CLIENT_CALL_Stop);
}

bool MediaClientProxy::pause()
{
    return _common_cmd(m_pHandler, CLIENT_CALL_Pause);
}

bool MediaClientProxy::reset()
{
    return _common_cmd(m_pHandler, CLIENT_CALL_Reset);
}

bool MediaClientProxy::playPause()
{
    return  _common_cmd(m_pHandler, CLIENT_CALL_PlayPause);
}

bool MediaClientProxy::setFile(const char *file)
{
    IPCDataPackage in;
    IPCDataPackage out;

    in.allocSetString(file);
    return _have_arg(m_pHandler, CLIENT_CALL_SetFile, &in, &out);
}

bool MediaClientProxy::setSubtitle(bool ishave)
{
    IPCDataPackage in;
    IPCDataPackage out;

    in.allocSetInt(ishave);
    return _have_arg(m_pHandler, CLIENT_CALL_SetFile, &in, &out);
}

bool MediaClientProxy::seekTo(long sec, int mode)
{
    IPCDataPackage in;
    IPCDataPackage out;

    in.allocSetInt(sec);
    in.allocSetInt(mode);
    return _have_arg(m_pHandler, CLIENT_CALL_SeekTo, &in, &out);
}

int MediaClientProxy::getPlayState()
{
    IPCDataPackage in;
    IPCDataPackage out;

    if (_have_arg(m_pHandler, CLIENT_CALL_getPlayState, &in, &out))
    {
        Data::Unpack un(out.packet());
        return out.dataGetInt(un);
    } else
    {
        return 0;
    }
}

int MediaClientProxy::getCurrentPosition()
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

int MediaClientProxy::getDuration()
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

bool MediaClientProxy::setPlaybackSpeed(double speed)
{
    IPCDataPackage in;
    IPCDataPackage out;

    in.allocSetFloat(speed);
    return _have_arg(m_pHandler, CLIENT_CALL_setPlaybackSpeed, &in, &out);
}

double MediaClientProxy::getPlaybackSpeed()
{
    IPCDataPackage in;
    IPCDataPackage out;

    if (_have_arg(m_pHandler, CLIENT_CALL_getPlaybackSpeed, &in, &out))
    {
        Data::Unpack un(out.packet());
        return out.dataGetFloat(un);
    } else
    {
        return 0.0;
    }
}

