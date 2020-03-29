#ifndef SINGER_LIST_PROC_H
#define SINGER_LIST_PROC_H

#include <QObject>

class CategoryModel;
class CateItemModel;
class SingerListProc : public QObject
{
    Q_OBJECT

    //! 当前歌手总共有几页
    Q_PROPERTY(int singerPages READ singerPages  NOTIFY singerPagesChanged);

    //! 当前正在处于那一页
    Q_PROPERTY(int currentPage READ currentPage  NOTIFY currentPageChanged);

public:
    SingerListProc();

    CategoryModel *areaHandler() const
    {
        return m_pSingerArea;
    }

    CategoryModel *indexHandler() const
    {
        return m_pSingerIndex;
    }

    CategoryModel *sexHandler() const
    {
        return m_pSingerSex;
    }

    CategoryModel *genreHandler() const
    {
        return m_pSingerGenre;
    }

    CateItemModel *singerListHandler() const
    {
        return m_pSingerList;
    }

    //! 获取歌手列表，根据当前参数
    void obtainSingerList();

    //! 属性函数， 根据当前歌手数量，计算出当前有几页，默认是80为一页
    int  singerPages();

    //! 属性函数，获取当前正处于那一页
    int  currentPage();

    //! 设置当前加载的歌手的总数量
    void setSingerTotal(int total);

    //! 设置当前页花生变化
    void setCurrentPage(int page);

public Q_SLOTS:
    /**
     * @brief singerListProc
     * @param type [in] 表示列表类型的点击
     *      1: 标签Index列表项的点击
     *      2: 标签Area列表项的点击
     *      3: 标签Sex列表项的点击
     *      4: 标签Genre列表项的点击
     *      5: 标签SignerList列表项的点击
     *      6: 标签PageTags列表项的点击
     * @param index
     * @note qqmusic 歌手列表项的点击
     *      ，qqmusic 歌手列表项的点击
     */
    void qmlSingerList(int type, int index);

Q_SIGNALS:
    //! 歌手列表总页数通知消息
    void singerPagesChanged();
    //! 通知歌手界面，当前正处于那一页
    void currentPageChanged();

private:
    //singerList Model Data
    CategoryModel    *m_pSingerIndex;
    CategoryModel    *m_pSingerArea;
    CategoryModel    *m_pSingerSex;
    CategoryModel    *m_pSingerGenre;
    CateItemModel    *m_pSingerList;
    //! 表示当前标签下的歌手总数量
    int mSingerTotals;
    //! 表示当前处于那一页
    int mCurrentPage;
};

#endif // SINGER_LIST_PROC_H
