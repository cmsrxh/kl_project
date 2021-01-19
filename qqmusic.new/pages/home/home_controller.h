#ifndef HOMECONTROLLER_H
#define HOMECONTROLLER_H

#include <global/win_global_common.h>
#include <QObject>

class QQuickItem;
class StatusPrivate;
class HomeController : public QObject
{
    Q_OBJECT

    Q_PROPERTY(bool topBarVisible READ topBarVisible WRITE setTopBarVisible NOTIFY topBarVisibleChanged)
    Q_PROPERTY(bool dockBarVisible READ dockBarVisible WRITE setDockBarVisible NOTIFY dockBarVisibleChanged)

    Q_PROPERTY(int leftTabIndex READ leftTabIndex WRITE setLeftTabIndex NOTIFY leftTabIndexChanged)
public:
    static HomeController *instance()
    {
        static HomeController *i = [] { return new HomeController; } ();
        return i;
    }


    bool topBarVisible() const;
    void setTopBarVisible(bool topBarVisible);

    bool dockBarVisible() const;
    void setDockBarVisible(bool dockBarVisible);

    int  leftTabIndex() const;
    void setLeftTabIndex(int leftTabIndex);

Q_SIGNALS:
    // property signal
    void topBarVisibleChanged();
    void dockBarVisibleChanged();
    void leftTabIndexChanged();

    // 状态栏点击处理
    void statusBarState(bool isPressed);

public Q_SLOTS:

    //! 设置状态栏下拉列表的MouseArea句柄
    //! 下拉显示时设置,隐藏时设置为空
    void qmlSetStatusViewMouseHandler(QQuickItem *item);

    //! 顶部栏MouseArea句柄,状态栏初始化设置一次 一直有效
    void qmlSetStatusBarMouseHandler(QQuickItem *item);

    //! 顶层状态栏相关按钮切换
    void qmlMainTabClick(int i);
    //! 顶层状态栏 right
    void qmlOperate(QString const &op);

    void onPageChanaged(WinSwitchInfo const &prevPage, WinSwitchInfo const &newPage);

protected:
    //! 为状态栏重新添加事件过滤,把触摸事件重新发送状态栏下拉界面中
    bool eventFilter(QObject *watched, QEvent *event);

private:
    explicit HomeController(QObject *parent = nullptr);

    bool mTopBarVisible;    // 顶部状态栏
    bool mDockBarVisible;   // 左侧停靠栏
    int  mLeftTabIndex;

    StatusPrivate   *statusEvents;
    QQuickItem      *m_pStatusViewMouseHandler;
    QQuickItem      *m_pStatusBarMouseHandler;
};

#endif // HOMECONTROLLER_H
