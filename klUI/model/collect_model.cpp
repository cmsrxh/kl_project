#include "kl_data_proc.h"
#include "collect_model.h"
#include <QDebug>

#define COLLECT_ITEM_NAME            Qt::UserRole
#define COLLECT_ITEM_ISCOLLECT      (Qt::UserRole+1)

CollectModel::CollectModel()
    : QAbstractListModel(NULL)
{
    roles[COLLECT_ITEM_NAME]      = "collectName";
    roles[COLLECT_ITEM_ISCOLLECT] = "isCollect";
#if 1
    CollectNode *tmp = new CollectNode;

    tmp->name = "ABC";

    mVec.push_back(tmp);
    mVec.push_back(tmp);
    mVec.push_back(tmp);
    mVec.push_back(tmp);
    mVec.push_back(tmp);
    mVec.push_back(tmp);
    mVec.push_back(tmp);
    mVec.push_back(tmp);

#endif
}

int CollectModel::rowCount(const QModelIndex &) const
{
    return mVec.size();
}

QVariant CollectModel::data(const QModelIndex &index, int role) const
{
    int i = index.row();
    if ( i < 0 || i >= (int)mVec.size())
    {
        return QVariant();
    }

    if (COLLECT_ITEM_NAME == role)
    {
        return QStringFromByteString(mVec[index.row()]->name);
    } else if (COLLECT_ITEM_ISCOLLECT == role)
    {
        return true;
    }

    return QVariant();
}

void CollectModel::resetAll()
{
    beginResetModel();
    endResetModel();
}

QHash<int, QByteArray> CollectModel::roleNames() const
{
    return roles;
}

int CollectModel::collectId() const
{
    return mCollectId;
}

void CollectModel::setCollectId(int collectId)
{
    mCollectId = collectId;
    Q_EMIT collectIdChanged(collectId);
}

void CollectModel::qmlCollectThirdLevelClick(int index)
{
    qDebug() << index;

    setCollectId(index);
}

void CollectModel::qmlCollectThirdLevelEnableClick(int index, bool enable)
{
    qDebug() << index << enable;
}
