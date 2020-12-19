#include <events/common_log.h>
#include "file_travel.h"
#include "media_hmi_ctrl.h"
#include "media_hmi_i_face.h"
#include "app_common.h"
#include "application.h"

Application::Application()
    : m_pTravel(new FileTravel)
{

}

void Application::initialize()
{
    // init_all(INIT_NEED_NOSIGNAL, "mediahmi", "/etc/zlog.conf");

    postEvent(SIG_TRAVEL_MEDIA_FILE);

    SimpleThread::start();
}

void Application::playVecFile(ListTable<ByteString>::vector &vec, int &index)
{
    if (vec.empty())
    {
        GEN_Printf(LOG_WARN, "Current Play list is empty.");
        return;
    }
    if (index < 0)
    {
        index = vec.size() - 1;
    } else if (index >= vec.size())
    {
        index = 0;
    }

    GEN_Printf(LOG_DEBUG, "----%s----", vec[index].string());
    MediaHMIIFace::instance()->setFile(vec[index].string());

    mediaApp->setCurrentIndex(mCurrentType, mCurrentIndex, vec[index].string());
}

void Application::run()
{
    ListTable<ByteString>::vector musicVec;
    ListTable<ByteString>::vector videoVec;

    const QEvt *evt;

    initThreadPrivate();

    GEN_Printf(LOG_DEBUG, "event loop is started");
    while(getMessage(evt))
    {
        switch (evt->sig)
        {        
        case SIG_TRAVEL_MEDIA_FILE:
        {
            char *filePath = (char *)(((GeneralQEvt *)evt)->wParam);
            if (!filePath)
            {
#ifdef PLATFORM_ARM
                filePath = (char *)"/mnt";
#else
                filePath = (char *)"/mnt/hgfs/shareToUbuntu/media";
#endif
            } /*else
            {
                int size = strlen(filePath);
                if (filePath[size - 1] != '/' )
                {
                    filePath[size] = '/';
                    filePath[size+1] = '\0';
                }
            }*/
            GEN_Printf(LOG_DEBUG, "travel dirname: %s", filePath);
            m_pTravel->clear();
            m_pTravel->readDir(filePath);


            ListTable<ByteString> &music = m_pTravel->music();
            ListTable<ByteString> &video = m_pTravel->video();
            ListTable<ByteString> &image = m_pTravel->image();

            Media::vector_s  std_music;
            Media::vector_s  std_video;
            Media::vector_s  std_image;

            music.genVector(musicVec);
            video.genVector(videoVec);

            ListTable<ByteString>::iterator it = music.begin();
            for (; it != music.end(); ++it)
            {
                if (!it->empty())
                {
                    std_music.push_back(it->string());
                } else
                {
                    GEN_Printf(LOG_DEBUG, "list string is empty.");
                }
            }

            it = video.begin();
            for (; it != video.end(); ++it)
            {
                if (!it->empty())
                {
                    std_video.push_back(it->string());
                } else
                {
                    GEN_Printf(LOG_DEBUG, "list string is empty.");
                }
            }
            it = image.begin();
            for (; it != image.end(); ++it)
            {
                if (!it->empty())
                {
                    std_image.push_back(it->string());
                } else
                {
                    GEN_Printf(LOG_DEBUG, "list string is empty.");
                }
            }

            mediaApp->setMediaFile(Media::MUSIC_TYPE, std_music);
            mediaApp->setMediaFile(Media::VIDEO_TYPE, std_video);
            mediaApp->setMediaFile(Media::IMAGE_TYPE, std_image);
            break;
        }
        case SIG_USB_DEVICE_REMOVE:
        {
            GEN_Printf(LOG_DEBUG, "device remove.");
            mediaApp->deviceRemove();
            break;
        }
        case SIG_MEDIA_CTRL_playFile:
        {
            ByteString name;
            mCurrentType = ((GeneralQEvt *)evt)->wParam;
            mCurrentIndex = ((GeneralQEvt *)evt)->lParam;
            GEN_Printf(LOG_DEBUG, "playFile: mediaType=%d, index=%d", mCurrentType, mCurrentIndex);
            if (0 == mCurrentType) //music list
            {
                name = musicVec[mCurrentIndex];
            } else if (1 == mCurrentType)
            {
                name = videoVec[mCurrentIndex];
            }
            GEN_Printf(LOG_DEBUG, "file: %s", name.string());
            mediaApp->setCurrentIndex(mCurrentType, mCurrentIndex, name.string());
            break;
        }
        case SIG_MEDIA_CTRL_autoPlayNext:
        {
            GEN_Printf(LOG_DEBUG, "autoPlayNext:");
            ++mCurrentIndex;
            if (0 == mCurrentType) //music list
            {
                playVecFile(musicVec, mCurrentIndex);
            } else if (1 == mCurrentType)
            {
                playVecFile(videoVec, mCurrentIndex);
            } else
            {
                GEN_Printf(LOG_WARN, "Invalid List type: %d", mCurrentType);
            }
            break;
        }
        case SIG_MEDIA_CTRL_showPlayingInfo:
            GEN_Printf(LOG_DEBUG, "showPlayingInfo:");
            break;
        case SIG_MEDIA_CTRL_prev:
        {
            GEN_Printf(LOG_DEBUG, "prev:");
            --mCurrentIndex;
            if (0 == mCurrentType) //music list
            {
                playVecFile(musicVec, mCurrentIndex);
            } else if (1 == mCurrentType)
            {
                playVecFile(videoVec, mCurrentIndex);
            } else
            {
                GEN_Printf(LOG_WARN, "Invalid List type: %d", mCurrentType);
            }
            break;
        }
        case SIG_MEDIA_CTRL_next:
        {
            GEN_Printf(LOG_DEBUG, "next:");
            ++mCurrentIndex;
            if (0 == mCurrentType) //music list
            {
                playVecFile(musicVec, mCurrentIndex);
            } else if (1 == mCurrentType)
            {
                playVecFile(videoVec, mCurrentIndex);
            } else
            {
                GEN_Printf(LOG_WARN, "Invalid List type: %d", mCurrentType);
            }
            break;
        }
        case SIG_MEDIA_CTRL_modelSwitch:
        {
            mCurrentType = ((GeneralQEvt *)evt)->wParam;
            GEN_Printf(LOG_DEBUG, "modelSwitch: mediaType=%d", mCurrentType);
            break;
        }
        case SIG_MEDIA_CTRL_pictureWallpaper:
            GEN_Printf(LOG_DEBUG, "pictureWallpaper:");
            break;
        case SIG_MEDIA_CTRL_pictureScaleModel:
            GEN_Printf(LOG_DEBUG, "pictureScaleModel:");
            break;
        case SIG_MEDIA_CTRL_pictureRotateModel:
            GEN_Printf(LOG_DEBUG, "pictureRotateModel:");
            break;
        case SIG_USER_UNUSED:
        default:
            GEN_Printf(LOG_WARN, "[%d] is UNKOWN.", evt->sig);
            break;
        }

        recycleEvent(evt);
    }

    GEN_Printf(LOG_WARN, "Event Loop is Exit.");
    exit(EXIT_SUCCESS);
}

bool Application::postEvent(int cmd, long ext1, long ext2, const char *str)
{
    GeneralQEvt *e = (GeneralQEvt *)newEvt(cmd, sizeof(GeneralQEvt));

    if (e)
    {
        e->wParam = ext1;
        e->lParam = ext2;
        e->pHander = (void *)str;
        return post(e);
    } else
    {
        GEN_Printf(LOG_ERROR, "Post Kl Cmd failed.");
        return false;
    }
}


