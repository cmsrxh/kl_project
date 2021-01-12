#ifndef PAGES_MANAGER_H
#define PAGES_MANAGER_H

#include <QObject>
#include <global/win_global_common.h>

class QQuickView;
class PagesManager : public QObject
{
    Q_OBJECT

    Q_PROPERTY(bool mainViewEnable READ mainViewEnable NOTIFY mainViewEnableChanged)

    Q_PROPERTY(bool backgroundEnable READ backgroundEnable NOTIFY backgroundEnableChanged)
public:
    static PagesManager *instance()
    {
        static PagesManager *i = [] { return new PagesManager; } ();
        return i;
    }

    void init(QQuickView *view);

    bool mainViewEnable() const;
    void setMainViewEnable(bool mainViewEnable);

    bool backgroundEnable() const;

public Q_SLOTS:
    //! 开始窗口的切换
    void start(int win, long arg = 0);

    //! 窗口回撤(后退)
    void back();

    //! 窗口创建失败,通知到当前管理类
    void onCreateFailed(int winType, QString const &reason);

Q_SIGNALS:
    void mainViewEnableChanged();
    void backgroundEnableChanged();

    //! 通知page_qml_item实例作出窗口切换动作
    //! @warning 内部使用
    void notifyPageId(WinSwitchInfo const &info);
    void notifyBack();

    //! 向外发送窗口发生了切换操作
    //! @warning 提供给外部使用用于作出各种处理
    void pageChanged(WinSwitchInfo const &prevPage, WinSwitchInfo const &newPage);

private:
    PagesManager();

    bool        mMainViewEnable;


    QQuickView *mViewHandler;
    QString     mMainUrl;
};

#endif // PAGES_MANAGER_H
