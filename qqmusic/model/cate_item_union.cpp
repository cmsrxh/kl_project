#include <events/common_log.h>
#include "kl_local_data_proc.h"
#include "kl_data_proc.h"
#include "pop_tip_manage.h"
#include "cate_item_model.h"
#include "cate_item_union.h"

CateItemUnion::CateItemUnion(int cid_type, CateItemModel *parent)
    : mCateItemType(cid_type), mLoadAction(-1)
    , m_pCateItem(nullptr)
    , m_pParentModel(parent)
{    
}

CateItemUnion::~CateItemUnion()
{
    if (!m_pCateItem) return;
}

void CateItemUnion::loadCateItem(int cid_or_type, int bsorttype_or_classfyid, int area_code)
{    
    GEN_Printf(LOG_DEBUG, "type: %d, cid=%d, handler: %p", mCateItemType, cid_or_type, m_pCateItem);
    int ret = false;
    mLoadAction = PopTipManage::LOAD_MAIN_PAGE;
    PopTipManage::instance()->klObjectObtainStart(ret, mCateItemType, mLoadAction);
}

void CateItemUnion::dataPrepare()
{
    PopTipManage::instance()->klObjectObtainOver(mCateItemType, mLoadAction);
    Q_EMIT m_pParentModel->dataLoadOver((long)this);
}

void CateItemUnion::errorInfo(int type, const ByteString &err_str)
{    
    switch (type)
    {
    case LOAD_EMPTY_DATA:        // 分析数据正确，但是得到的数据是空
        PopTipManage::instance()->klLoadDataExportEmpty(mCateItemType, mLoadAction, err_str);
        break;
    case LOAD_PRISER_JSOC_ERROR: // 不能正确解析json数据
        PopTipManage::instance()->klLoadDataPriserExcept(mCateItemType, mLoadAction, err_str);
        break;
    case LOAD_SYS_API_FAILED:    // libcurl下载反馈的错误信息
        PopTipManage::instance()->sysNetLoadApiExcept(mCateItemType, mLoadAction, err_str);
        break;
    default :
        assert(0);
        break;
    }
}

void CateItemUnion::onLoadOver(CateItemModel *model)
{
    }

bool CateItemUnion::isEmpty()
{
    bool res = true;
    switch (mCateItemType) {
    case CATE_ITEM_ALBUM:
//        res = ((kl::AlbumList *)m_pCateItem)->nodes().empty();
        break;
    case CATE_ITEM_OPERATE:
//        res = ((kl::OperateList *)m_pCateItem)->nodes().empty();
        break;
    case CATE_ITEM_TYPE_RADIO:
//        res = ((kl::TypeRadioList *)m_pCateItem)->nodes().empty();
        break;
    case CATE_ITEM_BDCAST:
//        res = ((kl::BroadcastItemList *)m_pCateItem)->nodes().empty();
        break;
    default:
        break;
    }
    return res;
}

bool CateItemUnion::loadNextPage()
{
    bool ret = false;
    mLoadAction = PopTipManage::LOAD_NEXT_PAGE;

    if (ret)
    {
        PopTipManage::instance()->klObjectObtainStart(true, mCateItemType, mLoadAction);
    }

    return ret;
}

int CateItemUnion::page()
{
    int ret = 1;

    return ret;
}

bool CateItemUnion::haveNext()
{
    bool ret = false;

    return ret;
}
