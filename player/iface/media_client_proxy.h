#ifndef MEDIA_CLIENT_PROXY_H
#define MEDIA_CLIENT_PROXY_H

#include "media_service_call_back.h"

//! 需要调用具体的IPC通讯的接口类
class MediaClientProxy
{    
public:
    static MediaClientProxy *instance()
    {
        static MediaClientProxy i;
        return &i;
    }

    MediaClientProxy();

    //! 注册回调消息
    void registerNotifyMsg();

    //! 设置Client的回调接口
    void setCallback(MediaServiceCallback *value);

    /**
     * @brief start
     * @param start_pos [in] 播放文件时，从初始位置开始播放，绝对位置（秒）
     * @note 开始播放，调用setFile之后，收到播放准备完成了，就start，
     * 或者在stop调用发生了，才会调用.
     */
    bool start(int start_pos = 0);

    /**
     * @brief play
     * @note 播放暂停的的文件
     */
    bool play();

    /**
     * @brief stop
     * @note 停止播放，如果需要重新播放，就要start
     */
    bool stop();

    /**
     * @brief pause
     * @note 暂停播放
     */
    bool pause();

    /**
     * @brief reset
     * @note 重置播放器
     */
    bool reset();

    /**
     * @brief playPause
     * @note 播放暂停之间相互切换
     */
    bool playPause();

    /**
     * @brief setFile
     * @param file [in] 播放地址
     * @note 设置当前播放文件地址，服务端收到之后，会发送准备开始
     */
    bool setFile(const char *file);

    /**
     * @brief setSubtitle
     * @param ishave [in] 使能字幕
     * @note 如果播放文件有字母文字发送上来，这个接口用来控制使能字幕
     */
    bool setSubtitle(bool ishave);

    /**
     * @brief seekTo
     * @param sec [in] 跳转到那个位置，默认范围（0～总时间）(s)
     * @param mode [in] Not Used
     * @note 播放文件跳转
     */
    bool seekTo(long sec, int mode = 0);

    /**
     * @brief getPlayState
     * @return 播放状态
     * @note 获取播放
     */
    int  getPlayState();

    /**
     * @brief getCurrentPosition
     * @return 返回（秒）
     * @note 获取播放总时间 （秒）
     */
    int  getCurrentPosition();

    /**
     * @brief getDuration
     * @return 返回（秒）
     * @note 获取播放文件的当前时间 （秒）
     */
    int  getDuration();

    /**
     * @brief setPlaybackSpeed
     * @param speed [in] 播放速度
     * @note 设置播放速度以1为基准,(0~1)为慢速, (1,...)为快速, 1为正常播放
     * @warning 一般只在播放的时候调用
     * @return 返回ipc通讯是否成功
     */
    bool setPlaybackSpeed(double speed);

    /**
     * @brief getPlaybackSpeed
     * @return speed 播放速度
     * @note 获取当前播放的速度
     */
    double getPlaybackSpeed();

};

#endif // MEDIA_CLIENT_PROXY_H
