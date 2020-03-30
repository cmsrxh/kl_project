#include <events/common_log.h>
#include "model/cate_item_model.h"
#include "model/category_model.h"
#include "qqmusicApi/qqmusic_singer_list.h"
#include "pop_tip_manage.h"
#include "qqmusicApi/qqmusic_singer_list.h"
#include "singer_list_proc.h"

SingerListProc::SingerListProc()
    : m_pSingerIndex(new CategoryModel)
    , m_pSingerArea(new CategoryModel)
    , m_pSingerSex(new CategoryModel)
    , m_pSingerGenre(new CategoryModel)
    , m_pSingerList(new CateItemModel)
    , m_pSingerChildList(new CateItemModel)
    , mSingerTotals(1023), mCurrentPage(3)
    , m_pSingerLoad(new qqmusic::SingerList)
{
    m_pSingerLoad->setUINotify(this);
    connect(this, SIGNAL(singerDataPrepare()), this, SLOT(onSingerDataPrepare()));
}

void SingerListProc::obtainSingerList()
{
    if (m_pSingerLoad->isEmpty())
    {
        m_pSingerLoad->obtain();
    }
}

int SingerListProc::singerPages()
{
    return m_pSingerLoad->getTotalPages();
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

void SingerListProc::dataPrepare()
{
    Q_EMIT singerDataPrepare();
}

void SingerListProc::errorInfo(int type, const ByteString &err_str)
{
    switch (type)
    {
    case LOAD_EMPTY_DATA:        // 分析数据正确，但是得到的数据是空
        PopTipManage::instance()->klLoadDataExportEmpty(mCateType, PopTipManage::LOAD_MAIN_PAGE, err_str);
        break;
    case LOAD_PRISER_JSOC_ERROR: // 不能正确解析json数据
        PopTipManage::instance()->klLoadDataPriserExcept(mCateType, PopTipManage::LOAD_MAIN_PAGE, err_str);
        break;
    case LOAD_SYS_API_FAILED:    // libcurl下载反馈的错误信息
        PopTipManage::instance()->sysNetLoadApiExcept(mCateType, PopTipManage::LOAD_MAIN_PAGE, err_str);
        break;
    default :
        assert(0);
        break;
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
        m_pSingerChildList->setCurrenIndex(index);
        break;
    case 7:
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

void SingerListProc::onSingerDataPrepare()
{
    VectorTable<SecondMenuUnion *> &modelSinger = m_pSingerList->vec();
    VectorTable<SecondMenuUnion *> &modelChildSinger = m_pSingerChildList->vec();

    modelSinger.clearPtr();
    modelChildSinger.clearPtr();

    ListTable<qqmusic::SingerNode> &lst = m_pSingerLoad->nodes();
    ListTable<qqmusic::SingerNode>::iterator it = lst.begin();
    for (int i = 0; it != lst.end(); ++it, ++i)
    {
        SecondMenuUnion *sec = new SecondMenuUnion;
        sec->id = it->singer_id;
        sec->name = it->singer_name;
        sec->img = it->singer_pic;
        sec->playUrl = it->singer_mid;

        if (i < 10)
        {
            modelSinger.push_back(sec);
        } else
        {
            modelChildSinger.push_back(sec);
        }
    }

    tagListProc(m_pSingerIndex->vec(), m_pSingerLoad->indexTag());
    tagListProc(m_pSingerArea->vec(), m_pSingerLoad->areaTag());
    tagListProc(m_pSingerSex->vec(), m_pSingerLoad->sexTag());
    tagListProc(m_pSingerGenre->vec(), m_pSingerLoad->genreTag());
}

void SingerListProc::tagListProc(VectorTable<FirstMenuUnion *> &vec,
                                 ListTable<qqmusic::TagNode> &tag)
{
    ListTable<qqmusic::TagNode>::iterator it = tag.begin();

    for (; it != tag.end(); ++it)
    {
        FirstMenuUnion *fist = new FirstMenuUnion;
        fist->id = it->id;
        fist->name = it->name;
        vec.push_back(fist);
    }
}
