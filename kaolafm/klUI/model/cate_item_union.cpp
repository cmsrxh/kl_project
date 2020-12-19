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
        SF_ASSERT(0);
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
        SF_ASSERT(0);
        break;
    }
}

void CateItemUnion::onLoadOver(CateItemModel *model)
{
    switch (mCateItemType) {
    case CATE_ITEM_ALBUM:
        genCateItemByAlbumItem(((kl::AlbumList *)m_pCateItem)->nodes(), model->vec());
        break;
    case CATE_ITEM_OPERATE:
        genCateItemByOperate(((kl::OperateList *)m_pCateItem)->nodes(), model->vec());
        break;
    case CATE_ITEM_TYPE_RADIO:
        genCateItemByTypeRadio(((kl::TypeRadioList *)m_pCateItem)->nodes(), model->vec());
        break;
    case CATE_ITEM_BDCAST:
        genCateItemByBDCast(((kl::BroadcastItemList *)m_pCateItem)->nodes(), model->vec());
        break;
    default:
        break;
    }
}

bool CateItemUnion::isEmpty()
{
    bool res = true;
    switch (mCateItemType) {
    case CATE_ITEM_ALBUM:
        res = ((kl::AlbumList *)m_pCateItem)->nodes().empty();
        break;
    case CATE_ITEM_OPERATE:
        res = ((kl::OperateList *)m_pCateItem)->nodes().empty();
        break;
    case CATE_ITEM_TYPE_RADIO:
        res = ((kl::TypeRadioList *)m_pCateItem)->nodes().empty();
        break;
    case CATE_ITEM_BDCAST:
        res = ((kl::BroadcastItemList *)m_pCateItem)->nodes().empty();
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
    switch (mCateItemType) {
    case CATE_ITEM_ALBUM:
        ret = ((kl::AlbumList *)m_pCateItem)->loadNextPage();
        break;
    case CATE_ITEM_OPERATE:
        ret = ((kl::OperateList *)m_pCateItem)->loadNextPage();
        break;
    case CATE_ITEM_TYPE_RADIO:
        ret = ((kl::TypeRadioList *)m_pCateItem)->loadNextPage();
        break;
    case CATE_ITEM_BDCAST:
        ret = ((kl::BroadcastItemList *)m_pCateItem)->loadNextPage();
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
        ret = ((kl::AlbumList *)m_pCateItem)->getPage();
        break;
    case CATE_ITEM_OPERATE:
        ret = ((kl::OperateList *)m_pCateItem)->getPage();
        break;
    case CATE_ITEM_BDCAST:
        ret = ((kl::BroadcastItemList *)m_pCateItem)->getPage();
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
        ret = ((kl::AlbumList *)m_pCateItem)->getHaveNext();
        break;
    case CATE_ITEM_OPERATE:
        ret = ((kl::OperateList *)m_pCateItem)->getHaveNext();
        break;
    case CATE_ITEM_BDCAST:
        ret = ((kl::BroadcastItemList *)m_pCateItem)->getHaveNext();
        break;
    case CATE_ITEM_TYPE_RADIO:
    default:
        break;
    }
    return ret;
}

void CateItemUnion::genCateItemByAlbumItem(ListTable<kl::AlbumItem> &nodes, VectorTable<MusicCateItemUnion *> &vec)
{
    int count = vec.size();
    ListTable<kl::AlbumItem>::iterator it = nodes.begin();
    for ( ; it != nodes.end() && count; ++it, --count);
    for ( ; it != nodes.end(); ++it)
    {
        MusicCateItemUnion *tmp = new MusicCateItemUnion;

        tmp->id   = it->id;
        tmp->name = it->name;
        tmp->img  = it->img;
        tmp->type = PLAY_CHIP_TYPE_ALBUM;

        vec.push_back(tmp);
    }
}

void CateItemUnion::genCateItemByOperate(ListTable<kl::Operate> &nodes, VectorTable<MusicCateItemUnion *> &vec)
{
    int count = vec.size();
    ListTable<kl::Operate>::iterator it = nodes.begin();
    for ( ; it != nodes.end() && count; ++it, --count);
    for ( ; it != nodes.end(); ++it)
    {
        int type = atoi(it->rtype.string());

        SF_ASSERT(0 == type || 3 == type);

        MusicCateItemUnion *tmp = new MusicCateItemUnion;

        tmp->id   = it->rid;
        tmp->name = it->rname;
        tmp->img  = it->image;
        tmp->type =  (0 == type) ? PLAY_CHIP_TYPE_ALBUM : PLAY_CHIP_TYPE_TYPE_RADIO;

        vec.push_back(tmp);
    }
}

void CateItemUnion::genCateItemByTypeRadio(ListTable<kl::TypeRadio> &nodes, VectorTable<MusicCateItemUnion *> &vec)
{
    int count = vec.size();
    ListTable<kl::TypeRadio>::iterator it = nodes.begin();
    for ( ; it != nodes.end() && count; ++it, --count);
    for ( ; it != nodes.end(); ++it)
    {
        MusicCateItemUnion *tmp = new MusicCateItemUnion;

        tmp->isCollect  = LocalDataProc::instance()->checkBDCItemIsCollect(PLAY_CHIP_TYPE_TYPE_RADIO, it->id);
        tmp->id         = it->id;
        tmp->name       = it->name;
        tmp->img        = it->img;
        tmp->type       = PLAY_CHIP_TYPE_TYPE_RADIO;

        vec.push_back(tmp);
    }
}

void CateItemUnion::genCateItemByBDCast(ListTable<kl::BDCastItem> &nodes, VectorTable<MusicCateItemUnion *> &vec)
{
    int count = vec.size();
    ListTable<kl::BDCastItem>::iterator it = nodes.begin();
    for ( ; it != nodes.end() && count; ++it, --count);
    for ( ; it != nodes.end(); ++it)
    {
        MusicCateItemUnion *tmp = new MusicCateItemUnion;

        tmp->isCollect  = LocalDataProc::instance()->checkBDCItemIsCollect(PLAY_CHIP_TYPE_BROADCAST, it->broadcastId);
        tmp->id         = it->broadcastId;
        tmp->name       = it->name;
        tmp->img        = it->image;
        tmp->type       = PLAY_CHIP_TYPE_BROADCAST;
        tmp->playUrl    = it->playUrl;
        vec.push_back(tmp);
    }
}
