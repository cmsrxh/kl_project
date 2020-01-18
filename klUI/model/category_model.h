#ifndef CATEGORY_ALL_MODEL_H
#define CATEGORY_ALL_MODEL_H

#include "kl_ui_data_union.h"
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
    // broadcast ctg index
    Q_PROPERTY(int bdcTabIndex READ bdcTabIndex NOTIFY bdcTabIndexChanged)

    // broadcast area index
    Q_PROPERTY(int bdcAreaIndex READ bdcAreaIndex NOTIFY bdcAreaIndexChanged)
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

    // property
    int bdcTabIndex() const;

    int bdcAreaIndex() const;

public Q_SLOTS:
    void onLoadOver(long ptr);

    // album 标签ID click
    void qmlCateTabClick(int index);

    // album ctg index
    int  qmlGetTabIndex();

    // broadcast 标签ID click
    void qmlBDCCateTabClick(int index);

    // broadcast area 标签ID click
    void qmlBDCAreaTabClick(int index);

Q_SIGNALS:
    void dataLoadOver(long ptr);

    void loadError(int type, const QString &info);

    //
    void bdcTabIndexChanged(int bdcTabIndex);

    void bdcAreaIndexChanged(int bdcAreaIndex);
    /**
     * @brief loadStart
     * @details 用于消息弹框，表示下载第一次开始
     */
    void loadStart();
protected:
    QHash<int, QByteArray> roleNames() const;

private:
    CategoryUnion                *m_pCateUnion;
    QHash<int, QByteArray>        roles;
    VectorTable<MusicCateUnion *> mVec;
};

#endif // CATEGORY_ALL_MODEL_H
