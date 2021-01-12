#ifndef CPL_CATE_ITEM_MODEL_H
#define CPL_CATE_ITEM_MODEL_H

#include <common/load_model_base.h>

namespace qqmusic {
class CategoryPlaylist;
}

class CPLCateItemModel : public LoadModelBase
{
    Q_OBJECT
public:
    struct ItemNode
    {
        ByteString id;
        ByteString name;
        ByteString img;
    };
    CPLCateItemModel();
    ~CPLCateItemModel();

    //! 用于下载初始数据
    void obtain();

    //! 根据标签下载
    void loadByCateId(int id);

    //! 页操作下载
    void loadPrevPage();
    void loadNextPage();
    void loadPage(int index);

    //! 获取标签页定位
    int getTotalPages();
    int getCurrentPage();
    int getCurCateId();

    int rowCount(const QModelIndex &parent = QModelIndex()) const;

    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;

Q_SIGNALS:
    void sigPlayListUpdate();

public Q_SLOTS:
    void onLoadOver();
    void onLoadFailed(int type, QString const &errStr);

private:
    qqmusic::CategoryPlaylist  *mCPL;
    QVector<ItemNode>           mList;
};

#endif // CPL_CATE_ITEM_MODEL_H
