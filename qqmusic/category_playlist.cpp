#include <events/common_log.h>
#include "model/cate_item_model.h"
#include "model/category_model.h"
#include "model/cate_item_union.h"
#include "qqmusicApi/qqmusic_category.h"
#include "qqmusicApi/qqmusic_category_playlist.h"

#include "pop_tip_manage.h"
#include "category_playlist.h"

CategoryPlaylistProc::CategoryPlaylistProc()
    : m_pCateLoad(new qqmusic::Category())
    , m_pPlayListLoad(new qqmusic::CategoryPlaylist())
    , m_pCateGroupEmptItem(new CategoryModel)
    , m_pPlayListModel(new CateItemModel)
    , mPageTotals(0), mCurrentPage(1)
    , mOldCategoryId(0)
{
    m_pProxyPlayList = new CateItemUnion(CateItemUnion::CATE_ITEM_CATE_PLAY_LIST, m_pPlayListModel);
    //model 得到数据下载代理的对象的指针
    m_pPlayListModel->setCateItemUnion(m_pProxyPlayList);
    //设置好播放列表的代理，所有的数据都在m_pProxyPlayList中处理
    m_pProxyPlayList->setProxyItem(m_pPlayListLoad);
    //数据下载数据，通知到代理对象中
    m_pPlayListLoad->setUINotify(m_pProxyPlayList);

    connect(m_pPlayListModel, SIGNAL(dataLoadOver(long)), this, SLOT(onPlayListLoadOver(long)));
    connect(this, SIGNAL(cateDataPrepare()), this, SLOT(onCateDataPrepare()));
}

int CategoryPlaylistProc::pageTotals()
{
    return m_pPlayListLoad->getTotalPages();
}

int CategoryPlaylistProc::currentPage()
{
    return m_pPlayListLoad->getCurrentPage();
}

void CategoryPlaylistProc::setPageTotals(int total)
{
    if (mPageTotals != total)
    {
        mPageTotals = total;
        Q_EMIT pageTotalsChanged();
    }
}

void CategoryPlaylistProc::setCurrentPage(int page)
{
    mCurrentPage = page;
    Q_EMIT currentPageChanged();
}

QStringList CategoryPlaylistProc::cateList() const
{
    return mCateList;
}

QList<QObject *> CategoryPlaylistProc::cateSubs() const
{
    return mCateGroupItems;
}

void CategoryPlaylistProc::dataPrepare()
{
    qDebug() << "Data Load Over !!!";
    Q_EMIT cateDataPrepare();
}

void CategoryPlaylistProc::errorInfo(int type, const ByteString &err_str)
{
    switch (type)
    {
    case LOAD_EMPTY_DATA:        // 分析数据正确，但是得到的数据是空
        PopTipManage::instance()->klLoadDataExportEmpty(qqmusic::OBJECT_NAME_CATEGORY, PopTipManage::LOAD_MAIN_PAGE, err_str);
        break;
    case LOAD_PRISER_JSOC_ERROR: // 不能正确解析json数据
        PopTipManage::instance()->klLoadDataPriserExcept(qqmusic::OBJECT_NAME_CATEGORY, PopTipManage::LOAD_MAIN_PAGE, err_str);
        break;
    case LOAD_SYS_API_FAILED:    // libcurl下载反馈的错误信息
        PopTipManage::instance()->sysNetLoadApiExcept(qqmusic::OBJECT_NAME_CATEGORY, PopTipManage::LOAD_MAIN_PAGE, err_str);
        break;
    default :
        SF_ASSERT(0);
        break;
    }
}

void CategoryPlaylistProc::qmlCatePageIndexOperate(int index)
{
    bool ret = false;
    m_pProxyPlayList->setLoadAction(PopTipManage::LOAD_NEXT_PAGE);
    if (-2 == index)
    {
        ret = m_pPlayListLoad->loadPrevPage();
    } else if (-1 == index)
    {
        ret = m_pPlayListLoad->loadNextPage();
    } else
    {
        ret = m_pPlayListLoad->loadPage(index);
    }

    PopTipManage::instance()->klObjectObtainStart(ret, qqmusic::OBJECT_NAME_CATEGORY_PLAY_LIST
                                                  , PopTipManage::LOAD_NEXT_PAGE);
}

