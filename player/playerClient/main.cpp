#include "events/common_log.h"
#include "events/sf_loop.h"
#include "ipc/client_iface_base.h"
#include "ipc/ipc_sock_common.h"
#include "ipc/ipc_data_package.h"
#include "media_iface_common.h"
#include "media_client_proxy.h"
#include "media_service_call_back.h"
#include "util/data_packet.h"

class MsgProc : public MediaServiceCallback
{
public:
    void mediaNotify(int msg, int ext1, int ext2, const char *str)
    {
        GEN_Printf(LOG_DUMP, "msg: %d (%d, %d), %s", msg, ext1, ext2, str);
    }
};

int main(int argc, char *argv[])
{
    SFLoop::init_all(INIT_NEED_NOSIGNAL, "default", "/etc/zlog.conf");
    if (argc != 2)
    {
        GEN_Printf(LOG_WARN, "Usage: %s <media file>", argv[0]);
        return -1;
    }

    MediaClientProxy *instace = new MediaClientProxy;

    const char *mediaFile = argv[1];

    instace->setCallback(new MsgProc);
    instace->registerNotifyMsg();

    char cmd[256];

    while (1)
    {
        scanf("%s", cmd);
        if (0 == strcmp(cmd, "attch"))
        {

        } else if (0 == strcmp(cmd, "start"))
        {
            instace->start(0);
        } else if (0 == strcmp(cmd, "play"))
        {
            instace->play();
        }  else if (0 == strcmp(cmd, "stop"))
        {
            instace->stop();
        }  else if (0 == strcmp(cmd, "pause"))
        {
            instace->pause();
        }  else if (0 == strcmp(cmd, "reset"))
        {
            instace->reset();
        }  else if (0 == strcmp(cmd, "playPause"))
        {
            instace->playPause();
        }  else if (0 == strcmp(cmd, "setFile"))
        {
            instace->setFile(mediaFile);
        }  else if (0 == strcmp(cmd, "state"))
        {
            GEN_Printf(LOG_DEBUG, "Playing State: %d", instace->getPlayState());
        }  else if (0 == strcmp(cmd, "seekTo"))
        {
            GEN_Printf(LOG_DEBUG, "Please input seek pos: ");
            scanf("%s", cmd);
            int sp = atoi(cmd);
            instace->seekTo(sp, 0);
        }  else if (0 == strcmp(cmd, "cur"))
        {
            GEN_Printf(LOG_DEBUG, "current postion: %d" , instace->getCurrentPosition());
        }  else if (0 == strcmp(cmd, "dur"))
        {
            GEN_Printf(LOG_DEBUG, "media duration: %d" , instace->getDuration());
        }  else if (0 == strcmp(cmd, "speed"))
        {
            GEN_Printf(LOG_DEBUG, "playback speed: %g" , instace->getPlaybackSpeed());
            GEN_Printf(LOG_DEBUG, "Set Playback Speed: ");
            scanf("%s", cmd);

            double sp = atof(cmd);

            instace->setPlaybackSpeed(sp);
        } else
        {
            GEN_Printf(LOG_DEBUG, "[%s] Unkown CMD.", cmd);
        }

    }

    return 0;
}
