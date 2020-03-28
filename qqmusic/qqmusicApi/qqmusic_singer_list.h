#ifndef QQMUSIC_SINGER_LIST_H
#define QQMUSIC_SINGER_LIST_H

#include "qqmusic_list_object.h"

namespace  qqmusic
{

class SingerTags
{
public:
    enum _TAG {
        AREA_TAG  = 0,
        GENRE_TAG = 1,
        INDEX_TAG = 2,
        SEX_TAG   = 3,
    };
    SingerTags()
        : tags{  {"area",  0, ListTable<TagNode>()}
               , {"genre", 0, ListTable<TagNode>()}
               , {"index", 0, ListTable<TagNode>()}
               , {"sex",   0, ListTable<TagNode>()}}
    {}

    struct
    {
        char name[8];
        int  curIndex;
        ListTable<TagNode> tag;
    } tags[4];
};

class SingerList : public ListObject<SingerNode>
{
public:
    /**
     * @brief SingerList
     * @param areaId [in] 地区
     * @param sex [in] 性别
     * @param genre [in] 类型，eg: 摇滚、电子
     * @param index [in] 字母标签 eg: A、B、
     * @param cur_page [in] 页码, 默认是第一页 =1
     * @default 默认加载全部歌手列表
     */
    SingerList(int areaId = -100, int sex = -100, int genre = -100, int index = -100);

    //! 生成请求字串
    NetUrl &genQueryUrl();

    //! 分析数据
    int genResult(NetBuffer *data);

    //! 打印当前页信息
    void profile();

    /**
     * @brief loadNextPage
     * @param page_index [in] 页码，-1表示下载下一页， -2表示下载上一页，正数表示 具体的某页
     * @note  下载某一页
     */
    bool loadNextPage(int page_index = -1);

    //! 获取当前总页数
    int  getTotalPages();

    //! 确定当前数据tag的标签位置
    void ensureTagsIndex();

    int  areaIndex() const
    {
        return mTags.tags[SingerTags::AREA_TAG].curIndex;
    }

    int  genreIndex() const
    {
        return mTags.tags[SingerTags::GENRE_TAG].curIndex;
    }

    int  indexIndex() const
    {
        return mTags.tags[SingerTags::INDEX_TAG].curIndex;
    }

    int  sexIndex() const
    {
        return mTags.tags[SingerTags::SEX_TAG].curIndex;
    }

private:
    //! 通过操作当前这五个参数，控制当前歌手列表
    //! 同时在加载完成数据时，也会返回当前数据的定位(当前四个参数)
    int mAreaId;
    int mSex;
    int mGenre;
    int mIndex;
    int mSin;  // 当前页的开始位置，默认页有80项数据，eg：第二页sin就 以80开始，等等.
    int mCurPage;

    char mDataBuffer[1024];

    int mTotals;

    static SingerTags mTags;
};
}

#endif // QQMUSIC_SINGER_LIST_H
