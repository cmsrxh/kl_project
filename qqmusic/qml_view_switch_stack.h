#ifndef QML_VIEW_SWITCH_STACK_H
#define QML_VIEW_SWITCH_STACK_H

#include <QObject>
#include <util/vector_table.h>

class ViewSwitchStack : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString source READ source WRITE setSource NOTIFY sourceChanged)

//    Q_PROPERTY(bool isShowReturn READ isShowReturn WRITE setIsShowReturn NOTIFY isShowReturnChanged)

    Q_PROPERTY(QString albumSource READ albumSource WRITE setAlbumSource NOTIFY albumSourceChanged)

    Q_PROPERTY(QString bdcSource READ bdcSource WRITE setBdcSource NOTIFY bdcSourceChanged)
public:
    ViewSwitchStack();

    QString source() { return mCurrentSource; }

    bool push(QString const &url);

    void pop();
    void showAlbumView();

    bool isShowReturn() const;
    void setIsShowReturn(bool isShowReturn);

    QString albumSource() const;
    void setAlbumSource(const QString &albumSource);

    QString bdcSource() const;
    void setBdcSource(const QString &bdcSource);

public slots:
    void setSource(const QString& source);
    void qmlPush(QString const &url)
    {
        push(url);
    }

    void qmlPop()
    {
        pop();
    }
signals:
    void sourceChanged(const QString& source);
    void isShowReturnChanged(bool isShowReturn);

    void albumSourceChanged(const QString &albumSource);

    void bdcSourceChanged(const QString &bdcSource);
private:
    QString              mCurrentSource;
    QString              mAlbumSource;
    QString              mBdcSource;
    bool                 mIsShowReturn;
    VectorTable<QString> mStack;
};

#endif // QML_VIEW_SWITCH_STACK_H
