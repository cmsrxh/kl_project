#ifndef COLLECT_MODEL_H
#define COLLECT_MODEL_H

#include "kl_ui_data_union.h"
#include <QAbstractListModel>
#include <QtQml>

class CollectModel : public QAbstractListModel
{
    Q_OBJECT

    Q_PROPERTY(int collectId READ collectId WRITE setCollectId NOTIFY collectIdChanged)

    Q_PROPERTY(int items READ items NOTIFY itemsChanged)

    Q_PROPERTY(QString loadInfo READ loadInfo NOTIFY loadInfoChanged)
public:
    explicit CollectModel();

    int rowCount(const QModelIndex &parent = QModelIndex()) const;

    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;

    void resetAll();

    int  collectId() const;
    void setCollectId(int collectId);

    int items() const
    {
        return mVec.size();
    }

    QString loadInfo()
    {
        return tr("共0.00M/3.12G");
    }

signals:
    void collectIdChanged(int collectId);

    void itemsChanged(int items);

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
    int                         mCollectId;
    QHash<int, QByteArray>      roles;
    VectorTable<CollectNode *>  mVec;
};

#endif // COLLECT_MODEL_H
