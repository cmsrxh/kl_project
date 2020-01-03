#ifndef COLLECT_MODEL_H
#define COLLECT_MODEL_H

#include "kl_ui_data_union.h"
#include <QAbstractListModel>
#include <QtQml>

class CollectModel : public QAbstractListModel
{
    Q_OBJECT

    Q_PROPERTY(int collectId READ collectId WRITE setCollectId NOTIFY collectIdChanged)
public:
    explicit CollectModel();

    int rowCount(const QModelIndex &parent = QModelIndex()) const;

    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;

    void resetAll();

    int  collectId() const;
    void setCollectId(int collectId);

signals:
    void collectIdChanged(int collectId);

public slots:
    void qmlCollectThirdLevelClick(int index);

    void qmlCollectThirdLevelEnableClick(int index, bool enable);
protected:
    QHash<int, QByteArray> roleNames() const;

private:
    int                         mCollectId;
    QHash<int, QByteArray>      roles;
    VectorTable<CollectNode *>  mVec;
};

#endif // COLLECT_MODEL_H
