#ifndef KL_UI_PROC_H
#define KL_UI_PROC_H

#include "qml_view_switch_stack.h"

class CategoryModel;
class CateItemModel;
class ChipItemModel;
class QQmlContext;
class ViewSwitchStack;
class KLUIProc : public QObject
{
    Q_OBJECT
    Q_PROPERTY(int playState READ playState WRITE setPlayState NOTIFY playStateChanged)

    Q_PROPERTY(bool canSeek READ canSeek WRITE setCanSeek NOTIFY canSeekChanged)
public:
    KLUIProc();

    void init(QQmlContext *);

    int  playState() const;
    void setPlayState(int playState);

    bool pushNew(QString const &url);

    void setSourceUrl(const char *url);
    bool canSeek() const;
    void setCanSeek(bool canSeek);

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
    //
    void onRecvNotify(int msg, int ext1, int ext2, const QString &str);

Q_SIGNALS:
    void recvNotify(int msg, int ext1, int ext2, const QString &str);

    // property signal
    void playStateChanged();
    void canSeekChanged();

    // qml
    void durationChanged(int duration);
    void playingInfo(QString const &name, QString const &desc);
    void albumInfoName(QString const &name);
    void albumInfoDesc(QString const &imgUrl, QString const &desc, const QString &hostName);
private:
    int               mPlayState;  //空闲=0, 播放=1, 暂停=2, 停止=3
    bool              mCanSeek;
    ViewSwitchStack  *m_pViewStack;

    CategoryModel    *m_pCate;
    CateItemModel    *m_pCateItem;
    ChipItemModel    *m_pChipItem;
    ChipItemModel    *m_pChipItemPlay;
};

#endif // KL_UI_PROC_H
