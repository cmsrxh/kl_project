#ifndef CHIP_ITEM_UNION_H
#define CHIP_ITEM_UNION_H

#include "ui_data_union.h"
#include "pop_tip_manage.h"

// ChipAudioList
// ChipRadioList
class ChipItemModel;
class ChipItemUnion : public UINotifyIface
{
public:
    // 下载动作
    enum {
        LOAD_OVER_ALBUM_NORMAL_SHOW = PopTipManage::LOAD_ALBUM_NORMAL_SHOW,        // 正常加载数据，并显示出来，应用于专辑相关的界面
        LOAD_OVER_ALBUM_IN_PLAYVIEW = PopTipManage::LOAD_ALBUM_IN_PLAYVIEW,        // 正常加载的数据，显示在节目列表界面中，用于专辑当前正在播放的列表,发生在当前播放列表已经放完需要后台加载。
        LOAD_OVER_BDCPROGRAM_IN_PLAYVIEW = PopTipManage::LOAD_BDCPROGRAM_IN_PLAYVIEW,   // 显示广播节目，在节目列表界面上, 然后要自动播放最新的电台节目
        LOAD_OVER_BACK_PLAY_OP = PopTipManage::LOAD_BACK_PLAY_OP,             // 直接播放专辑等二级列表项
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

    void errorInfo(int, const ByteString &);

    void onLoadOver(ChipItemModel *parent);

    bool isEmpty();

    int itemCount();

    /**
     * @brief loadNextPage
     * @return 返回是否还有下一页
     */
    bool loadNextPage(int loadAction);

    /**
     * @brief getUnionLatestInfo
     * @param info [out] 输出index对应的信息
     * @return index 超过范围就返回false，就是out数据无效
     * @details 用于当前列表界面，与上一次的界面不一样时，还能够继续上一次chip列表播放
     *          用于自动播放下一曲的功能
     */
    bool getUnionInfo(MusicChipItemUnion &info, int &index);

    bool getUnionIndex(ByteString const &id, int &index);

    void getUnionSlideBase(int &cur, int &dur, int index);
    /**
     * @brief getChipType
     * @return PLAY_CHIP_TYPE_...
     */
    int getChipType() const
    {
        return mChipType;
    }
    /**
     * @brief setChipHandler
     * @param ptr
     * @details 给下载记录列表使用
     */
    void setChipHandler(UIChipItemList *ptr)
    {
        m_pChip = ptr;
    }

private:
    int                  mLoadAction;
    int                  mChipType;
    UIChipItemList      *m_pChip;
};

#endif // CHIP_ITEM_UNION_H
