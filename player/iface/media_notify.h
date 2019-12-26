#ifndef MEDIA_NOTIFY_H
#define MEDIA_NOTIFY_H

#include "media_iface_common.h"

enum {
    MEDIA_NOP = 0, // interface test message
    MEDIA_PREPARED = 1,
    MEDIA_PLAYBACK_COMPLETE = 2,
    MEDIA_BUFFERING_UPDATE = 3,
    MEDIA_SEEK_COMPLETE = 4,
    MEDIA_SET_VIDEO_SIZE = 5,
    MEDIA_STARTED = 6,
    MEDIA_PAUSED = 7,
    MEDIA_STOPPED = 8,
    MEDIA_SKIPPED = 9,
    MEDIA_NOTIFY_TIME = 98,
    MEDIA_TIMED_TEXT = 99,
    MEDIA_ERROR = 100,
    MEDIA_INFO = 200,
    MEDIA_SUBTITLE_DATA = 201,
    MEDIA_META_DATA = 202,
    MEDIA_DRM_INFO = 210,
    MEDIA_TIME_DISCONTINUITY = 211,
    MEDIA_AUDIO_ROUTING_CHANGED = 10000,
};

class MediaServiceCallback;
class MediaNotify
{
public:    
    inline static MediaNotify *instance()
    {
        static MediaNotify i;
        return &i;
    }
    void init_notify();

    void notifySeekEnd();
    void notifyPlayError(int err, int sub, const char *str);

    //通知播放完成
    void notifyPlayComplete();

    //播放准备开始
    void notifyPrepare();

    //通知播放开始
    void notifyPlayStart(const char *file);

    //播放器处于空闲状态
    void notifyPlayerIdle();

    //播放器开始播放（非暂停状态）
    void notifyPlayerPlaying();

    //播放器进入暂停状态
    void notifyPlayerPause();

    //通知当前播放文件总时间
    void notifyDuration(int dur);

    //通知当前时间进度条变化值
    void notifySlider(int cur, int dur);

    void notifyScanStart();
    void notifyScanEnd();

    //通知播放模式切换
    void notifyModelSwitch(int mediaType, int mode);

    //通知媒体文件列表
//    void notifyMusicList(Media::vector_s const &l);
//    void notifyVideoList(Media::vector_s const &l);
//    void notifyImageList(Media::vector_s const &l);

    //通知媒体文件列表 index changed
    void notifyMusicIndex(int i);
    void notifyVideoIndex(int i);
    void notifyImageIndex(int i);

    //通知音乐文件的元信息
    void notifyMusicInfo(const char *author, const char *album);

    void notifyMusicImageInfo(uint8_t *data, int w, int h, int stride);


    //通知图片播放开始
    void notifyPictureStart(const char *name);

    //字幕文字通知界面
    void notifySubtitle(const char *sub);

    // 是否静音
    void notifyMute(bool st);

    // 通知当前音量值
    void notifyVolume(int val);
private:
    MediaNotify();
    MediaNotify(MediaNotify &);
    MediaNotify &operator =(MediaNotify&);

    void notify(int msg, int ext1 = 0, int ext2 = 0, const char *str = nullptr);

};

#endif // MEDIA_NOTIFY_H
