#ifndef SINGER_LIST_PROC_H
#define SINGER_LIST_PROC_H

#include "common/app_load_common.h"
#include <QObject>

namespace qqmusic
{
class SingerList;
}
class SingerTagInfo;
class SingerListModel;
class SingerListProc : public QObject, public UINotifyIface
{
    Q_OBJECT

    //! 当前歌手总共有几页
    Q_PROPERTY(int singerPages READ singerPages  NOTIFY singerPagesChanged)

    //! 当前正在处于那一页
    Q_PROPERTY(int currentPage READ currentPage  NOTIFY currentPageChanged)

    Q_PROPERTY(QObject * singerModel READ singerModel NOTIFY singerModelChanged)
    Q_PROPERTY(QObject * singerChildModel READ singerChildModel NOTIFY singerChildModelChanged)

    Q_PROPERTY(QObject * singerIndexModel READ singerIndexModel NOTIFY singerIndexModelChanged)
    Q_PROPERTY(QObject * singerAreaModel READ singerAreaModel NOTIFY singerAreaModelChanged)
    Q_PROPERTY(QObject * singerSexModel READ singerSexModel NOTIFY singerSexModelChanged)
    Q_PROPERTY(QObject * singerGenreModel READ singerGenreModel NOTIFY singerGenreModelChanged)
public:
    static SingerListProc *instance()
    {
        static SingerListProc *i = [] { return new SingerListProc; } ();
        return i;
    }

    //! 属性函数， 根据当前歌手数量，计算出当前有几页，默认是80为一页
    int  singerPages();

    //! 属性函数，获取当前正处于那一页
    int  currentPage();

    QObject *singerModel();
    QObject *singerChildModel();

    QObject *singerIndexModel();
    QObject *singerAreaModel();
    QObject *singerSexModel();
    QObject *singerGenreModel();

    //! 设置当前加载的歌手的总数量
    void setSingerTotal(int total);

    //! 设置当前页花生变化
    void setCurrentPage(int page);

    //! 下载的数据准备好，只能以消息的形式发送出来
    void dataPrepare();

    //! 下载出现错误了
    void errorInfo(int, const ByteString &);

public Q_SLOTS:
    /**
     * @brief singerListProc
     * @param type [in] 表示列表类型的点击
     *      1: 标签Index列表项的点击
     *      2: 标签Area列表项的点击
     *      3: 标签Sex列表项的点击
     *      4: 标签Genre列表项的点击
     *      5: 标签SingerList列表项的点击
     *      6: 标签SingerList兄弟列表的点击，此列表只是显示文字
     *      7: 标签PageTags列表项的点击
     * @param index
     * @note qqmusic 歌手列表项的点击
     *      ，qqmusic 歌手列表项的点击
     */
    void qmlSingerList(int type, int index);

    //! 获取歌手列表，根据当前参数
    void qmlObtainSingerList();

    //! 主线程处理数据
    void onSingerDataPrepare();

Q_SIGNALS:
    //! 歌手列表总页数通知消息
    void singerPagesChanged();
    //! 通知歌手界面，当前正处于那一页
    void currentPageChanged();

    void singerModelChanged();
    void singerChildModelChanged();
    void singerIndexModelChanged();
    void singerAreaModelChanged();
    void singerSexModelChanged();
    void singerGenreModelChanged();

    //! 下载数据已经准备好了，通知到主线程
    void singerDataPrepare();

private:
    SingerListProc();

    //singerList Model Data
    SingerTagInfo    *m_pSingerIndex;
    SingerTagInfo    *m_pSingerArea;
    SingerTagInfo    *m_pSingerSex;
    SingerTagInfo    *m_pSingerGenre;
    //! 歌手列表显示前面十项，有图片显示
    SingerListModel    *m_pSingerList;
    //! 歌手列表只是显示文字，因为一页有80项，都显示图片，太长
    SingerListModel    *m_pSingerChildList;

    //! 表示当前标签下的歌手总数量
    int mSingerTotals;
    //! 表示当前处于那一页
    int mCurrentPage;

    //! 数据下载实体
    qqmusic::SingerList *m_pSingerLoad;    
};

#endif // SINGER_LIST_PROC_H
