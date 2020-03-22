#include "chip_item_model.h"
#include "chip_item_union.h"
#include "data_proc.h"
#include <time.h>
#include <QDebug>

#define CHIP_ITEM_NAME     Qt::UserRole

ChipItemModel::ChipItemModel()
    : m_pUnion(NULL)
{
    roles[CHIP_ITEM_NAME]    = "chipItemName";

#if 0
    MusicChipItemUnion *uni = new MusicChipItemUnion;

    uni->name = "asdasfasf";
    mVec.push_back(uni);
    mVec.push_back(uni);
    mVec.push_back(uni);
    mVec.push_back(uni);
    mVec.push_back(uni);
    mVec.push_back(uni);
    mVec.push_back(uni);
    mVec.push_back(uni);
    mVec.push_back(uni);
    mVec.push_back(uni);
    mVec.push_back(uni);
    mVec.push_back(uni);
    mVec.push_back(uni);
    mVec.push_back(uni);
    mVec.push_back(uni);
    mVec.push_back(uni);
#endif
}

void ChipItemModel::assign(ChipItemModel *th)
{
    m_pUnion = th->m_pUnion;
    mVec.clear();
    m_pUnion->onLoadOver(this);
}

int ChipItemModel::rowCount(const QModelIndex &) const
{
    return mVec.size();
}

QVariant ChipItemModel::data(const QModelIndex &index, int role) const
{
    int i = index.row();
    if ( i < 0 || i >= (int)mVec.size())
    {
        return QVariant();
    }

    if (CHIP_ITEM_NAME == role)
    {
        return QStringFromByteString(mVec[index.row()]->name);
    }

    return QVariant();
}

void ChipItemModel::resetAll()
{
    beginResetModel();
    endResetModel();
}

void ChipItemModel::setChipItemUnion(ChipItemUnion *pUnion)
{
    m_pUnion = pUnion;
}

void ChipItemModel::clear()
{
    qDebug() << "vector clean";
    mVec.clearPtr();

    beginResetModel();
    endResetModel();
}

void ChipItemModel::chipLoadOver(long ptr)
{
    int start = mVec.size();

    if ((long)m_pUnion != ptr || !m_pUnion)
    {
        qWarning() << "Current is not need.";
        return;
    }

    m_pUnion->onLoadOver(this);

    qDebug() << "Load over, size = " << mVec.size() << "start = " << start;
//    for (int i = 0; i < mVec.size(); ++i)
//    {
//        GEN_Printf(LOG_DEBUG, "[%d] %p,", i, mVec[i]/*name.string()*/);
//    }

    if (0 == start)
    {
        beginResetModel();
        endResetModel();
    } else
    {
        beginInsertRows(QModelIndex(), start, mVec.size() - 1);
        endInsertRows();
    }
}

void ChipItemModel::chipLocalLoad(ChipItemUnion *pUnion)
{
    qDebug() << "local chip load over, chip type" << pUnion->getChipType();
    mVec.clearPtr();
    m_pUnion = pUnion;
    m_pUnion->onLoadOver(this);
    qDebug() << "LoadOver Size=" << mVec.size();

    beginResetModel();
    endResetModel();
}

void ChipItemModel::localDataChange(ChipItemUnion *pUnion)
{
    //qDebug() << "----------------" << (long)pUnion << (long)m_pUnion;
    if (pUnion == m_pUnion)
    {
        mVec.clearPtr();
        m_pUnion->onLoadOver(this);
        beginResetModel();
        endResetModel();
        Q_EMIT playingIndexChanged(0);
    }
}

int ChipItemModel::getChipType() const
{
    return m_pUnion ? m_pUnion->getChipType() : -1;
}

void ChipItemModel::loadNextPage(int loadAction)
{
    m_pUnion->loadNextPage(loadAction);
}

void ChipItemModel::chipItemClick(int index)
{
    //qDebug() << "Chip Item Click " << index;
    DataProc::instance()->chipAudioThirdChick(index);
}

bool ChipItemModel::needNextPage()
{
    return m_pUnion->loadNextPage(ChipItemUnion::LOAD_OVER_ALBUM_NORMAL_SHOW);
}

bool ChipItemModel::playNeedNextPage()
{
    //qDebug() << "Player Chip Need Next Page";
    return m_pUnion->loadNextPage(ChipItemUnion::LOAD_OVER_ALBUM_IN_PLAYVIEW);
}

void ChipItemModel::playItemClick(int index)
{
    //qDebug() << "Click Index=" << index;
    DataProc::instance()->chipPlayThirdClick(index);
}

void ChipItemModel::qmlCurrentCollectClick()
{
    //qDebug() << "Collect Current or not.";
    DataProc::instance()->currentIsCollect();
}

QHash<int, QByteArray> ChipItemModel::roleNames() const
{
    return roles;
}

int ChipItemModel::playingIndex()
{
    return DataProc::instance()->getPlayThirdIndex(this);
}

int ChipItemModel::itemCount() const
{
    //qDebug() << "itemCount: " << (long) m_pUnion;
    return m_pUnion ? m_pUnion->itemCount() : 0;
}

void ChipItemModel::clean()
{
    //qDebug() << "vector clean";
    mVec.clearPtr();
}

void ChipItemModel::getSliderBase(int &cur, int &dur, int index)
{
    MusicChipItemUnion *item = mVec[index];

    if (item->startTime.empty()
            || item->finishTime.empty())
    {
        return;
    }

    uint64_t start  = strtoull(item->startTime.string(), NULL, 10);
    uint64_t finish = strtoull(item->finishTime.string(), NULL, 10);

    cur  = (start  / 1000) % (24 * 60 * 60);
    dur  = (finish / 1000) % (24 * 60 * 60);

//    struct tm result;
//    time_t timep;
//    struct tm *localtime_r(const time_t *timep, struct tm *result);
}

bool ChipItemModel::isCollect()
{
    return DataProc::instance()->isCollect();
}
