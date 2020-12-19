#include <events/common_log.h>
#include "media_hmi_ctrl.h"
#include "image_provider.h"
#include "media_file_model.h"
#include "media_hmi_i_face.h"
#include <QDebug>
#include <QQmlContext>
#include <QQuickView>
#include <QQmlEngine>
#include <QTimer>

#ifdef QT_NO_DEBUG_OUTPUT
#undef QT_NO_DEBUG_OUTPUT
#endif

MediaHMICtrl::MediaHMICtrl()
    : mPlayState(false), mCanSeek(true)
    , mMediaType(Media::MUSIC_TYPE)
    , mViewIndex(FILE_VIEW)
    , mVideoName("----")
    , mMusicName(""), mMusicAuther("")
    , mMusicAlbum(""), mMusicNumList("")
    , mSliderClick(false)
    , mMsgType(-1)
    , mCurPosition(0), mCurDuring(0)
    , mPlayAction(PLAYER_IDLE)
    , mPicRotate(Media::ROTATE_0)
    , mPicScale(Media::SCALE_NORMAL_0)
    , mMsgTimer(new QTimer)
    , mSliderTimer(new QTimer)
    , mCpuTimer(new QTimer)
    , mProvider(new ImageProvider)
    , mVideos(new MediaFileModel(Media::VIDEO_TYPE))
    , mMusics(new MediaFileModel(Media::MUSIC_TYPE))
    , mImages(new MediaFileModel(Media::IMAGE_TYPE))
{
    qDebug() << "Media Ctrl instruction.";

    mMedias[Media::MUSIC_TYPE] = {mMusics, MUSIC_VIEW};
    mMedias[Media::VIDEO_TYPE] = {mVideos, VIDEO_VIEW};
    mMedias[Media::IMAGE_TYPE] = {mImages, IMAGE_VIEW};

}

void MediaHMICtrl::init(QQuickView *view)
{
    QQmlContext *ctx = view->rootContext();
    view->engine()->addImageProvider("media", mProvider);

    ctx->setContextProperty("videoModel", mVideos);
    ctx->setContextProperty("musicModel", mMusics);
    ctx->setContextProperty("imageModel", mImages);

    mMsgTimer->setSingleShot(true);
    connect(mMsgTimer, SIGNAL(timeout()), this, SLOT(onMsgHide()));
    connect(mSliderTimer, SIGNAL(timeout()), this, SLOT(onSliderTimer()));
    connect(mCpuTimer, SIGNAL(timeout()), this, SLOT(onCpuTempTimer()));

    connect(this, SIGNAL(showMsgBox(int, int)), this, SLOT(onShowMsgBox(int, int)));
    connect(this, SIGNAL(deviceRemoveSig()), this, SLOT(onSliderTimer()));

    mCpuTimer->start(1000);
}

void MediaHMICtrl::onShowMsgBox(int msgType, int timeout)
{
    int boxType = 1;
    int msgArgs = msgType;
    QString content = "";
    QString title = "";
    mMsgType = msgType;

    if( mMsgTimer->isActive() )
    {
        mMsgTimer->stop();
    }
    switch (msgType)
    {
    case USB_TRAVEL_FILE_START:      //开始浏览U盘文件
        content = tr("开始浏览U盘文件");
        break;
    case USB_TARVEL_FILE_END:        //浏览完成
        content = tr("浏览完成");
        break;
    default:
        qCritical() << "Invalid Message: " << msgType;
        return;
    }

    if( timeout > 0 )
    {
        mMsgTimer->start(timeout);
    }

    qDebug() << "boxType: " << boxType << ", msgArg: "
             << msgArgs <<", timeout: " << timeout;
    Q_EMIT showMsg(boxType, msgArgs, title, content);
}

void MediaHMICtrl::qmlStart()
{
    MediaHMIIFace::instance()->start();
}

void MediaHMICtrl::qmlPlay()
{
    MediaHMIIFace::instance()->play();
}

void MediaHMICtrl::qmlStop()
{
    MediaHMIIFace::instance()->stop();
}

void MediaHMICtrl::qmlPause()
{
    MediaHMIIFace::instance()->pause();
}

void MediaHMICtrl::qmlReset()
{
    MediaHMIIFace::instance()->reset();
    Q_EMIT playingInfo("", "");
}

