#include <events/common_log.h>
#include "kl_url/kl_type_radio_list.h"
#include "kl_url/kl_album_list.h"
#include "kl_url/kl_operate_list.h"
#include "kl_url/kl_chip_radio_list.h"
#include "cate_item_model.h"
#include "cate_item_union.h"

CateItemUnion::CateItemUnion(int cid, CateItemModel *parent)
    : mCateItemType(0)
    , m_pCateItem(nullptr)
    , m_pParentModel(parent)
{
    GEN_Printf(LOG_DEBUG, "New a Cate Item: %d", cid);
    if (-1 == cid) //operate list
    {
        mCateItemType = CATE_ITEM_OPERATE;
    } else if (-2 == cid) //type radio list
    {
        mCateItemType = CATE_ITEM_TYPE_RADIO;
    } else  //album list
    {
        mCateItemType = CATE_ITEM_ALBUM;
    }
}

void CateItemUnion::loadCateItem(int cid, bool sorttype)
{    
    GEN_Printf(LOG_DEBUG, "type: %d, cid=%d, handler: %p", mCateItemType, cid, m_pCateItem);
    switch (mCateItemType) {
    case CATE_ITEM_ALBUM:
    {
        kl::AlbumList *album;
        if (m_pCateItem)
        {
            album = (kl::AlbumList *)m_pCateItem;


        } else
        {
            album = new kl::AlbumList(cid, sorttype);
            album->setUINotify(this);
            album->obtain();
            m_pCateItem = album;
        }

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
            operate->setUINotify(this);
            operate->obtain();
            m_pCateItem = operate;
        }
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
            typeRadio->setUINotify(this);
            typeRadio->obtain();
            m_pCateItem = typeRadio;
        }
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
    default:
        break;
    }

    if (isEmpty)
    {
        GEN_Printf(LOG_DEBUG, "Category Item List is empty.");
        Q_EMIT m_pParentModel->loadError(0, "Category Item List is empty.");
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
    default:
        break;
    }
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
    case CATE_ITEM_TYPE_RADIO:
    default:
        break;
    }
    return ret;
}

void CateItemUnion::genCateItemByAlbumItem(ListTable<kl::AlbumItem> &nodes, VectorTable<MusicCateItemUnion *> &vec)
{
    ListTable<kl::AlbumItem>::iterator it = nodes.begin();

    for ( ; it != nodes.end(); ++it)
    {
        MusicCateItemUnion *tmp = new MusicCateItemUnion;

        tmp->id   = it->id;
        tmp->name = it->name;
        tmp->img  = it->img;
        tmp->type = ByteString("0", 1);

        vec.push_back(tmp);
    }
}

void CateItemUnion::genCateItemByOperate(ListTable<kl::Operate> &nodes, VectorTable<MusicCateItemUnion *> &vec)
{
    ListTable<kl::Operate>::iterator it = nodes.begin();

    for ( ; it != nodes.end(); ++it)
    {
        MusicCateItemUnion *tmp = new MusicCateItemUnion;

        tmp->id   = it->rid;
        tmp->name = it->rname;
        tmp->img  = it->image;
        tmp->type = it->rtype;

        vec.push_back(tmp);
    }
}

void CateItemUnion::genCateItemByTypeRadio(ListTable<kl::TypeRadio> &nodes, VectorTable<MusicCateItemUnion *> &vec)
{
    ListTable<kl::TypeRadio>::iterator it = nodes.begin();

    for ( ; it != nodes.end(); ++it)
    {
        MusicCateItemUnion *tmp = new MusicCateItemUnion;

        tmp->id   = it->id;
        tmp->name = it->name;
        tmp->img  = it->img;
        tmp->type = ByteString("3", 1);

        vec.push_back(tmp);
    }
}
