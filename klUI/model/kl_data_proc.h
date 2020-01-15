#ifndef KL_DATA_PROC_H
#define KL_DATA_PROC_H

#include "util/map_table.h"
#include "kl_ui_data_union.h"

enum
{
    CURREN_PLAY_SOURCE_ALBUM_AUDIO_LIST,        // 专辑碎片列表点击播放
    CURREN_PLAY_SOURCE_BDC_SECOND_LIST,         // 电台节目列表中的中二级列表点击播放
    CURREN_PLAY_SOURCE_COLLECT_LIST,            // 从收藏列表中开始播放
    CURREN_PLAY_SOURCE_DOWNLOAD_LIST,           // 从下载列表中开始播放
    CURREN_PLAY_SOURCE_HISTORY_RECORD_LIST,     // 从历史记录列表中开始播放
    CURREN_PLAY_SOURCE_CLIENT_SEARCH_LIST,      // 从客户端搜索列表中开始播放
};

class CategoryUnion;
class CateItemUnion;
class ChipItemUnion;
class CategoryModel;
class CateItemModel;
class ChipItemModel;
class DetailUnion;
class ChipPlayManage;
class CollectModel;
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
            , current_play_source(-1)
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
        int current_play_source;
        ChipItemUnion *current_play_list;

        struct {
            int bdc_cate_tab_index;
            int bdc_area_index;
            int bdc_item_index;
        } bdc;
        struct {
            int table_type;
            int table_list_main_index;
        } local;
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

    ~KLDataProc();

    void initSockService();

    void initMedia();

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
     * @details  例如“推荐 --> 专辑项”，获得的audio碎片列表ID点击
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

    /**
     * @brief currentIsCollect
     * @brief notifyCurIsCollect
     * @brief isCollect
     * @details 处理界面当前播放项是否已经被收藏了
     */
    void currentIsCollect();
    void notifyCurIsCollect(bool isCollect);
    void notifyBDCCollectChange(int index, bool isCollect);
    bool isCollect() const
    {
        return mCurrentIsCollect;
    }

    CollectNode &getCurrentPlayInfo()
    {
        return mPlayInfo;
    }

    /**
     * @brief localItemTypeRadioPlay
     * @brief localItemBroadcastPlay
     * @brief localItemAlbumPlay
     * @param parentId [in] 输入二级标签ID, 如专辑ID
     * @param id [in] 输入三级标签ID, 如专辑碎片ID
     * @details 根据本地信息（收藏 历史记录 或者下载记录），进行播放处理
     */
    void localItemTypeRadioPlay(int local_type, int main_index, ByteString const &parentId, ByteString const &id);
    void localItemBroadcastPlay(int local_type, int main_index, ByteString const &parentId, ByteString const &id);
    void localItemAlbumPlay(int local_type, int main_index, ByteString const &parentId, ByteString const &id);
    void localItemAlbumAudioPlay(int local_type, int main_index, ByteString const &id);
    void localItemDownLoadPlay(int local_type, int main_index);

    /**
     * @brief playDefaultItem
     * @details 加载列表完成播放默认的曲目
     */
    void playDefaultItem();
private:
    KLDataProc();
    void enterBroadcastView();

    bool              mCurrentIsCollect;
    ChipPlayManage   *m_pPlayManage;
    //download
    ChipItemUnion    *m_pHistoryChip;

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
    CollectNode       mPlayInfo;

    MapTable<int, CateItemUnion*>   mCidMap;
    MapTable<int, CateItemUnion*>   mBDCMap;
    MapTable<ByteString, AlbunView> mChipMap;

    void setPlayInfo(MusicChipItemUnion &chip);

};

#endif // KL_DATA_PROC_H
