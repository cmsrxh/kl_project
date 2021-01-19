#include <QDebug>
#include "singer_tag_info.h"

SingerTagInfo::SingerTagInfo()
    : mCurIndex(-1)
{
    roles[Qt::UserRole]    = "ctgname";
}

SingerTagInfo::~SingerTagInfo()
{
    qDebug() << "singer tag info destruct.";
    Q_ASSERT(0);
}

void SingerTagInfo::resetAll()
{
    beginResetModel();
    endResetModel();
}

void SingerTagInfo::toTagInfo(ListTable<qqmusic::TagNode> &tag)
{
    ListTable<qqmusic::TagNode>::iterator it = tag.begin();

    for (; it != tag.end(); ++it)
    {
        ByteString name = it->name;
        mID.push_back(it->id);

        mName.push_back(QStringFromByteString(name));
    }
}

int SingerTagInfo::curIndex() const
{
    return mCurIndex;
}

void SingerTagInfo::setCurIndex(int curIndex)
{
    mCurIndex = curIndex;
    Q_EMIT curIndexChanged();
}

int SingerTagInfo::rowCount(const QModelIndex &) const
{
    return mID.size();
}

QVariant SingerTagInfo::data(const QModelIndex &index, int role) const
{
    int i = index.row();
    if ( i < 0 || i >= (int)mName.size())
    {
        return QVariant();
    }

    return mName[i];
}
