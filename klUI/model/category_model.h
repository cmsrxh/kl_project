#ifndef CATEGORY_ALL_MODEL_H
#define CATEGORY_ALL_MODEL_H

#include "kl_ui_data_union.h"
#include <QAbstractListModel>
#include <QtQml>

class CateItemModel;
class CategoryModel : public QAbstractListModel, public kl::UINotifyIface
{
    Q_OBJECT

    Q_PROPERTY(QList<CateItemModel *> cateItem READ cateItem)

public:
    enum {
        MAIN_CATE = 0,
        SUB_CATE  = 1,
    };
    CategoryModel(int cate_type);

    void loadCategory(int cate_type, int cid);

    int rowCount(const QModelIndex &parent = QModelIndex()) const;

    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;

    void resetAll();

    void dataPrepare();

    void errorInfo(int, const char *);

    QList<CateItemModel *> cateItem() const
    {
        return mCateItem;
    }

public Q_SLOTS:
    void onLoadOver();

    // 标签ID
    void qmlCateTabClick(int index);

Q_SIGNALS:
    void dataLoadOver();
    void loadError(int type, const QString &info);

protected:
    QHash<int, QByteArray> roleNames() const;

private:
    const int                   mCateType;
    UICategory                 *m_pCate;
    QHash<int, QByteArray>      roles;

    VectorTable<MusicCateUnion> mVec;
    QList<CateItemModel *>      mCateItem;

   void genCatesByCateMain(ListTable<kl::CateMain> &nodes);
   void genCatesByCateSub(ListTable<kl::CateSub> &nodes);
};

#endif // CATEGORY_ALL_MODEL_H
