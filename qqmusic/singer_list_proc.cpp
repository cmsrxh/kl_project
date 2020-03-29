#include <events/common_log.h>
#include "model/cate_item_model.h"
#include "model/category_model.h"
#include "qqmusicApi/qqmusic_singer_list.h"
#include "singer_list_proc.h"

SingerListProc::SingerListProc()
    : m_pSingerIndex(new CategoryModel)
    , m_pSingerArea(new CategoryModel)
    , m_pSingerSex(new CategoryModel)
    , m_pSingerGenre(new CategoryModel)
    , m_pSingerList(new CateItemModel)
    , mSingerTotals(1023), mCurrentPage(3)
{
}

void SingerListProc::obtainSingerList()
{

}

int SingerListProc::singerPages()
{
    return mSingerTotals / 80;
}

int SingerListProc::currentPage()
{
    return mCurrentPage;
}

void SingerListProc::setSingerTotal(int total)
{
    if (mSingerTotals != total)
    {
        mSingerTotals = total;
        Q_EMIT singerPagesChanged();
    }
}

void SingerListProc::setCurrentPage(int page)
{
    if (mCurrentPage != page)
    {
        mCurrentPage = page;
        Q_EMIT currentPageChanged();
    }
}

void SingerListProc::qmlSingerList(int type, int index)
{
    GEN_Printf(LOG_DEBUG, "SingerList Proc: type=%d, index=%d", type, index);
    switch (type)
    {
    case 1:
        m_pSingerIndex->setCurIndex(index);
        break;
    case 2:
        m_pSingerArea->setCurIndex(index);
        break;
    case 3:
        m_pSingerSex->setCurIndex(index);
        break;
    case 4:
        m_pSingerGenre->setCurIndex(index);
        break;
    case 5:
        m_pSingerList->setCurrenIndex(index);
        break;
    case 6:
    {
        int cur = index;
        int total = singerPages();
        if (-1 == index) //上一页
        {
            cur = (mCurrentPage < 2) ? 1 : mCurrentPage - 1;
        } else if (-2 == index) // 下一页
        {
            cur = (mCurrentPage >= total) ? total : mCurrentPage + 1;
        }
        setCurrentPage(cur);
        break;
    }
    default:
        GEN_Printf(LOG_WARN, "UNkown Index change: type=%d, index=%d", type, index);
        break;
    }
}
