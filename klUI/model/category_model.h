#ifndef CATEGORY_ALL_MODEL_H
#define CATEGORY_ALL_MODEL_H

#include "kl_ui_data_union.h"
#include <QAbstractListModel>
#include <QtQml>

class CategoryUnion;
class CategoryModel : public QAbstractListModel
{
    Q_OBJECT

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

    VectorTable<MusicCateUnion *> &vec()
    {
        return mVec;
    }

    ByteString getCID(int index);

public Q_SLOTS:
    void onLoadOver(long ptr);

    // 标签ID
    void qmlCateTabClick(int index);

    int  qmlGetTabIndex();
Q_SIGNALS:
    void dataLoadOver(long ptr);

    void loadError(int type, const QString &info);

protected:
    QHash<int, QByteArray> roleNames() const;

private:
    CategoryUnion                *m_pCateUnion;
    QHash<int, QByteArray>        roles;
    VectorTable<MusicCateUnion *> mVec;
};

#endif // CATEGORY_ALL_MODEL_H
