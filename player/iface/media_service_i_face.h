#ifndef MEDIA_IFACE_H
#define MEDIA_IFACE_H

class MediaServiceCallback;
class ClientIfaceBase;
class MediaServiceIFace
{    
public:
    static MediaServiceIFace *instance()
    {
        static MediaServiceIFace i;
        return &i;
    }

    bool initClientIface(MediaServiceCallback *);

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

private:
    MediaServiceIFace();
    ClientIfaceBase *m_pHandler;
};

#endif // MEDIA_IFACE_H
