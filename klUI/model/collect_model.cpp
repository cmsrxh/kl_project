#include "kl_data_proc.h"
#include "collect_model.h"
#include <QDebug>

#define COLLECT_ITEM_NAME            Qt::UserRole
#define COLLECT_ITEM_ISCOLLECT      (Qt::UserRole+1)
#define COLLECT_ITEM_IMAGE_URL      (Qt::UserRole+2)
#define COLLECT_ITEM_TITLE          (Qt::UserRole+3)

CollectModel::CollectModel()
    : QAbstractListModel(NULL)
{
    roles[COLLECT_ITEM_NAME]      = "collectName";
    roles[COLLECT_ITEM_ISCOLLECT] = "isCollect";
    roles[COLLECT_ITEM_IMAGE_URL] = "pictureUrl";
    roles[COLLECT_ITEM_TITLE]     = "collectTitle";

#if 1
    CollectNode *tmp = new CollectNode;

    tmp->name = "ABC";
    tmp->parentName = "PARREN";

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

    switch (role)
    {
    case COLLECT_ITEM_NAME:
        return QStringFromByteString(mVec[index.row()]->name);
    case COLLECT_ITEM_ISCOLLECT:
        return true;
    case COLLECT_ITEM_IMAGE_URL:
        return QStringFromByteString(mVec[index.row()]->image);
    case COLLECT_ITEM_TITLE:
        return QStringFromByteString(mVec[index.row()]->parentName);
    default:
        break;
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

void CollectModel::qmlDownloadThreadLevelClick(int index)
{
    qDebug() << index;
}

void CollectModel::qmlHistoryRecordClick(int index)
{
    qDebug() << index;
}

void CollectModel::qmlHistoryItemsClear()
{
    qDebug() << "clear";
}
