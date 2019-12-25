#include "cate_item_model.h"

#define CATE_ITEM_NAME     Qt::UserRole
#define CATE_ITEM_IMG_URL (Qt::UserRole+1)

CateItemModel::CateItemModel()
    : QAbstractListModel()
    , m_pCateItem(new kl::OperateList)
{
    m_pCateItem->setUINotify(this);

    roles[CATE_ITEM_NAME]    = "cateItemName";
    roles[CATE_ITEM_IMG_URL] = "imageUri";

    connect(this, SIGNAL(dataLoadOver()), this, SLOT(onLoadOver()));
}

int CateItemModel::rowCount(const QModelIndex &parent) const
{
    return mVec.size();
}

QVariant CateItemModel::data(const QModelIndex &index, int role) const
{
    if ( index.row() < 0 || index.row() >= (int)mVec.size())
    {
        return QVariant();
    }

    if (CATE_ITEM_NAME == role)
    {
        return mVec[index.row()].rname.string();
    } else if(CATE_ITEM_IMG_URL == role)
    {
        return mVec[index.row()].image.string();
    }

    return QVariant();
}

void CateItemModel::resetAll()
{
    beginResetModel();
    endResetModel();
}

void CateItemModel::dataPrepare()
{
    if (m_pCateItem->nodes().empty())
    {
        qDebug() << "CategoryAllModel is empty.";
        Q_EMIT loadError(0, "CategoryAllModel is empty.");
    } else
    {
        Q_EMIT dataLoadOver();
    }
}

void CateItemModel::errorInfo(int type, const char *err_str)
{
    qDebug() << "CategoryAllModel Error." << err_str;
    Q_EMIT loadError(type, err_str);
}

void CateItemModel::obtain()
{
    if (m_pCateItem->nodes().empty())
    {
        m_pCateItem->obtain();
    }
}

void CateItemModel::onLoadOver()
{
    m_pCateItem->nodes().genVector(mVec);

    resetAll();
}

QHash<int, QByteArray> CateItemModel::roleNames() const
{
    return roles;
}
