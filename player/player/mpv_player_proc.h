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
    void checkSurface(int64_t wid);
    void onMpvEvents();
    void reCreatePlayer();

    void setHaveVideo(bool has);

    void setFile(const char *file);
    void playSwitch();
    bool isIdle();
    void seek(int i);

    inline void selectSubtileTrack(int i)
    {
        mCmd->selectSubtileTrack(i);
    }

    inline void start(int start_pos = 0)
    {
        mCmd->playFile(fileName, start_pos);
    }   

    int getPlayState()
    {
        return playerState;
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

    void setSubtitle(bool st)
    {
        is_need_subtitle = st;
    }

    void setPlaybackSpeed(double speed)
    {
        mCmd->setPlaybackSpeed(speed);
    }
    double getPlaybackSpeed()
    {
        return mCmd->getPlaybackSpeed();
    }

private:
    MPVPlayerProc();
    MPVPlayerProc(MPVPlayerProc &);
    MPVPlayerProc &operator=(MPVPlayerProc &);

    void priserEndFile(void *ptr);
    mpv_handle          *mpv;
    MPVCommand          *mCmd;

    int  playerState;
    bool is_end_file;
    bool is_need_subtitle;
    bool is_seek_end;

    char *fileName;
};


#endif // PLAYER_PROC_H
