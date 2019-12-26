/*************************************************************************
	> File Name: media_iface_common.h
	> Author: 
	> Mail: 
	> Created Time: 2019年07月20日 星期六 16时00分33秒
 ************************************************************************/

#ifndef _MEDIA_IFACE_COMMON_H
#define _MEDIA_IFACE_COMMON_H

#include <vector>
#include <string>

namespace Media
{

//对应界面的顺序
enum MediaType
{
    MUSIC_TYPE,
    VIDEO_TYPE,
    IMAGE_TYPE,
};

//对应界面的顺序
enum PlayModel
{
    LOOP_MODE,
    RANDOM_MODE,
    SINGLE_MODE,
};

enum RotateMode
{
    ROTATE_0,
    ROTATE_90,
    ROTATE_180,
    ROTATE_270,
};

enum ScaleMode
{
    SCALE_ZOOM_OUT_4,
    SCALE_ZOOM_OUT_2,
    SCALE_ZOOM_OUT_1,

    SCALE_NORMAL_0,

    SCALE_STRETCH_1,
    SCALE_STRETCH_2,
    SCALE_STRETCH_4,
};

//各类属性定义
enum PropertyType
{
    CURRENT_PLAYING_STATUS,         //当前播放状态
    CURRENT_PLAYING_DURATION,       //当前播放的总时间
    TRAVEL_FILE_STATUS,             //正在浏览U盘文件的状态
    PLAY_MODEL_SWITCH,              //播放模式切换
    MEDIA_TYPE_SWITCH,              //当前播放的媒体类型切换
    MEDIA_SUBTITLE_SHOW,            //字幕文字通知界面

    PICTURE_SHOW,                   //图片显示，根据Index或名字
    PICTURE_WALL_PAPER,
    PICTURE_ROTATE,                 //图片旋转
    PICTURE_SCALE,                  //图片缩放
};

//各种状态变化迁移
enum ActionStatus
{
#define PLAYER_IDLE   Media::ACTION_IDLE
    ACTION_IDLE     = 0x01,    //初始空闲状态

#define PLAYER_START  Media::ACTION_STARTED
    ACTION_STARTED  = 0x02,    //开始

#define PLAYER_PLAY   Media::ACTION_OVER
    ACTION_OVER     = 0x04,    //有两层含义，(1)结束，(2)代表某些只有三种状态的true

#define PLAYER_PAUSE  Media::ACTION_UNOVER
    ACTION_UNOVER   = 0x08,    //类似于false 表示未完成，仅仅代表某些只有三种状态的false

    ACTION_FAILED   = 0x10,    //失败


    ACTION_RUNNING  = 0x20,    //正在进行中
};

}
#endif
