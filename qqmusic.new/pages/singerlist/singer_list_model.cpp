#include "singer_list_model.h"

#define CATE_ITEM_NAME     Qt::UserRole
#define CATE_ITEM_IMG_URL (Qt::UserRole+1)
#define CATE_ITEM_COLLECT (Qt::UserRole+2)

SingerListModel::SingerListModel()
    : mCurrentIndex(-1)
{
    roles[CATE_ITEM_NAME]    = "cateItemName";
    roles[CATE_ITEM_IMG_URL] = "imageUri";
//    roles[CATE_ITEM_COLLECT] = "collect";
}

void SingerListModel::reset()
{
    beginResetModel();
    endResetModel();
}

int SingerListModel::rowCount(const QModelIndex &parent) const
{
    return mVec.size();
}

QVariant SingerListModel::data(const QModelIndex &index, int role) const
{
    int i = index.row();
    if ( i < 0 || i >= (int)mVec.size())
    {
        return QVariant();
    }

    const ItemInfo &info = mVec[i];
    switch (role) {
    case CATE_ITEM_NAME: return QStringFromByteString(info.name);
    case CATE_ITEM_IMG_URL: return QStringFromByteString(info.img);
    default:
        break;
    }

    return QVariant();
}

int SingerListModel::currentIndex() const
{
    return mCurrentIndex;
}

void SingerListModel::setCurrentIndex(int currentIndex)
{
    mCurrentIndex = currentIndex;
    Q_EMIT currentIndexChanged();
}
