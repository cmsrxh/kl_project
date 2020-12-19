#include <events/common_log.h>
#include "mpv_player_proc.h"
#include "application.h"
#include "fsm/sf_state_mache.h"
#include "media_service_cb.h"

MediaServiceCB::MediaServiceCB()
{

}

void MediaServiceCB::start(int start_pos)
{
    stateMache->postEvent(state::STATE_EVENT_START, start_pos);
}

void MediaServiceCB::play()
{
    stateMache->postEvent(state::STATE_EVENT_PLAY);
}

void MediaServiceCB::stop()
{
    stateMache->postEvent(state::STATE_EVENT_STOP);
}

void MediaServiceCB::pause()
{
    stateMache->postEvent(state::STATE_EVENT_PAUSE);
}

void MediaServiceCB::reset()
{
    stateMache->postEvent(state::STATE_EVENT_RESET);
}

void MediaServiceCB::playPause()
{
    stateMache->postEvent(state::STATE_EVENT_PLAY_PAUSE);
}

void MediaServiceCB::setFile(const char *file)
{
    stateMache->postEvent(state::STATE_EVENT_SET_FILE, 0, 0, file);
}

void MediaServiceCB::setSubtitle(bool ishave)
{
    MPVPlayerProc::instance()->setSubtitle(ishave);
}

void MediaServiceCB::seekTo(long sec, int mode)
{
    stateMache->postEvent(state::STATE_EVENT_SEEK_TO, sec, mode);
}

int MediaServiceCB::getPlayState()
{
    return (stateMache->checkState(state::STATE_PLAYING))
            ? MPVPlayerProc::instance()->getPlayState()
            : 0;
}

int MediaServiceCB::getCurrentPosition()
{
   return (stateMache->checkState(state::STATE_PLAYING))
           ? MPVPlayerProc::instance()->getCurrentPos()
           : 0;
}

int MediaServiceCB::getDuration()
{
    return (stateMache->checkState(state::STATE_PLAYING))
            ? MPVPlayerProc::instance()->getDuration()
            : 0;
}

void MediaServiceCB::setPlaybackSpeed(double speed)
{
    if (stateMache->checkState(state::STATE_PLAYING))
    {
        MPVPlayerProc::instance()->setPlaybackSpeed(speed);
    } else
    {
        GEN_Printf(LOG_WARN, "Not in playing.");
    }
}

double MediaServiceCB::getPlaybackSpeed()
{
    return (stateMache->checkState(state::STATE_PLAYING))
            ? MPVPlayerProc::instance()->getPlaybackSpeed()
            : 1.0;
}
