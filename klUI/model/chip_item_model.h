#ifndef CHIP_ITEM_MODEL_H
#define CHIP_ITEM_MODEL_H

#include "kl_ui_data_union.h"
#include <QAbstractListModel>
#include <QtQml>

class ChipItemUnion;
class ChipItemModel : public QAbstractListModel
{
    Q_OBJECT

    Q_PROPERTY(int playingIndex READ playingIndex WRITE setPlayingIndex NOTIFY playingIndexChanged)
public:
    ChipItemModel();

    int rowCount(const QModelIndex &parent = QModelIndex()) const;

    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;

    void resetAll();

    void setChipItemUnion(ChipItemUnion *pUnion);

    void clear();

    VectorTable<MusicChipItemUnion *> &vec()
    {
        return mVec;
    }

    // property
    int  playingIndex() const;
    void setPlayingIndex(int playingIndex);


public Q_SLOTS:
    void onLoadOver(long ptr);

    void chipItemClick(int index);

    void needNextPage();

    int  itemCount() const;
Q_SIGNALS:
    void dataLoadOver(long ptr);

    void loadError(int type, const QString &info);

    // property
    void playingIndexChanged(int playingIndex);

protected:
    QHash<int, QByteArray> roleNames() const;

private:
    ChipItemUnion                    *m_pUnion;
    int                               mPlayingIndex;
    QHash<int, QByteArray>            roles;
    VectorTable<MusicChipItemUnion *> mVec;
};

#endif // CHIP_ITEM_MODEL_H
