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

public:
    KLUIProc();

    void init(QQmlContext *);

    int  playState() const;
    void setPlayState(int playState);

    void pushNew(QString const &url);

    void setSourceUrl(const char *url);
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
    //
    void onRecvNotify(int msg, int ext1, int ext2, const QString &str);

Q_SIGNALS:
    void recvNotify(int msg, int ext1, int ext2, const QString &str);

    // property signal
    void playStateChanged();

    // qml
    void durationChanged(int duration);
    void playingInfo(QString const &name, QString const &desc);

private:
    int               mPlayState;  //空闲=0, 播放=1, 暂停=2, 停止=3
    ViewSwitchStack  *m_pViewStack;

    CategoryModel    *m_pCate;
    CateItemModel    *m_pCateItem;
    ChipItemModel    *m_pChipItem;
};

#endif // KL_UI_PROC_H
