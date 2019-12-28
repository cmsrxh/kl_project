#include "chip_item_model.h"
#include "chip_item_union.h"
#include "kl_data_proc.h"
#include <QDebug>

#define CHIP_ITEM_NAME     Qt::UserRole

ChipItemModel::ChipItemModel()
    : m_pUnion(NULL)
    , mPlayingIndex(-1)
{
    roles[CHIP_ITEM_NAME]    = "chipItemName";

    connect(this, SIGNAL(dataLoadOver(long)), this, SLOT(onLoadOver(long)));
}

int ChipItemModel::rowCount(const QModelIndex &) const
{
    return mVec.size();
}

QVariant ChipItemModel::data(const QModelIndex &index, int role) const
{
    if ( index.row() < 0 || index.row() >= (int)mVec.size())
    {
        return QVariant();
    }

    if (CHIP_ITEM_NAME == role)
    {
        return mVec[index.row()]->name.string();
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
    mVec.clearPtr();

    beginResetModel();
    endResetModel();
}

void ChipItemModel::onLoadOver(long ptr)
{
    int start = mVec.size();

    assert(ptr == (long)m_pUnion);

    if (mVec.empty())
    {
        KLDataProc::instance()->enterAlbumView();
    }

    m_pUnion->onLoadOver();

    qDebug() << "Load over, size = " << mVec.size() << "start = " << start;
    if (0 == start)
    {
        beginResetModel();
        endResetModel();
    } else
    {
        beginInsertRows(QModelIndex(), start, mVec.size());
        endInsertRows();
    }
}

void ChipItemModel::chipItemClick(int index)
{
    qDebug() << "Chip Item Click " << index;
    KLDataProc::instance()->chipItemChick(index);
}

void ChipItemModel::needNextPage()
{
    qDebug() << "Chip Need Next Page";
    m_pUnion->loadNextPage();
}

QHash<int, QByteArray> ChipItemModel::roleNames() const
{
    return roles;
}

int ChipItemModel::playingIndex() const
{
    return mPlayingIndex;
}

void ChipItemModel::setPlayingIndex(int playingIndex)
{
    if (mPlayingIndex != playingIndex)
    {
        mPlayingIndex = playingIndex;
        Q_EMIT playingIndexChanged(playingIndex);
    }
}

int ChipItemModel::itemCount() const
{
    return m_pUnion->itemCount();
}
