#include <events/common_log.h>
#include <mpv/client.h>
#include <stdexcept>
#include "app_common.h"
#include "application.h"
#include "fsm/sf_state_mache.h"
#include "iface/media_notify.h"
#include "mpv_property_event.h"
#include "mpv_player_proc.h"

#define check_error(op) \
    if(UNLIKELY((status = op) < 0)){ \
        GEN_Printf(LOG_ERROR, "mpv API error: %s", mpv_error_string(status)); \
        throw std::runtime_error("mpv init failed !"); \
    }

void on_mpv_events(void *ctx)
{
    UNUSED(ctx);
    Application::instance()->postCmd(SIG_ON_MPV_EVENT);
}

MPVPlayerProc::MPVPlayerProc()
    : mpv(nullptr)
    , mCmd(new MPVCommand)
    , playerState(PLAYER_IDLE)
    , is_end_file(false), is_need_subtitle(false)
    , is_seek_end(true)
    , fileName(NULL)
{    
    reCreatePlayer();
}

MPVPlayerProc::~MPVPlayerProc()
{
    mCmd->stop();
    mCmd->quit();

    delete mCmd;

    mpv_terminate_destroy(mpv);
}

void MPVPlayerProc::reCreatePlayer()
{
    int status;

    if (mpv)
    {
        mpv_destroy(mpv);
    }

    mpv = mpv_create();
    if (!mpv)
    {
        GEN_Printf(LOG_ERROR, "failed creating context.");
        throw std::runtime_error("could not create mpv context");
    }

    mCmd->setMpvHandle(mpv);

    check_error(mpv_set_option_string(mpv, "terminal", "yes"));
    check_error(mpv_set_option_string(mpv, "msg-level", "vd=v,ao=v")); // "all=trace" vd=v,lavf=trace

    // 禁止缓存，使得播放视频时，内存不会大幅增加
    check_error(mpv_set_option_string(mpv, "cache", "auto"));
    int64_t max_bytes = 50 * 1024 * 1024;
    int64_t max_bytes_bw = 10 * 1024 *1024;
    check_error(mpv_set_option(mpv, "demuxer-max-bytes", MPV_FORMAT_INT64, &max_bytes));
    check_error(mpv_set_option(mpv, "demuxer-max-back-bytes", MPV_FORMAT_INT64, &max_bytes_bw));
    // check_error(mpv_set_option_string(mpv, "demuxer-max-bytes",  "40 mib"));
    // check_error(mpv_set_option_string(mpv, "demuxer-max-back-bytes", "10 mib"));

    // 强制设置视频宽高比为1, 禁止解码拉伸
    float aspect = 0;
    check_error(mpv_set_option(mpv, "video-aspect", MPV_FORMAT_DOUBLE, &aspect));

    // 显示buffer是否自己申请
    // 发现直接用户层解码到硬件buffer上的时候，系统效率，极度缓慢。
    // int vd_lavc_dr = 1;  // vd_lavc.c
    // check_error(mpv_set_option(mpv, "vd-lavc-dr", MPV_FORMAT_INT64, &vd_lavc_dr));

    // check_error(mpv_set_option_string(mpv, "hwdec", "auto"));
    // int64_t ringBufferSize = 0;
    // 表示读取本地文件，stream禁止使用ringbuffer -- INVALID
    // check_error(mpv_set_option(mpv, "stream-buffer-size", MPV_FORMAT_INT64, &ringBufferSize));

    // check_error(mpv_set_option_string(mpv, "vo", "null"));
#if 0  // Android
    check_error(mpv_set_option_string(mpv, "vo", "gpu"));
    check_error(mpv_set_option_string(mpv, "gpu-context", "android"));
    check_error(mpv_set_option_string(mpv, "ao", "opensles"));
#endif

#ifdef PLATFORM_PC
//    check_error(mpv_set_option_string(mpv, "vo", "gpu"));
//    check_error(mpv_set_option_string(mpv, "video", "no"));
    check_error(mpv_set_option_string(mpv, "ao", "alsa"));
#endif

#ifdef PLATFORM_ARM
    check_error(mpv_set_option_string(mpv, "vo", "allwinner"));

    // 设置alsa设备的设备的名字
    check_error(mpv_set_option_string(mpv, "audio-device", "softvol-media"));
    check_error(mpv_set_option_string(mpv, "ao", "alsa"));

    // 把声音播放链路关闭
    // check_error(mpv_set_option_string(mpv, "audio", "no"));
#endif
    // Done setting up options.
    check_error(mpv_initialize(mpv));

    MpvPropertyEvent::registerProperty(mpv);

    mpv_set_wakeup_callback(mpv, on_mpv_events, nullptr);

    // mpv_request_event(mpv, MPV_EVENT_ERROR, 1);
}

