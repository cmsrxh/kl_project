#ifndef CHIP_ITEM_UNION_H
#define CHIP_ITEM_UNION_H

#include "kl_ui_data_union.h"

// ChipAudioList
// ChipRadioList
class ChipItemModel;
class ChipItemUnion : public kl::UINotifyIface
{
public:
    enum {
        LOAD_OVER_ALBUM_NORMAL_SHOW,        // 正常加载数据，并显示出来，应用于专辑相关的界面
        LOAD_OVER_ALBUM_IN_PLAYVIEW,        // 正常加载的数据，显示在节目列表界面中，用于专辑当前正在播放的列表,发生在当前播放列表已经放完需要后台加载。
        LOAD_OVER_BDCPROGRAM_IN_PLAYVIEW,   // 显示广播节目，在节目列表界面上, 然后要自动播放最新的电台节目
    };
    // 资源类型 (0:专辑|3:电台:11:传统电台)
    ChipItemUnion(int type);
    virtual ~ChipItemUnion();

    /**
     * @brief loadChipList
     * @param id [in]
     * @param sorttype [in]
     * @param loadAction [in] 下载动作，默认是前台下载，表示需要更新数据
     */
    void loadChipList(const ByteString &id, bool sorttype = true, int loadAction = LOAD_OVER_ALBUM_NORMAL_SHOW);

    void dataPrepare();

    void errorInfo(int, const char *);

    void onLoadOver(ChipItemModel *parent);

    int itemCount();

    /**
     * @brief loadNextPage
     * @return 返回是否还有下一页
     */
    bool loadNextPage(int loadAction);

    /**
     * @brief getUnionInfoByIndex
     * @param info [out] 输出index对应的信息
     * @param index [in]
     * @return index 超过范围就返回false，就是out数据无效
     * @details 用于当前列表界面，与上一次的界面不一样时，还能够继续上一次chip列表播放
     *          用于自动播放下一曲的功能
     */
    bool getUnionInfoByIndex(MusicChipItemUnion &info, int index);

    int getChipType() const
    {
        return mChipType;
    }

private:
    int                  mLoadAction;
    int                  mChipType;
    UIChipItemList      *m_pChip;

    void genCatesByRadioItem(ListTable<kl::RadioItem> &nodes, VectorTable<MusicChipItemUnion *> &vec);
    void genCatesByAudioItem(ListTable<kl::AudioItem> &nodes, VectorTable<MusicChipItemUnion *> &vec);
    void genCatesByBDCProgramItem(ListTable<kl::BDCastProgramItem> &nodes, VectorTable<MusicChipItemUnion *> &vec);
};

#endif // CHIP_ITEM_UNION_H
