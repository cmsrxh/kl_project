#include "model/category_model.h"
#include "model/cate_item_model.h"
#include "model/chip_item_model.h"
#include "model/collect_model.h"
#include "iface/media_service_i_face.h"
#include "iface/media_notify.h"
#include "model/kl_data_proc.h"
#include "model/detail_qobject.h"
#include "qml_view_switch_stack.h"
#include "kl_ui_proc.h"
#include <QDebug>

extern KLUIProc *gInstance;

KLUIProc::KLUIProc()
    : QObject()
    , mPlayState(0)
    , mCanSeek(true)
    , mDuringBase(0), mPositionBase(0)
    , m_pViewStack(new ViewSwitchStack)
{
    connect(this, SIGNAL(recvNotify(int,int,int,QString)), this, SLOT(onRecvNotify(int,int,int,QString)));
}

void KLUIProc::init(QQmlContext *ctx)
{    
    m_pCate         = new CategoryModel;
    m_pCateItem     = new CateItemModel;
    m_pChipItem     = new ChipItemModel(false);
    m_pChipItemPlay = new ChipItemModel(true);

    KLDataProc::instance()->initAlbum(m_pCate, m_pCateItem, m_pChipItem, m_pChipItemPlay);

    m_pBDCTab       = new CategoryModel;
    m_pBDCArea      = new CategoryModel;
    m_pBDCItem      = new CateItemModel;

    KLDataProc::instance()->initBroadcast(m_pBDCTab, m_pBDCArea, m_pBDCItem);

    m_pCollect      = new CollectModel;

    // album
    ctx->setContextProperty("cateItemModel", m_pCateItem);
    ctx->setContextProperty("cateModel", m_pCate);
    ctx->setContextProperty("chipList", m_pChipItem);

    //player
    ctx->setContextProperty("playList", m_pChipItemPlay);

    //broadcast
    ctx->setContextProperty("bdcTab", m_pBDCTab);
    ctx->setContextProperty("bdcArea", m_pBDCArea);
    ctx->setContextProperty("bdcItem", m_pBDCItem);


    //detail
    ctx->setContextProperty("detailObject", DetailQobject::instance());

    //collect
    ctx->setContextProperty("collectList", m_pCollect);


    // view switch
    ctx->setContextProperty("stack", m_pViewStack);    
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
    int cur = MediaServiceIFace::instance()->getCurrentPosition();

    Q_EMIT positionChanged(numToTimeStr(cur + mPositionBase));

    return cur;
}

int KLUIProc::qmlGetDuration()
{
    return mDuringBase ? (mDuringBase - mPositionBase) : MediaServiceIFace::instance()->getDuration();
}

void KLUIProc::qmlPlayPrev()
{
    KLDataProc::instance()->playPrev();
}

void KLUIProc::qmlPlayNext()
{
    KLDataProc::instance()->playNext();
}

void KLUIProc::qmlMainTabClick(int index)
{
    qDebug() << "main tab index=" << index;
    KLDataProc::instance()->mainTabClick(index);
}

void KLUIProc::onRecvNotify(int msg, int ext1, int ext2, const QString &str)
{
    switch (msg)
    {
    case MEDIA_SEEK_COMPLETE:
        break;
    case MEDIA_ERROR:
        qWarning() << "error info" << ext1 << ext2 << str;
        break;
    case MEDIA_PLAYBACK_COMPLETE:
        setPlayState(3);
        KLDataProc::instance()->playNext();
        break;
    case MEDIA_PREPARED:
        qmlStart();
        KLDataProc::instance()->showPlayingInfo();
        // qDebug() << "Time Base: " << mPositionBase << mDuringBase;
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
    {
        int durStr = ext1;
        if (mDuringBase)
        {
            ext1   = mDuringBase - mPositionBase;
            durStr = mDuringBase;
        }
        // qDebug() << "Time Base: " << mPositionBase << mDuringBase << ext1;
        Q_EMIT durationChanged(ext1, numToTimeStr(durStr));
        break;    
    }
    case MEDIA_SUBTITLE_DATA:
        break;
    default:
        qDebug() << "msg=" << msg << ext1 << ext2 << str;
        break;
    }
}

QString KLUIProc::numToTimeStr(int num)
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

bool KLUIProc::canSeek() const
{
    return mCanSeek;
}

void KLUIProc::setCanSeek(bool canSeek)
{
    if (mCanSeek != canSeek)
    {
        mCanSeek = canSeek;
        Q_EMIT canSeekChanged();
    }
}

void KLUIProc::setSliderBase(int cur, int dur)
{
    mPositionBase = cur;
    mDuringBase   = dur;
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

bool KLUIProc::pushNew(const QString &url)
{
    return m_pViewStack->push(url);
}

void KLUIProc::viewAlbumBDCSwitch(const QString &url)
{
    m_pViewStack->setAlbumSource(url);
}

void KLUIProc::viewBDCItemAreaSwicth(const QString &url)
{
    m_pViewStack->setBdcSource(url);
}

void KLUIProc::setSourceUrl(const char *url)
{
    MediaServiceIFace::instance()->setFile(url);
}

void MediaNotify::notify(int msg, int ext1, int ext2, const char *str)
{
    Q_EMIT gInstance->recvNotify(msg, ext1, ext2, str);
}