void CategoryPlaylistProc::qmlObtainCatePlayList()
{
    qDebug() << "Obtain Category And Current Play List.";
    if (m_pCateLoad->isEmpty())
    {
        m_pCateLoad->loadNodesFile();

        if (m_pCateLoad->isEmpty())
        {
            m_pCateLoad->obtain();
        } else
        {
            onCateDataPrepare();
        }
    }

    if (m_pPlayListModel->empty())
    {
        m_pProxyPlayList->loadCateItem();
    }
}

void CategoryPlaylistProc::qmlGroupCateIndex(int groud_index, int index)
{
    qDebug() << "Group ID=" << groud_index << "Current Id=" << index;

    if (groud_index < 0 || groud_index >= mCateGroupItems.size())
    {
        qWarning() << "Cate Group Item index is invalid";
        return;
    }
    VectorTable<FirstMenuUnion *> &vec = dynamic_cast<CategoryModel *>(mCateGroupItems[groud_index])->vec();

    if (index < 0 || index >= vec.size())
    {
        qWarning() << "Cate Group Sub Item index is invalid";
        return ;
    }
    if (vec[index]->id != m_pPlayListLoad->getCategoryId())
    {
        //Load Current Category playlist by id
        m_pProxyPlayList->setLoadAction(PopTipManage::LOAD_NEXT_PAGE);

        mOldCategoryId = m_pPlayListLoad->getCategoryId();
        PopTipManage::instance()->klObjectObtainStart(m_pPlayListLoad->loadByCateId(vec[index]->id)
                                                      , qqmusic::OBJECT_NAME_CATEGORY_PLAY_LIST
                                                      , PopTipManage::LOAD_MAIN_PAGE);
    } else
    {
        //表明在同样的ID上进行了点击，不需要处理
        qDebug() << "Common Clicked !!!";
    }
}

void CategoryPlaylistProc::qmlPlayListIndexClick(int index)
{
    qDebug() << "Music Item Clicked " << index;
}

void CategoryPlaylistProc::onCateDataPrepare()
{
    qDebug() << "Category Data Load Over !";
    mCateList.clear();
    ListTable<qqmusic::CateNode>::iterator it = m_pCateLoad->nodes().begin();
    for (int i = 0; it != m_pCateLoad->nodes().end(); ++it, i++)
    {
        if (mCateGroupItems.size() == i)
        {
            CategoryModel *newNode = new CategoryModel;
            mCateGroupItems.push_back(newNode);
        }

        VectorTable<FirstMenuUnion *> &vec = dynamic_cast<CategoryModel *>(mCateGroupItems[i])->vec();
        ListTable<qqmusic::CateNode::Cate>::iterator its = it->items.begin();
        for (; its != it->items.end(); ++its)
        {
            FirstMenuUnion *item = new FirstMenuUnion;
            item->id = its->categoryId;
            item->name = its->categoryName;
            vec.push_back(item);
        }

        mCateList.push_back(QStringFromByteString(it->categoryGroupName));
    }

    Q_ASSERT(mCateGroupItems.size() == mCateList.size());

    if (!mCateGroupItems.isEmpty())
    {
        // 默认下载第一项标签数据
        dynamic_cast<CategoryModel *>(mCateGroupItems[0])->setCurIndex(0);
    }
    Q_EMIT cateListChanged();
}

void CategoryPlaylistProc::onPlayListLoadOver(long ptr)
{
    qDebug() << "Data Play List Load Over --------";
    //更新页码
    setCurrentPage(0);

    //更新标签ID
    int times = 2;
    int categoryId = m_pPlayListLoad->getCategoryId();

    if (mOldCategoryId == categoryId)
    {
        return;
    }
    ListTable<qqmusic::CateNode>::iterator it = m_pCateLoad->nodes().begin();
    for (int i = 0; it != m_pCateLoad->nodes().end(); ++it, i++)
    {
        ListTable<qqmusic::CateNode::Cate>::iterator its = it->items.begin();
        for (int j = 0; its != it->items.end(); ++its, j++)
        {
            if (its->categoryId == mOldCategoryId)
            {
                dynamic_cast<CategoryModel *>(mCateGroupItems[i])->setCurIndex(-1);
                if (--times == 0) return ;
            } else if (its->categoryId == categoryId)
            {
                dynamic_cast<CategoryModel *>(mCateGroupItems[i])->setCurIndex(j);
                if (--times == 0) return ;
            }
        }
    }
}

CateItemModel *CategoryPlaylistProc::playListModel() const
{
    return m_pPlayListModel;
}
