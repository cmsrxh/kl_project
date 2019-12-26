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
    , currentPos(0), duration(0)
    , playerState(PLAYER_IDLE)
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
    check_error(mpv_set_option_string(mpv, "msg-level", "vd=v")); // "all=trace"

//    check_error(mpv_set_option_string(mpv, "hwdec", "auto"));

    check_error(mpv_set_option_string(mpv, "vo", "null"));
//    check_error(mpv_set_option_string(mpv, "gpu-context", "android"));
    check_error(mpv_set_option_string(mpv, "ao", "opensles"));

    // Done setting up options.
    check_error(mpv_initialize(mpv));

    MpvPropertyEvent::registerProperty(mpv);

    mpv_set_wakeup_callback(mpv, on_mpv_events, nullptr);

//    mpv_request_event(mpv, MPV_EVENT_ERROR, 1);
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
    GEN_Printf(LOG_DEBUG, "set surface %ld", wid);

    if (wid)
    {
        mCmd->switchTrack(-1);
        mpv_set_option(mpv, "wid", MPV_FORMAT_INT64, (void*) &wid);
    } else
    {
        mCmd->switchTrack(-2);
    }
}

void MPVPlayerProc::setFile(const char *file)
{
    if(fileName)
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
    return playerState == PLAYER_IDLE;
}

int MPVPlayerProc::getCurrentPos() const
{
    int64_t tmp = mCmd->getTimePos();
    // GEN_Printf(LOG_DEBUG, "current: %ld", tmp);

    return tmp;
}

int MPVPlayerProc::getDuration() const
{
    int64_t tmp = mCmd->getDuration();

    GEN_Printf(LOG_DEBUG, "duration: %ld", tmp);
    return tmp;
}

void MPVPlayerProc::onMpvEvents()
{
    bool  is_end_file = false;

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
            currentPos = 0;
            duration   = 0;
            playerState = PLAYER_IDLE;
            GEN_Printf(LOG_DEBUG, "event: %s, is_end_file: %d\n", mpv_event_name(event->event_id), is_end_file);
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
            stateMache->postEvent(state::STATE_EVENT_ENTER_PLAYING, 0, 0, fileName);
            break;

        case MPV_EVENT_END_FILE: {//event: 7,  end-file            
            if (event->data)
            {
                priserEndFile(event->data);
            }
            break;
        }

        case MPV_EVENT_SEEK:
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

//        case MPV_EVENT_ERROR:
//            GEN_Printf(LOG_DEBUG, "%s msg is tigger.", mpv_event_name(event->event_id));
//            if (event->data)
//            {
//                MediaNotify::instance()->notifyPlayError(ERROR_PTS_INVLAID, 0, "Time schedule anomaly");
//            }
//            break;

//        case MPV_EVENT_SUBTITLE:
//            if (event->data)
//            {
//                MediaNotify::instance()->notifySubtitle((char *)event->data);
//            }
//            break;

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

