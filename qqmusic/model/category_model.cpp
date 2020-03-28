#include "category_model.h"
#include "category_union.h"
#include "data_proc.h"
#include <QDebug>

#define CATEGORY_NAME Qt::UserRole

CategoryModel::CategoryModel()
    : QAbstractListModel()
    , m_pCateUnion(nullptr)
{
    roles[CATEGORY_NAME] = "ctgname";

    connect(this, SIGNAL(dataLoadOver(long)), this, SLOT(onLoadOver(long)));

#if 0
    MusicCateUnion *cate = new MusicCateUnion;

    cate->name = "ABC";

    mVec.push_back(cate);
    mVec.push_back(cate);
    mVec.push_back(cate);
    mVec.push_back(cate);
    mVec.push_back(cate);
    mVec.push_back(cate);
    mVec.push_back(cate);
    mVec.push_back(cate);
    mVec.push_back(cate);
    mVec.push_back(cate);
    mVec.push_back(cate);
    mVec.push_back(cate);
    mVec.push_back(cate);
    mVec.push_back(cate);
    mVec.push_back(cate);
    mVec.push_back(cate);

#endif
}

int CategoryModel::rowCount(const QModelIndex &) const
{
    return mVec.size();
}

QVariant CategoryModel::data(const QModelIndex &index, int role) const
{
    if ( index.row() < 0 || index.row() >= (int)mVec.size())
    {
        return QVariant();
    }

    if (CATEGORY_NAME == role)
    {
        return mVec[index.row()]->name.string();
    }

    return QVariant();
}

void CategoryModel::resetAll()
{
    beginResetModel();
    endResetModel();
}

int CategoryModel::curIndex() const
{

}

void CategoryModel::onLoadOver(long ptr)
{
    if ((long)m_pCateUnion != ptr || !m_pCateUnion)
    {
        qWarning() << "Current is not need.";
        return;
    }
    qDebug() << "Category load over.";

    m_pCateUnion->onLoadOver(mVec);

    beginResetModel();
    endResetModel();
}

void CategoryModel::qmlCateTabClick(int index)
{
    //qDebug() << "CateTabClick index =" << index;
    DataProc::instance()->albumFirstClick(index);
}

QHash<int, QByteArray> CategoryModel::roleNames() const
{
    return roles;
}

