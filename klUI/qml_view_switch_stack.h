#ifndef QML_VIEW_SWITCH_STACK_H
#define QML_VIEW_SWITCH_STACK_H

#include <QObject>
#include <util/vector_table.h>

class ViewSwitchStack : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString source
               READ source
               WRITE setSource
               NOTIFY sourceChanged)

    Q_PROPERTY(bool isShowReturn READ isShowReturn WRITE setIsShowReturn NOTIFY isShowReturnChanged)
public:
    ViewSwitchStack();

    QString source() { return mCurrentSource; }

    bool push(QString const &url);

    void pop();

    bool isShowReturn() const;
    void setIsShowReturn(bool isShowReturn);

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
private:
    QString              mCurrentSource;
    bool                 mIsShowReturn;
    VectorTable<QString> mStack;
};

#endif // QML_VIEW_SWITCH_STACK_H
