#ifndef CHIP_ITEM_MODEL_H
#define CHIP_ITEM_MODEL_H

#include "ui_data_union.h"
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

    Q_PROPERTY(bool isCollect READ isCollect NOTIFY isCollectChanged)

    ChipItemModel(ChipItemModel &);
public:
    ChipItemModel();

//    ChipItemModel &operator=(ChipItemModel &);
//    bool operator==(ChipItemModel &);
    void assign(ChipItemModel *);
    bool equal(ChipItemModel *th)
    {
        return m_pUnion == th->m_pUnion;
    }

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
    int  playingIndex();

    int  itemCount() const;

    int  size() const
    {
        return mVec.size();
    }
    void  clean();

    bool checkUnion(ChipItemUnion *uni)
    {
        return m_pUnion != uni;
    }

    /**
     * @brief getSliderBase
     * @param cur [out] 开始时间
     * @param dur [out] 结束时间
     * @param index [in]
     * @details 电台播放的时候获取当前播放节目的时间进度信息
     */
    void getSliderBase(int &cur, int &dur, int index);

    /**
     * @brief isCollect
     * @return 返回当前播放的节目是否在收藏列表中
     */
    bool isCollect();

    /**
     * @brief chipLoadOver
     * @param ptr [in] ChipItemUnion 实例指针
     * @details 必须在主线程中调用，即在信号槽函数中调用
     */
    void chipLoadOver(long ptr);
    void chipLocalLoad(ChipItemUnion *pUnion);

    /**
     * @brief localDataChange
     * @param pUnion [in] 当前播放源类型Local Chip Data
     * @details 表示本地数据收藏、历史记录等数据发生变化，如果当前播放列表是基于本地数据，则就要同时应用到播放列表
     */
    void localDataChange(ChipItemUnion *pUnion);

    bool isEmpty()
    {
        return mVec.empty();
    }
    int getChipType() const;

    void loadNextPage(int loadAction);

    ChipItemUnion *getPUnion() const
    {
        return m_pUnion;
    }

public Q_SLOTS:

    void chipItemClick(int index);

    bool needNextPage();

    bool playNeedNextPage();

    void playItemClick(int index);

    void qmlCurrentCollectClick();
Q_SIGNALS:    
    // NOT USED
    // void loadError(int type, const QString &info);

    // property
    void playingIndexChanged(int playingIndex);

    void itemCountChanged();

    // collect current
    void isCollectChanged(bool isCollect);

    /**
     * @brief loadStartNewPage
     * @details 用于消息弹框，表示下载第一次开始
     */
    void loadStartNewPage();

    /**
     * @brief loadStartNextPage
     * @details  用于消息弹框，表示下载下一页开始
     */
    void loadStartNextPage();

    /**
     * @brief loadOver
     * @details 下载完成，取消弹框
     */
    void loadOver();
protected:
    QHash<int, QByteArray> roleNames() const;

private:
    ChipItemUnion                    *m_pUnion;
    QHash<int, QByteArray>            roles;
    VectorTable<MusicChipItemUnion *> mVec;
};

#endif // CHIP_ITEM_MODEL_H
