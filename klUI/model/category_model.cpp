#include "kl_url/kl_category_sub_list.h"
#include "kl_url/kl_category_all.h"
#include "category_model.h"
#include "cate_item_model.h"
#include <QDebug>

#define CATEGORY_NAME Qt::UserRole

CategoryModel::CategoryModel(int cate_type)
    : QAbstractListModel()
    , mCateType(cate_type)
    , m_pCate(NULL)
{
    roles[CATEGORY_NAME] = "ctgname";

    connect(this, SIGNAL(dataLoadOver()), this, SLOT(onLoadOver()));
}

void CategoryModel::loadCategory(int cate_type, int cid)
{
    assert(mCateType == cate_type);

    switch (cate_type)
    {
    case MAIN_CATE:
    {
        kl::CategoryAll  *cate;

        if (m_pCate)
        {
            cate = (kl::CategoryAll *)m_pCate;
        } else
        {
            cate = new kl::CategoryAll;
            cate->setUINotify(this);
            m_pCate = (UICategory *)cate;
        }

        if (cate->nodes().empty())
        {
            cate->loadNodesFile();
            if (cate->nodes().empty())
            {
                cate->obtain();
            }
        }
        if (!cate->nodes().empty())
        {
            genCatesByCateMain(cate->nodes());
        }

        break;
    }
    case SUB_CATE:
    {
        kl::CategorySublist *subCate;

        if (m_pCate)
        {
            subCate = (kl::CategorySublist *) m_pCate;
        } else
        {
            subCate = new kl::CategorySublist(cid);
            subCate->setUINotify(this);
            m_pCate = (UICategory *)subCate;
        }

        if (subCate->nodes().empty())
        {
            subCate->obtain();
        }

        break;
    }
    default:
        assert(0);
        break;
    }    
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
        return mVec[index.row()].name.string();
    }

    return QVariant();
}

void CategoryModel::resetAll()
{
    beginResetModel();
    endResetModel();
}

void CategoryModel::dataPrepare()
{
    bool isEmpty = (mCateType == MAIN_CATE) ? ((kl::CategoryAll *)m_pCate)->nodes().empty()
                                            : ((kl::CategorySublist *) m_pCate)->nodes().empty();

    if (isEmpty)
    {
        qDebug() << "CategoryModel is empty.";
        Q_EMIT loadError(0, "CategoryModel is empty.");
    } else
    {
        Q_EMIT dataLoadOver();
    }
}

void CategoryModel::errorInfo(int type, const char *err_str)
{
    qDebug() << "CategoryAllModel Error." << err_str;
    Q_EMIT loadError(type, err_str);
}

void CategoryModel::onLoadOver()
{
    if (mCateType == MAIN_CATE)
    {
        genCatesByCateMain(((kl::CategoryAll *)m_pCate)->nodes());
    } else
    {
        genCatesByCateSub(((kl::CategorySublist *) m_pCate)->nodes());
    }

    resetAll();
}

void CategoryModel::qmlCateTabClick(int index)
{
    qDebug() << "CateTabClick index =" << index;
}

QHash<int, QByteArray> CategoryModel::roleNames() const
{
    return roles;
}

void CategoryModel::genCatesByCateMain(ListTable<kl::CateMain> &nodes)
{
    ListTable<kl::CateMain>::iterator it = nodes.begin();

    for ( ; it != nodes.end(); ++it)
    {
        MusicCateUnion *tmp = new MusicCateUnion;

        tmp->cid    = it->cid;
        tmp->name   = it->name;
        tmp->hasSub = it->hasSub;
        tmp->hasSub = it->hasSub;

        mVec.push_back(tmp);
    }
}

void CategoryModel::genCatesByCateSub(ListTable<kl::CateSub> &nodes)
{
    ListTable<kl::CateSub>::iterator it = nodes.begin();

    for ( ; it != nodes.end(); ++it)
    {
        MusicCateUnion *tmp = new MusicCateUnion;

        tmp->cid    = it->cid;
        tmp->name   = it->name;
        tmp->hasSub = it->hasSub;
        tmp->hasSub = it->hasSub;

        mVec.push_back(tmp);
    }
}


