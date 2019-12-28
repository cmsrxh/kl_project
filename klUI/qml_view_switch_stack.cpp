#include "qml_view_switch_stack.h"
#include <QDebug>

ViewSwitchStack::ViewSwitchStack()
    : mCurrentSource("CategoryView.qml")
    , mIsShowReturn(true)
{
}

void ViewSwitchStack::push(const QString &url)
{
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
}

void ViewSwitchStack::pop()
{
    if (mStack.empty())
    {
        setSource("CategoryView.qml");
        setIsShowReturn(false);
    } else
    {
        setSource(mStack.pop_back());
        setIsShowReturn(true);
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
