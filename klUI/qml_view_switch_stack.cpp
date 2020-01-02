#include "qml_view_switch_stack.h"
#include <QDebug>

ViewSwitchStack::ViewSwitchStack()
    : mCurrentSource("qrc:/CategoryView.qml")
    , mAlbumSource("qrc:/CateItemListView.qml") //"bdc/KlInlineBroadcast.qml"
    , mBdcSource("qrc:/bdc/KlInlineChannelView.qml") // KlInlineAreaView.qml
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

    if ("qrc:/CategoryView.qml" == url)
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
        setSource("qrc:/CategoryView.qml");
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

QString ViewSwitchStack::bdcSource() const
{
    return mBdcSource;
}

void ViewSwitchStack::setBdcSource(const QString &bdcSource)
{
    if (mBdcSource != bdcSource)
    {
        mBdcSource = bdcSource;
        Q_EMIT bdcSourceChanged(bdcSource);
    }
}

QString ViewSwitchStack::albumSource() const
{
    return mAlbumSource;
}

void ViewSwitchStack::setAlbumSource(const QString &albumSource)
{
    mAlbumSource = albumSource;
    Q_EMIT albumSourceChanged(albumSource);
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