void MPVPlayerProc::setHaveVideo(bool has)
{
    int ret = mpv_set_option_string(mpv, "video", has ? "yes" : "no");
    if(ret < 0)
    {
        GEN_Printf(LOG_ERROR, "mpv API error: %s", mpv_error_string(ret));
    }
}



void MPVPlayerProc::setSurface(int64_t wid)
{
    bool enable = mCmd->getVideoTrackEnable();

    GEN_Printf(LOG_DEBUG, "set surface %ld, video track is enable: %d", wid, enable);

    if (wid)
    {
        if (!enable)
        {
            mCmd->enableVideoTrack(true);
        }
        mpv_set_option(mpv, "wid", MPV_FORMAT_INT64, (void*) &wid);
    } else if (enable)
    {
        mCmd->enableVideoTrack(false);
    }
}
/**
 * @brief MPVPlayerProc::checkSurface
 * @param wid [in] surface句柄
 * @note 核对当前视频播放轨道是否使能了，如果当前有surface，就使能一下视频轨道
 */
void MPVPlayerProc::checkSurface(int64_t wid)
{
    bool enable = mCmd->getVideoTrackEnable();

    GEN_Printf (LOG_DEBUG, "Video Track is enable: %d", enable);
    if (wid && !enable)
    {
        mCmd->enableVideoTrack(true);
    }
    else if (!wid && enable)
    {
        mCmd->enableVideoTrack(false);
    }
}

void MPVPlayerProc::setFile(const char *file)
{
    if(fileName && fileName != file)
    {
        free(fileName);
    }
    fileName = (char *)file;
}

void MPVPlayerProc::playSwitch()
{
    if(mCmd->isPause())
    {
        mCmd->play();
    }else
    {
        mCmd->pause();
    }    
}

bool MPVPlayerProc::isIdle()
{
    return playerState == PLAYER_IDLE || playerState == PLAYER_STOP;
}

void MPVPlayerProc::seek(int seek_pos)
{
    if (is_seek_end)
    {
        int currentPos = mCmd->getTimePos();

        mCmd->seek(seek_pos - currentPos);
        is_seek_end = false;
    } else
    {
        GEN_Printf(LOG_WARN, "Current is Seeking.");
    }
}

int MPVPlayerProc::getCurrentPos() const
{
    int64_t tmp = mCmd->getTimePos();
    //GEN_Printf(LOG_DEBUG, "current: %ld", tmp);
    return tmp;
}

int MPVPlayerProc::getDuration() const
{
    int64_t tmp = mCmd->getDuration();

    //GEN_Printf(LOG_DEBUG, "duration: %ld", tmp);
    return tmp;
}

void MPVPlayerProc::onMpvEvents()
{    
    while (mpv)
    {
        mpv_event *event = mpv_wait_event(mpv, 0);

        switch (event->event_id) {
        case MPV_EVENT_PROPERTY_CHANGE:
        {
            GEN_Printf(LOG_DEBUG, "event: %s", mpv_event_name(event->event_id));
            MpvPropertyEvent a((mpv_event_property *)event->data);
            break;
        }

        case MPV_EVENT_IDLE:
            is_seek_end = true;
            if (is_end_file)
            {
                is_end_file = false;
            } else
            {
                playerState = PLAYER_IDLE;
            }
            GEN_Printf(LOG_DEBUG, "event: %s\n", mpv_event_name(event->event_id));
            stateMache->postEvent(state::STATE_EVENT_PLAYING_IDLE);
            break;

        case MPV_EVENT_PAUSE:
            GEN_Printf(LOG_DEBUG, "event: %s\n", mpv_event_name(event->event_id));
            playerState = PLAYER_PAUSE;
            MediaNotify::instance()->notifyPlayerPause();
            break;

        case MPV_EVENT_UNPAUSE:
            GEN_Printf(LOG_DEBUG, "event: %s\n", mpv_event_name(event->event_id));
            playerState = PLAYER_PLAY;
            MediaNotify::instance()->notifyPlayerPlaying();
            break;

        case MPV_EVENT_PLAYBACK_RESTART:
            GEN_Printf(LOG_DEBUG, "event: %s\n", mpv_event_name(event->event_id));
            playerState = PLAYER_PLAY;
            is_seek_end = true;
            stateMache->postEvent(state::STATE_EVENT_ENTER_PLAYING, 0, 0, fileName);
            MediaNotify::instance()->notifyPlayStart(fileName);
            break;

        case MPV_EVENT_END_FILE: {//event: 7,  end-file            
            is_end_file = true;
            playerState = PLAYER_STOP;
            if (event->data)
            {
                priserEndFile(event->data);
            }
            break;
        }

        case MPV_EVENT_SEEK:
            is_seek_end = true;
            MediaNotify::instance()->notifySeekEnd();
            break;

        case MPV_EVENT_SHUTDOWN:
            stateMache->exitState();
            Application::instance()->postCmd(SIG_MVP_NEED_RECREARE);
            MediaNotify::instance()->notifyPlayError(ERROR_PLAYER_SHUTDOWN, 0, "Inner Player occur failed !");

            GEN_Printf(LOG_DEBUG, "SHUTDOWN: thread exit.");
            return;

        case MPV_EVENT_NONE:
            return;

        case MPV_EVENT_ERROR:
            GEN_Printf(LOG_DEBUG, "%s msg is tigger.", mpv_event_name(event->event_id));
            if (event->data)
            {
                MediaNotify::instance()->notifyPlayError(ERROR_PTS_INVLAID, 0, "Time schedule anomaly");
            }
            break;

        case MPV_EVENT_SUBTITLE:
            if (event->data && is_need_subtitle)
            {
                MediaNotify::instance()->notifySubtitle((char *)event->data);
            }
            break;

        default: ;
            // GEN_Printf(LOG_DEBUG, "event: %d,  %s\n", event->event_id, mpv_event_name(event->event_id));
            // Ignore uninteresting or unknown events.
        }        
    }

    return;
}

