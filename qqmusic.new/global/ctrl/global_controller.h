#ifndef GLOBAL_CONTROLLER_H
#define GLOBAL_CONTROLLER_H

#include <global/win_global_common.h>
#include <QObject>

class QTimer;
class QQuickView;
class GlobalControls : public QObject
{
    Q_OBJECT

private:
    explicit GlobalControls(QObject *parent = 0);

public:
    static GlobalControls *instance()
    {
        static GlobalControls *i = [] { return new GlobalControls; } ();
        return i;
    }
    ~GlobalControls();

    void init(QQuickView *view);

    //! 窗口最小化
    void windowMinimize();

    //! 程序退出,用户点击右上角的X
    void programExit();

Q_SIGNALS:

public Q_SLOTS:
    void onPageChanaged(WinSwitchInfo const &prevPage, WinSwitchInfo const &newPage);

private:
    QQuickView *mViewHandler;
};

#endif // GLOBAL_CONTROLLER_H
