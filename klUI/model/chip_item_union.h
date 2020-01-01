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
        CHIP_ITEM_AUDIO = 1,
        CHIP_ITEM_RADIO,
        CHIP_ITEM_BDC_PROGRAM
    };
    // 资源类型 (0:专辑|3:电台:11:传统电台)
    ChipItemUnion(int type);

    /**
     * @brief loadChipList
     * @param id [in]
     * @param sorttype [in]
     * @param loadAction [in] 下载动作，默认是前台下载，表示需要更新数据
     */
    void loadChipList(const ByteString &id, bool sorttype = true, bool loadAction = true);

    void dataPrepare();

    void errorInfo(int, const char *);

    void onLoadOver(ChipItemModel *parent);

    int itemCount();

    /**
     * @brief loadNextPage
     * @return 返回是否还有下一页
     */
    bool loadNextPage(bool loadAction);

    /**
     * @brief getUnionInfoByIndex
     * @param info [out] 输出index对应的信息
     * @param index [in]
     * @return index 超过范围就返回false，就是out数据无效
     * @details 用于当前列表界面，与上一次的界面不一样时，还能够继续上一次chip列表播放
     *          用于自动播放下一曲的功能
     */
    bool getUnionInfoByIndex(MusicChipItemUnion &info, int index);
private:
    bool                 mLoadAction;
    int                  mChipType;
    UIChipItemList      *m_pChip;

    void genCatesByRadioItem(ListTable<kl::RadioItem> &nodes, VectorTable<MusicChipItemUnion *> &vec);
    void genCatesByAudioItem(ListTable<kl::AudioItem> &nodes, VectorTable<MusicChipItemUnion *> &vec);
    void genCatesByBDCProgramItem(ListTable<kl::BDCastProgramItem> &nodes, VectorTable<MusicChipItemUnion *> &vec);
};

#endif // CHIP_ITEM_UNION_H
