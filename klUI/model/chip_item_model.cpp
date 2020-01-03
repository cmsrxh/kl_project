#include "chip_item_model.h"
#include "chip_item_union.h"
#include "kl_data_proc.h"
#include <time.h>
#include <QDebug>

#define CHIP_ITEM_NAME     Qt::UserRole

ChipItemModel::ChipItemModel(bool isPlayModel)
    : mIsPlayModel(isPlayModel), m_pUnion(NULL)
{
    roles[CHIP_ITEM_NAME]    = "chipItemName";

    connect(this, SIGNAL(dataLoadOver(long)), this, SLOT(onLoadOver(long)));

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

void ChipItemModel::onLoadOver(long ptr)
{
    int start = mVec.size();

    Q_ASSERT(ptr == (long)m_pUnion);

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

void ChipItemModel::chipItemClick(int index)
{
    qDebug() << "Chip Item Click " << index;
    KLDataProc::instance()->chipAudioThirdChick(index);
}

void ChipItemModel::needNextPage()
{
    qDebug() << "Chip Need Next Page";
    m_pUnion->loadNextPage(ChipItemUnion::LOAD_OVER_ALBUM_NORMAL_SHOW);
}

void ChipItemModel::playNeedNextPage()
{
    qDebug() << "Player Chip Need Next Page";
    m_pUnion->loadNextPage(ChipItemUnion::LOAD_OVER_ALBUM_IN_PLAYVIEW);
}

void ChipItemModel::playItemClick(int index)
{
    qDebug() << "Click Index=" << index;
    KLDataProc::instance()->chipPlayThirdClick(index);
}

QHash<int, QByteArray> ChipItemModel::roleNames() const
{
    return roles;
}

int ChipItemModel::playingIndex() const
{
    return mIsPlayModel ? KLDataProc::instance()->getPlayThirdIndex()
                        : KLDataProc::instance()->getChipAudioThirdIndex();
}

int ChipItemModel::itemCount() const
{
    return m_pUnion->itemCount();
}

void ChipItemModel::clean()
{
    qDebug() << "vector clean";
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
