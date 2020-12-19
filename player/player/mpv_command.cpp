#include <events/common_log.h>
#include "mpv_command.h"
#include <mpv/client.h>

MPVCommand::MPVCommand()
{

}

void MPVCommand::playStartPosition(int pos)
{
    double value = pos;

    int ret = mpv_set_property(mMpvHandle, "start", MPV_FORMAT_DOUBLE, &value);
    if (ret < 0)
    {
        GEN_Printf(LOG_ERROR, "Set option start pos failed, %d, pos=%d", ret, pos);
    }
}

void MPVCommand::playFile(const char *fileName, int start_pos)
{
    int             ret;
    mpv_node        node;
    mpv_node_list   list;
    mpv_node        array[2];
    mpv_node        result;

    playStartPosition(start_pos);

    node.format = MPV_FORMAT_NODE_ARRAY;
    node.u.list = &list;

    list.values = array;
    list.num    = 2;

    array[0].format   = MPV_FORMAT_STRING;
    array[0].u.string = (char *)"loadfile";

    array[1].format   = MPV_FORMAT_STRING;
    array[1].u.string = (char *)fileName;

    ret = mpv_command_node(mMpvHandle, &node, &result);

    if(ret < 0)
    {
        GEN_Printf(LOG_ERROR, "run commad failed, format: %d.", result.format);
    }
}

void MPVCommand::seek(int pos)
{
    int             ret;
    mpv_node        node;
    mpv_node_list   list;
    mpv_node        array[3];
    mpv_node        result;

    node.format = MPV_FORMAT_NODE_ARRAY;
    node.u.list = &list;

    list.values = array;
    list.num    = 3;

    array[0].format   = MPV_FORMAT_STRING;
    array[0].u.string = (char *)"seek";

    array[1].format   = MPV_FORMAT_INT64;
    array[1].u.int64  = pos;

    array[2].format   = MPV_FORMAT_STRING;
//    array[2].u.string = (char *)"absolute";
    array[2].u.string = (char *)"relative";

    ret = mpv_command_node(mMpvHandle, &node, &result);

    if(ret < 0)
    {
        GEN_Printf(LOG_ERROR, "run commad failed, format: %d.", result.format);
    }
}

bool MPVCommand::canSeek()
{
    int st = 0;

    mpv_get_property(mMpvHandle, "seekable", MPV_FORMAT_FLAG, &st);

    return st;
}

void MPVCommand::pause()
{
#if 1
    int             ret;
    mpv_node        node;

    node.format  = MPV_FORMAT_FLAG;
    node.u.flag  = 1;

    ret = mpv_set_property(mMpvHandle, "pause", MPV_FORMAT_NODE, &node);

    if(ret < 0)
    {
        GEN_Printf(LOG_ERROR, "set property failed, format: %d.", node.format);
    }
#else
    GEN_Printf(LOG_DEBUG, "Set state: %d", st);
    mpv_set_property(mMpvHandle, "pause", MPV_FORMAT_FLAG, &st);
#endif
}

void MPVCommand::play()
{
    int st = 0;
    mpv_set_property(mMpvHandle, "pause", MPV_FORMAT_FLAG, &st);
}

void MPVCommand::quit()
{
#if 0
    int st = 1;
    mpv_set_property(mMpvHandle, "playback-abort", MPV_FORMAT_FLAG, &st);
#elif 1
    int             ret;
    mpv_node        node;
    mpv_node_list   list;
    mpv_node        array[1];
    mpv_node        result;

    node.format = MPV_FORMAT_NODE_ARRAY;
    node.u.list = &list;

    list.values = array;
    list.num    = 1;

    array[0].format   = MPV_FORMAT_STRING;
    array[0].u.string = (char *)"quit";

    ret = mpv_command_node(mMpvHandle, &node, &result);
    if(ret < 0)
    {
        GEN_Printf(LOG_ERROR, "run commad failed: %d, format: %d.", ret, result.format);
    }

#else
#endif
}

void MPVCommand::stop()
{
    int             ret;
    mpv_node        node;
    mpv_node_list   list;
    mpv_node        array[1];
    mpv_node        result;

    node.format = MPV_FORMAT_NODE_ARRAY;
    node.u.list = &list;

    list.values = array;
    list.num    = 1;

    array[0].format   = MPV_FORMAT_STRING;
    array[0].u.string = (char *)"stop";

    ret = mpv_command_node(mMpvHandle, &node, &result);
    if(ret < 0)
    {
        GEN_Printf(LOG_ERROR, "run commad failed: %d, format: %d.", ret, result.format);
    }
}

