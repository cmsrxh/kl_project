#include "events/common_log.h"
#include "kl_url/kl_category_sub_list.h"
#include "kl_url/kl_category_all.h"
#include "kl_url/kl_broadcast_area_list.h"
#include "kl_url/kl_category_broadcast.h"
#include "category_model.h"
#include "cate_item_model.h"
#include "category_union.h"

CategoryUnion::CategoryUnion(int cate_type, CategoryModel *parent)
    : mCateType(cate_type)
    , m_pCate(NULL), m_pParentModel(parent)
{
}

CategoryUnion::~CategoryUnion()
{
    if (!m_pCate) return;

    switch (mCateType)
    {
    case MAIN_CATE:
        delete ((kl::CategoryAll *)m_pCate);
        break;
    case SUB_CATE:
        delete ((kl::CategorySublist *) m_pCate);
        break;
    case BDC_CATE:
        delete ((kl::CategoryBroadcast *)m_pCate);
        break;
    case BDC_AREA_CATE:
        delete ((kl::BroadcastAreaList *)m_pCate);
        break;
    default:
        assert(0);
        break;
    }
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
        cate->setUINotify(this);

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
        subCate->setUINotify(this);

        if (subCate->nodes().empty())
        {
            subCate->obtain();
        }

        break;
    }
    case BDC_CATE:
    {
        kl::CategoryBroadcast *bdc;
        if (m_pCate)
        {
            bdc = (kl::CategoryBroadcast *)m_pCate;
        } else
        {
            bdc = new kl::CategoryBroadcast;

            m_pCate = bdc;
        }
        bdc->setUINotify(this);

        if (bdc->nodes().empty())
        {
            bdc->loadNodesFile();
            if (bdc->nodes().empty())
            {
                bdc->obtain();
            }
        }
        if (!bdc->nodes().empty())
        {
            genCatesByCateBDC(bdc->nodes(), m_pParentModel->vec());
        }
        break;
    }
    case BDC_AREA_CATE:
    {
        kl::BroadcastAreaList *area;
        if (m_pCate)
        {
            area = (kl::BroadcastAreaList *)m_pCate;
        } else
        {
            area = new kl::BroadcastAreaList;

            m_pCate = area;
        }
        area->setUINotify(this);

        if (area->nodes().empty())
        {
            area->loadNodesFile();
            if (area->nodes().empty())
            {
                area->obtain();
            }
        }
        if (!area->nodes().empty())
        {
            genCatesByBDCArea(area->nodes(), m_pParentModel->vec());
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
    bool isEmpty = false;

    switch (mCateType)
    {
    case MAIN_CATE:
        isEmpty = ((kl::CategoryAll *)m_pCate)->nodes().empty();
        break;
    case SUB_CATE:
        isEmpty = ((kl::CategorySublist *) m_pCate)->nodes().empty();
        break;
    case BDC_CATE:
        isEmpty = ((kl::CategoryBroadcast *)m_pCate)->nodes().empty();
        break;
    case BDC_AREA_CATE:
        isEmpty = ((kl::BroadcastAreaList *)m_pCate)->nodes().empty();
        break;
    default:
        assert(0);
        break;
    }

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
    switch (mCateType)
    {
    case MAIN_CATE:
        genCatesByCateMain(((kl::CategoryAll *)m_pCate)->nodes(), vec);
        break;
    case SUB_CATE:
        genCatesByCateSub(((kl::CategorySublist *) m_pCate)->nodes(), vec);
        break;
    case BDC_CATE:
        genCatesByCateBDC(((kl::CategoryBroadcast *) m_pCate)->nodes(), vec);
        break;
    case BDC_AREA_CATE:
        genCatesByBDCArea(((kl::BroadcastAreaList *) m_pCate)->nodes(), vec);
        break;
    default:
        break;
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
        tmp->img    = it->img;

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
        tmp->img    = it->img;

        vec.push_back(tmp);
    }
}

void CategoryUnion::genCatesByBDCArea(ListTable<kl::AreaItem> &nodes, VectorTable<MusicCateUnion *> &vec)
{
    ListTable<kl::AreaItem>::iterator it = nodes.begin();

    for ( ; it != nodes.end(); ++it)
    {
        MusicCateUnion *tmp = new MusicCateUnion;

        tmp->cid    = it->id;
        tmp->name   = it->name;

        vec.push_back(tmp);
    }
}

void CategoryUnion::genCatesByCateBDC(ListTable<kl::CateBCast> &nodes, VectorTable<MusicCateUnion *> &vec)
{
    ListTable<kl::CateBCast>::iterator it = nodes.begin();

    for ( ; it != nodes.end(); ++it)
    {
        MusicCateUnion *tmp = new MusicCateUnion;

        tmp->cid    = it->id;
        tmp->name   = it->name;
        tmp->hasSub = it->type;
        tmp->img    = it->img;

        vec.push_back(tmp);
    }
}


