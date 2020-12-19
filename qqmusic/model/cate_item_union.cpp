#include <events/common_log.h>
#include "local_data_proc.h"
#include "data_proc.h"
#include "qqmusicApi/qqmusic_category_playlist.h"
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
}

void CateItemUnion::loadCateItem(int arg1)
{    
    GEN_Printf(LOG_DEBUG, "type: %d, cid=%d, handler: %p", mCateItemType, arg1, m_pCateItem);
    switch (mCateItemType)
    {
    case CATE_ITEM_CATE_PLAY_LIST:
    {
        qqmusic::CategoryPlaylist *cate = 0;
        if (!m_pCateItem)
        {
            cate = new qqmusic::CategoryPlaylist();
            cate->setUINotify(this);
        } else
        {
            cate = reinterpret_cast<qqmusic::CategoryPlaylist *>(m_pCateItem);
        }
        if (cate->isEmpty())
        {
            bool ret = cate->obtain();
            mLoadAction = PopTipManage::LOAD_MAIN_PAGE;
            PopTipManage::instance()->klObjectObtainStart(ret, mCateItemType, mLoadAction);
        } else
        {
            Q_EMIT m_pParentModel->dataLoadOver((long)this);
        }
        break;
    }
    default:
        SF_ASSERT(0);
        break;
    }
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

void CateItemUnion::dataLoadOver(CateItemModel *model)
{
    switch (mCateItemType)
    {
    case CATE_ITEM_CATE_PLAY_LIST:
        genCateItemByCatePlayList(reinterpret_cast<qqmusic::CategoryPlaylist *>(m_pCateItem)->nodes()
                                  , model->vec());
        break;
    default:
        break;
    }
}

bool CateItemUnion::isEmpty()
{
    bool res = true;
    switch (mCateItemType) {
    case CATE_ITEM_CATE_PLAY_LIST:
        res = reinterpret_cast<qqmusic::CategoryPlaylist *>(m_pCateItem)->isEmpty();
        break;
    default:
        break;
    }
    return res;
}

void CateItemUnion::genCateItemByCatePlayList(ListTable<qqmusic::CatePlayList> &nodes, VectorTable<SecondMenuUnion *> &vec)
{
    vec.clearPtr();
    ListTable<qqmusic::CatePlayList>::iterator it = nodes.begin();
    for (; it != nodes.end(); ++it)
    {
        SecondMenuUnion *tmp = new SecondMenuUnion;
        tmp->id = it->dissid;
        tmp->name = it->dissname;
        tmp->img = it->imgurl;

        vec.push_back(tmp);
    }
}
