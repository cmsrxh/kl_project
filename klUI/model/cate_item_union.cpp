#include <events/common_log.h>
#include "kl_url/kl_type_radio_list.h"
#include "kl_url/kl_album_list.h"
#include "kl_url/kl_operate_list.h"
#include "kl_url/kl_broadcast_item_list.h"
#include "kl_url/kl_chip_radio_list.h"
#include "kl_local_data_proc.h"
#include "cate_item_model.h"
#include "cate_item_union.h"

CateItemUnion::CateItemUnion(int cid_type, CateItemModel *parent)
    : mCateItemType(cid_type)
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
        album->obtain();
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
        operate->obtain();
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
        typeRadio->obtain();
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
        bdc->obtain();
        break;
    }
    default:
        assert(0);
        break;
    }
}

void CateItemUnion::dataPrepare()
{
    bool isEmpty = true;
    switch (mCateItemType) {
    case CATE_ITEM_ALBUM:
        isEmpty = ((kl::AlbumList *)m_pCateItem)->nodes().empty();
        break;
    case CATE_ITEM_OPERATE:
        isEmpty = ((kl::OperateList *)m_pCateItem)->nodes().empty();
        break;
    case CATE_ITEM_TYPE_RADIO:
        isEmpty = ((kl::TypeRadioList *)m_pCateItem)->nodes().empty();
        break;
    case CATE_ITEM_BDCAST:
        isEmpty = ((kl::BroadcastItemList *)m_pCateItem)->nodes().empty();
        break;
    default:
        break;
    }

    if (isEmpty)
    {
//Not Used signal
//        GEN_Printf(LOG_WARN, "Category Item List is empty.");
//        Q_EMIT m_pParentModel->loadError(0, "Category Item List is empty.");
    } else
    {
        Q_EMIT m_pParentModel->dataLoadOver((long)this);
    }
}

void CateItemUnion::errorInfo(int type, const char *err_str)
{
    GEN_Printf(LOG_DEBUG, "Cate Item List Error, %s", err_str);
    Q_EMIT m_pParentModel->loadError(type, err_str);
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

        assert(0 == type || 3 == type);

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