void MediaHMICtrl::qmlPlayPause()
{
    MediaHMIIFace::instance()->playPause();
}

void MediaHMICtrl::qmlSetFile(const QString &file)
{
    MediaHMIIFace::instance()->setFile(file.toStdString().c_str());
}

void MediaHMICtrl::qmlSeekTo(long msec, int mode)
{
    MediaHMIIFace::instance()->seekTo(msec, mode);
}

int MediaHMICtrl::qmlGetCurrentPosition()
{
    mCurPosition = MediaHMIIFace::instance()->getCurrentPosition();

    if (mCurDuring < 1)
    {
        qmlGetDuration();
    }
    Q_EMIT positionChanged(numToTimeStr(mCurPosition + mPositionBase),
                           numToTimeStr(mCurDuring + mDuringBase));

    // qDebug() << mCurPosition << mCurDuring;
    return mCurPosition < 0 ? 0 : mCurPosition;
}

QString MediaHMICtrl::numToTimeStr(int num)
{
    QString str(16);

    ushort sec  = num % 60;
    num /= 60;
    ushort min  = num % 60;
    num /= 60;
    ushort hour = num % 24;

    str[0] = '0' + hour / 10;
    str[1] = '0' + hour % 10;
    str[2] = ':';

    str[3] = '0' + min / 10;
    str[4] = '0' + min % 10;

    str[5] = ':';
    str[6] = '0' + sec / 10;
    str[7] = '0' + sec % 10;

    return str;
}

int MediaHMICtrl::qmlGetDuration()
{
    mCurDuring =  mDuringBase ? (mDuringBase - mPositionBase) : MediaHMIIFace::instance()->getDuration();

    if (mCurDuring >= mCacheValue)
    {
        Q_EMIT cacheDataChanged(mCacheValue);
    }
    return mCurDuring < 1 ? 1 : mCurDuring;
}

void MediaHMICtrl::onSliderTimer()
{
    mCurPosition = MediaHMIIFace::instance()->getCurrentPosition();

    if (mCurPosition < mCurDuring)
    {
        mCurDuring =  MediaHMIIFace::instance()->getDuration();
    }

//    Q_EMIT positionChanged(numToTimeStr(mCurPosition + mPositionBase),
//                           numToTimeStr(mCurDuring + mDuringBase));

    Q_EMIT sliderValue(mCurPosition, mCurDuring);
}

void MediaHMICtrl::onCpuTempTimer()
{
    char buff[16] = {0};
    int fd = ::open("/sys/class/thermal/thermal_zone0/temp", O_RDONLY);

    if (fd > 0)
    {

        ::read(fd, buff, 16);
        ::close(fd);
        int value = ::atoi(buff);
        double temp = value / 1000.0;

        snprintf(buff, 16, "%g", temp);

        setCpuTemp(buff);
    } else
    {
         mCpuTimer->stop();
        setCpuTemp("0.0");
    }
//     GEN_Printf(LOG_DEBUG, "fd=%d, cputemp: %s", fd, buff);
}

QString MediaHMICtrl::cpuTemp() const
{
    return mCpuTemp;
}

void MediaHMICtrl::setCpuTemp(const QString &cpuTemp)
{
    mCpuTemp = cpuTemp;
    Q_EMIT cpuTempChanged();
}

void MediaHMICtrl::onRecvNotify(int msg, int ext1, int ext2, const QString &str)
{
    qDebug() << "onRecvNotify: " << msg << ext1 << ext2 << str;
    switch (msg)
    {
    case MEDIA_SEEK_COMPLETE:
        break;
    case MEDIA_ERROR:
        mSliderTimer->stop();
        qWarning() << "error info" << ext1 << ext2 << str;
        MediaHMIIFace::instance()->autoPlayNext();
        break;
    case MEDIA_PLAYBACK_COMPLETE:
        setPlayStatus(3);
        Q_EMIT positionChanged("00:00:00", "00:00:00");
        MediaHMIIFace::instance()->autoPlayNext();
        break;
    case MEDIA_PREPARED:
        qmlStart();
        mPositionBase = 0;
        mDuringBase   = 0;
        mCurDuring    = 0;
        Q_EMIT durationChanged(0);
        MediaHMIIFace::instance()->showPlayingInfo();
        // qDebug() << "Time Base: " << mPositionBase << mDuringBase;
        break;
    case MEDIA_STARTED:
        setPlayStatus(1);
        break;
    case MEDIA_STOPPED:
        setPlayStatus(3);
        break;
    case MEDIA_PAUSED:
        setPlayStatus(2);
        break;
    case MEDIA_CACHE_TIME:
        // qDebug() << "cache position: " << ext1;
        mCacheValue = ext1;
        Q_EMIT cacheDataChanged(ext1);
        break;
    case MEDIA_NOTIFY_TIME:
    {
        if (mDuringBase)
        {
            ext1   = mDuringBase - mPositionBase;
        }
        // qDebug() << "Time Base: " << mPositionBase << mDuringBase << "duration=" << ext1;
        Q_EMIT durationChanged(ext1);
        break;
    }
    case MEDIA_SUBTITLE_DATA:
        break;
    default:
        qDebug() << "msg=" << msg << ext1 << ext2 << str;
        break;
    }
}

