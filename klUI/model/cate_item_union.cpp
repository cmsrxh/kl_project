#include <events/common_log.h>
#include "kl_url/kl_type_radio_list.h"
#include "kl_url/kl_album_list.h"
#include "kl_url/kl_operate_list.h"
#include "kl_url/kl_broadcast_item_list.h"
#include "kl_url/kl_chip_radio_list.h"
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
    switch (mCateItemType) {
    case CATE_ITEM_ALBUM: delete ((kl::AlbumList *)m_pCateItem);
        break;
    case CATE_ITEM_OPERATE: delete ((kl::OperateList *)m_pCateItem);
        break;
    case CATE_ITEM_TYPE_RADIO: delete ((kl::TypeRadioList *)m_pCateItem);
        break;
    case CATE_ITEM_BDCAST: delete ((kl::BroadcastItemList *)m_pCateItem);
        break;
    }
}

void CateItemUnion::loadCateItem(int cid_or_type, int bsorttype_or_classfyid, int area_code)
{    
    GEN_Printf(LOG_DEBUG, "type: %d, cid=%d, handler: %p", mCateItemType, cid_or_type, m_pCateItem);
    int ret = false;
    mLoadAction = PopTipManage::LOAD_MAIN_PAGE;
    switch (mCateItemType) {
    case CATE_ITEM_ALBUM:
    {
        kl::AlbumList *album;
        if (m_pCateItem)
        {
            album = (kl::AlbumList *)m_pCateItem;
        } else
        {
            album = new kl::AlbumList(cid_or_type, bsorttype_or_classfyid);
            m_pCateItem = album;
        }

        album->setUINotify(this);
        ret = album->obtain();
        break;
    }
    case CATE_ITEM_OPERATE:
    {
        kl::OperateList *operate;
        if (m_pCateItem)
        {
            operate = (kl::OperateList *)m_pCateItem;
        } else
        {
            operate = new kl::OperateList();            
            m_pCateItem = operate;
        }
        operate->setUINotify(this);
        ret = operate->obtain();
        break;
    }
    case CATE_ITEM_TYPE_RADIO:
    {
        kl::TypeRadioList *typeRadio;
        if (m_pCateItem)
        {
            typeRadio = (kl::TypeRadioList *)m_pCateItem;
        } else
        {
            typeRadio = new kl::TypeRadioList();            
            m_pCateItem = typeRadio;
        }
        typeRadio->setUINotify(this);
        ret = typeRadio->obtain();
        break;
    }
    case CATE_ITEM_BDCAST:
    {
        kl::BroadcastItemList *bdc;
        if (m_pCateItem)
        {
            bdc = (kl::BroadcastItemList *)m_pCateItem;
        } else
        {
            bdc = new kl::BroadcastItemList(cid_or_type, bsorttype_or_classfyid, area_code);
            m_pCateItem = bdc;
        }
        bdc->setUINotify(this);
        ret = bdc->obtain();
        break;
    }
    default:
        assert(0);
        break;
    }
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

    return res;
}

bool CateItemUnion::loadNextPage()
{
    bool ret = false;
    mLoadAction = PopTipManage::LOAD_NEXT_PAGE;
    switch (mCateItemType) {
    case CATE_ITEM_ALBUM:
        break;
    case CATE_ITEM_OPERATE:
        break;
    case CATE_ITEM_TYPE_RADIO:
        break;
    case CATE_ITEM_BDCAST:
        break;
    default:
        break;
    }
    if (ret)
    {
        PopTipManage::instance()->klObjectObtainStart(true, mCateItemType, mLoadAction);
    }

    return ret;
}

int CateItemUnion::page()
{
    int ret = 1;
    switch (mCateItemType) {
    case CATE_ITEM_ALBUM:

        break;
    case CATE_ITEM_OPERATE:
        break;
    case CATE_ITEM_BDCAST:
        break;
    case CATE_ITEM_TYPE_RADIO:
    default:
        break;
    }
    return ret;
}

bool CateItemUnion::haveNext()
{
    bool ret = false;
    switch (mCateItemType) {
    case CATE_ITEM_ALBUM:
        break;
    case CATE_ITEM_OPERATE:
        break;
    case CATE_ITEM_BDCAST:
        break;
    case CATE_ITEM_TYPE_RADIO:
    default:
        break;
    }
    return ret;
}
