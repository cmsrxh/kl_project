#ifndef MEDIA_NOTIFY_H
#define MEDIA_NOTIFY_H

#include "media_iface_common.h"

class MediaServiceCallback;
class ServiceIfaceBase;
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

    ServiceIfaceBase *m_pHandler;
};

#endif // MEDIA_NOTIFY_H
