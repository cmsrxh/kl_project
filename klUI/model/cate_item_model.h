#ifndef CATE_ITEM_MODEL_H
#define CATE_ITEM_MODEL_H

#include "kl_ui_data_union.h"
#include <QAbstractListModel>
#include <QtQml>

// 下面有专辑: AlbumList
// 推荐: OperateList
// 智能电台: TypeRadioList
class CateItemModel : public QAbstractListModel, public kl::UINotifyIface
{
    Q_OBJECT
public:
    enum {
        CATE_ITEM_ALBUM,
        CATE_ITEM_OPERATE,
        CATE_ITEM_TYPE_RADIO,
    };
    CateItemModel(int cate_item_type);

    void loadCateItem(int cate_item_type, int cid = -1, bool sorttype = true);

    int rowCount(const QModelIndex &parent = QModelIndex()) const;

    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;

    void resetAll();

    void dataPrepare();

    void errorInfo(int, const char *);

public Q_SLOTS:
    void onLoadOver();

    void qmlCtgNextPage();

    void qmlClickCategory(int index);

Q_SIGNALS:
    void dataLoadOver();
    void loadError(int type, const QString &info);

protected:
    QHash<int, QByteArray> roleNames() const;

private:
    const int                       mCateItemType;
    UICategoryItemList             *m_pCateItem;
    QHash<int, QByteArray>          roles;
    VectorTable<MusicCateItemUnion> mVec;

    void genCateItemByAlbumItem(ListTable<kl::AlbumItem> &nodes);
    void genCateItemByOperate(ListTable<kl::Operate> &nodes);
    void genCateItemByTypeRadio(ListTable<kl::TypeRadio> &nodes);
};

#endif // CATE_ITEM_MODEL_H
