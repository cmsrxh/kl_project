#include "model/category_model.h"
#include "model/cate_item_model.h"
#include "iface/media_service_i_face.h"
#include "iface/media_notify.h"
#include "kl_ui_proc.h"
#include <QDebug>

extern KLUIProc *gInstance;

KLUIProc::KLUIProc()
    : QObject()
    , mPlayState(0)
{

    connect(this, SIGNAL(recvNotify(int,int,int,QString)), this, SLOT(onRecvNotify(int,int,int,QString)));
}

void KLUIProc::init(QQmlContext *ctx)
{
//    m_pCate = new CategoryModel;
//    m_pCateItem = new CateItemModel;

//    ctx->setContextProperty("cateItemModel", m_pCateItem);
//    ctx->setContextProperty("cateModel", m_pCate);
}

void KLUIProc::qmlStart()
{
    MediaServiceIFace::instance()->start();
}

void KLUIProc::qmlPlay()
{
    MediaServiceIFace::instance()->play();
}

void KLUIProc::qmlStop()
{
    MediaServiceIFace::instance()->stop();
}

void KLUIProc::qmlPause()
{
    MediaServiceIFace::instance()->pause();
}

void KLUIProc::qmlReset()
{
    MediaServiceIFace::instance()->reset();
}

void KLUIProc::qmlPlayPause()
{
    MediaServiceIFace::instance()->playPause();
}

void KLUIProc::qmlSetFile(const QString &file)
{
    MediaServiceIFace::instance()->setFile(file.toStdString().c_str());
}

void KLUIProc::qmlSeekTo(long msec, int mode)
{
    MediaServiceIFace::instance()->seekTo(msec, mode);
}

int KLUIProc::qmlGetCurrentPosition()
{
    return MediaServiceIFace::instance()->getCurrentPosition();
}

int KLUIProc::qmlGetDuration()
{
    return MediaServiceIFace::instance()->getDuration();
}

void KLUIProc::qmlPlayPrev()
{

}

void KLUIProc::qmlPlayNext()
{

}

void KLUIProc::onRecvNotify(int msg, int ext1, int ext2, const QString &str)
{
    switch (msg)
    {
    case MEDIA_SEEK_COMPLETE:
        break;
    case MEDIA_ERROR:
        qDebug() << "error info" << ext1 << ext2 << str;
        break;
    case MEDIA_PLAYBACK_COMPLETE:
        setPlayState(3);
        break;
    case MEDIA_PREPARED:
        qmlStart();
        break;
    case MEDIA_STARTED:
        setPlayState(1);
        break;
    case MEDIA_STOPPED:
        setPlayState(3);
        break;
    case MEDIA_PAUSED:
        setPlayState(2);
        break;
    case MEDIA_NOTIFY_TIME:
        Q_EMIT durationChanged(ext1);
        break;
    case MEDIA_SUBTITLE_DATA:
        break;
    default:
        qDebug() << "msg=" << msg << ext1 << ext2 << str;
        break;
    }
}

int KLUIProc::playState() const
{
    return mPlayState;
}

void KLUIProc::setPlayState(int playState)
{
    if (mPlayState != playState)
    {
        mPlayState = playState;
        Q_EMIT playStateChanged();
    }
}

void MediaNotify::notify(int msg, int ext1, int ext2, const char *str)
{
    Q_EMIT gInstance->recvNotify(msg, ext1, ext2, str);
}
