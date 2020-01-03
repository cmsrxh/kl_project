#ifndef CHIP_ITEM_MODEL_H
#define CHIP_ITEM_MODEL_H

#include "kl_ui_data_union.h"
#include <QAbstractListModel>
#include <QtQml>

class ChipItemUnion;
/**
 * @brief The ChipItemModel class
 * @details 三级标签
 */
class ChipItemModel : public QAbstractListModel
{
    Q_OBJECT

    Q_PROPERTY(int playingIndex READ playingIndex NOTIFY playingIndexChanged)

    Q_PROPERTY(int itemCount READ itemCount NOTIFY itemCountChanged)
public:
    ChipItemModel(bool isPlayModel);

    int rowCount(const QModelIndex &parent = QModelIndex()) const;

    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;

    void resetAll();

    void setChipItemUnion(ChipItemUnion *pUnion);

    void clear();

    /**
     * @brief vec
     * @return 返回的是引用
     * @warning 警告，获取结构的时候，也必须使用引用，否则定义 的局部变量会释放，vector容器中的指针指向的地址
     */
    VectorTable<MusicChipItemUnion *> &vec()
    {
        return mVec;
    }

    // property
    int  playingIndex() const;

    int  itemCount() const;

    int  size() const
    {
        return mVec.size();
    }
    void  clean();

    bool isPlayModel() const
    {
        return mIsPlayModel;
    }

    bool checkUnion(ChipItemUnion *uni)
    {
        return m_pUnion != uni;
    }

    void getSliderBase(int &cur, int &dur, int index);

public Q_SLOTS:
    void onLoadOver(long ptr);

    void chipItemClick(int index);

    void needNextPage();

    void playNeedNextPage();

    void playItemClick(int index);

Q_SIGNALS:
    void dataLoadOver(long ptr);

    void loadError(int type, const QString &info);

    // property
    void playingIndexChanged(int playingIndex);

    void itemCountChanged();

protected:
    QHash<int, QByteArray> roleNames() const;

private:
    const bool                        mIsPlayModel;
    ChipItemUnion                    *m_pUnion;
    QHash<int, QByteArray>            roles;
    VectorTable<MusicChipItemUnion *> mVec;
};

#endif // CHIP_ITEM_MODEL_H