void MediaHMICtrl::onDeviceRemove()
{

}

void MediaHMICtrl::setPlayAction(int playAction)
{
    mPlayAction = playAction;
    if(playAction == PLAYER_PLAY)
    {
        setPlayStatus(true);
    }else
    {
        setPlayStatus(false);
        if(playAction == PLAYER_IDLE)
        {
            //TODO: other
        }
    }
}

int MediaHMICtrl::viewIndex() const
{
    return mViewIndex;
}

void MediaHMICtrl::setViewIndex(int viewIndex)
{
    if(mViewIndex != viewIndex)
    {
        mViewIndex = viewIndex;
        Q_EMIT viewIndexChanged();
    }
}

void MediaHMICtrl::hideMsgBox()
{
    Q_EMIT hideMsg();
}

bool MediaHMICtrl::playStatus() const
{
    return mPlayState;
}

void MediaHMICtrl::setPlayStatus(int playStatus)
{
    if (mPlayState != playStatus)
    {
        mPlayState = playStatus;
        Q_EMIT playStatusChanged();
        if (1 == mPlayState)
        {
            mSliderTimer->start(1000);
        } else if (mSliderTimer->isActive())
        {
            mSliderTimer->stop();
        }
    }
}

void MediaHMICtrl::btnMenuClicked()
{
    setViewIndex(FILE_VIEW);
}

void MediaHMICtrl::btnPlayPause()
{
    MediaHMIIFace::instance()->playPause();
}

void MediaHMICtrl::btnPreviousClicked()
{
    MediaHMIIFace::instance()->prev();
}

void MediaHMICtrl::btnNextClicked()
{
    MediaHMIIFace::instance()->next();
}

void MediaHMICtrl::btnModeClicked()
{
    MediaHMIIFace::instance()->modelSwitch(mMediaType);
}

void MediaHMICtrl::btnWallpaperClicked()
{
    MediaHMIIFace::instance()->pictureWallpaper();
}

void MediaHMICtrl::btnRotateClicked()
{
    mPicRotate = (mPicRotate + 1) % (Media::ROTATE_270 + 1);
    MediaHMIIFace::instance()->pictureRotateModel((Media::RotateMode)mPicRotate);
}

void MediaHMICtrl::btnScaleClicked()
{
    ++mPicScale;
    MediaHMIIFace::instance()->pictureScaleModel((Media::ScaleMode)mPicScale);
}

void MediaHMICtrl::btnBackClicked()
{
    qDebug() << "mMediaType: " << mMediaType;
    switch (mMediaType)
    {
    case Media::MUSIC_TYPE:
        setViewIndex(MUSIC_VIEW);
        break;
    case Media::VIDEO_TYPE:
        setViewIndex(VIDEO_VIEW);
        break;
    case Media::IMAGE_TYPE:
        setViewIndex(IMAGE_VIEW);
        break;
    default:
        break;
    }
}

void MediaHMICtrl::qmlSpaceClicked()
{

}

void MediaHMICtrl::qmlBtnYesPress(int type)
{
    qDebug() << "BTN Yes Press" << type;
}

void MediaHMICtrl::qmlBtnNoPress(int type)
{
    qDebug() << "BTN No Press" << type;
}

void MediaHMICtrl::qmlSeekingSwitch()
{
    mSliderClick = true;
}

