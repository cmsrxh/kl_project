#include "qml_view_switch_stack.h"
#include <QDebug>

ViewSwitchStack::ViewSwitchStack()
    : mCurrentSource("CategoryView.qml")
    , mIsShowReturn(false)
{
}

bool ViewSwitchStack::push(const QString &url)
{
    if (url == mCurrentSource)
    {
        qWarning() << "Current View: " << url << mCurrentSource;
        return false;
    }

    if ("CategoryView.qml" == url)
    {
        mStack.clear();
        setIsShowReturn(false);
    } else
    {
        mStack.push_back(url);
        setIsShowReturn(true);
    }
    setSource(url);

    return true;
}

void ViewSwitchStack::pop()
{
    //把当前界面弹出，然后显示上一个界面
    mStack.pop_back();

    if (mStack.empty())
    {
        setSource("CategoryView.qml");
        setIsShowReturn(false);
        qDebug() << "pop" << mCurrentSource;
    } else
    {
        setSource(mStack.pop_back());
        setIsShowReturn(true);
        qDebug() << "pop" << mCurrentSource;
    }
}

void ViewSwitchStack::setSource(const QString &source)
{
    mCurrentSource = source;
    Q_EMIT sourceChanged(source);
}

bool ViewSwitchStack::isShowReturn() const
{
    return mIsShowReturn;
}

void ViewSwitchStack::setIsShowReturn(bool isShowReturn)
{
    if (mIsShowReturn != isShowReturn)
    {
        mIsShowReturn = isShowReturn;
        Q_EMIT isShowReturnChanged(isShowReturn);
    }
}
