#ifndef CATE_ITEM_MODEL_H
#define CATE_ITEM_MODEL_H

#include "kl_url/kl_type_radio_list.h"
#include "kl_url/kl_album_list.h"
#include "kl_url/kl_operate_list.h"
#include <QAbstractListModel>
#include <QtQml>

// 下面有专辑: AlbumList
// 推荐: OperateList
// 智能电台: TypeRadioList
class CateItemModel : public QAbstractListModel, public kl::UINotifyIface
{
    Q_OBJECT
public:
    CateItemModel();

    int rowCount(const QModelIndex &parent = QModelIndex()) const;

    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;

    void resetAll();

    void dataPrepare();

    void errorInfo(int, const char *);

    void obtain();

public Q_SLOTS:
    void onLoadOver();

Q_SIGNALS:
    void dataLoadOver();
    void loadError(int type, const QString &info);

protected:
    QHash<int, QByteArray> roleNames() const;

private:
    QHash<int, QByteArray>  roles;

    kl::OperateList           *m_pCateItem;

    ListTable<kl::Operate>::vector mVec;
};

#endif // CATE_ITEM_MODEL_H
