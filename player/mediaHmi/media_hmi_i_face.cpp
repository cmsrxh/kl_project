#include <events/common_log.h>
#include "manage/application.h"
#include "media_hmi_ctrl.h"
#include "media_hmi_i_face.h"

void MediaHMIIFace::playFile(int mediaType, int index)
{
    Application::instance()->postEvent(SIG_MEDIA_CTRL_playFile, mediaType, index);
}

void MediaHMIIFace::autoPlayNext()
{
    Application::instance()->postEvent(SIG_MEDIA_CTRL_autoPlayNext);
}

void MediaHMIIFace::showPlayingInfo()
{
    Application::instance()->postEvent(SIG_MEDIA_CTRL_showPlayingInfo);
}

void MediaHMIIFace::prev()
{
    Application::instance()->postEvent(SIG_MEDIA_CTRL_prev);
}

void MediaHMIIFace::next()
{
    Application::instance()->postEvent(SIG_MEDIA_CTRL_next);
}

void MediaHMIIFace::modelSwitch(int mediaType)
{
    Application::instance()->postEvent(SIG_MEDIA_CTRL_modelSwitch, mediaType);
}

void MediaHMIIFace::pictureWallpaper()
{
    Application::instance()->postEvent(SIG_MEDIA_CTRL_pictureWallpaper);
}

void MediaHMIIFace::pictureScaleModel(int model)
{
    Application::instance()->postEvent(SIG_MEDIA_CTRL_pictureScaleModel, model);
}

void MediaHMIIFace::pictureRotateModel(int model)
{
    Application::instance()->postEvent(SIG_MEDIA_CTRL_pictureRotateModel, model);
}
