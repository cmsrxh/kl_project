#ifndef CATE_ITEM_MODEL_H
#define CATE_ITEM_MODEL_H

#include "kl_ui_data_union.h"
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

    // album index
    Q_PROPERTY(int currenIndex READ currenIndex NOTIFY currenIndexChanged)

    // broadcast index
    Q_PROPERTY(int currenBDCIndex READ currenBDCIndex NOTIFY currenBDCIndexChanged)
public:    
    CateItemModel();

    int rowCount(const QModelIndex &parent = QModelIndex()) const;

    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;

    void resetAll();

    bool haveNext() const;

    int  currenIndex() const;

    int  currenBDCIndex() const;

    void setCateItemUnion(CateItemUnion *pUnion);

    void clear();

    VectorTable<MusicCateItemUnion *> &vec()
    {
        return mVec;
    }

    void isCollectItemContentChange(int i, bool en);

public Q_SLOTS:
    void onLoadOver(long ptr);

    void qmlCtgNextPage();

    // album info item click
    void qmlClickCategory(int index);

    // broadcast info item click
    void qmlClickBDCItem(int index, bool isInArea);

    // broadcast info item click for collect or not
    void qmlClickBDCItemCollect(int index, bool isCollect);
Q_SIGNALS:
    void dataLoadOver(long ptr);

    void loadError(int type, const QString &info);

    // property
    void currenIndexChanged(int currentIndex);
    void currenBDCIndexChanged(int currenBDCIndex);

protected:
    QHash<int, QByteArray> roleNames() const;

private:
    CateItemUnion                    *m_pUnion;
    QHash<int, QByteArray>            roles;
    VectorTable<MusicCateItemUnion *> mVec;
};

#endif // CATE_ITEM_MODEL_H
