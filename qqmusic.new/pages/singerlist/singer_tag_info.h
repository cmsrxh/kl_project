#ifndef SINGER_TAG_INFO_H
#define SINGER_TAG_INFO_H

#include <common/load_model_base.h>
#include "application/loadApi/qqmusic_singer_list.h"

class SingerTagInfo : public QAbstractListModel
{
    Q_OBJECT

    Q_PROPERTY(int curIndex READ curIndex WRITE setCurIndex NOTIFY curIndexChanged)
public:
    SingerTagInfo();
    ~SingerTagInfo();

    int size ()
    {
        return mID.size();
    }

    int getId(int i)
    {
        return mID[i];
    }

    bool isEmpty()
    {
        return mID.empty();
    }

    void resetAll();

    void toTagInfo(ListTable<qqmusic::TagNode> &tag);

    int  curIndex() const;
    void setCurIndex(int curIndex);

    int rowCount(const QModelIndex &parent = QModelIndex()) const;

    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;

    QHash<int, QByteArray> roleNames() const
    {
        return roles;
    }

Q_SIGNALS:
    void curIndexChanged();

private:
    int                     mCurIndex;
    QHash<int, QByteArray>  roles;
    QStringList             mName;
    QVector<int>            mID;
};

#endif // SINGER_TAG_INFO_H
