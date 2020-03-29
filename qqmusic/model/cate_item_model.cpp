#include "cate_item_union.h"
#include "cate_item_model.h"
#include "data_proc.h"

#define CATE_ITEM_NAME     Qt::UserRole
#define CATE_ITEM_IMG_URL (Qt::UserRole+1)
#define CATE_ITEM_COLLECT (Qt::UserRole+2)

CateItemModel::CateItemModel()
    : QAbstractListModel()    
    , mCurrenIndex(0), m_pUnion(nullptr)
{
    roles[CATE_ITEM_NAME]    = "cateItemName";
    roles[CATE_ITEM_IMG_URL] = "imageUri";
    roles[CATE_ITEM_COLLECT] = "collect";

    connect(this, SIGNAL(dataLoadOver(long)), this, SLOT(onLoadOver(long)));
#if 1
    SecondMenuUnion *sec = new SecondMenuUnion;
    sec->name = "absd";
    sec->img = "http:\/\/y.gtimg.cn\/music\/photo_new\/T001R150x150M000002DYpxl3hW3EP.webp";
    sec->isCollect = false;

    mVec.push_back(sec);
    mVec.push_back(sec);
    mVec.push_back(sec);
    mVec.push_back(sec);
    mVec.push_back(sec);
    mVec.push_back(sec);
    mVec.push_back(sec);
    mVec.push_back(sec);
    mVec.push_back(sec);
    mVec.push_back(sec);
    mVec.push_back(sec);
    mVec.push_back(sec);
    mVec.push_back(sec);
    mVec.push_back(sec);
    mVec.push_back(sec);
     sec = new SecondMenuUnion;
    sec->name = "sdfsasf";
    sec->img = "";
    sec->isCollect = false;
    mVec.push_back(sec);
    mVec.push_back(sec);
    mVec.push_back(sec);
    mVec.push_back(sec);
    mVec.push_back(sec);
    mVec.push_back(sec);

#endif
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
    } else if (CATE_ITEM_COLLECT == role)
    {
        return mVec[index.row()]->isCollect;
    }

    return QVariant();
}

void CateItemModel::resetAll()
{
    beginResetModel();
    endResetModel();

    Q_EMIT currenIndexChanged(0);
}

void CateItemModel::onLoadOver(long ptr)
{   
    int start = mVec.size();

    if ((long)m_pUnion != ptr || !m_pUnion)
    {
        qWarning() << "Current is not need.";
        return;
    }

    m_pUnion->onLoadOver(this);

    qDebug() << "Load over, size = " << mVec.size();

    if (0 == start)
    {
        beginResetModel();
        endResetModel();

        Q_EMIT currenIndexChanged(0);
    } else
    {
        beginInsertRows(QModelIndex(), start, mVec.size() - 1);
        endInsertRows();
    }
}

bool CateItemModel::qmlCtgNextPage()
{
    qDebug() << "Category Next Page";
    return m_pUnion->loadNextPage();
}

void CateItemModel::qmlClickCategory(int index)
{
    qDebug() << "Click Category List Item index =" << index;

    DataProc::instance()->albumSecondClick(index);
}

void CateItemModel::qmlClickBDCItem(int index, bool isInArea)
{
    qDebug() << index << isInArea;
    DataProc::instance()->bdcSecondItemClick(index, isInArea);
}

void CateItemModel::qmlClickBDCItemCollect(int index, bool isCollect)
{
    qDebug() << index << isCollect;
    DataProc::instance()->bdcSecondItemCollectClick(index, isCollect);
}

QHash<int, QByteArray> CateItemModel::roleNames() const
{
    return roles;
}

void CateItemModel::setCurrenIndex(int currenIndex)
{
    if (mCurrenIndex != currenIndex)
    {
        mCurrenIndex = currenIndex;
        Q_EMIT currenIndexChanged(currenIndex);
    }
}

bool CateItemModel::haveNext() const
{
    return m_pUnion ? m_pUnion->haveNext() : false;
}

int CateItemModel::currenIndex() const
{
    return mCurrenIndex;
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

void CateItemModel::isCollectItemContentChange(int i, bool en)
{
    if (i >= 0 && i < mVec.size())
    {
        mVec[i]->isCollect = en;
        Q_EMIT dataChanged(this->index(i), this->index(i));
    }
}

int CateItemModel::getCateType()
{
    return m_pUnion ? m_pUnion->getCateType() : -1;
}

