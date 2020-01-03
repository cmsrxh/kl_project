#ifndef KL_DATA_PROC_H
#define KL_DATA_PROC_H

#include "util/map_table.h"
#include "util/byte_string.h"
#include <QString>

class CategoryUnion;
class CateItemUnion;
class ChipItemUnion;
class CategoryModel;
class CateItemModel;
class ChipItemModel;
class DetailUnion;
class ChipPlayManage;
class KLDataProc
{
public:
    enum
    {
        MEDIA_TYPE_BROADCAST,
        MEDIA_TYPE_ALBUMINFO,
    };
    /**
     * @brief The SwitchPath struct
     * @details 当前标签点击定位
     */
    struct SwitchPath
    {
    public:
        SwitchPath()
            : mainTabIndex(0)
            , media_type(MEDIA_TYPE_ALBUMINFO)
            , cate_tab_index(0)
            , cate_tab_sub_index(-1)
            , cate_item_index(-1)
            , chip_item_index(-1)
            , current_play_list(nullptr)
            , bdc {-1, -1, -1}
        {}
        void setMainTabIndex(int index)
        {
            mainTabIndex = index;
        }
        void setCateTabIndex(int index)
        {
            cate_tab_index     = index;
            cate_tab_sub_index = -1;
            cate_item_index    = -1;
            chip_item_index    = -1;
        }
        void setCateTabSubIndex(int index)
        {
            // cate_tab_index;
            cate_tab_sub_index = index;
            cate_item_index    = -1;
            chip_item_index    = -1;
        }
        void setCateItemIndex(int index)
        {
            cate_item_index = index;
            chip_item_index = -1;
        }
        void setChipItemIndex(int index)
        {
            chip_item_index = index;
        }
        void setCurrentPlayList(ChipItemUnion *chip)
        {
            current_play_list = chip;
        }
        int mainTabIndex;
        int media_type;
        int cate_tab_index;
        int cate_tab_sub_index;
        int cate_item_index;
        int chip_item_index;
        ChipItemUnion *current_play_list;

        struct {
            int bdc_cate_tab_index;
            int bdc_area_index;
            int bdc_item_index;
        } bdc;
    };

    struct AlbunView
    {
        AlbunView()
            : chip_item(nullptr)
            , datail_item(nullptr)
        {}
        AlbunView(ChipItemUnion *item)
            : chip_item(item)
            , datail_item(nullptr)
        {}

        ChipItemUnion *chip_item;
        DetailUnion   *datail_item;
    };
    static KLDataProc *instance()
    {
        static KLDataProc i;
        return &i;
    }

    void initAlbum(CategoryModel *cate,
                   CateItemModel *cateItem,
                   ChipItemModel *chip,
                   ChipItemModel *player);

    void initBroadcast(CategoryModel *bdcTab,
                       CategoryModel *bdcArea,
                       CateItemModel *bdcItem);

    /**
     * @brief mainTabClick
     * @param index
     * @details 主页“精品、我的” 标签点击ID
     */
    void mainTabClick(int index);

    /**
     * @brief albumFirstClick
     * @param index
     * @details 主页上“推荐、音乐” 等标签栏的点击ID
     */
    void albumFirstClick(int index);

    /**
     * @brief albumSecondClick
     * @param index
     * @details 主页上“推荐、音乐” 等标签对应的列表某项点击
     */
    void albumSecondClick(int index);

    /**
     * @brief chipAudioThirdChick
     * @param index
     * @details  例如“推荐 --> 专辑项” 点击，获得的audio碎片列表ID点击
     */
    void chipAudioThirdChick(int index);

    /**
     * @brief chipPlayThirdClick
     * @param index
     * @details 当前主页上，正在播放的节目列表ID点击，有可能跟audio碎片相等
     */
    void chipPlayThirdClick(int index);

    void bdcFirstCateTabClick(int index);

    void bdcFirstAreaTabClick(int index);

    void bdcSecondItemClick(int index, bool isInArea);

    void bdcSecondItemCollectClick(int index, bool isCollect);

    void bdcProgramListAction();
    /**
     * @brief showPlayingInfo
     * @details 在主页信息显示栏，显示当前播放信息，只有在开始播放的时候才播放
     */
    void showPlayingInfo();

    /**
     * @brief enterAlbumView
     * @details 进入专辑信息界面
     */
    void enterAlbumView();

    /**
     * @brief getAlbumFirstIndex
     * @return 音乐tab列表上的标签
     */
    int  getAlbumFirstIndex();

    /**
     * @brief getChipAudioThirdIndex
     * @return 专辑audio列表需要获取的ID
     */
    int  getChipAudioThirdIndex();

    /**
     * @brief getPlayThirdIndex
     * @return 播放列表需要获取的id
     */
    int  getPlayThirdIndex();

    /**
     * @brief getAlbumSecondIndex
     * @return 返回当前album二级列表的index
     */
    int  getAlbumSecondIndex();

    /**
     * @brief getBDCSecondIndex
     * @return 返回当前电台的二级标签ID
     */
    int  getBDCSecondIndex();

    /**
     * @brief getBDCFirstTabIndex
     * @return 返回当前电台的一级标签ID
     */
    int  getBDCFirstTabIndex();

    /**
     * @brief getBDCFirstAreaIndex
     * @return 返回当前电台的一级地区列表标签ID
     */
    int  getBDCFirstAreaIndex();

    void playNext();
    void playPrev();

private:
    KLDataProc();
    void enterBroadcastView();

    ChipPlayManage   *m_pPlayManage;

    // album
    CategoryUnion    *m_pCateData;
    CategoryModel    *m_pCate;
    CateItemModel    *m_pCateItem;
    ChipItemModel    *m_pChipItem;

    // player
    ChipItemModel    *m_pChipItemPlay;

    // broadcast
    CategoryUnion    *m_pBDCTabData;
    CategoryUnion    *m_pBDCAreaData;
    CategoryModel    *m_pBDCTab;
    CategoryModel    *m_pBDCArea;
    CateItemModel    *m_pBDCItem;

    SwitchPath        mSwitch;
    SwitchPath        mPlayPath;

    MapTable<int, CateItemUnion*>   mCidMap;
    MapTable<int, CateItemUnion*>   mBDCMap;
    MapTable<ByteString, AlbunView> mChipMap;
};

#endif // KL_DATA_PROC_H
