#include "kl_url/kl_type_radio_list.h"
#include "kl_url/kl_album_list.h"
#include "kl_url/kl_operate_list.h"
#include "kl_url/kl_chip_radio_list.h"
#include "cate_item_model.h"

#define CATE_ITEM_NAME     Qt::UserRole
#define CATE_ITEM_IMG_URL (Qt::UserRole+1)

CateItemModel::CateItemModel(int cate_item_type)
    : QAbstractListModel()
    , mCateItemType(cate_item_type)
    , m_pCateItem(nullptr)
{

    roles[CATE_ITEM_NAME]    = "cateItemName";
    roles[CATE_ITEM_IMG_URL] = "imageUri";

    connect(this, SIGNAL(dataLoadOver()), this, SLOT(onLoadOver()));
}

void CateItemModel::loadCateItem(int cate_item_type, int cid, bool sorttype)
{
    assert(cate_item_type == mCateItemType);

    switch (cate_item_type) {
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
            m_pCateItem = typeRadio;
        }
        break;
    }
    default:
        assert(0);
        break;
    }
}

int CateItemModel::rowCount(const QModelIndex &) const
{
    return mVec.size();
}

QVariant CateItemModel::data(const QModelIndex &index, int role) const
{
    if ( index.row() < 0 || index.row() >= (int)mVec.size())
    {
        return QVariant();
    }

    if (CATE_ITEM_NAME == role)
    {
        return mVec[index.row()].name.string();
    } else if(CATE_ITEM_IMG_URL == role)
    {
        return mVec[index.row()].img.string();
    }

    return QVariant();
}

void CateItemModel::resetAll()
{
    beginResetModel();
    endResetModel();
}

void CateItemModel::dataPrepare()
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
        qDebug() << "CategoryAllModel is empty.";
        Q_EMIT loadError(0, "CategoryAllModel is empty.");
    } else
    {
        Q_EMIT dataLoadOver();
    }
}

void CateItemModel::errorInfo(int type, const char *err_str)
{
    qDebug() << "CategoryAllModel Error." << err_str;
    Q_EMIT loadError(type, err_str);
}

void CateItemModel::onLoadOver()
{
    switch (mCateItemType) {
    case CATE_ITEM_ALBUM:
        genCateItemByAlbumItem(((kl::AlbumList *)m_pCateItem)->nodes());
        break;
    case CATE_ITEM_OPERATE:
        genCateItemByOperate(((kl::OperateList *)m_pCateItem)->nodes());
        break;
    case CATE_ITEM_TYPE_RADIO:
        genCateItemByTypeRadio(((kl::TypeRadioList *)m_pCateItem)->nodes());
        break;
    default:
        break;
    }

    resetAll();
}

void CateItemModel::qmlCtgNextPage()
{
    qDebug() << "Category Next Page";
}

void CateItemModel::qmlClickCategory(int index)
{
    qDebug() << "ClickCategory index =" << index;

    qDebug() << mVec[index].id.string()
             << mVec[index].name.string()
             << mVec[index].type.string();

}

QHash<int, QByteArray> CateItemModel::roleNames() const
{
    return roles;
}

void CateItemModel::genCateItemByAlbumItem(ListTable<kl::AlbumItem> &nodes)
{

}

void CateItemModel::genCateItemByOperate(ListTable<kl::Operate> &nodes)
{

}

void CateItemModel::genCateItemByTypeRadio(ListTable<kl::TypeRadio> &nodes)
{

}
