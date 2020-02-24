#ifndef KL_DATA_PROC_H
#define KL_DATA_PROC_H

#include "util/map_table.h"
#include "kl_ui_data_union.h"

class ATimer;
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
            , chip_item_sub_index(-1)
            , current_play_source(-1)
            , bdc {-1, -1, -1}
            , local{-1, 0, -1}
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

        int mainTabIndex;
        int media_type;
        int cate_tab_index;
        int cate_tab_sub_index;
        int cate_item_index;
        int chip_item_index;
        int chip_item_sub_index;
        int current_play_source;

        struct {
            int bdc_cate_tab_index;
            int bdc_area_index;
            int bdc_item_index;
        } bdc;
        struct {
            int table_type;
            int self_tab_index;
            int table_list_item_index;
        } local;
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
     * @brief detailLoadAlbumInfo
     * @details 界面启动后，专辑标签数据加载完毕后，需要加载默认一页
     */
    void detailLoadAlbumInfo();

    /**
     * @brief getCurrentShowView
     * @return 返回当前处于哪一个界面，然后才把当前播放变化的信息通知到这个界面上
     */
    int getCurrentShowView();

    /**
     * @brief mainTabClick
     * @param index
     * @details 主页“精品、我的” 标签点击ID
     */
    void mainTabClick(int index);

    /**
     * @brief selfTabClick
     * @param index
     * @details “我的”标签下面的各种设置标签点击，标识切换到对应的界面
     */
    void selfTabClick(int index);
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
    void chipPlayThirdClick(int index, bool isKeyPress = false);

    void bdcFirstCateTabClick(int index);

    void bdcFirstAreaTabClick(int index);

    void bdcSecondItemClick(int index, bool isInArea);

    void bdcSecondItemCollectClick(int index, bool isCollect);

    /**
     * @brief bdcProgramListAction
     * @details 其中包含智能电台和传统电台两种类型，
     * 智能电台需要从开始处播放，传统电台默认播放当前时间。
     */
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
     * @note not used
     */
    int  getChipAudioThirdIndex();

    /**
     * @brief getPlayThirdIndex
     * @return 播放列表需要获取的id
     */
    int  getPlayThirdIndex(ChipItemModel *model);

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

    void playNext(bool isKeyPress = false);
    void playPrev();

    /**
     * @brief currentIsCollect
     * @brief notifyCurIsCollect
     * @brief isCollect
     * @details 处理界面当前播放项是否已经被收藏了
     */
    void currentIsCollect();
    void notifyCurIsCollect(bool isCollect);
    bool isCollect() const
    {
        return mCurrentIsCollect;
    }
    /**
     * @brief notifyBDCCollectChange
     * @param index [in] 标签ID
     * @param isCollect [in] 当前是否收藏（没有用到）
     * @details 电台界面收藏处理
     */
    void notifyBDCCollectChange(int index, bool isCollect);

    /**
     * @brief getCurrentPlayInfo
     * @param parentId
     * @param id
     * @return
     * @details 获取当前播放的父子ID
     */
    bool getCurrentPlayInfo(ByteString &parentId, ByteString &id);

    /**
     * @brief localItemPlay
     * @param local_type
     * @param main_index
     * @details 根据本地信息（收藏 历史记录 或者下载记录），进行播放处理
     */
    void localItemPlay(int type, int index, ChipItemUnion *pUnion) ;

    /**
     * @brief playDefaultItem
     * @details 加载列表完成播放默认的曲目
     */
    void playDefaultItem(ChipItemUnion *pUnion);

    /**
     * @brief audioDetailLoadOver
     * @param detail
     * @details 根据专辑audioID号，获取其详情，然后播放，并下载对应的专辑列表
     */
    void audioDetailLoadOver(MusicDetail &detail);

    /**
     * @brief klObjectObtainState
     * @param state [in] 调用下载函数的返回状态
     * @param objectName [in] 对象名字
     * @details 基于KlObject派生类开始调用obtain函数，获取kl数据
     * @note 在App事件循环子线程中执行
     */
    void klObjectObtainState(bool state, int objectName);

    /**
     * @brief klObjectObtainOver
     * @param objectName
     * @details 基于KlObject派生类, 数据下载完成
     */
    void klObjectObtainOver(int objectName);

    /**
     * @brief klLoadDataExportEmpty
     * @details  kl下载的数据，是空列表
     * @note 在App事件循环子线程中执行
     */
    void klLoadDataExportEmpty(int objectName);

    /**
     * @brief klLoadDataPriserExcept
     * @param str
     * @details  kl下载的数据，json格式数据解析失败了（大多都是传递 了错误的参数，或者服务器那边出现问题）
     * @note 在App事件循环子线程中执行
     */
    void klLoadDataPriserExcept(int objectName, const ByteString &str);

    /**
     * @brief sysNetLoadApiExcept
     * @param type
     * @param str
     * @details 调用libcurl下载kl数据失败，一般时本地网络出现问题
     *          例如：下载超时、网络断开等等原因。
     * @note 在App事件循环子线程中执行
     */
    void sysNetLoadApiExcept(int objectName, int type, const char *str);

    /**
     * @brief reloadErrObject
     * @details 重新加载错误的对象
     */
    void reloadErrObject();

private:
    KLDataProc();
    void enterBroadcastView();
    void playSubItem(MusicChipItemUnion *chip);
    bool playCurSubItemSubNext(MusicChipItemUnion *);
    static void msgTipTimer(ATimer *that, void *ptr);
    void showDelayMsgBox();

    bool              mCurrentIsCollect;
    ChipPlayManage   *m_pPlayManage;

    // album
    CategoryUnion    *m_pCateData;
    CategoryModel    *m_pCate;
    CateItemModel    *m_pCateItem;
    ChipItemModel    *m_pChipItem;

    // player
    ChipItemModel    *m_pChipItemPlay;
    ChipItemUnion    *m_pCurPlayUnion;

    // broadcast
    CategoryUnion    *m_pBDCTabData;
    CategoryUnion    *m_pBDCAreaData;
    CategoryModel    *m_pBDCTab;
    CategoryModel    *m_pBDCArea;
    CateItemModel    *m_pBDCItem;

    ATimer           *m_pMsgPopDelayTimer;
    int               mCurrenObjectName;

    SwitchPath        mSwitch;
    SwitchPath        mPlayPath;
    CollectNode       mPlayInfo;

    MapTable<int, CateItemUnion*>         mCidMap;
    MapTable<int, CateItemUnion*>         mBDCMap;
    MapTable<ByteString, ChipItemUnion *> mChipMap;

    void setPlayInfo(MusicChipItemUnion &chip);

};

#endif // KL_DATA_PROC_H
