#ifndef MEDIA_SERVICE_CALLBACK_H
#define MEDIA_SERVICE_CALLBACK_H

#include <media_client_call_back.h>

class MediaServiceCB : public MediaClientCallback
{
public:
    MediaServiceCB();

    void start(int start_pos);
    void play();
    void stop();
    void pause();
    void reset();
    void playPause();
    void setFile(const char *file);
    void setSubtitle(bool ishave);
    void seekTo(long sec, int mode);

    int  getPlayState();
    int  getCurrentPosition();
    int  getDuration();

    void setPlaybackSpeed(double speed);
    double getPlaybackSpeed();
};

#endif // MEDIA_SERVICE_CALLBACK_H
