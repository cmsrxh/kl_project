#include <signal.h>
#include <events/common_log.h>
#include "media_iface_ipc_socket_priv.h"
#include "ipc/ipc_data_package.h"
#include "ipc/service_iface_base.h"
#include "media_service_proxy.h"

static MediaClientCallback  _default_callback;
static MediaClientCallback *_g_callback = &_default_callback;

class MediaPriser : public ServiceCmdPriserProc
{
public:
    void cmdPriser(IPCDataPackage *in, IPCDataPackage *out)
    {
        out->setCmdType(ipc::SERVICE_REPLY_OK);

        switch (in->getCmd())
        {
        case CLIENT_CALL_Start:
        {
            Data::Unpack un(in->packet());
            int start_pos = in->dataGetInt(un);
            _g_callback->start(start_pos);
            break;
        }
        case CLIENT_CALL_Play:
            _g_callback->play();
            break;
        case CLIENT_CALL_Stop:
            _g_callback->stop();
            break;
        case CLIENT_CALL_Pause:
            _g_callback->pause();
            break;
        case CLIENT_CALL_Reset:
            _g_callback->reset();
            break;
        case CLIENT_CALL_PlayPause:
            _g_callback->playPause();
            break;
        case CLIENT_CALL_SetFile:
        {
            Data::Unpack un(in->packet());
            char *file = in->dataGetString(un);
            _g_callback->setFile(file);
            break;
        }
        case CLIENT_CALL_getPlayState:
            out->allocSetInt(_g_callback->getPlayState());
            break;
        case CLIENT_CALL_SeekTo:
        {
            Data::Unpack un(in->packet());
            int sec  = in->dataGetInt(un);
            int mode = in->dataGetInt(un);
            _g_callback->seekTo(sec, mode);
            break;
        }
        case CLIENT_CALL_Get_Current_Position:
            out->allocSetInt(_g_callback->getCurrentPosition());
            break;
        case CLIENT_CALL_Get_Duration:
            out->allocSetInt(_g_callback->getDuration());
            break;
        case CLIENT_CALL_setPlaybackSpeed:
        {
            Data::Unpack un(in->packet());
            double speed  = in->dataGetFloat(un);
            _g_callback->setPlaybackSpeed(speed);
            break;
        }
        case CLIENT_CALL_getPlaybackSpeed:
            out->allocSetFloat(_g_callback->getPlaybackSpeed());
            break;
        default:
            GEN_Printf(LOG_DEBUG, "UNKown Cmd: %d", in->getCmd());
            break;
        }
    }
};

static MediaPriser gPriserInstance;
static ServiceIfaceBase *m_pHandler = nullptr;
MediaServiceProxy::MediaServiceProxy()
{
    // 解决socket，一方关闭，另一方连续send后，发出的管道破裂的信息，而导致系统死亡。
    signal(SIGPIPE, SIG_IGN);
    if (!m_pHandler)
    {
        m_pHandler = new ServiceIfaceBase(MEDIA_SERVICE_PORT, 256);
        m_pHandler->setCmdPriser(&gPriserInstance);
        m_pHandler->init();
    }
}

void MediaServiceProxy::mediaNotify(int msg, int ext1, int ext2, const char *str)
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

void MediaServiceProxy::setCallback(MediaClientCallback *value)
{
    _g_callback = value;
}