void MPVPlayerProc::priserEndFile(void *ptr)
{
    mpv_event_end_file *reason = (mpv_event_end_file *)ptr;
    GEN_Printf(LOG_INFO, "Player End Reason: %d, error: %d", reason->reason, reason->error);
    switch (reason->reason)
    {
    case MPV_END_FILE_REASON_EOF:
        MediaNotify::instance()->notifyPlayComplete();
        break;

    case MPV_END_FILE_REASON_STOP:
        Application::instance()->postCmd(SIG_MPV_PLAY_STOP);
        break;

    case MPV_END_FILE_REASON_QUIT:
        break;

    case MPV_END_FILE_REASON_ERROR:
        switch (reason->error)
        {
        case MPV_ERROR_UNKNOWN_FORMAT:
            MediaNotify::instance()->notifyPlayError(ERROR_LOADING_FAILED, reason->error, "Format not supported.");
            break;
        case MPV_ERROR_LOADING_FAILED:
            MediaNotify::instance()->notifyPlayError(ERROR_LOADING_FAILED, reason->error, "File load failed.");
            break;
        case MPV_ERROR_NOTHING_TO_PLAY:
            MediaNotify::instance()->notifyPlayError(ERROR_NOTHING_TO_PLAY, reason->error, "Nothing to play.");
            break;

        case MPV_ERROR_EVENT_QUEUE_FULL:
        case MPV_ERROR_NOMEM:
        case MPV_ERROR_UNINITIALIZED:        
            MediaNotify::instance()->notifyPlayError(ERROR_PLAYER_SYSTEM, reason->error, "Internal error in playback system.");
            break;
        case MPV_ERROR_AO_INIT_FAILED:
            MediaNotify::instance()->notifyPlayError(ERROR_AO_INIT_FAILED, reason->error, "Noise equipment initialization error.");
            break;
        case MPV_ERROR_VO_INIT_FAILED:
            MediaNotify::instance()->notifyPlayError(ERROR_VO_INIT_FAILED, reason->error, "Video device initialization error.");
            break;
        case MPV_ERROR_INVALID_PARAMETER:
        case MPV_ERROR_OPTION_NOT_FOUND:
        case MPV_ERROR_OPTION_FORMAT:
        case MPV_ERROR_OPTION_ERROR:
        case MPV_ERROR_PROPERTY_NOT_FOUND:
        case MPV_ERROR_PROPERTY_FORMAT:
        case MPV_ERROR_PROPERTY_UNAVAILABLE:
        case MPV_ERROR_PROPERTY_ERROR:
        case MPV_ERROR_COMMAND:
        case MPV_ERROR_UNSUPPORTED:
        case MPV_ERROR_NOT_IMPLEMENTED:
        case MPV_ERROR_GENERIC:
            MediaNotify::instance()->notifyPlayError(ERROR_GENERIC, reason->error, "Error generic.");
            break;
        default:
            break;
        }
        break;

    case MPV_END_FILE_REASON_REDIRECT:
        break;

    default:
        break;
    }
}

