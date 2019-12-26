#ifndef MEDIA_IFACE_H
#define MEDIA_IFACE_H

class MediaServiceIFace
{    
public:
    static MediaServiceIFace *instance()
    {
        static MediaServiceIFace i;
        return &i;
    }

    void start();
    void play();
    void stop();
    void pause();
    void reset();
    void playPause();
    void setFile(const char *file);

    bool isPlaying();
    void seekTo(long msec, int mode);
    int  getCurrentPosition();
    int  getDuration();
};

#endif // MEDIA_IFACE_H
