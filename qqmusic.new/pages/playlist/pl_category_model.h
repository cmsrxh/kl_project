#ifndef PL_CATEGORY_MODEL_H
#define PL_CATEGORY_MODEL_H

#include <common/load_model_base.h>

namespace qqmusic {
class Category;
}
class PLCategoryModel : public LoadModelBase
{
    Q_OBJECT
public:    
    PLCategoryModel();
    ~PLCategoryModel();

    //! 用于下载初始数据
    void obtain();

    int rowCount(const QModelIndex &parent = QModelIndex()) const;

    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;

    // qml call
    //! @warning qml可能传递负数，造成数组越界
    //! 获取标签子对象
    QObject * getSubCate(int i);

    //! UI点击获取子列表的cateId数据
    bool getCateId(int groud_index, int index, int &cateId);

    void updateIndex(int cateId, int oldCateId);

public Q_SLOTS:
    void onLoadOver();
    void onLoadFailed(int type, QString const &errStr);

private:
    qqmusic::Category  *mCateLoad;

    //! 专辑标签列表
    QStringList         mCateList;

    //! 相应标签组对应的标签列表
    QVector<QObject *>  mCateGroupItems;
};

class PLCategorySubItemModel : public QAbstractListModel
{
    Q_OBJECT

    Q_PROPERTY(int curIndex READ curIndex WRITE setCurIndex NOTIFY curIndexChanged)
public:
    struct ItemNode
    {
        int         id;
        ByteString  name;
    };
    PLCategorySubItemModel();
    ~PLCategorySubItemModel();

    void pushNode(const ItemNode &n);

    int  curIndex() const;
    void setCurIndex(int i);

    bool getCateId(int index, int &cateId);

    QHash<int, QByteArray> roleNames() const
    {
        return roles;
    }
    int rowCount(const QModelIndex &parent = QModelIndex()) const;

    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;

Q_SIGNALS:
    void curIndexChanged();

private:
    QHash<int, QByteArray>  roles;

    int                     mCurIndex;
    QVector<ItemNode>       subCate;
};

#endif // PL_CATEGORY_MODEL_H
