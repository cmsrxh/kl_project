#ifndef LOAD_MODEL_BASE_H
#define LOAD_MODEL_BASE_H

#include "app_load_common.h"
#include <QAbstractListModel>
#include <QtQml>

class LoadModelBase : public QAbstractListModel, public UINotifyIface
{
    Q_OBJECT
public:
    LoadModelBase();
    ~LoadModelBase();

    //! 播放列表标签数据，下载完成，只能以消息的形式发送出来
    void dataPrepare();

    //! 播放列表标签数据，下载出现错误了
    void errorInfo(int, const ByteString &);

    void reset();
Q_SIGNALS:
    void sigLoadOver();
    void sigLoadFailed(int type, QString const &errStr);

public:
    QHash<int, QByteArray> roleNames() const
    {
        return roles;
    }

    QHash<int, QByteArray>  roles;
};

#endif // LOAD_MODEL_BASE_H
