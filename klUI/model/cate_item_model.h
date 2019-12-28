#ifndef CATE_ITEM_MODEL_H
#define CATE_ITEM_MODEL_H

#include "kl_ui_data_union.h"
#include <QAbstractListModel>
#include <QtQml>

class CateItemUnion;
class CateItemModel : public QAbstractListModel
{
    Q_OBJECT

    Q_PROPERTY(bool haveNext READ haveNext)
public:    
    CateItemModel();

    int rowCount(const QModelIndex &parent = QModelIndex()) const;

    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;

    void resetAll();

    bool haveNext() const;

    void setCateItemUnion(CateItemUnion *pUnion);

    void clear();

    VectorTable<MusicCateItemUnion *> &vec()
    {
        return mVec;
    }
public Q_SLOTS:
    void onLoadOver(long ptr);

    void qmlCtgNextPage();

    void qmlClickCategory(int index);

Q_SIGNALS:
    void dataLoadOver(long ptr);

    void loadError(int type, const QString &info);

protected:
    QHash<int, QByteArray> roleNames() const;

private:
    CateItemUnion                    *m_pUnion;
    QHash<int, QByteArray>            roles;
    VectorTable<MusicCateItemUnion *> mVec;
};

#endif // CATE_ITEM_MODEL_H
