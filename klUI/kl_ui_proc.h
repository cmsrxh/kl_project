#ifndef KL_UI_PROC_H
#define KL_UI_PROC_H

#include "qml_view_switch_stack.h"

class CategoryModel;
class CateItemModel;
class ChipItemModel;
class QQmlContext;
class CollectModel;
class ViewSwitchStack;
class KLUIProc : public QObject
{
    Q_OBJECT
    Q_PROPERTY(int playState READ playState WRITE setPlayState NOTIFY playStateChanged)

    Q_PROPERTY(bool canSeek READ canSeek WRITE setCanSeek NOTIFY canSeekChanged)
public:
    enum MsgTipBoxType
    {
//        case 0: return nullEmpty;
        nullEmpty,
//        case 1: return generalBox;
        generalBox,
//        case 2: return msgBufferring;
        msgBufferring,
//        case 3: return twoBtnBox;
        twoBtnBox,
//        case 4: return bottomTip;
        bottomTip,
//        case 5: return failTip;
        failTip,
//        case 6: return emptyData;
        emptyData,
    };

    KLUIProc();
    ~KLUIProc();

    void init(QQmlContext *);

    int  playState() const;
    void setPlayState(int playState);

    bool pushNew(QString const &url);

    // qrc:/CateItemListView.qml
    // qrc:/bdc/KlInlineBroadcast.qml
    void viewAlbumBDCSwitch(QString const &url);

    // qrc:/bdc/KlInlineChannelView.qml
    // qrc:/bdc/KlInlineAreaView.qml
    void viewBDCItemAreaSwicth(QString const &url);

    bool isAudioView();

    void setSourceUrl(const char *url);
    bool canSeek() const;
    void setCanSeek(bool canSeek);

    void setSliderBase(int cur, int dur);

    int getCurrentPlayingPosition()
    {
        return mCurPosition;
    }
public Q_SLOTS:
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

    //
    void qmlPlayPrev();
    void qmlPlayNext();
    void qmlMainTabClick(int index);
    void qmlSelfTabClick(int index);

    int  qmlGetSelfTabIndex();

    void qmlReloadErrObject();

    // 播放控制反馈回来的处理信息
    void onRecvNotify(int msg, int ext1, int ext2, const QString &str);

    // 搜索服务端接收同步处理的事件类型
    void onSearchProc(int type, int index, long searchPtr);

    // 首页专辑标签下载完毕，表示要自动下载默认标签页下面的信息
    void onAlbumTabLoadOver(long pUnion);
Q_SIGNALS:
    void recvNotify(int msg, int ext1, int ext2, const QString &str);

    void searchProc(int type, int index, long searchPtr);

    // property signal
    void playStateChanged();
    void canSeekChanged();

    // qml
    void durationChanged(int duration);
    void positionChanged(QString const &curPos, QString const &durStr);
    void cacheDataChanged(int curPos);
    void playingInfo(QString const &name, QString const &desc);

    /**
     * @brief msgTipCateItem
     * @param boxType [in] enum MsgTipBoxType 消息框的种类
     * @param msgContent [in] 消息文本
     * @details CateItemListView.qml界面消息弹框处理
     */
    void msgTipCateItem(int boxType, QString const &msgContent);
    // main.qml
    void msgTipGlobal(int boxType, QString const &msgContent);

    // AudioList.qml
    void msgTipAudioList(int boxType, QString const &msgContent);

    //KlInlineChannelView.qml
    void msgTipBroadcast(int boxType, QString const &msgContent);

private:
    QString numToTimeStr(int num);

    int               mPlayState;  //空闲=0, 播放=1, 暂停=2, 停止=3
    bool              mCanSeek;
    //  两变量用来处理广播时，时间进度问题
    int               mDuringBase;
    int               mPositionBase;
    int               mCurPosition;
    int               mCurDuring;
    int               mCacheValue;
    ViewSwitchStack  *m_pViewStack;

    // album
    CategoryModel    *m_pCate;
    CateItemModel    *m_pCateItem;
    ChipItemModel    *m_pChipItem;

    // player
    ChipItemModel    *m_pChipItemPlay;

    // broadcast
    CategoryModel    *m_pBDCTab;
    CategoryModel    *m_pBDCArea;
    CateItemModel    *m_pBDCItem;

    //collect
    CollectModel      *m_pCollect;
    CollectModel      *m_pDownload;
    CollectModel      *m_pHistory;
};

#endif // KL_UI_PROC_H
