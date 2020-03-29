#ifndef CATE_ITEM_MODEL_H
#define CATE_ITEM_MODEL_H

#include "ui_data_union.h"
#include <QAbstractListModel>
#include <QtQml>

class CateItemUnion;
/**
 * @brief The CateItemModel class
 * @details 二级标签
 */
class CateItemModel : public QAbstractListModel
{
    Q_OBJECT
    Q_PROPERTY(int currenIndex READ currenIndex NOTIFY currenIndexChanged)
public:    
    CateItemModel();

    int rowCount(const QModelIndex &parent = QModelIndex()) const;

    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;

    void resetAll();

    bool haveNext() const;

    int  currenIndex() const;
    void setCurrenIndex(int currenIndex);

    void setCateItemUnion(CateItemUnion *pUnion);

    void clear();

    VectorTable<SecondMenuUnion *> &vec()
    {
        return mVec;
    }

    void isCollectItemContentChange(int i, bool en);

    int getCateType();

    bool empty()
    {
        return mVec.empty();
    }

public Q_SLOTS:
    void onLoadOver(long ptr);

    bool qmlCtgNextPage();

    // album info item click
    void qmlClickCategory(int index);

    // broadcast info item click
    void qmlClickBDCItem(int index, bool isInArea);

    // broadcast info item click for collect or not
    void qmlClickBDCItemCollect(int index, bool isCollect);
Q_SIGNALS:
    void dataLoadOver(long ptr);

    // 废弃使用，使用全局通知
    // void loadError(int type, const QString &info);

    // property
    void currenIndexChanged(int currentIndex);

    /**
     * @brief loadStartNewPage
     * @details 用于消息弹框，表示下载第一次开始
     */
    void loadStartNewPage();

    /**
     * @brief loadStartNextPage
     * @details 用于消息弹框，表示下载下一页开始
     */
    void loadStartNextPage();

protected:
    QHash<int, QByteArray> roleNames() const;

private:
    int                               mCurrenIndex;
    CateItemUnion                    *m_pUnion;
    QHash<int, QByteArray>            roles;
    VectorTable<SecondMenuUnion *> mVec;
};

#endif // CATE_ITEM_MODEL_H
