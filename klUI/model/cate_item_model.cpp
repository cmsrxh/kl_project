#include "kl_url/kl_type_radio_list.h"
#include "kl_url/kl_album_list.h"
#include "kl_url/kl_operate_list.h"
#include "kl_url/kl_chip_radio_list.h"
#include "cate_item_union.h"
#include "cate_item_model.h"
#include "kl_data_proc.h"

#define CATE_ITEM_NAME     Qt::UserRole
#define CATE_ITEM_IMG_URL (Qt::UserRole+1)

CateItemModel::CateItemModel()
    : QAbstractListModel()    
    , m_pUnion(nullptr)
{
    roles[CATE_ITEM_NAME]    = "cateItemName";
    roles[CATE_ITEM_IMG_URL] = "imageUri";

    connect(this, SIGNAL(dataLoadOver(long)), this, SLOT(onLoadOver(long)));
}

int CateItemModel::rowCount(const QModelIndex &) const
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
        return mVec[index.row()]->name.string();
    } else if(CATE_ITEM_IMG_URL == role)
    {
        return mVec[index.row()]->img.string();
    }

    return QVariant();
}

void CateItemModel::resetAll()
{
    beginResetModel();
    endResetModel();
}

void CateItemModel::onLoadOver(long ptr)
{   
    int start = mVec.size();

    assert(ptr == (long)m_pUnion);

    m_pUnion->onLoadOver(this);

    qDebug() << "Load over, size = " << mVec.size();

    if (0 == start)
    {
        beginResetModel();
        endResetModel();
    } else
    {
        beginInsertRows(QModelIndex(), start, mVec.size() - 1);
        endInsertRows();
    }
}

void CateItemModel::qmlCtgNextPage()
{
    qDebug() << "Category Next Page";
    m_pUnion->loadNextPage();
}

void CateItemModel::qmlClickCategory(int index)
{
    qDebug() << "Click Category List Item index =" << index;

    KLDataProc::instance()->cateItemClick(index);
}

QHash<int, QByteArray> CateItemModel::roleNames() const
{
    return roles;
}

bool CateItemModel::haveNext() const
{
    return m_pUnion->haveNext();
}

void CateItemModel::setCateItemUnion(CateItemUnion *pUnion)
{
    m_pUnion = pUnion;
}

void CateItemModel::clear()
{
    mVec.clearPtr();

    beginResetModel();
    endResetModel();
}

