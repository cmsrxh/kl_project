#include "events/common_log.h"
#include "kl_url/kl_category_sub_list.h"
#include "kl_url/kl_category_all.h"
#include "category_model.h"
#include "cate_item_model.h"
#include "category_union.h"

CategoryUnion::CategoryUnion(int cate_type, CategoryModel *parent)
    : mCateType(cate_type)
    , m_pCate(NULL), m_pParentModel(parent)
{
}

void CategoryUnion::loadCategory(int cate_type, int cid)
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
            genCatesByCateMain(cate->nodes(), m_pParentModel->vec());
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


void CategoryUnion::dataPrepare()
{
    bool isEmpty = (mCateType == MAIN_CATE) ? ((kl::CategoryAll *)m_pCate)->nodes().empty()
                                            : ((kl::CategorySublist *) m_pCate)->nodes().empty();

    if (isEmpty)
    {
        qDebug() << "CategoryModel is empty.";
        Q_EMIT m_pParentModel->loadError(0, "CategoryModel is empty.");
    } else
    {
        Q_EMIT m_pParentModel->dataLoadOver((long)this);
    }
}

void CategoryUnion::errorInfo(int type, const char *err_str)
{
    qDebug() << "CategoryAllModel Error." << err_str;
    Q_EMIT m_pParentModel->loadError(type, err_str);
}

void CategoryUnion::onLoadOver(VectorTable<MusicCateUnion *> &vec)
{
    if (mCateType == MAIN_CATE)
    {
        genCatesByCateMain(((kl::CategoryAll *)m_pCate)->nodes(), vec);
    } else
    {
        genCatesByCateSub(((kl::CategorySublist *) m_pCate)->nodes(), vec);
    }
}

void CategoryUnion::genCatesByCateMain(ListTable<kl::CateMain> &nodes, VectorTable<MusicCateUnion *> &vec)
{
    ListTable<kl::CateMain>::iterator it = nodes.begin();

    for ( ; it != nodes.end(); ++it)
    {
        MusicCateUnion *tmp = new MusicCateUnion;

        tmp->cid    = it->cid;
        tmp->name   = it->name;
        tmp->hasSub = it->hasSub;
        tmp->hasSub = it->hasSub;

        vec.push_back(tmp);
    }
}

void CategoryUnion::genCatesByCateSub(ListTable<kl::CateSub> &nodes, VectorTable<MusicCateUnion *> &vec)
{
    ListTable<kl::CateSub>::iterator it = nodes.begin();

    for ( ; it != nodes.end(); ++it)
    {
        MusicCateUnion *tmp = new MusicCateUnion;

        tmp->cid    = it->cid;
        tmp->name   = it->name;
        tmp->hasSub = it->hasSub;
        tmp->hasSub = it->hasSub;

        vec.push_back(tmp);
    }
}


