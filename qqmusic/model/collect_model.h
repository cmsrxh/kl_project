#ifndef COLLECT_MODEL_H
#define COLLECT_MODEL_H

#include "ui_data_union.h"
#include <QAbstractListModel>
#include <QtQml>

class CollectModel : public QAbstractListModel
{
    Q_OBJECT
    // 表示当前播放的index
    Q_PROPERTY(int collectId READ collectId NOTIFY collectIdChanged)

    // 收听历史总数
    Q_PROPERTY(int items READ items NOTIFY itemsChanged)

    // 下载音频的内存
    Q_PROPERTY(QString loadInfo READ loadInfo NOTIFY loadInfoChanged)
public:
    explicit CollectModel();

    int rowCount(const QModelIndex &parent = QModelIndex()) const;

    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;

    void resetAll();

    int  collectId();

    int items() const
    {
        return mVec.size();
    }

    QString loadInfo();

    void itemsContentChange(int i);
signals:
    void collectIdChanged();

    void itemsChanged();

    void loadInfoChanged(QString const *loadInfo);
public slots:
    void qmlCollectThirdLevelClick(int index);

    void qmlCollectThirdLevelEnableClick(int index, bool enable);

    void qmlDownloadThreadLevelClick(int index);

    void qmlHistoryRecordClick(int index);

    void qmlHistoryItemsClear();
protected:
    QHash<int, QByteArray> roleNames() const;

private:
    QHash<int, QByteArray>            roles;
    ListTable<CollectNode>::vector mVec;
};

#endif // COLLECT_MODEL_H
