#ifndef QQMUSIC_CATEGORY_PLAYLIST_H
#define QQMUSIC_CATEGORY_PLAYLIST_H

#include "qqmusic_list_object.h"

namespace  qqmusic
{
class CategoryPlaylist : public ListObject<CatePlayList>
{
public:
    CategoryPlaylist(int categoryId = 10000000);

    //! 生成请求字串
    NetUrl &genQueryUrl();

    //! 分析数据
    int genResult(NetBuffer *data);

    //! 打印当前页信息
    void profile();

    //! 获取当前总页数
    int  getTotalPages();

    //! 获取当前页码
    int getCurrentPage();

    /**
     * @brief loadPage
     * @param index [in] 下载当前指定的页码
     * @return 返回当前指定的页码是否合法
     */
    bool loadPage(int index);

    //! 下载下一页
    inline bool loadNextPage()
    {
        return loadPage(mCurrentPage + 1);
    }

    //! 下载上一页
    inline bool loadPrevPage()
    {
        return loadPage(mCurrentPage - 1);
    }
    //! 下载新标签数据
    bool loadByCateId(int categoryId);

    int getCategoryId() const
    {
        return mCategoryId;
    }
private:
    int mCategoryId;
    int mCurrentPage;

    int mTotals;

    ListTable<CatePlayList> mBackUp;
};
}

#endif // QQMUSIC_CATEGORY_PLAYLIST_H
