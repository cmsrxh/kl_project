#include "category_all_model.h"
#include "cate_item_model.h"
#include <QDebug>

#define CATEGORY_NAME Qt::UserRole

CategoryAllModel::CategoryAllModel()
    : QAbstractListModel()
    , m_pCate(new kl::CategoryAll)
{
    m_pCate->setUINotify(this);
    roles[CATEGORY_NAME] = "ctgname";

    connect(this, SIGNAL(dataLoadOver()), this, SLOT(onLoadOver()));

    m_pCate->loadNodesFile();

    if (m_pCate->nodes().empty())
    {
        m_pCate->obtain();
    } else
    {
        m_pCate->nodes().genVector(mVec);
    }
}

int CategoryAllModel::rowCount(const QModelIndex &) const
{
    return mVec.size();
}

QVariant CategoryAllModel::data(const QModelIndex &index, int role) const
{
    if ( index.row() < 0 || index.row() >= (int)mVec.size())
    {
        return QVariant();
    }

    if (CATEGORY_NAME == role)
    {
        return mVec[index.row()].name.string();
    }

    return QVariant();
}

void CategoryAllModel::resetAll()
{
    beginResetModel();
    endResetModel();
}

void CategoryAllModel::dataPrepare()
{
    if (m_pCate->nodes().empty())
    {
        qDebug() << "CategoryAllModel is empty.";
        Q_EMIT loadError(0, "CategoryAllModel is empty.");
    } else
    {
        Q_EMIT dataLoadOver();
    }
}

void CategoryAllModel::errorInfo(int type, const char *err_str)
{
    qDebug() << "CategoryAllModel Error." << err_str;
    Q_EMIT loadError(type, err_str);
}

void CategoryAllModel::onLoadOver()
{
    m_pCate->nodes().genVector(mVec);

    resetAll();
}

void CategoryAllModel::qmlCtgNextPage()
{
    qDebug() << "Category Next Page";
}

void CategoryAllModel::qmlClickCategory(int index)
{
    qDebug() << "ClickCategory index =" << index;
}

void CategoryAllModel::qmlCateTabClick(int index)
{
    qDebug() << "CateTabClick index =" << index;

    m_pTmpUse->obtain();
}

QHash<int, QByteArray> CategoryAllModel::roleNames() const
{
    return roles;
}