void MPVCommand::playNext()
{
    int             ret;
    mpv_node        node;
    mpv_node_list   list;
    mpv_node        array[1];
    mpv_node        result;

    node.format = MPV_FORMAT_NODE_ARRAY;
    node.u.list = &list;

    list.values = array;
    list.num    = 1;

    array[0].format   = MPV_FORMAT_STRING;
    array[0].u.string = (char *)"playlist-next";

    ret = mpv_command_node(mMpvHandle, &node, &result);

    if(ret < 0)
    {
        GEN_Printf(LOG_ERROR, "run commad failed, format: %d.", result.format);
    }
}

void MPVCommand::playPrev()
{
    int             ret;
    mpv_node        node;
    mpv_node_list   list;
    mpv_node        array[1];
    mpv_node        result;

    node.format = MPV_FORMAT_NODE_ARRAY;
    node.u.list = &list;

    list.values = array;
    list.num    = 1;

    array[0].format   = MPV_FORMAT_STRING;
    array[0].u.string = (char *)"playlist-prev";

    ret = mpv_command_node(mMpvHandle, &node, &result);

    if(ret < 0)
    {
        GEN_Printf(LOG_ERROR, "run commad failed, format: %d.", result.format);
    }
}

void MPVCommand::setPlayShuffle()
{
    int             ret;
    mpv_node        node;
    mpv_node_list   list;
    mpv_node        array[1];
    mpv_node        result;

    node.format = MPV_FORMAT_NODE_ARRAY;
    node.u.list = &list;

    list.values = array;
    list.num    = 1;

    array[0].format   = MPV_FORMAT_STRING;
    array[0].u.string = (char *)"playlist-shuffle";

    ret = mpv_command_node(mMpvHandle, &node, &result);

    if(ret < 0)
    {
        GEN_Printf(LOG_ERROR, "run commad failed, format: %d.", result.format);
    }
}

void MPVCommand::playFirst()
{
    int st = 1;
    mpv_set_property(mMpvHandle, "playlist-pos-1", MPV_FORMAT_FLAG, &st);
}

void MPVCommand::playIndex(int i)
{
    int st = i;
    mpv_set_property(mMpvHandle, "playlist-pos", MPV_FORMAT_FLAG, &st);
}

bool MPVCommand::isPause()
{
#if 0
    mpv_node node;
    if (mpv_get_property(mMpvHandle, "pause", MPV_FORMAT_NODE, &node) < 0)
    {
        GEN_Printf(LOG_ERROR, "get property failed");
        return false;
    }
    GEN_Printf(LOG_DEBUG, "node: %d, flag: %d", node.format, node.u.flag);

    if(node.format == MPV_FORMAT_FLAG)
    {
        return node.u.flag;
    }
    return false;
#else
    int st = 0;

    mpv_get_property(mMpvHandle, "pause", MPV_FORMAT_FLAG, &st);

    //GEN_Printf(LOG_DEBUG, "pause state: %d", st);

    return st;
#endif
}

int MPVCommand::getPlayState()
{
    GEN_Printf(LOG_DEBUG, "NOT USED");
    return 0;
}

int64_t MPVCommand::getDuration()
{
    int64_t tmp = 0;

    mpv_get_property(mMpvHandle, "duration", MPV_FORMAT_INT64, &tmp);

    return tmp;
}

int64_t MPVCommand::getTimePos()
{
    int64_t tmp = 0;

    mpv_get_property(mMpvHandle, "time-pos", MPV_FORMAT_INT64, &tmp);

    return tmp;
}

void MPVCommand::setMute(bool st)
{
    int64_t tmp = st;

    mpv_set_property(mMpvHandle, "ao-mute", MPV_FORMAT_INT64, &tmp);
}

void MPVCommand::setVolume(int val)
{
    int64_t tmp = val;

    mpv_set_property(mMpvHandle, "ao-volume", MPV_FORMAT_INT64, &tmp);
}

int MPVCommand::getVolume()
{
    int64_t tmp = -1;

    mpv_get_property(mMpvHandle, "ao-volume", MPV_FORMAT_INT64, &tmp);

    return tmp;
}

void MPVCommand::audioRemove()
{
    const char *cmd[] = { "audio-remove", NULL };

    int ret = mpv_command(mMpvHandle, cmd);
    if(ret < 0)
    {
        GEN_Printf(LOG_ERROR, "run commad failed: %d.", ret);
    }
}

/**
 * @brief MPVCommand::switchTrack
 * @param enable [in] 是否使能视频媒体播放轨道
 * @details 对应mpv中的属性stream_id -2表示禁止当前媒体轨道，-1表示播放当前轨道
 */
