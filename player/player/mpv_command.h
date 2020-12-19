#ifndef PLAYER_COMMAND_H
#define PLAYER_COMMAND_H

typedef struct mpv_handle mpv_handle;
class MPVCommand
{
public:
    MPVCommand();

    void playFile(const char *fileName, int start_pos);

    void seek(int pos);
    bool canSeek();

    void pause();
    void play();
    void quit();
    void stop();
    bool isPause();
    int getPlayState();
    int64_t getDuration();
    int64_t getTimePos();

    void setMute(bool st);
    void setVolume(int val);
    int  getVolume();

    void audioRemove();
    void selectSubtileTrack(int i);

    // not used
    void playNext();
    void playPrev();

    void setPlayList(const char *files[], int num_files);
    void setPlayShuffle();
    void playFirst();
    void playIndex(int i);

    void setMpvHandle(mpv_handle *mpvHandle);
    void enableVideoTrack(bool track);
    bool getVideoTrackEnable();

    // 设置播放速度以1为基准,(0~1)为慢速, (1,...)为快速, 1为正常播放
    void setPlaybackSpeed(double speed);
    double getPlaybackSpeed();

protected:
    void playStartPosition(int pos);

private:
    mpv_handle *mMpvHandle;
};

#endif // PLAYER_COMMAND_H
