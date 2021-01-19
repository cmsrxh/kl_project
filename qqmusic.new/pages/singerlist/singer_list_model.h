#ifndef SINGER_LIST_MODEL_H
#define SINGER_LIST_MODEL_H

#include <common/load_model_base.h>

class SingerListModel : public QAbstractListModel
{
    Q_OBJECT

    Q_PROPERTY(int currentIndex READ currentIndex WRITE setCurrentIndex NOTIFY currentIndexChanged)
public:
    struct ItemInfo
    {
        ByteString id;
        ByteString name;
        ByteString img;
    };
    SingerListModel();

    void reset();

    int rowCount(const QModelIndex &parent = QModelIndex()) const;

    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;

    QHash<int, QByteArray> roleNames() const
    {
        return roles;
    }
    void pushItem(const ItemInfo &info)
    {
        mVec.push_back(info);
    }

    bool empty() const
    {
        return mVec.empty();
    }

    void clean ()
    {
        mVec.clear();
    }

    int size()
    {
        return mVec.size();
    }

    int  currentIndex() const;
    void setCurrentIndex(int currentIndex);

Q_SIGNALS:
    void currentIndexChanged();

private:
    int                      mCurrentIndex;
    QHash<int, QByteArray>   roles;
    QVector<ItemInfo>        mVec;
};

#endif // SINGER_LIST_MODEL_H
