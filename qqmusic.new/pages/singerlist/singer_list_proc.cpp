#include <events/common_log.h>
#include "singer_tag_info.h"
#include "singer_list_model.h"
#include "singer_list_proc.h"

SingerListProc::SingerListProc()
    : m_pSingerIndex(new SingerTagInfo)
    , m_pSingerArea(new SingerTagInfo)
    , m_pSingerSex(new SingerTagInfo)
    , m_pSingerGenre(new SingerTagInfo)
    , m_pSingerList(new SingerListModel)
    , m_pSingerChildList(new SingerListModel)
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

QObject *SingerListProc::singerModel()
{
    return m_pSingerList;
}

QObject *SingerListProc::singerChildModel()
{
    return m_pSingerChildList;
}

QObject *SingerListProc::singerIndexModel()
{
    return m_pSingerIndex;
}

QObject *SingerListProc::singerAreaModel()
{
    return m_pSingerArea;
}

QObject *SingerListProc::singerSexModel()
{
    return m_pSingerSex;
}

QObject *SingerListProc::singerGenreModel()
{
    return m_pSingerGenre;
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
    qDebug () << "Load singer list" << err_str.string();
    switch (type)
    {
    case LOAD_EMPTY_DATA:        // 分析数据正确，但是得到的数据是空
        break;
    case LOAD_PRISER_JSOC_ERROR: // 不能正确解析json数据
        break;
    case LOAD_SYS_API_FAILED:    // libcurl下载反馈的错误信息
        break;
    default :
        SF_ASSERT(0);
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
        int size = m_pSingerIndex->size();
        if (index >= 0 && index < size)
        {
            m_pSingerLoad->loadByIndex(m_pSingerIndex->getId(index));
        }
        //m_pSingerIndex->setCurIndex(index);
        break;
    }
    case 2:
    {
        int size = m_pSingerArea->size();
        if (index >= 0 && index < size)
        {
            m_pSingerLoad->loadByArea(m_pSingerArea->getId(index));
        }
        //m_pSingerArea->setCurIndex(index);
        break;
    }
    case 3:
    {
        int size = m_pSingerSex->size();
        if (index >= 0 && index < size)
        {
            m_pSingerLoad->loadBySex(m_pSingerSex->getId(index));
        }
        //m_pSingerSex->setCurIndex(index);
        break;
    }
    case 4:
    {
        int size = m_pSingerGenre->size();
        if (index >= 0 && index < size)
        {
            m_pSingerLoad->loadByGenre(m_pSingerGenre->getId(index));
        }
        //m_pSingerGenre->setCurIndex(index);
        break;
    }
    case 5:
        //直接设置index，会使得界面跳到开始或末尾位置
        //m_pSingerList->setCurrenIndex(index);
        break;
    case 6:
        m_pSingerChildList->setCurrentIndex(index);
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
    m_pSingerList->clean();
    m_pSingerChildList->clean();

    ListTable<qqmusic::SingerNode> &lst = m_pSingerLoad->nodes();
    ListTable<qqmusic::SingerNode>::iterator it = lst.begin();
    for (int i = 0; it != lst.end(); ++it, ++i)
    {
        SingerListModel::ItemInfo item;

        item.id  = it->singer_id;
        item.name = it->singer_name;
        item.img  = it->singer_pic;

        if (i < 10)
        {
            m_pSingerList->pushItem(item);
        } else
        {
            m_pSingerChildList->pushItem(item);
        }
    }
    qDebug() << "Data Load over, image singer item size =" << m_pSingerList->size()
             << "text singer item size =" << m_pSingerChildList->size();
    m_pSingerList->reset();
    m_pSingerChildList->reset();

    m_pSingerLoad->ensureTagsIndex();

    if (m_pSingerIndex->isEmpty())
    {
        m_pSingerIndex->toTagInfo(m_pSingerLoad->indexTag());
        m_pSingerIndex->resetAll();
    }
    m_pSingerIndex->setCurIndex(m_pSingerLoad->indexIndex());

    if (m_pSingerArea->isEmpty())
    {
        m_pSingerArea->toTagInfo(m_pSingerLoad->areaTag());
        m_pSingerArea->resetAll();
    }
    m_pSingerArea->setCurIndex(m_pSingerLoad->areaIndex());

    if (m_pSingerSex->isEmpty())
    {
        m_pSingerSex->toTagInfo(m_pSingerLoad->sexTag());
        m_pSingerSex->resetAll();
    }
    m_pSingerSex->setCurIndex(m_pSingerLoad->sexIndex());

    if (m_pSingerGenre->isEmpty())
    {
        m_pSingerGenre->toTagInfo(m_pSingerLoad->genreTag());
        m_pSingerGenre->resetAll();
    }
    m_pSingerGenre->setCurIndex(m_pSingerLoad->genreIndex());

    setSingerTotal(m_pSingerLoad->getTotalPages());
    setCurrentPage(m_pSingerLoad->getCurrentPage());
}

