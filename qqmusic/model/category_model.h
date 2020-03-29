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

    // property
    int curIndex() const;

    void setCurIndex(int curIndex);

public Q_SLOTS:
    void onLoadOver(long ptr);

    // tab标签ID click
    void qmlCateTabClick(int index);

Q_SIGNALS:
    void dataLoadOver(long ptr);

    //
    void curIndexChanged();

    /**
     * @brief loadStart
     * @details 用于消息弹框，表示下载第一次开始
     */
    void loadStart();
protected:
    QHash<int, QByteArray> roleNames() const;

private:
    int                           mCurIndex;
    CategoryUnion                *m_pCateUnion;
    QHash<int, QByteArray>        roles;
    VectorTable<FirstMenuUnion *> mVec;
};

#endif // CATEGORY_ALL_MODEL_H
