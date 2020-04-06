#ifndef CATEGORY_ALL_MODEL_H
#define CATEGORY_ALL_MODEL_H

#include "ui_data_union.h"
#include <QAbstractListModel>
#include <QtQml>

class CategoryUnion;
/**
 * @brief The CategoryModel class
 * @details 一级标签
 */
class CategoryModel : public QAbstractListModel
{
    Q_OBJECT
    Q_PROPERTY(int curIndex READ curIndex NOTIFY curIndexChanged)

public:
    CategoryModel();
    ~CategoryModel();

    int rowCount(const QModelIndex &parent = QModelIndex()) const;

    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;

    void resetAll();  

    CategoryUnion *cateUnion() const
    {
        return m_pCateUnion;
    }

    void setCateUnion(CategoryUnion *pCateUnion)
    {
        m_pCateUnion = pCateUnion;
    }

    VectorTable<FirstMenuUnion *> &vec()
    {
        return mVec;
    }
    bool isEmpty()
    {
        return mVec.empty();
    }

    //! 属性获取函数
    int curIndex() const;
    //! 属性设置函数
    void setCurIndex(int curIndex);

public Q_SLOTS:
    void onLoadOver(long ptr);

Q_SIGNALS:
    //! 数据下载完成通知
    void dataLoadOver(long ptr);

    //! 当前index通知变化
    void curIndexChanged();

protected:
    QHash<int, QByteArray> roleNames() const;

private:
    int                           mCurIndex;
    CategoryUnion                *m_pCateUnion;
    QHash<int, QByteArray>        roles;
    VectorTable<FirstMenuUnion *> mVec;
};

#endif // CATEGORY_ALL_MODEL_H
