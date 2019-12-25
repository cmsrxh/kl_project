#ifndef CATEGORY_ALL_MODEL_H
#define CATEGORY_ALL_MODEL_H

#include "kl_url/kl_category_all.h"
#include <QAbstractListModel>
#include <QtQml>

class CateItemModel;
class CategoryAllModel : public QAbstractListModel, public kl::UINotifyIface
{
    Q_OBJECT
    // 标签ID
//    Q_PROPERTY(int cateIndex READ cateIndex WRITE setCateIndex NOTIFY cateIndexChanged)
    // 当前标签下面的专辑项ID
//    Q_PROPERTY(int cateItemIndex READ cateItemIndex WRITE setCateItemIndex NOTIFY cateItemIndexChanged)
public:
    CategoryAllModel();

    int rowCount(const QModelIndex &parent = QModelIndex()) const;

    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;

    void resetAll();

    void dataPrepare();

    void errorInfo(int, const char *);

    void setCateItemModel(CateItemModel *cate)
    {
        m_pTmpUse = cate;
    }
public Q_SLOTS:
    void onLoadOver();

    void qmlCtgNextPage();

    void qmlClickCategory(int index);

    // 标签ID
    void qmlCateTabClick(int index);
Q_SIGNALS:
    void dataLoadOver();
    void loadError(int type, const QString &info);
protected:
    QHash<int, QByteArray> roleNames() const;

private:
    QHash<int, QByteArray>  roles;
    kl::CategoryAll        *m_pCate;
    ListTable<kl::CateMain>::vector mVec;
    CateItemModel *m_pTmpUse;
};

#endif // CATEGORY_ALL_MODEL_H
