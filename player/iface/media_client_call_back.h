#ifndef MEDIA_CLIENT_CALL_BACK_H
#define MEDIA_CLIENT_CALL_BACK_H

#include "media_iface_common.h"

/**
 * @brief The MediaClientCallback class
 * @note 给Service继承的类，用于处理客户端发来的消息
 */
class MediaClientCallback
{
public:
    MediaClientCallback() {}

    /**
     * @brief start
     * @param start_pos [in] 播放文件时，从初始位置开始播放，绝对位置（秒）
     * @note 开始播放，调用setFile之后，收到播放准备完成了，就start，
     * 或者在stop调用发生了，才会调用.
     */
    virtual void start(int /*start_pos*/) {}

    /**
     * @brief play
     * @note 播放暂停的的文件
     */
    virtual void play(){}

    /**
     * @brief stop
     * @note 停止播放，如果需要重新播放，就要start
     */
    virtual void stop() {}

    /**
     * @brief pause
     * @note 暂停播放
     */
    virtual void pause() {}

    /**
     * @brief reset
     * @note 重置播放器
     */
    virtual void reset() {}

    /**
     * @brief playPause
     * @note 播放暂停之间相互切换
     */
    virtual void playPause() {}

    /**
     * @brief setFile
     * @param file [in] 播放地址
     * @note 设置当前播放文件地址，服务端收到之后，会发送准备开始
     */
    virtual void setFile(const char */*file*/) {}

    /**
     * @brief setSubtitle
     * @param ishave [in] 使能字幕
     * @note 如果播放文件有字母文字发送上来，这个接口用来控制使能字幕
     */
    virtual void setSubtitle(bool /*ishave*/) {}

    /**
     * @brief seekTo
     * @param sec [in] 跳转到那个位置，默认范围（0～总时间）(s)
     * @param mode [in] Not Used
     * @note 播放文件跳转
     */
    virtual void seekTo(long /*sec*/, int /*mode*/ ) {}

    /**
     * @brief getPlayState
     * @return 播放状态
     * @note 获取播放
     */
    virtual int  getPlayState() { return -1; }

    /**
     * @brief getCurrentPosition
     * @return 返回（秒）
     * @note 获取播放总时间 （秒）
     */
    virtual int  getCurrentPosition() { return 0; }

    /**
     * @brief getDuration
     * @return 返回（秒）
     * @note 获取播放文件的当前时间 （秒）
     */
    virtual int  getDuration() { return 0; }

    /**
     * @brief setPlaybackSpeed
     * @param speed [in] 播放速度
     * @note 设置播放速度以1为基准,(0~1)为慢速, (1,...)为快速, 1为正常播放
     * @warning 一般只在播放的时候调用
     */
    virtual void setPlaybackSpeed(double /*speed*/) {}

    /**
     * @brief getPlaybackSpeed
     * @return speed 播放速度
     * @note 获取当前播放的速度
     */
    virtual double getPlaybackSpeed() { return 1.0; }

};

#endif // MEDIA_CLIENT_CALL_BACK_H
