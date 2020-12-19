#ifndef MEDIA_HMI_CTRL_H
#define MEDIA_HMI_CTRL_H

#include <QObject>
#include <QImage>
#include <media_iface_common.h>
#include "media_file_internal.h"

class QTimer;
class QQuickView;
class ImageProvider;
class MediaFileModel;
class MediaHMICtrl : public QObject
{
    Q_OBJECT

    Q_PROPERTY(int playStatus READ playStatus WRITE setPlayStatus NOTIFY playStatusChanged)

    Q_PROPERTY(bool canSeek READ canSeek WRITE setCanSeek NOTIFY canSeekChanged)

    Q_PROPERTY(int mediaType READ mediaType  WRITE setMediaType  NOTIFY mediaTypeChanged)
    Q_PROPERTY(int viewIndex READ viewIndex WRITE setViewIndex NOTIFY viewIndexChanged)

    Q_PROPERTY(QString videoName READ videoName WRITE setVideoName NOTIFY videoNameChanged)

    Q_PROPERTY(QString musicName READ musicName WRITE setMusicName NOTIFY musicNameChanged)
    Q_PROPERTY(QString musicAuther READ musicAuther WRITE setMusicAuther NOTIFY musicAutherChanged)
    Q_PROPERTY(QString musicAlbum READ musicAlbum WRITE setMusicAlbum NOTIFY musicAlbumChanged)
    Q_PROPERTY(QString musicNumList READ musicNumList WRITE setMusicNumList NOTIFY musicNumListChanged)

    Q_PROPERTY(QString imageName READ imageName WRITE setImageName NOTIFY imageNameChanged)

    Q_PROPERTY(QString cpuTemp READ cpuTemp WRITE setCpuTemp NOTIFY cpuTempChanged)
public:
    static MediaHMICtrl* instance()
    {
        static MediaHMICtrl *i = []
        {
            return new MediaHMICtrl;
        }();
        return i;
    }
    enum MsgType
    {
        USB_TRAVEL_FILE_START,      //开始浏览U盘文件
        USB_TARVEL_FILE_END,        //浏览完成
    };

    enum {
        FILE_VIEW,
        MUSIC_VIEW,
        VIDEO_VIEW,
        IMAGE_VIEW,
    };
    MediaHMICtrl();

    void init(QQuickView *view);    
    void hideMsgBox();
    void sliderChanged(int current, int duration);
    void setMediaFile(int type, Media::vector_s &lst);
    void setImage(const QImage &img);
    void deviceRemove();
    void setCurrentIndex(int mediaType, int index, const char *filename);

    inline MediaFileModel *images() const { return mImages; }
    inline MediaFileModel *musics() const { return mMusics; }
    inline MediaFileModel *videos() const { return mVideos; }

    //property
    bool playStatus() const;
    void setPlayStatus(int playStatus);

    bool canSeek() const;
    void setCanSeek(bool canSeek);

    QString videoName() const;
    void setVideoName(const QString &videoName);

    QString musicName() const;
    void setMusicName(const QString &musicName);

    QString musicAuther() const;
    void setMusicAuther(const QString &musicAuther);

    QString musicAlbum() const;
    void setMusicAlbum(const QString &musicAlbum);

    QString musicNumList() const;
    void setMusicNumList(const QString &musicNumList);

    int mediaType() const;
    void setMediaType(int mediaType);

    QString imageName() const;
    void setImageName(const QString &imageName);

    int viewIndex() const;
    void setViewIndex(int viewIndex);

    void setPlayAction(int playAction);

    QString cpuTemp() const;
    void setCpuTemp(const QString &cpuTemp);

public Q_SLOTS:
    void btnMenuClicked();
    void btnPlayPause();
    void btnPreviousClicked();
    void btnNextClicked();
    void btnModeClicked();
    void btnWallpaperClicked();
    void btnRotateClicked();
    void btnScaleClicked();
    void btnBackClicked();
    void qmlSpaceClicked();
    void qmlBtnYesPress(int type);          //按钮“是/确认”被点击
    void qmlBtnNoPress(int type);           //按钮“否/取消”被点击
    void qmlSeekingSwitch();
    void qmlPlaySeek(int value);
    int  qmlCurrentPos() { return mCurPosition; }
    int  qmlCurrentDur() { return mCurDuring; }
    //消息隐藏
    void onMsgHide();
    void onShowMsgBox(int msgType, int timeout = 1500);

    // 播放控制反馈回来的处理信息
    void onRecvNotify(int msg, int ext1, int ext2, const QString &str);

    void onDeviceRemove();

    //播放控制命令
    void qmlStart();
    void qmlPlay();
    void qmlStop();
    void qmlPause();
    void qmlReset();
    void qmlPlayPause();
    void qmlSetFile(const QString &file);

    //bool isPlaying();
    void qmlSeekTo(long msec, int mode);
    int  qmlGetCurrentPosition();
    int  qmlGetDuration();

    void onSliderTimer();
    void onCpuTempTimer();
Q_SIGNALS:
    //property changed message
    void playStatusChanged();    
    void canSeekChanged();
    void videoNameChanged();
    void musicNameChanged();
    void musicAutherChanged();
    void musicAlbumChanged();
    void musicNumListChanged();
    void mediaTypeChanged();
    void imageNameChanged();
    void viewIndexChanged();
    void cpuTempChanged();

    //message box message
    //消息框通知显示, 消息框类型    消息参数            标题提示                消息显示内容
    void showMsg(int boxType, int msgArgs, QString const &title, QString const &content);
    //通知消息隐藏
    void hideMsg();
    void showMsgBox(int msgType, int timeout = 1500);
    //进度条跳动
    void sliderValue(int current, int duration);
    //刷新音乐文件的图片
    void flushPicture();
    // usb device remove
    void deviceRemoveSig();

    // qml
    void durationChanged(int duration);
    void positionChanged(QString const &curPos, QString const &durStr);
    void cacheDataChanged(int curPos);
    void playingInfo(QString const &name, QString const &desc);

private:
    int  mPlayState;  //空闲, 播放, 暂停, 停止
    bool mCanSeek;
    int  mMediaType;
    int  mViewIndex;

    QString mVideoName;

    QString mMusicName;
    QString mMusicAuther;
    QString mMusicAlbum;
    QString mMusicNumList;

    QString mImageName;
    QString mCpuTemp;

    bool    mSliderClick;
    int     mMsgType;
    int     mCurPosition;
    int     mCurDuring;
    int     mPlayAction;
    int     mPicRotate;
    int     mPicScale;

    QTimer* mMsgTimer;
    QTimer* mSliderTimer;
    QTimer* mCpuTimer;
    ImageProvider * mProvider;
    MediaFileModel* mVideos;
    MediaFileModel* mMusics;
    MediaFileModel* mImages;
    struct {
        MediaFileModel* md;
        int viewIndex;
    }mMedias[3];

    //  两变量用来处理广播时，时间进度问题
    int               mDuringBase;
    int               mPositionBase;
    int               mCacheValue;

    QString numToTimeStr(int num);
};

#define mediaApp (MediaHMICtrl::instance())
#endif // MEDIA_HMI_CTRL_H
