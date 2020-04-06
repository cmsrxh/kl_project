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
    ~CateItemModel();

    int rowCount(const QModelIndex &parent = QModelIndex()) const;

    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;

    void resetAll();

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

Q_SIGNALS:
    void dataLoadOver(long ptr);

    // property
    void currenIndexChanged(int currentIndex);

protected:
    QHash<int, QByteArray> roleNames() const;

private:
    int                               mCurrenIndex;
    CateItemUnion                    *m_pUnion;
    QHash<int, QByteArray>            roles;
    VectorTable<SecondMenuUnion *>    mVec;
};

#endif // CATE_ITEM_MODEL_H
