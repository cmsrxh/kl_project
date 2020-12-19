#include <events/common_log.h>
#include "mpv_player_proc.h"
#include "application.h"
#include "fsm/sf_state_mache.h"
#include "media_service_proxy.h"
#include "media_service_cb.h"
#include "media_notify.h"

static MediaServiceCB _gMediaServiceInstance;

MediaNotify::MediaNotify()
{
    MediaServiceProxy::instace()->setCallback(&_gMediaServiceInstance);
}

void MediaNotify::init_notify()
{    

}

void MediaNotify::notifySeekEnd()
{
    MediaServiceProxy::instace()->mediaNotify(MEDIA_SEEK_COMPLETE);
}

void MediaNotify::notifyPlayError(int err, int sub, const char *str)
{
    MediaServiceProxy::instace()->mediaNotify(MEDIA_ERROR, err, sub, str);
}

void MediaNotify::notifyPlayComplete()
{
    MediaServiceProxy::instace()->mediaNotify(MEDIA_PLAYBACK_COMPLETE);
}

void MediaNotify::notifyPrepare()
{
    MediaServiceProxy::instace()->mediaNotify(MEDIA_PREPARED);
}

void MediaNotify::notifyPlayStart(const char *file)
{
    MediaServiceProxy::instace()->mediaNotify(MEDIA_STARTED);
    UNUSED(file);
}

void MediaNotify::notifyPlayerIdle()
{
    MediaServiceProxy::instace()->mediaNotify(MEDIA_STOPPED);
}

void MediaNotify::notifyPlayerPlaying()
{
    MediaServiceProxy::instace()->mediaNotify(MEDIA_STARTED);
}

void MediaNotify::notifyPlayerPause()
{
    MediaServiceProxy::instace()->mediaNotify(MEDIA_PAUSED);
}

void MediaNotify::notifyDuration(int dur)
{
    UNUSED(dur);
    MediaServiceProxy::instace()->mediaNotify(MEDIA_NOTIFY_TIME, dur);
}

void MediaNotify::notifySlider(int cur, int dur)
{
    UNUSED(cur);
    UNUSED(dur);
}

void MediaNotify::notifyCacheDuration(int dur)
{
    GEN_Printf(LOG_WARN, "NOT USED, Cache Duration: %d", dur);
}

void MediaNotify::notifyCacheTime(int cur)
{
//    GEN_Printf(LOG_DEBUG, "Cache time: %d", cur);
    MediaServiceProxy::instace()->mediaNotify(MEDIA_CACHE_TIME, cur);
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
    MediaServiceProxy::instace()->mediaNotify(MEDIA_SUBTITLE_DATA, 0, 0, sub);
}

void MediaNotify::notifyMute(bool st)
{
    GEN_Printf(LOG_DEBUG, "MUTE: %s", st ? "true" : "false");
}

void MediaNotify::notifyVolume(int val)
{
    GEN_Printf(LOG_DEBUG, "volume value: %d", val);
}

