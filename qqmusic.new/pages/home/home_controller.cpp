#include <QtQml>
#include <QDebug>
#include <QQmlContext>
#include <QQuickView>
#include <QQmlEngine>
#include <QQuickItem>
#include <QGuiApplication>
#include "global/manage/pages_manager.h"
#include "home_controller.h"


class StatusPrivate
{
public:
    enum {
        STATE_NORMAL,
        STATE_PRESS_START,
        STATE_SEED_PRESS,
        STATE_SEED_MOVE,
    };
    StatusPrivate()
        : store(QEvent::None,
                QPointF(0, 0),
                Qt::LeftButton, Qt::LeftButton, Qt::NoModifier)
        , stateProc(STATE_NORMAL)
    {}

    QEvent *newEvent(QEvent *event)
    {
        QMouseEvent *old = dynamic_cast<QMouseEvent *>(event);
        QMouseEvent *e = new QMouseEvent(*old);

        return e;
    }

    QMouseEvent *newMouse (QEvent::Type type, qreal x, qreal y)
    {
        return new  QMouseEvent(type, QPointF(x, y),
                                Qt::LeftButton, Qt::LeftButton, Qt::NoModifier);
    }

    QEvent *newStore()
    {
        return new QMouseEvent(store);
    }

    bool maybeSendRelease()
    {
        return stateProc >= STATE_SEED_PRESS;
    }
    bool maybeSendPress()
    {
        return STATE_SEED_PRESS == stateProc;
    }
    bool maybeSendMove()
    {
        return STATE_SEED_MOVE == stateProc;
    }

    void sendPressOver()
    {
        stateProc = STATE_SEED_MOVE;
    }

    void pressEvent(QEvent *event)
    {
        QMouseEvent *old = dynamic_cast<QMouseEvent *>(event);
        store = *old;

        stateProc = STATE_PRESS_START;
    }

    void releaseEvent()
    {
        stateProc = STATE_NORMAL;
    }

    void moveEvent(QMouseEvent *event)
    {
        if (STATE_PRESS_START == stateProc)
        {
            // 在状态事件中,只处理向下移动的事件,上移不需要处理
            int diff = event->y() - store.y();

            qDebug() << "difference =" << diff;
            if (diff > 8)
            {
                stateProc = STATE_SEED_PRESS;
            }
        }
    }

    QMouseEvent store; // 状态栏按下位置事件记录
    int         stateProc;
};

HomeController::HomeController(QObject *parent)
    : QObject(parent)
    , mTopBarVisible(true)
    , mDockBarVisible(true)
    , statusEvents(new StatusPrivate)
    , m_pStatusViewMouseHandler(nullptr)
    , m_pStatusBarMouseHandler(nullptr)
{

    // connect(ActivityManager::getInstance(), SIGNAL(activityChanged(Activity*,Activity*)), this, SLOT(onActivityChanged(Activity*,Activity*)));
}

bool HomeController::dockBarVisible() const
{
    return mDockBarVisible;
}

void HomeController::setDockBarVisible(bool dockBarVisible)
{
    mDockBarVisible = dockBarVisible;
    Q_EMIT dockBarVisibleChanged();
}

bool HomeController::topBarVisible() const
{
    return mTopBarVisible;
}

void HomeController::setTopBarVisible(bool topBarVisible)
{
    mTopBarVisible = topBarVisible;
    Q_EMIT topBarVisibleChanged();
}

void HomeController::qmlSetStatusViewMouseHandler(QQuickItem *item)
{
    m_pStatusViewMouseHandler = item;
}

void HomeController::qmlSetStatusBarMouseHandler(QQuickItem *item)
{
    item->installEventFilter(this);

    m_pStatusBarMouseHandler = item;
}

void HomeController::qmlPowerModeClicked()
{
    qDebug() << "power mode clicked, not used.";
}

void HomeController::qmlBtSwitchClicked()
{
    qDebug() << "bt btn clicked, not used.";

}

void HomeController::qmlHomeOneBtn()
{
    PagesManager::instance()->start(WIN_TYPE_ONE);
}

void HomeController::qmlHomeTwoBtn()
{
    PagesManager::instance()->start(WIN_TYPE_TWO);
}

void HomeController::qmlHomeThreeBtn()
{
    PagesManager::instance()->start(WIN_TYPE_THREE);
}

bool HomeController::eventFilter(QObject *watched, QEvent *event)
{
    // 状态栏的鼠标事件监听
    if (m_pStatusBarMouseHandler == watched)
    {
        // qDebug() << event->type();
        // GEN_Printf(LOG_DEBUG, "status bar, type: %d", event->type());
        // GEN_Printf(LOG_DEBUG, "status view handler: %p", m_pStatusViewMouseHandler);

        switch (event->type()) {
        case QEvent::MouseButtonPress:
            // 状态栏点击,尝试显示状态下拉界面,需要重新向子界面(状态 下拉界面)上报鼠标按下事件
            // 所以先记录这个按下点位置
            statusEvents->pressEvent(event);
            break;
        case QEvent::MouseButtonRelease:
            if ((m_pStatusViewMouseHandler) && statusEvents->maybeSendRelease())
            {
                qApp->postEvent(m_pStatusViewMouseHandler, statusEvents->newEvent(event));
            }
            statusEvents->releaseEvent();
            break;
        case QEvent::MouseMove:
        {
            statusEvents->moveEvent(dynamic_cast<QMouseEvent *>(event));

            if ((m_pStatusViewMouseHandler))
            {
                if (statusEvents->maybeSendMove())
                {
                    qApp->postEvent(m_pStatusViewMouseHandler, statusEvents->newEvent(event));
                } else if (statusEvents->maybeSendPress())
                {
                    qApp->postEvent(m_pStatusViewMouseHandler, statusEvents->newStore());
                    statusEvents->sendPressOver();
                }
            }
            break;
        }
        default:
            break;
        }
    }

    return QObject::eventFilter(watched, event);
}

