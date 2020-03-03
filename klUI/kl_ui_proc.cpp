#include "model/category_model.h"
#include "model/cate_item_model.h"
#include "model/chip_item_model.h"
#include "model/collect_model.h"
#include "iface/media_service_i_face.h"
#include "iface/media_iface_common.h"
#include "model/kl_local_data_proc.h"
#include "model/kl_data_proc.h"
#include "model/detail_qobject.h"
#include "kl_search_manage.h"
#include "qml_view_switch_stack.h"
#include "current_backup.h"
#include "kl_ui_proc.h"
#include <QDebug>

extern KLUIProc *gInstance;

KLUIProc::KLUIProc()
    : QObject()
    , mPlayState(0)
    , mCanSeek(true)
    , mDuringBase(0), mPositionBase(0)
    , mCurPosition(0), mCurDuring(0)
    , m_pViewStack(new ViewSwitchStack)
{
    connect(this, SIGNAL(recvNotify(int,int,int,QString)), this, SLOT(onRecvNotify(int,int,int,QString)));
    connect(this, SIGNAL(searchProc(int,int,long)), this, SLOT(onSearchProc(int,int,long)), Qt::QueuedConnection);
}

KLUIProc::~KLUIProc()
{
    delete m_pCate;
    delete m_pCateItem;
    delete m_pChipItem;
    delete m_pChipItemPlay;

    delete m_pBDCTab;
    delete m_pBDCArea;
    delete m_pBDCItem;

    delete m_pCollect;
    delete m_pDownload;
    delete m_pHistory;
}

void KLUIProc::init(QQmlContext *ctx)
{    
    m_pCate         = new CategoryModel;
    m_pCateItem     = new CateItemModel;
    m_pChipItem     = new ChipItemModel();
    m_pChipItemPlay = new ChipItemModel();

    connect(m_pCate, SIGNAL(dataLoadOver(long)), this, SLOT(onAlbumTabLoadOver(long)));
    KLDataProc::instance()->initAlbum(m_pCate, m_pCateItem, m_pChipItem, m_pChipItemPlay);

    m_pBDCTab       = new CategoryModel;
    m_pBDCArea      = new CategoryModel;
    m_pBDCItem      = new CateItemModel;

    KLDataProc::instance()->initBroadcast(m_pBDCTab, m_pBDCArea, m_pBDCItem);

    m_pCollect      = new CollectModel;
    m_pDownload     = new CollectModel;
    m_pHistory      = new CollectModel;

    LocalDataProc::instance()->initLocal(m_pCollect, m_pDownload, m_pHistory);

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
    ctx->setContextProperty("loadList", m_pDownload);
    ctx->setContextProperty("historyList", m_pHistory);

    // view switch
    ctx->setContextProperty("stack", m_pViewStack);    

    qDebug() << "--------Start recovery-------";
    CurrentBackup::instance()->recoveryCurrent();
    CurrentBackup::instance()->playPrevRecordInfo();
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
    Q_EMIT playingInfo("", "");
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
    mCurPosition = MediaServiceIFace::instance()->getCurrentPosition();

    if (mCurDuring < 1)
    {
        qmlGetDuration();
    }
    Q_EMIT positionChanged(numToTimeStr(mCurPosition + mPositionBase),
                           numToTimeStr(mCurDuring + mDuringBase));

    // qDebug() << mCurPosition << mCurDuring;
    return mCurPosition < 0 ? 0 : mCurPosition;
}

int KLUIProc::qmlGetDuration()
{    
    mCurDuring =  mDuringBase ? (mDuringBase - mPositionBase) : MediaServiceIFace::instance()->getDuration();

    if (mCurDuring >= mCacheValue)
    {
        Q_EMIT cacheDataChanged(mCacheValue);
    }
    return mCurDuring < 1 ? 1 : mCurDuring;
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

    if (1 == index)
    {
        m_pViewStack->setSource("qrc:/self/KlDlgOptionView.qml");
    } else
    {
        m_pViewStack->setSource("qrc:/CategoryView.qml");
    }
}

void KLUIProc::qmlSelfTabClick(int index)
{
    // qDebug() << "------------" << index;
    KLDataProc::instance()->selfTabClick(index);
}

int KLUIProc::qmlGetSelfTabIndex()
{
    return KLDataProc::instance()->getSelfTabIndex();
}

void KLUIProc::qmlReloadErrObject()
{
    KLDataProc::instance()->reloadErrObject();
}

void KLUIProc::onRecvNotify(int msg, int ext1, int ext2, const QString &str)
{
    switch (msg)
    {
    case MEDIA_SEEK_COMPLETE:
        break;
    case MEDIA_ERROR:
        qWarning() << "error info" << ext1 << ext2 << str;
        KLDataProc::instance()->autoPlayNext();
        break;
    case MEDIA_PLAYBACK_COMPLETE:
        setPlayState(3);
        Q_EMIT positionChanged("00:00:00", "00:00:00");
        KLDataProc::instance()->autoPlayNext();
        break;
    case MEDIA_PREPARED:
        qmlStart();
        mPositionBase = 0;
        mDuringBase   = 0;
        mCurDuring    = 0;
        Q_EMIT durationChanged(0);
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

void KLUIProc::onSearchProc(int type, int index, long searchPtr)
{
    qDebug() << "search proc" << type << index << searchPtr;
    switch (type)
    {
    case 1:
    {
        KLDataProc::instance()->localItemPlay(CURREN_PLAY_SOURCE_CLIENT_SEARCH_LIST,
                                              index, kl::SearchManage::instance()->genUnion(reinterpret_cast<kl::VoiceSearchAll *>(searchPtr)));

        kl::SearchManage::instance()->setCurSearch(reinterpret_cast<kl::VoiceSearchAll *>(searchPtr));
        break;
    }
    case 2:
        KLDataProc::instance()->playNext();
        break;
    case 3:
        KLDataProc::instance()->playPrev();
        break;
    case 4:
        KLDataProc::instance()->chipPlayThirdClick(index);
        break;
    default:
        break;
    }
}

void KLUIProc::onAlbumTabLoadOver(long pUnion)
{
    qDebug() << "album tab load over, and need load first " << pUnion;
    KLDataProc::instance()->detailLoadAlbumInfo();
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

bool KLUIProc::isAudioView()
{
    return m_pViewStack->source() == "qrc:/CateItemInfoView.qml";
}

void KLUIProc::setSourceUrl(const char *url)
{
    // qDebug() << url;
    MediaServiceIFace::instance()->setFile(url);
}