void MediaHMICtrl::qmlPlaySeek(int value)
{
    qDebug() << "Seek Position: " << value << mCanSeek;
    if(mCanSeek)
    {
        //TODO:
        //禁止再跳，等到服务端至位
        //mCanSeek = false;
        MediaHMIIFace::instance()->seekTo(value);
    }
    //设置跳转状态结束
    mSliderClick = false;
}

void MediaHMICtrl::onMsgHide()
{
    // qDebug() << "hide msg box";
    Q_EMIT hideMsg();
}

void MediaHMICtrl::sliderChanged(int current, int duration)
{
    if( !mSliderClick )
    {
        mCurPosition = current;
        mCurDuring = duration;
        Q_EMIT sliderValue(current, duration);
    }
}

void MediaHMICtrl::setMediaFile(int type, Media::vector_s &lst)
{
    qDebug() << "MediaType: " << type << "size: " << lst.size();
    switch (type)
    {
    case Media::MUSIC_TYPE:
        mMusics->setList(lst);
        Q_EMIT mMusics->updateMediaFile();
        break;
    case Media::VIDEO_TYPE:
        mVideos->setList(lst);
        Q_EMIT mVideos->updateMediaFile();
        break;
    case Media::IMAGE_TYPE:
        mImages->setList(lst);
        Q_EMIT mImages->updateMediaFile();
        break;
    default:
        qWarning() << "invalid type: " << type;
        break;
    }
}

void MediaHMICtrl::setImage(const QImage &img)
{
    mProvider->setImage(img);
    Q_EMIT flushPicture();
}

void MediaHMICtrl::deviceRemove()
{
    Q_EMIT deviceRemoveSig();
}

void MediaHMICtrl::setCurrentIndex(int mediaType, int index, const char *filename)
{
    switch (mediaType) {
    case Media::MUSIC_TYPE:
        mMusics->setCurrentIndex(index);
        setMusicName(filename);
        break;
    case Media::VIDEO_TYPE:
        mVideos->setCurrentIndex(index);
        setVideoName(filename);
        break;
    case Media::IMAGE_TYPE:
        mImages->setCurrentIndex(index);
        break;
    default:
        GEN_Printf(LOG_ERROR, "invalid type: %d", mediaType);
        break;
    }
}

QString MediaHMICtrl::imageName() const
{
    return mImageName;
}

void MediaHMICtrl::setImageName(const QString &imageName)
{
    mImageName = imageName;
    Q_EMIT imageNameChanged();
}

int MediaHMICtrl::mediaType() const
{
    return mMediaType;
}

void MediaHMICtrl::setMediaType(int mediaType)
{
    if(mMediaType != mediaType)
    {
        mMediaType = mediaType;
        Q_EMIT mediaTypeChanged();
    }
}    

QString MediaHMICtrl::musicNumList() const
{    
    return mMusicNumList;
}

void MediaHMICtrl::setMusicNumList(const QString &/*musicNumList*/)
{
    mMusicNumList.sprintf("%d/%d", mMusics->currentIndex(), mMusics->lstSize());
    Q_EMIT musicNumListChanged();
}

QString MediaHMICtrl::musicAlbum() const
{
    return mMusicAlbum;
}

void MediaHMICtrl::setMusicAlbum(const QString &musicAlbum)
{
    mMusicAlbum = musicAlbum;
    Q_EMIT musicAlbumChanged();
}

QString MediaHMICtrl::musicAuther() const
{
    return mMusicAuther;
}

void MediaHMICtrl::setMusicAuther(const QString &musicAuther)
{
    mMusicAuther = musicAuther;
    Q_EMIT musicAutherChanged();
}

QString MediaHMICtrl::musicName() const
{
    return mMusicName;
}

void MediaHMICtrl::setMusicName(const QString &musicName)
{
    mMusicName = musicName;
    Q_EMIT musicNameChanged();
}

QString MediaHMICtrl::videoName() const
{
    return mVideoName;
}

void MediaHMICtrl::setVideoName(const QString &videoName)
{
    mVideoName = videoName;
    Q_EMIT videoNameChanged();
}

bool MediaHMICtrl::canSeek() const
{
    return mCanSeek;
}

void MediaHMICtrl::setCanSeek(bool canSeek)
{
    mCanSeek = canSeek;
    Q_EMIT canSeekChanged();
}
