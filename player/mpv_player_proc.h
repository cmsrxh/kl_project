#ifndef PLAYER_PROC_H
#define PLAYER_PROC_H

#include "mpv_command.h"
#include <mpv/client.h>

class MPVPlayerProc
{
public:
    inline static MPVPlayerProc* instance()
    {
        static MPVPlayerProc i;
        return &i;
    }
    ~MPVPlayerProc();

    void setSurface(int64_t wid);
    void onMpvEvents();
    void reCreatePlayer();

    void setHaveVideo(bool has);

    void setFile(const char *file);
    void playSwitch();
    bool isIdle();

    inline void selectSubtileTrack(int i)
    {
        mCmd->selectSubtileTrack(i);
    }

    inline void start()
    {
        mCmd->playFile(fileName);
    }

    inline void seek(int i)
    {
        mCmd->seek(i);
    }

    inline bool isPause()
    {
        return mCmd->isPause();
    }

    inline void pause()
    {
        mCmd->pause();
    }

    inline void play()
    {
        mCmd->play();
    }

    inline void stop()
    {
        mCmd->stop();
    }

    inline bool canSeekable() const
    {
        return mCmd->canSeek();
    }

    int  getCurrentPos() const;

    int  getDuration() const;

private:
    MPVPlayerProc();
    MPVPlayerProc(MPVPlayerProc &);
    MPVPlayerProc &operator=(MPVPlayerProc &);

    void priserEndFile(void *ptr);
    mpv_handle          *mpv;
    MPVCommand          *mCmd;

    int  currentPos;
    int  duration;
    int  playerState;

    char *fileName;
};


#endif // PLAYER_PROC_H
