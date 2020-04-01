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
    , mSingerTotals(0), mCurrentPage(1)
    , m_pSingerLoad(new qqmusic::SingerList)
{
    m_pSingerLoad->setUINotify(this);
    connect(this, SIGNAL(singerDataPrepare()), this, SLOT(onSingerDataPrepare()));
}

void SingerListProc::qmlObtainSingerList()
{
    qDebug() << "Obtain Singer List.";
    if (m_pSingerLoad->isEmpty())
    {                
        m_pSingerLoad->loadNodesFile();

        if (m_pSingerLoad->isEmpty())
        {
            m_pSingerLoad->obtain();
        } else if (m_pSingerList->empty())
        {
            onSingerDataPrepare();
        }
    }
}

int SingerListProc::singerPages()
{
    return mSingerTotals; // m_pSingerLoad->getTotalPages();
}

int SingerListProc::currentPage()
{
    return mCurrentPage; // m_pSingerLoad->getCurrentPage();
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
    mCurrentPage = page;
    Q_EMIT currentPageChanged();
}

void SingerListProc::dataPrepare()
{
    qDebug() << "Data Load Over !!!";
    Q_EMIT singerDataPrepare();
}

void SingerListProc::errorInfo(int type, const ByteString &err_str)
{
    switch (type)
    {
    case LOAD_EMPTY_DATA:        // 分析数据正确，但是得到的数据是空
        PopTipManage::instance()->klLoadDataExportEmpty(qqmusic::OBJECT_NAME_SINGER_LIST, PopTipManage::LOAD_MAIN_PAGE, err_str);
        break;
    case LOAD_PRISER_JSOC_ERROR: // 不能正确解析json数据
        PopTipManage::instance()->klLoadDataPriserExcept(qqmusic::OBJECT_NAME_SINGER_LIST, PopTipManage::LOAD_MAIN_PAGE, err_str);
        break;
    case LOAD_SYS_API_FAILED:    // libcurl下载反馈的错误信息
        PopTipManage::instance()->sysNetLoadApiExcept(qqmusic::OBJECT_NAME_SINGER_LIST, PopTipManage::LOAD_MAIN_PAGE, err_str);
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
    {
        int size = m_pSingerIndex->vec().size();
        if (index >= 0 && index < size)
        {
            m_pSingerLoad->loadByIndex(m_pSingerIndex->vec()[index]->id);
        }
        //m_pSingerIndex->setCurIndex(index);
        break;
    }
    case 2:
    {
        int size = m_pSingerArea->vec().size();
        if (index >= 0 && index < size)
        {
            m_pSingerLoad->loadByArea(m_pSingerArea->vec()[index]->id);
        }
        //m_pSingerArea->setCurIndex(index);
        break;
    }
    case 3:
    {
        int size = m_pSingerSex->vec().size();
        if (index >= 0 && index < size)
        {
            m_pSingerLoad->loadBySex(m_pSingerSex->vec()[index]->id);
        }
        //m_pSingerSex->setCurIndex(index);
        break;
    }
    case 4:
    {
        int size = m_pSingerGenre->vec().size();
        if (index >= 0 && index < size)
        {
            m_pSingerLoad->loadByGenre(m_pSingerGenre->vec()[index]->id);
        }
        //m_pSingerGenre->setCurIndex(index);
        break;
    }
    case 5:
        //直接设置index，会使得界面跳到开始或末尾位置
        //m_pSingerList->setCurrenIndex(index);
        break;
    case 6:
        m_pSingerChildList->setCurrenIndex(index);
        break;
    case 7:
    {
        m_pSingerLoad->loadNextPage(index);
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
    qDebug() << "Data Load over, image singer item size =" << modelSinger.size()
             << "text singer item size =" << modelChildSinger.size();
    m_pSingerList->resetAll();
    m_pSingerChildList->resetAll();

    m_pSingerLoad->ensureTagsIndex();

    if (m_pSingerIndex->isEmpty())
    {
        tagListProc(m_pSingerIndex->vec(), m_pSingerLoad->indexTag());
        m_pSingerIndex->resetAll();
    }
    m_pSingerIndex->setCurIndex(m_pSingerLoad->indexIndex());

    if (m_pSingerArea->isEmpty())
    {
        tagListProc(m_pSingerArea->vec(), m_pSingerLoad->areaTag());
        m_pSingerArea->resetAll();
    }
    m_pSingerArea->setCurIndex(m_pSingerLoad->areaIndex());

    if (m_pSingerSex->isEmpty())
    {
        tagListProc(m_pSingerSex->vec(), m_pSingerLoad->sexTag());
        m_pSingerSex->resetAll();
    }
    m_pSingerSex->setCurIndex(m_pSingerLoad->sexIndex());

    if (m_pSingerGenre->isEmpty())
    {
        tagListProc(m_pSingerGenre->vec(), m_pSingerLoad->genreTag());
        m_pSingerGenre->resetAll();
    }
    m_pSingerGenre->setCurIndex(m_pSingerLoad->genreIndex());

    setSingerTotal(m_pSingerLoad->getTotalPages());
    setCurrentPage(m_pSingerLoad->getCurrentPage());
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
