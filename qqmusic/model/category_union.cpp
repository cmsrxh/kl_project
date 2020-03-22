#include "events/common_log.h"
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
}

void CategoryUnion::loadCategory(int cate_type, int cid)
{
    bool ret = false;
    assert(mCateType == cate_type);

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
        assert(0);
        break;
    }
}

void CategoryUnion::onLoadOver(VectorTable<MusicCateUnion *> &vec)
{
    switch (mCateType)
    {
    case MAIN_CATE:
//        genCatesByCateMain(((kl::CategoryAll *)m_pCate)->nodes(), vec);
        break;
    case SUB_CATE:
//        genCatesByCateSub(((kl::CategorySublist *) m_pCate)->nodes(), vec);
        break;
    case BDC_CATE:
//        genCatesByCateBDC(((kl::CategoryBroadcast *) m_pCate)->nodes(), vec);
        break;
    case BDC_AREA_CATE:
//        genCatesByBDCArea(((kl::BroadcastAreaList *) m_pCate)->nodes(), vec);
        break;
    default:
        break;
    }
}



