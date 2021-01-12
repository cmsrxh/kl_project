#include <events/common_log.h>
#include "pl_category_model.h"
#include "cpl_cate_item_model.h"
#include <QQmlContext>
#include "application/loadApi/qqmusic_category.h"
#include "application/loadApi/qqmusic_category_playlist.h"

#include "category_playlist.h"

CategoryPlaylistProc::CategoryPlaylistProc()
    : mCateModel(new PLCategoryModel)
    , mPlayListModel(new CPLCateItemModel)
    , mPageTotals(0), mCurrentPage(1)
    , mOldCategoryId(0)
{
    connect(mPlayListModel, SIGNAL(sigPlayListUpdate()), this, SLOT(onPlayListUpdate()));
}

void CategoryPlaylistProc::init(QQmlContext *)
{
}

int CategoryPlaylistProc::pageTotals()
{
    return mPlayListModel->getTotalPages();
}

int CategoryPlaylistProc::currentPage()
{
    return mPlayListModel->getCurrentPage();
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

QObject * CategoryPlaylistProc::cateList() const
{
    return mCateModel;
}

QObject *CategoryPlaylistProc::catePlayList() const
{
    return mPlayListModel;
}

void CategoryPlaylistProc::qmlCatePageIndexOperate(int index)
{
    if (-2 == index)
    {
        mPlayListModel->loadPrevPage();
    } else if (-1 == index)
    {
        mPlayListModel->loadNextPage();
    } else
    {
        mPlayListModel->loadPage(index);
    }
}

void CategoryPlaylistProc::qmlObtainCatePlayList()
{
    qDebug() << "Obtain Category And Current Play List.";
    mCateModel->obtain();

    mPlayListModel->obtain();
}

void CategoryPlaylistProc::qmlGroupCateIndex(int groud_index, int index)
{
    int cateId = 0;
    qDebug() << "Group ID=" << groud_index << "Current Id=" << index;

    if (mCateModel->getCateId(groud_index, index, cateId))
    {
        mOldCategoryId = mPlayListModel->getCurCateId();

        qDebug() << "old =" << mOldCategoryId << "new ="<< cateId;
        if (mOldCategoryId != cateId)
        {
            mPlayListModel->loadByCateId(cateId);
        }
    }
}

void CategoryPlaylistProc::qmlPlayListIndexClick(int index)
{
    qDebug() << "Music Item Clicked " << index;
}

void CategoryPlaylistProc::onPlayListUpdate()
{
    //更新页码
    Q_EMIT currentPageChanged();

    // 更新标签ID
    int cateId = mPlayListModel->getCurCateId();

    if (mOldCategoryId != cateId)
    {
        mCateModel->updateIndex(cateId, mOldCategoryId);
    }
}


