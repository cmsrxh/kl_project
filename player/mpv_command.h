#ifndef PLAYER_COMMAND_H
#define PLAYER_COMMAND_H

typedef struct mpv_handle mpv_handle;
class MPVCommand
{
public:
    MPVCommand();

    void playFile(const char *fileName);

    void seek(int pos);
    bool canSeek();

    void pause();
    void play();
    void quit();
    void stop();
    bool isPause();
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
    void switchTrack(int track);
private:
    mpv_handle *mMpvHandle;
};

#endif // PLAYER_COMMAND_H
