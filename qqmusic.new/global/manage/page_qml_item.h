#ifndef PAGE_QML_ITEM_H
#define PAGE_QML_ITEM_H

#include <QQuickItem>
#include "pages_factory.h"

class QQmlEngine;
class PageQmlItem : public QQuickItem
{
    Q_OBJECT

    Q_PROPERTY(QObject* currentItem READ currentItem WRITE setCurrentItem NOTIFY currentItemChanged)
public:
    PageQmlItem();

    QObject *currentItem() const;
    void setCurrentItem(QObject *currentItem);

    Q_INVOKABLE QObject *getNeedShowItem();
Q_SIGNALS:
    void createFailed(int winType, QString const &reason);

    // property signal
    void currentItemChanged();

public Q_SLOTS:
    //! 页面前进
    void onPageChanged(WinSwitchInfo const &info);

    //! 页面后撤
    void onBack();

    //! 当前控件构造完成之后会被调用,用来确认显示的第一个窗口
    void qmlItemOver();

    //! qml中调用,用来返回createItem创建成功之后的窗口句柄
    void setCurrentCreateResult(QObject *comp, QObject *item);
    //! qml调用,返回错误信息
    void setCurrentErrorString(QString const &errStr);

private:
    void createPage(WinSwitchInfo const &info);
    void show(PagesFactory::Node const &node, long arg);

    QQuickItem   *mCurrentItem;
    QQuickItem   *mNeedShowItem;
    PagesFactory *mFactory;

    // priv
    QObject      *mCurComp;
    QObject      *mCurItem;
    QString       mErrorString;
};

inline QObject *PageQmlItem::currentItem() const
{
    return mCurrentItem;
}

inline void PageQmlItem::setCurrentItem(QObject *currentItem)
{
    mCurrentItem = reinterpret_cast<QQuickItem *> (currentItem);
}

#endif // PAGE_QML_ITEM_H