void MPVCommand::enableVideoTrack(bool enable)
{
    int             ret;
    mpv_node        node;

    node.format = MPV_FORMAT_INT64;
    node.u.int64 = enable ? -1 : -2;

    ret = mpv_set_property(mMpvHandle, "vid", MPV_FORMAT_NODE, &node);

    if(ret < 0)
    {
        GEN_Printf(LOG_ERROR, "run commad failed, ret = %d", ret);
    }
}

bool MPVCommand::getVideoTrackEnable()
{
    int             ret;
    mpv_node        node;

    node.format = MPV_FORMAT_INT64;
    node.u.int64 = 1000;

    ret = mpv_get_property(mMpvHandle, "vid", MPV_FORMAT_NODE, &node);

    if(ret < 0)
    {
        GEN_Printf(LOG_ERROR, "run commad failed, ret = %d", ret);
    }
    // 等于-1表示，使能了视频播放轨道
    return node.u.int64 == -1;
}

void MPVCommand::setPlaybackSpeed(double speed)
{
    int ret;
    double tmp, cur;

    GEN_Printf(LOG_DEBUG, "set playback speed: %g", speed);

//    if (speed < 0.000001)
//    {
//        GEN_Printf(LOG_DEBUG, "speed value=%g is invalid", speed);
//        return;
//    }

    ret = mpv_get_property(mMpvHandle, "speed", MPV_FORMAT_DOUBLE, &cur);
    if (ret < 0)
    {
        GEN_Printf(LOG_ERROR, "get playback speed failed, %d.", ret);
        return;
    }

    tmp = cur - speed;
    if (tmp > -0.000001 && tmp < 0.000001)
    {
        GEN_Printf(LOG_DEBUG, "Current Playback Speed equal set value");
        return;
    }

    tmp = speed;

    ret = mpv_set_property(mMpvHandle, "speed", MPV_FORMAT_DOUBLE, &tmp);
    if (ret < 0)
    {
        GEN_Printf(LOG_ERROR, "set playback speed failed, %d.", ret);
    }
}

double MPVCommand::getPlaybackSpeed()
{
    int    ret;
    double cur;

    ret = mpv_get_property(mMpvHandle, "speed", MPV_FORMAT_DOUBLE, &cur);
    if (ret < 0)
    {
        GEN_Printf(LOG_ERROR, "get playback speed failed, %d.", ret);
        return 1.0;
    }

    GEN_Printf(LOG_DEBUG, "get playback speed: %g", cur);

    return cur;
}

void MPVCommand::selectSubtileTrack(int i)
{
    int64_t tmp = i;

    GEN_Printf(LOG_DEBUG, "select subtitle: %d", i);
    int ret = mpv_set_property(mMpvHandle, "sid", MPV_FORMAT_INT64, &tmp);
    if(ret < 0)
    {
        GEN_Printf(LOG_ERROR, "set property[sid] failed: %d.", ret);
    }
}

void MPVCommand::setPlayList(const char *files[], int num_files)
{
#if 0
    //功能MPV未完成
    int             ret;
    mpv_node        node;
    mpv_node_list   list;
    mpv_node        array[num_files];
    mpv_node        result;

    node.format = MPV_FORMAT_NODE_ARRAY;
    node.u.list = &list;

    list.values = array;
    list.num    = num_files;

    for(int i = 0; i < num_files; ++i)
    {
        array[i].format   = MPV_FORMAT_STRING;
        array[i].u.string = (char *)files[i];
    }

    ret = mpv_set_option/*mpv_set_property*/(mMpvHandle, "playlist", MPV_FORMAT_NODE, &node);

    if(ret < 0)
    {
        GEN_Printf(LOG_ERROR, "run commad failed(%d), format: %d.", ret, result.format);
    }
#else
    int len = num_files + 20;
    for(int i = 0; i < num_files; ++i)
    {
        len += strlen(files[i]);
    }
    char *tmp = new char [len];
    int pos = sprintf(tmp, "{%s", files[0]);
    for(int i = 1; i < num_files; ++i)
    {
        pos += sprintf(tmp + pos, ",%s", files[i]);
    }
    tmp[pos++] = '}';
    tmp[pos] = '\0';

    //printf("%s\n", tmp);
    int ret = mpv_set_option_string(mMpvHandle, "playlist", tmp);
    if(ret < 0)
    {
        GEN_Printf(LOG_ERROR, "set option failed(%d).", ret);
    }
    delete [] tmp;
#endif
}

void MPVCommand::setMpvHandle(mpv_handle *mpvHandle)
{
    mMpvHandle = mpvHandle;
}
