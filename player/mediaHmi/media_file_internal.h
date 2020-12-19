#ifndef MEDIA_FILE_INTERNAL_H
#define MEDIA_FILE_INTERNAL_H

#include <vector>
#include <string>

namespace Media {
typedef std::vector<std::string> vector_s;

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

}

#endif // MEDIA_FILE_INTERNAL_H
