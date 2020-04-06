#ifndef CATEGORY_PLAYLIST_H
#define CATEGORY_PLAYLIST_H

#include "model/ui_data_union.h"
#include <QObject>

namespace qqmusic
{
class Category;
class CategoryPlaylist;
}
class CategoryModel;
class CateItemModel;
class CateItemUnion;
class CategoryPlaylistProc : public QObject, public UINotifyIface
{
    Q_OBJECT

    //! 当前歌手总共有几页
    Q_PROPERTY(int pageTotals READ pageTotals  NOTIFY pageTotalsChanged)

    //! 当前正在处于那一页
    Q_PROPERTY(int currentPage READ currentPage  NOTIFY currentPageChanged)

    //! 专辑标签列表
    Q_PROPERTY(QStringList cateList READ cateList  NOTIFY cateListChanged)

    Q_PROPERTY(QList<QObject *> cateSubs READ cateSubs NOTIFY cateSubsChanged)

public:
    CategoryPlaylistProc();

    //! 属性函数， 根据当前歌手数量，计算出当前有几页，默认是80为一页
    int  pageTotals();

    //! 属性函数，获取当前正处于那一页
    int  currentPage();

    //! 设置当前加载的歌手的总数量
    void setPageTotals(int total);

    //! 设置当前页花生变化
    void setCurrentPage(int page);

    //! 获取标签组列表
    QStringList cateList() const;

    //! 获取标签子对象
    QList<QObject *> cateSubs() const;

    //! 播放列表标签数据，下载完成，只能以消息的形式发送出来
    void dataPrepare();

    //! 播放列表标签数据，下载出现错误了
    void errorInfo(int, const ByteString &);

    //! 标签下对应的播放数据model
    CateItemModel *playListModel() const;

public Q_SLOTS:
    /**
     * @brief qmlCatePageIndexOperate
     * @param index [in] 其中-2表示上一页， -1表示下一页，正数表示要切换的当前页码
     * @note 被QML中，playList PageFooter.qml调用
     */
    void qmlCatePageIndexOperate(int index);

    //! 获取标签和当前的播放列表
    void qmlObtainCatePlayList();

    /**
     * @brief qmlGroupCateIndex
     * @param groud_index [in] 组ID
     * @param index [in] 时间标签项被点击了
     * @note 表示播放标签组中的某项被点击了
     */
    void qmlGroupCateIndex(int groud_index, int index);

    //! 歌曲列表项点击
    void qmlPlayListIndexClick(int index);

    //! 主线程处理播放标签列表数据
    void onCateDataPrepare();

    //! 播放列表数据下载完成，用来接收Model的dataLoadOver消息
    void onPlayListLoadOver(long ptr);

Q_SIGNALS:
    //! 歌手列表总页数通知消息
    void pageTotalsChanged();
    //! 通知歌手界面，当前正处于那一页
    void currentPageChanged();

    //! 下载数据已经准备好了，通知到主线程
    void cateDataPrepare();

    //! 专辑标签主题花生变化
    void cateListChanged();

    //! 标签子对象发生变化
    void cateSubsChanged();
private:
    qqmusic::Category           *m_pCateLoad;
    qqmusic::CategoryPlaylist   *m_pPlayListLoad;
    CategoryModel               *m_pCateGroupEmptItem;  //!< 标签空列表
    CateItemModel               *m_pPlayListModel;      //!< 具体播放列表model数据整合.年
    int     mPageTotals;                //!< 表示当前标签下的歌手总数量
    int     mCurrentPage;               //!< 表示当前处于那一页
    int     mOldCategoryId;             //!< 记入点击时的标签ID
    QList<QObject*>              mCateGroupItems;   //!< 相应标签组对应的标签列表
    QStringList                  mCateList;         //!< 标签组列表
    CateItemUnion               *m_pProxyPlayList;  //!< 标签播放列表代理对象,用来代理“CategoryPlaylist”


};

#endif // CATEGORY_PLAYLIST_H
