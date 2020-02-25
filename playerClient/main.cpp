#include "events/common_log.h"
#include "ipc/client_iface_base.h"
#include "ipc/ipc_sock_common.h"
#include "ipc/ipc_data_package.h"
#include "media_iface_common.h"
#include "media_service_i_face.h"
#include "media_service_call_back.h"
#include "util/data_packet.h"

class MsgProc : public MediaServiceCallback
{
public:
    void msgLoopExit()
    {
        GEN_Printf(LOG_ERROR, "msg loop exit, need restart");
    }

    void mediaNotify(int msg, int ext1, int ext2, const char *str)
    {
        GEN_Printf(LOG_DUMP, "msg: %d (%d, %d), %s", msg, ext1, ext2, str);
    }
};

int main(int , char *[])
{
#if 0
    IPCDataPackage in;

    in.setCmdType(1 + ipc::SERVICE_NOTIFY_Method_Base);
    in.allocSetInt(0);
    in.allocSetInt(0);
    in.allocSetString(NULL);
    in.genBuffer();

    Data::Unpack un(in.packet());
    GEN_Printf(LOG_INFO, "CMD: %d, %d, %d, %d", in.getCmd() - ipc::SERVICE_NOTIFY_Method_Base,
               in.getHeadDataSize(), in.bufferLen(), in.dataLen());

    GEN_Printf(LOG_INFO, "CMD: %d", in.getCmd());
    GEN_Printf(LOG_INFO, "%d", in.dataGetInt(un));
    GEN_Printf(LOG_INFO, "%d", in.dataGetInt(un));
    GEN_Printf(LOG_INFO, "%s", in.dataGetString(un));

    in.packet().showBuffer();
    // Data::testDataProc();

    return 0;
#endif

    MediaServiceIFace::instance()->initClientIface(new MsgProc);

    char cmd[256];

    while (1)
    {
        scanf("%s", cmd);
        if (0 == strcmp(cmd, "attch"))
        {

        } else if (0 == strcmp(cmd, "start"))
        {
            MediaServiceIFace::instance()->start();
        } else if (0 == strcmp(cmd, "play"))
        {
            MediaServiceIFace::instance()->play();
        }  else if (0 == strcmp(cmd, "stop"))
        {
            MediaServiceIFace::instance()->stop();
        }  else if (0 == strcmp(cmd, "pause"))
        {
            MediaServiceIFace::instance()->pause();
        }  else if (0 == strcmp(cmd, "reset"))
        {
            MediaServiceIFace::instance()->reset();
        }  else if (0 == strcmp(cmd, "playPause"))
        {
            MediaServiceIFace::instance()->playPause();
        }  else if (0 == strcmp(cmd, "setFile"))
        {
//            MediaServiceIFace::instance()->setFile("/mnt/hgfs/shareToUbuntu/videos/MP4/1-1080p-MP4.MP4");
            MediaServiceIFace::instance()->setFile("http://stream.iqilu.com/vod_bag_2016//2020/02/16/903BE158056C44fcA9524B118A5BF230/903BE158056C44fcA9524B118A5BF230_H264_mp4_500K.mp4");

        }  else if (0 == strcmp(cmd, "isPlaying"))
        {
            GEN_Printf(LOG_DEBUG, "is Playing: %s", MediaServiceIFace::instance()->isPlaying() ? "true" : "false" );
        }  else if (0 == strcmp(cmd, "seekTo"))
        {
            MediaServiceIFace::instance()->seekTo(5, 0);
        }  else if (0 == strcmp(cmd, "cur"))
        {
            GEN_Printf(LOG_DEBUG, "current postion: %d" , MediaServiceIFace::instance()->getCurrentPosition());
        }  else if (0 == strcmp(cmd, "dur"))
        {
            GEN_Printf(LOG_DEBUG, "media duration: %d" , MediaServiceIFace::instance()->getDuration());
        } else
        {
            GEN_Printf(LOG_DEBUG, "[%s] Unkown CMD.", cmd);
        }

    }


    return 0;
}
