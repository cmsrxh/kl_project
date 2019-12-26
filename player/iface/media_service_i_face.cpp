#include <events/common_log.h>
#include "mpv_player_proc.h"
#include "application.h"
#include "fsm/sf_state_mache.h"
#include "media_service_i_face.h"

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



