#include "events/common_log.h"
#include "kl_url/kl_category_sub_list.h"
#include "kl_url/kl_category_all.h"
#include "kl_url/kl_broadcast_area_list.h"
#include "kl_url/kl_category_broadcast.h"
#include "pop_tip_manage.h"
#include <application.h>
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
        SF_ASSERT(0);
        break;
    }
}

void CategoryUnion::loadCategory(int cate_type, int cid)
{
    bool ret = false;
    SF_ASSERT(mCateType == cate_type);

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

        if (cate->checkFile())
        {
            ret = cate->obtain();
            PopTipManage::instance()->klObjectObtainStart(ret, cate_type, PopTipManage::LOAD_MAIN_PAGE);
        } else if (cate->nodes().empty())
        {
            cate->loadNodesFile();
            if (cate->nodes().empty())
            {
                ret = cate->obtain();
                PopTipManage::instance()->klObjectObtainStart(ret, cate_type, PopTipManage::LOAD_MAIN_PAGE);
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
            ret = subCate->obtain();
            PopTipManage::instance()->klObjectObtainStart(ret, cate_type, PopTipManage::LOAD_MAIN_PAGE);
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
                ret = bdc->obtain();
                PopTipManage::instance()->klObjectObtainStart(ret, cate_type, PopTipManage::LOAD_MAIN_PAGE);
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
                ret = area->obtain();
                PopTipManage::instance()->klObjectObtainStart(ret, cate_type, PopTipManage::LOAD_MAIN_PAGE);
            }
        }
        if (!area->nodes().empty())
        {
            genCatesByBDCArea(area->nodes(), m_pParentModel->vec());
        }
        break;
    }
    default:
        SF_ASSERT(0);
        break;
    }    
}

void CategoryUnion::dataPrepare()
{
    PopTipManage::instance()->klObjectObtainOver(mCateType, PopTipManage::LOAD_MAIN_PAGE);
    Q_EMIT m_pParentModel->dataLoadOver((long)this);
}

void CategoryUnion::errorInfo(int type, const ByteString &err_str)
{
    switch (type)
    {
    case LOAD_EMPTY_DATA:        // 分析数据正确，但是得到的数据是空
        PopTipManage::instance()->klLoadDataExportEmpty(mCateType, PopTipManage::LOAD_MAIN_PAGE, err_str);
        break;
    case LOAD_PRISER_JSOC_ERROR: // 不能正确解析json数据
        PopTipManage::instance()->klLoadDataPriserExcept(mCateType, PopTipManage::LOAD_MAIN_PAGE, err_str);
        break;
    case LOAD_SYS_API_FAILED:    // libcurl下载反馈的错误信息
        PopTipManage::instance()->sysNetLoadApiExcept(mCateType, PopTipManage::LOAD_MAIN_PAGE, err_str);
        break;
    default :
        SF_ASSERT(0);
        break;
    }
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

    MusicCateUnion *tmp = new MusicCateUnion;
    kl::AreaItem *pos = Application::instance()->positionArea();

    tmp->cid    = pos->id;
    tmp->name   = pos->name;

    vec.push_back(tmp);

    for ( ; it != nodes.end(); ++it)
    {
        MusicCateUnion *tmp = new MusicCateUnion;

        tmp->cid    = it->id;
        tmp->name   = it->name;
        tmp->hasSub = it->type;
        tmp->img    = it->img;

        vec.push_back(tmp);
    }

//    for (int i = 0; i < vec.size(); ++i)
//    {
//        MusicCateUnion *addr = vec[i];
//        GEN_Printf(LOG_DEBUG, "id=%s, name=%s", addr->cid.string(), addr->name.string());
//    }
}


