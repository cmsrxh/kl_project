#ifndef CATEGORY_PLAYLIST_H
#define CATEGORY_PLAYLIST_H

#include <QObject>

namespace qqmusic
{
class Category;
class CategoryPlaylist;
}
class CategoryModel;
class CateItemModel;
class CateItemUnion;
class QQmlContext;
class PLCategoryModel;
class CPLCateItemModel;
class CategoryPlaylistProc : public QObject
{
    Q_OBJECT

    //! 当前歌手总共有几页
    Q_PROPERTY(int pageTotals READ pageTotals  NOTIFY pageTotalsChanged)

    //! 当前正在处于那一页
    Q_PROPERTY(int currentPage READ currentPage  NOTIFY currentPageChanged)

    //! 专辑标签列表
    Q_PROPERTY(QObject * cateList READ cateList  NOTIFY cateListChanged)

    //! 专辑标签下的播放列表对象model
    Q_PROPERTY(QObject * catePlayList READ catePlayList NOTIFY catePlayListChanged)
public:
    static CategoryPlaylistProc *instance()
    {
        static CategoryPlaylistProc *i = [] { return new CategoryPlaylistProc; } ();
        return i;
    }

    void init(QQmlContext *);

    //! 属性函数， 根据当前歌手数量，计算出当前有几页，默认是80为一页
    int  pageTotals();

    //! 属性函数，获取当前正处于那一页
    int  currentPage();

    //! 设置当前加载的歌手的总数量
    void setPageTotals(int total);

    //! 设置当前页花生变化
    void setCurrentPage(int page);

    //! 获取标签组列表
    QObject * cateList() const;

    //! 专辑标签下的播放列表对象model
    QObject * catePlayList() const;

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

    //! 播放列表数据更新完成,需要更改UI效果
    void onPlayListUpdate();

Q_SIGNALS:
    //! 歌手列表总页数通知消息
    void pageTotalsChanged();
    //! 通知歌手界面，当前正处于那一页
    void currentPageChanged();

    //! 下载数据已经准备好了，通知到主线程
    void cateDataPrepare();

    //! 专辑标签主题花生变化
    void cateListChanged();
    void catePlayListChanged();

private:
    CategoryPlaylistProc();

    PLCategoryModel     *mCateModel;
    CPLCateItemModel    *mPlayListModel;

    int     mPageTotals;                //!< 表示当前标签下的歌手总数量
    int     mCurrentPage;               //!< 表示当前处于那一页
    int     mOldCategoryId;             //!< 记入点击时的标签ID
};

#endif // CATEGORY_PLAYLIST_H
