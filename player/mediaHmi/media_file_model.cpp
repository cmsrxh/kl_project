#include "media_file_model.h"
#include "media_hmi_i_face.h"
#include <QDebug>

MediaFileModel::MediaFileModel(int mediaType, QObject *parent)
    : QAbstractListModel(parent)
    , mMediaType(mediaType), mCurrentIndex(-1)
    , mPlayMode(Media::LOOP_MODE)
{
    roles[FileName] = "filename";

    connect(this, SIGNAL(updateMediaFile()), this, SLOT(resetAll()), Qt::QueuedConnection);
}

QVariant MediaFileModel::data(const QModelIndex &index, int role) const
{
    if( index.row() < 0 || index.row() >= (int)mList.size())
    {
        return QVariant();
    }

    const char *file = mList[index.row()].c_str();

    if(FileName == role)
    {
        const char *name = ::rindex(file, '/');

        name = name ? 1 + name : file;

        return name;
    }

    return QVariant();
}

void MediaFileModel::resetAll()
{
    qDebug() << "list size: " << mList.size() ;
//    if(0 == head)
//    {
        beginResetModel();
        endResetModel();
//    }else
//    {
//        beginInsertRows(QModelIndex(), head, tail);;
//        endInsertRows();
        //    }
}

void MediaFileModel::mediaItemClicked(int index)
{
    qDebug() << mMediaType << index;

    MediaHMIIFace::instance()->playFile(mMediaType, index);
    MediaHMIIFace::instance()->setFile(mList[index].c_str());
}

void MediaFileModel::appendList(Media::vector_s &lst, bool haveNext, int startPos)
{
    if(startPos == 0)
    {
        mList.swap(lst);
    }else
    {
        for(const std::string &item : lst)
        {
            mList.push_back(item);
        }
    }

    head = startPos;
    tail = mList.size() - 1;
    this->mHaveNext = haveNext;
}

int MediaFileModel::currentIndex() const
{
    return mCurrentIndex;
}

void MediaFileModel::setCurrentIndex(int currentIndex)
{
    if(mCurrentIndex != currentIndex)
    {
        mCurrentIndex = currentIndex;
        Q_EMIT currentIndexChanged();
    }
}

int  MediaFileModel::playMode() const
{
    return mPlayMode;
}

void MediaFileModel::setPlayMode(int playMode)
{
    //qDebug() << "----------------------------" << playMode;
    mPlayMode = playMode;
    Q_EMIT playModeChanged();
}

