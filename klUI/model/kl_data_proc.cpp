#include <events/common_log.h>
#include "cate_item_union.h"
#include "cate_item_model.h"
#include "category_union.h"
#include "category_model.h"
#include "chip_item_model.h"
#include "chip_item_union.h"
#include "detail_qobject.h"
#include "detail_union.h"
#include "kl_data_proc.h"
#include "chip_item_play_manage.h"
#include "kl_ui_proc.h"
#include "events/app_timer.h"
#include "kl_local_data_proc.h"
#include "kl_download_manage.h"
#include "kl_collect_manage.h"
#include "kl_record_manage.h"
#include "kl_search_manage.h"
#include "iface/media_service_i_face.h"
#include "iface/media_iface_common.h"
#include "iface/media_service_call_back.h"
#include "application.h"
#include "../klIface/kl_service_notify.h"
#include "../klIface/kl_service_priser.h"

extern KLUIProc *gInstance;

class MpvMsgPriser : public MediaServiceCallback
{
public:
    void mediaNotify(int msg, int ext1, int ext2, const char *str)
    {
        //GEN_Printf(LOG_DUMP, "msg: %d (%d, %d), %s", msg, ext1, ext2, str);
        Q_EMIT gInstance->recvNotify(msg, ext1, ext2, QString::fromUtf8(str));
    }
};

KLDataProc::KLDataProc()
    : mCurrentIsCollect(false)
    , m_pPlayManage(new ChipPlayManage)  
{    
    m_pMsgPopDelayTimer = new ATimer;
    m_pMsgPopDelayTimer->setHandler(KLDataProc::msgTipTimer, this);
    m_pMsgPopDelayTimer->setRepeat(false);
    m_pMsgPopDelayTimer->setInternal(0, 800);

    m_pCurPlayUnion = nullptr;
}

void KLDataProc::enterBroadcastView()
{
    if (mSwitch.media_type != MEDIA_TYPE_BROADCAST)
    {
        mSwitch.media_type = MEDIA_TYPE_BROADCAST;

        gInstance->viewAlbumBDCSwitch("bdc/KlInlineBroadcast.qml");
    }

    if (-1 == mSwitch.bdc.bdc_cate_tab_index)
    {
        bdcFirstCateTabClick(0);
    }
}

void KLDataProc::playSubItem(MusicChipItemUnion *chip)
{
    switch (chip->sub_type)
    {
    case PLAY_CHIP_TYPE_ALBUM        :    // 专辑二级标签item
        // 用于收藏,历史记录和搜索列表中的播放
        // 直接播放专辑，但是专辑不是直接播放的实体，子集下面是节目列表需要下载然后才能播
    case PLAY_CHIP_TYPE_BROADCAST    :    // 电台二级标签item
        // 用于收藏,历史记录和搜索列表中的播放
        // 直接播放电台，但是电台不是直接播放的实体，子集下面是节目列表需要下载然后才能播
    case PLAY_CHIP_TYPE_TYPE_RADIO   :    // 智能电台二级标签item
        // 用于收藏,历史记录和搜索列表中的播放
        // 直接播放智能电台，但是智能电台不是直接播放的实体，子集下面是节目列表需要下载然后才能播
    {
        ByteString id = ByteString::allocString(chip->parentId);
        ChipItemUnion *&chip_item = mChipMap[id];
        if (chip_item)
        {
            id.clear();
            playDefaultItem(chip_item);
        } else
        {
            chip_item = new ChipItemUnion(chip->sub_type);
            m_pChipItemPlay->setChipItemUnion(chip_item);
            chip_item->loadChipList(id, true, ChipItemUnion::LOAD_OVER_BACK_PLAY_OP);
        }
        break;
    }
    case PLAY_CHIP_TYPE_AUDIO_CHIP   :    // 专辑音乐节目碎片三级标签item
    {
        DetailQobject::instance()->loadDetail(PLAY_CHIP_TYPE_AUDIO_CHIP, chip->chipId,
                                              DetailUnion::LOAD_DETAIL_AUDIO_PLAYING);
        break;
    }
    default:
        GEN_Printf(LOG_ERROR, "sub_type: %d out of range", chip->sub_type);
        assert(0);
        break;
    }
}

bool KLDataProc::playCurSubItemSubNext(MusicChipItemUnion *)
{
    MusicChipItemUnion info;
    if (m_pCurPlayUnion
            && m_pCurPlayUnion->getUnionInfo(info, mPlayPath.chip_item_sub_index))
    {
        gInstance->setSourceUrl(info.playUrl.string());
        return true;
    }
    return false;
}

void KLDataProc::setPlayInfo(MusicChipItemUnion &chip)
{
    mPlayInfo.isLocal   = 0;
    switch (chip.type)
    {
    case PLAY_CHIP_TYPE_COLLECT_RECORD:   // 加载收藏
    case PLAY_CHIP_TYPE_HISTROY_RECORD:   // 加载历史记录
    case PLAY_CHIP_TYPE_SEARCH_LOAD:      // 加载搜索列表
         mPlayInfo.type      = chip.sub_type;
        break;
    default:
         mPlayInfo.type      = chip.type;
        break;
    }
    mPlayInfo.id        = chip.chipId;
    mPlayInfo.parentId  = chip.parentId;
    mPlayInfo.name      = chip.name;
    mPlayInfo.parentName= chip.parentName;
    mPlayInfo.image     = chip.image;
    mPlayInfo.playUrl   = chip.playUrl;

    LocalDataProc::instance()->checkCurIsCollect(&mPlayInfo);
}

KLDataProc::~KLDataProc()
{
    MapTable<int, CateItemUnion*>::iterator it = mCidMap.begin();
    for (; it != mCidMap.begin(); ++it)
    {
        delete it->value;
    }

    MapTable<int, CateItemUnion*>::iterator it2 = mBDCMap.begin();
    for (; it2 != mBDCMap.begin(); ++it2)
    {
        delete it2->value;
    }

    MapTable<ByteString, ChipItemUnion *>::iterator it3 = mChipMap.begin();
    for (; it3 != mChipMap.begin(); ++it3)
    {
        it3->key.clear();

        if (it3->value)
        {
            delete it3->value;
        }
    }

    delete m_pCateData;
    delete m_pBDCTabData;
    delete m_pBDCAreaData;
    delete m_pPlayManage;
}

void KLDataProc::initSockService()
{
    static KLServicePriser i;
    KLServiceNotify::instance()->initPriser(&i);
}

void KLDataProc::initMedia()
{
    static MpvMsgPriser i;
    MediaServiceIFace::instance()->initClientIface(&i);
}

void KLDataProc::initAlbum(CategoryModel *cate, CateItemModel *cateItem, ChipItemModel *chip, ChipItemModel *player)
{
    m_pCate         = cate;
    m_pCateItem     = cateItem;
    m_pChipItem     = chip;
    m_pChipItemPlay = player;

    m_pCateData       = new CategoryUnion(CategoryUnion::MAIN_CATE, cate);

    m_pPlayManage->setChipShow(chip);
    m_pPlayManage->setPlayModel(player);

    m_pCate->setCateUnion(m_pCateData);
    m_pCateData->loadCategory(CategoryUnion::MAIN_CATE, 0);
}

void KLDataProc::initBroadcast(CategoryModel *bdcTab, CategoryModel *bdcArea, CateItemModel *bdcItem)
{
    m_pBDCTab   = bdcTab;
    m_pBDCArea  = bdcArea;
    m_pBDCItem  = bdcItem;

    m_pBDCTabData  = new CategoryUnion(CategoryUnion::BDC_CATE, bdcTab);
    m_pBDCAreaData = new CategoryUnion(CategoryUnion::BDC_AREA_CATE, bdcArea);

    m_pBDCTab->setCateUnion(m_pBDCTabData);
    m_pBDCArea->setCateUnion(m_pBDCAreaData);

    m_pBDCTabData->loadCategory(CategoryUnion::BDC_CATE, 0);
    m_pBDCAreaData->loadCategory(CategoryUnion::BDC_AREA_CATE, 0);
}

void KLDataProc::detailLoadAlbumInfo()
{
   albumFirstClick(getAlbumFirstIndex());
}

// 当前的界面汇总分配ID
enum {
    CURRENT_VIEW_IN_ALBUM_AUDIOLIST_AND_INFO,   // CategoryView.qml --> CateItemInfoView.qml
    CURRENT_VIEW_IN_ALBUM_INFO_LIST,            // CategoryView.qml --> CateItemListView.qml
    CURRENT_VIEW_IN_BROADCAST,                  // CategoryView.qml --> bdc/KlInlineBroadcast.qml
    CURRENT_VIEW_IN_COLLECT,            // self/KlDlgOptionView.qml --> KlCollectView.qml
    CURRENT_VIEW_IN_DOWNLOAD,           // self/KlDlgOptionView.qml --> KlLoadView.qml
    CURRENT_VIEW_IN_HISTROY,            // self/KlDlgOptionView.qml --> KlHistoryRecord.qml
    CURRENT_VIEW_IN_VOICE_GUIDANCE,     // self/KlDlgOptionView.qml --> KlVioceGuide.qml
    CURRENT_VIEW_IN_SETTING,            // self/KlDlgOptionView.qml --> KlSettingView.qml
};

int KLDataProc::getCurrentShowView()
{
    if (0 == mSwitch.mainTabIndex) //表示在精品界面
    {
        if (MEDIA_TYPE_ALBUMINFO == mSwitch.media_type)
        {
            return (gInstance->isAudioView()) ? CURRENT_VIEW_IN_ALBUM_AUDIOLIST_AND_INFO
                                              : CURRENT_VIEW_IN_ALBUM_INFO_LIST;
        } else if (MEDIA_TYPE_BROADCAST == mSwitch.media_type)
        {
            return CURRENT_VIEW_IN_BROADCAST;
        } else
        {
            GEN_Printf(LOG_ERROR, "is Not invalid view tab");
            assert(0);
        }
    } else if (1 == mSwitch.mainTabIndex) //表示在我的界面
    {
        switch (mSwitch.local.self_tab_index)
        {
        case 0: return CURRENT_VIEW_IN_COLLECT;
        case 1: return CURRENT_VIEW_IN_DOWNLOAD;
        case 2: return CURRENT_VIEW_IN_HISTROY;
        case 3: return CURRENT_VIEW_IN_VOICE_GUIDANCE;
        case 4: return CURRENT_VIEW_IN_SETTING;
        default:
            GEN_Printf(LOG_ERROR, "is Not invalid view tab");
            assert(0);
        }
    } else
    {
        GEN_Printf(LOG_ERROR, "is Not invalid view tab");
        assert(0);
    }
    return -1;
}

void KLDataProc::mainTabClick(int index)
{
    mSwitch.setMainTabIndex(index);
}

void KLDataProc::selfTabClick(int index)
{
    mSwitch.local.self_tab_index = index;
}

void KLDataProc::albumFirstClick(int index)
{
    ByteString cid = m_pCate->getCID(index);
    if (cid.empty())
    {
        GEN_Printf(LOG_WARN, "index: %d is invalid", index);
    } else
    {
        int cid_num = atoi(cid.string());
        int cid_type= CateItemUnion::CATE_ITEM_ALBUM;

        if (-1 == cid_num)
        {
            cid_type = CateItemUnion::CATE_ITEM_OPERATE;
        } else if (-2 == cid_num)
        {
            // enter broadcast view
            enterBroadcastView();
            return;
        }

        GEN_Printf(LOG_DEBUG, "cid = %s, cid num = %s", cid.string(), cid.string());

        m_pCateItem->clear();

        CateItemUnion *& tmp = mCidMap[cid_num];
        if (tmp)
        {            
            m_pCateItem->setCateItemUnion(tmp);
            if (tmp->isEmpty())
            {
                tmp->loadCateItem();
            } else
            {
                tmp->onLoadOver(m_pCateItem);
                msgBoxLocalLoadOver(kl::OBJECT_ALBUM_LIST);
            }
            m_pCateItem->resetAll();
        } else
        {
            tmp = new CateItemUnion(cid_type, m_pCateItem);

            m_pCateItem->setCateItemUnion(tmp);
            tmp->loadCateItem(cid_num);
        }

        if (mSwitch.media_type != MEDIA_TYPE_ALBUMINFO)
        {
            mSwitch.media_type = MEDIA_TYPE_ALBUMINFO;
            gInstance->viewAlbumBDCSwitch("qrc:/CateItemListView.qml");
        }

        mSwitch.setCateTabIndex(index);
    }
}

void KLDataProc::albumSecondClick(int index)
{
    VectorTable<MusicCateItemUnion *> &vec = m_pCateItem->vec();

    if (index < 0 || index >= vec.size())
    {
        GEN_Printf(LOG_ERROR, "Index=%d Out of range=%d", index, vec.size());
        return;
    }

    ByteString id = ByteString::allocString(vec[index]->id);
    int type = vec[index]->type;

    GEN_Printf(LOG_DEBUG, "Load Type: %d, id=%s", type, id.string());
    mSwitch.setCateItemIndex(index);
    m_pChipItem->clear();

    DetailQobject::instance()->setDetailName(vec[index]->name);

    ChipItemUnion *&chip_item = mChipMap[id];

    if (chip_item)
    {
        m_pChipItem->setChipItemUnion(chip_item);
        if (chip_item->isEmpty())
        {
            chip_item->loadChipList(ByteString());
        } else
        {
            chip_item->onLoadOver(m_pChipItem);
            msgBoxLocalLoadOver(kl::OBJECT_CHIP_AUDIO_LIST);
        }
        m_pChipItem->resetAll();
    } else
    {
        chip_item = new ChipItemUnion(type);
        m_pChipItem->setChipItemUnion(chip_item);
        chip_item->loadChipList(id);
    }
    // 现在改成提前显示这个界面
    gInstance->pushNew("qrc:/CateItemInfoView.qml");
    DetailQobject::instance()->loadDetail(type, id, DetailUnion::LOAD_DETAIL_SHOW_IN_ALBUM_VIEW);
}

void KLDataProc::chipAudioThirdChick(int index)
{
    bool clickValid = true;
    if (!m_pChipItemPlay->equal(m_pChipItem))
    {
        m_pChipItemPlay->assign(m_pChipItem);
        m_pChipItemPlay->resetAll();
        mPlayPath = mSwitch;
        mPlayPath.current_play_source = CURREN_PLAY_SOURCE_ALBUM_AUDIO_LIST;
    } else
    {
        // 在同样的播放列表中点击, 表明点击的播放列表与当前播放列表是相等的
        clickValid = (mPlayPath.chip_item_index == index) ? false : true;
    }
    if (clickValid)
    {
        VectorTable<MusicChipItemUnion *> &vec = m_pChipItem->vec();

        if (index < 0 || index >= vec.size())
        {
            GEN_Printf(LOG_ERROR, "Index=%d Out of range=%d", index, vec.size());
            return;
        }
        mSwitch.setChipItemIndex(index);
        mPlayPath.setChipItemIndex(index);

        // start play music
        gInstance->setSourceUrl(vec[index]->playUrl.string());
    } else
    {
        GEN_Printf(LOG_DEBUG, "index: %d was setted.", index);
    }
}

void KLDataProc::chipPlayThirdClick(int index, bool isKeyPress)
{
    VectorTable<MusicChipItemUnion *> &vec = m_pChipItemPlay->vec();

    if (index < 0 || index >= vec.size())
    {
        GEN_Printf(LOG_ERROR, "Index=%d Out of range=%d", index, vec.size());
        return;
    }

    if (mPlayPath.chip_item_index != index)
    {
        GEN_Printf(LOG_INFO, "play list click, play type=%d", vec[index]->type);
        switch(vec[index]->type)
        {
        case PLAY_CHIP_TYPE_AUDIO_CHIP   :    // 专辑音乐节目碎片三级标签item
        case PLAY_CHIP_TYPE_RADIO_CHIP   :    // 智能电台节目碎片三级标签item
        case PLAY_CHIP_TYPE_BDC_PROGRAM_CHIP: // 电台节目碎片三级标签item
        case PLAY_CHIP_TYPE_LOCAL_LOAD   :    // 下载的音频碎片（专辑音乐碎片）
            // start play music
            gInstance->setSourceUrl(vec[index]->playUrl.string());
            break;
        case PLAY_CHIP_TYPE_COLLECT_RECORD:   // 加载收藏
        case PLAY_CHIP_TYPE_HISTROY_RECORD:   // 加载历史记录
        case PLAY_CHIP_TYPE_SEARCH_LOAD:      // 加载搜索列表
        {
            int curIndex = mPlayPath.chip_item_index;
            if (isKeyPress || curIndex < 0 || curIndex >= vec.size())
            {
                playSubItem(vec[index]);
            } else
            {
                if (!playCurSubItemSubNext(vec[curIndex]))
                {
                    playSubItem(vec[index]);
                }
            }
            break;
        }
        default:
            GEN_Printf(LOG_ERROR, "play type: %d out of range", vec[index]->type);
            assert(0);
            break;
        }

        mPlayPath.setChipItemIndex(index);
    } else
    {
        GEN_Printf(LOG_DEBUG, "index: %d was setted.", index);
    }
}

#define AREA_TAB_ID 2
void KLDataProc::bdcFirstCateTabClick(int index)
{
    if (mSwitch.bdc.bdc_cate_tab_index != index)
    {
        VectorTable<MusicCateUnion *> &vec = m_pBDCTab->vec();
        if (index < 0 || index > vec.size())
        {
            GEN_Printf(LOG_WARN, "index=%d is invalid", index);
            return;
        }

        MusicCateUnion *bdcCate = vec[index];

        if (bdcCate->cid.empty()
                || bdcCate->hasSub.empty())
        {
            GEN_Printf(LOG_WARN, "ID num is empty, %s.", bdcCate->hasSub.string());
            return;
        }

        int cid_type   = CateItemUnion::CATE_ITEM_BDCAST;
        int cid_num    = atoi(bdcCate->cid.string());
        int bdc_type   = atoi(bdcCate->hasSub.string());
        int area_code  = 0;
        bool isAreaLab = false;


        if (-1 == cid_num) //type radio
        {
            cid_type= CateItemUnion::CATE_ITEM_TYPE_RADIO;
        } else if (AREA_TAB_ID == cid_num) // 省市台
        {
            isAreaLab = true;
            VectorTable<MusicCateUnion *> &areaVec = m_pBDCArea->vec();
            if (mSwitch.bdc.bdc_area_index >= 0 && mSwitch.bdc.bdc_area_index < areaVec.size())
            {
                if (false == areaVec[mSwitch.bdc.bdc_area_index]->cid.empty())
                {
                    area_code = atoi(areaVec[mSwitch.bdc.bdc_area_index]->cid.string());
                }
            }
        }

        if (isAreaLab)
        {
            gInstance->viewBDCItemAreaSwicth("qrc:/bdc/KlInlineAreaView.qml");
        } else
        {
            gInstance->viewBDCItemAreaSwicth("qrc:/bdc/KlInlineChannelView.qml");
        }

        m_pBDCItem->clear();

        CateItemUnion *& tmp = mBDCMap[(cid_num << 16) + area_code];
        if (tmp)
        {
            m_pBDCItem->setCateItemUnion(tmp);
            if (tmp->isEmpty())
            {
                tmp->loadCateItem();
            } else
            {
                tmp->onLoadOver(m_pBDCItem);
                msgBoxLocalLoadOver(kl::OBJECT_BDC_ITEM_LIST);
            }
            m_pBDCItem->resetAll();
        } else
        {
            tmp = new CateItemUnion(cid_type, m_pBDCItem);

            m_pBDCItem->setCateItemUnion(tmp);
            tmp->loadCateItem(bdc_type, cid_num, area_code);
        }

        mSwitch.bdc.bdc_cate_tab_index = index;
        Q_EMIT m_pBDCTab->bdcTabIndexChanged(index);
    } else
    {
        GEN_Printf(LOG_DEBUG, "index: %d was setted", index);
    }
}

void KLDataProc::bdcFirstAreaTabClick(int index)
{
    if (mSwitch.bdc.bdc_area_index != index)
    {
        VectorTable<MusicCateUnion *> &vec = m_pBDCArea->vec();
        if (index < 0 || index >= vec.size())
        {
            GEN_Printf(LOG_WARN, "index=%d is invalid", index);
            return;
        }

        VectorTable<MusicCateUnion *> &cateVec = m_pBDCTab->vec();

        if (mSwitch.bdc.bdc_cate_tab_index < 0
                || mSwitch.bdc.bdc_cate_tab_index >= cateVec.size())
        {
            GEN_Printf(LOG_WARN, "Current is not in area tab, cate tab: %d", mSwitch.bdc.bdc_cate_tab_index);
        } else
        {
            MusicCateUnion *bdcCate = m_pBDCTab->vec()[mSwitch.bdc.bdc_cate_tab_index];

            if (bdcCate->cid.empty()
                    || bdcCate->hasSub.empty()
                    || vec[index]->cid.empty())
            {
                GEN_Printf(LOG_WARN, "ID num is empty, %s, %s.", bdcCate->hasSub.string(), vec[index]->cid.string());
                return;
            }

            int cid_num    = atoi(bdcCate->cid.string());
            int bdc_type   = atoi(bdcCate->hasSub.string());
            int area_code  = atoi(vec[index]->cid.string());

            m_pBDCItem->clear();

            CateItemUnion *& tmp = mBDCMap[(cid_num << 16) + area_code];
            if (tmp)
            {
                m_pBDCItem->setCateItemUnion(tmp);
                tmp->onLoadOver(m_pBDCItem);
                m_pBDCItem->resetAll();
            } else
            {
                tmp = new CateItemUnion(CateItemUnion::CATE_ITEM_BDCAST, m_pBDCItem);

                m_pBDCItem->setCateItemUnion(tmp);
                tmp->loadCateItem(bdc_type, cid_num, area_code);
            }
        }

        mSwitch.bdc.bdc_area_index = index;

        Q_EMIT m_pBDCArea->bdcAreaIndexChanged(index);
    } else
    {
        mSwitch.bdc.bdc_area_index = -1;
        Q_EMIT m_pBDCArea->bdcAreaIndexChanged(-1);
//        GEN_Printf(LOG_DEBUG, "index: %d was setted", index);
    }
}

void KLDataProc::bdcSecondItemClick(int index, bool /*isInArea*/)
{
    if (mSwitch.bdc.bdc_item_index != index)
    {
        VectorTable<MusicCateItemUnion *> &vec = m_pBDCItem->vec();

        if (index < 0 || index >= vec.size())
        {
            GEN_Printf(LOG_ERROR, "Index=%d Out of range=%d", index, vec.size());
            return;
        }

        ByteString id = ByteString::allocString(vec[index]->id);
        int type = vec[index]->type;

        ChipItemUnion *&chip_item = mChipMap[id];
        if (chip_item)
        {
            m_pChipItemPlay->chipLocalLoad(chip_item);
            id.clear();
        } else
        {
            chip_item = new ChipItemUnion(type);
            m_pChipItemPlay->setChipItemUnion(chip_item);
            chip_item->loadChipList(id, true, ChipItemUnion::LOAD_OVER_BDCPROGRAM_IN_PLAYVIEW);
        }

        mSwitch.bdc.bdc_item_index = index;

        mPlayPath = mSwitch;
        mPlayPath.current_play_source = CURREN_PLAY_SOURCE_BDC_SECOND_LIST;

        mPlayPath.chip_item_index = index;
    } else
    {
        GEN_Printf(LOG_DEBUG, "index: %d was setted", index);
    }
}

void KLDataProc::bdcSecondItemCollectClick(int index, bool isCollect)
{
    GEN_Printf(LOG_DEBUG, "index=%d collect.", index, isCollect);

    VectorTable<MusicCateItemUnion *> &vec = m_pBDCItem->vec();
    if (index < 0 || index >= vec.size())
    {
        GEN_Printf(LOG_WARN, "Collect Index=%d is out of range.", index, isCollect);
        return;
    }

    CollectNode *tmp = new CollectNode;

    tmp->isLocal    = 0;
    tmp->type       = vec[index]->type;
    tmp->id         = ByteString();
    tmp->parentId   = ByteString::allocString(vec[index]->id);
    tmp->name       = ByteString();
    tmp->parentName = ByteString::allocString(vec[index]->name);
    tmp->image      = ByteString::allocString(vec[index]->img);
    tmp->playUrl    = ByteString::allocString(vec[index]->playUrl);

    LocalDataProc::instance()->bdcTypeRadioCollect(index, tmp);
}

void KLDataProc::bdcProgramListAction()
{
    GEN_Printf(LOG_DEBUG, "broadcast load over, play index=%d", mSwitch.bdc.bdc_item_index);
    VectorTable<MusicChipItemUnion *> &vec = m_pChipItemPlay->vec();

    if (PLAY_CHIP_TYPE_RADIO_CHIP == m_pChipItemPlay->getChipType())
    {
        mPlayPath.chip_item_index = 0;
    } else
    {
        mPlayPath.chip_item_index = vec.size()-1;
    }

    if (mPlayPath.chip_item_index >= 0 && vec.size() > 0)
    {
        GEN_Printf(LOG_DEBUG, "Play: %d-%d, url: %s", mPlayPath.chip_item_index, vec.size(), vec[mPlayPath.chip_item_index]->playUrl.string());
        gInstance->setSourceUrl(vec[mPlayPath.chip_item_index]->playUrl.string());
    } else
    {
        GEN_Printf(LOG_WARN, "Load broadcast program list is empty.");
    }
}

/**
 * @brief KLDataProc::showPlayingInfo
 * @details 表示当前已经进行播放准备了
 */
void KLDataProc::showPlayingInfo()
{
    bool needRecord = true;
    int index    = mPlayPath.chip_item_index;
    int viewType = getCurrentShowView();
    VectorTable<MusicChipItemUnion *> &vec = m_pChipItemPlay->vec();

    if (vec.empty())
    {
        GEN_Printf(LOG_WARN, "Current is not playing. play list is empty");
        return;
    }

    if (index < 0 || index >= vec.size())
    {
        GEN_Printf(LOG_ERROR, "Index=%d Out of range=%d, Can't show detail info.", index, vec.size());
        return;
    }

    switch (vec[index]->type)
    {
    case PLAY_CHIP_TYPE_AUDIO_CHIP   :    // 专辑音乐节目碎片三级标签item
    case PLAY_CHIP_TYPE_RADIO_CHIP   :    // 智能电台节目碎片三级标签item
        break;
    case PLAY_CHIP_TYPE_BDC_PROGRAM_CHIP: // 电台节目碎片三级标签item
    {
        int cur = 0, dur = 0;
        m_pChipItemPlay->getSliderBase(cur, dur, index);
        gInstance->setSliderBase(cur, dur);
        break;
    }
    case PLAY_CHIP_TYPE_LOCAL_LOAD   :    // 下载的音频碎片（专辑音乐碎片）
        break;
    case PLAY_CHIP_TYPE_COLLECT_RECORD:   // 加载收藏
        break;
    case PLAY_CHIP_TYPE_HISTROY_RECORD:   // 加载历史记录
        needRecord = false;
        break;
    case PLAY_CHIP_TYPE_SEARCH_LOAD:      // 加载搜索列表
        break;
    default:
        GEN_Printf(LOG_ERROR, "type: %d out of range", vec[index]->type);
        assert(0);
        break;
    }

    GEN_Printf(LOG_INFO, "view: %d", viewType);

    setPlayInfo(*(vec[index]));

    Q_EMIT m_pChipItemPlay->playingIndexChanged(index);

    // 播放信息的显示因该以具体的界面为基础进行通知
    switch(viewType)
    {
    case CURRENT_VIEW_IN_ALBUM_AUDIOLIST_AND_INFO:
        Q_EMIT m_pChipItem->playingIndexChanged(index);
        break;
    case CURRENT_VIEW_IN_ALBUM_INFO_LIST:
        Q_EMIT m_pCateItem->currenIndexChanged(mPlayPath.cate_item_index);
        break;
    case CURRENT_VIEW_IN_BROADCAST:
        Q_EMIT m_pBDCItem->currenBDCIndexChanged(mPlayPath.bdc.bdc_item_index);
        break;
    case CURRENT_VIEW_IN_COLLECT:
        LocalDataProc::instance()->showCollectIndex();
        break;
    case CURRENT_VIEW_IN_DOWNLOAD:
        LocalDataProc::instance()->showDownloadIndex();
        break;
    case CURRENT_VIEW_IN_HISTROY:
        LocalDataProc::instance()->showHistoryIndex();
        break;
    case CURRENT_VIEW_IN_VOICE_GUIDANCE:
        break;
    case CURRENT_VIEW_IN_SETTING:
        break;
    default:
        GEN_Printf(LOG_ERROR, "invalid view");
        assert(0);
    }

//    switch (mPlayPath.current_play_source)
//    {
//    case CURREN_PLAY_SOURCE_HISTORY_RECORD_LIST:     // 从历史记录列表中开始播放
//        needRecord = false;
//        break;
//    default:
//        break;
//    }

    // 显示当前播放信息
    Q_EMIT gInstance->playingInfo(QStringFromByteString(vec[index]->name),
                                  QStringFromByteString(vec[index]->desc));

    // 如果播放源来源于历史记录，那么就不要记录当前播放
    if (needRecord)
    {
        LocalDataProc::instance()->recordCurrentPlayItem(&mPlayInfo);
    }
    GEN_Printf(LOG_DEBUG, "Show Over, %d", vec.size());
}

//void KLDataProc::enterAlbumView()
//{
//    gInstance->pushNew("qrc:/CateItemInfoView.qml");
//}

int KLDataProc::getAlbumSecondIndex()
{
#if 1
    int index  = mPlayPath.cate_item_index;
    int index2 = mPlayPath.chip_item_index;
    VectorTable<MusicCateItemUnion *> &vec = m_pCateItem->vec();
    VectorTable<MusicChipItemUnion *> &vec2 = m_pChipItemPlay->vec();

    if (index2 < 0 || index2 >= vec2.size())
    {
        // GEN_Printf(LOG_ERROR, "Index=%d Out of range=%d", index2, vec2.size());
        return -1;
    }
    if (index >= 0 && index < vec.size())
    {
        if (vec[index]->id == vec2[index2]->parentId)
        {
            return index;
        }
    }

    // 需要通知电台列表，当前播放id，使之显示播放标志
    for (int i = 0; i < vec.size(); ++i)
    {
        if (vec[i]->id == vec2[index2]->parentId)
        {
            mPlayPath.cate_item_index = i;
            return i;
        }
    }

    return -1;
#else
    return mSwitch.cate_item_index;
#endif
}

int KLDataProc::getBDCSecondIndex()
{
#if 1
    int index  = mPlayPath.bdc.bdc_item_index;
    int index2 = mPlayPath.chip_item_index;
    VectorTable<MusicCateItemUnion *> &vec = m_pBDCItem->vec();
    VectorTable<MusicChipItemUnion *> &vec2 = m_pChipItemPlay->vec();

    if (index2 < 0 || index2 >= vec2.size())
    {
        // GEN_Printf(LOG_ERROR, "Index=%d Out of range=%d", index2, vec2.size());
        return -1;
    }

    if (index >= 0 && index < vec.size())
    {
        if (vec[index]->id == vec2[index2]->parentId)
        {
            return index;
        }
    }

    // 需要通知电台列表，当前播放id，使之显示播放标志
    for (int i = 0; i < vec.size(); ++i)
    {
        if (vec[i]->id == vec2[index2]->parentId)
        {
            mPlayPath.bdc.bdc_item_index = i;
            return i;
        }
    }

    return -1;
#else
    if ((PLAY_CHIP_TYPE_BDC_PROGRAM_CHIP == m_pChipItemPlay->getChipType())
            && (mSwitch.bdc.bdc_cate_tab_index == mPlayPath.bdc.bdc_cate_tab_index)
            && (MEDIA_TYPE_BROADCAST == mPlayPath.media_type) )
    {
        // GEN_Printf(LOG_WARN, "second section: %d", mPlayPath.bdc.bdc_item_index);
        return mPlayPath.bdc.bdc_item_index;
    } else
    {
        return -1;
    }
#endif
}

int KLDataProc::getBDCFirstTabIndex()
{
    return mSwitch.bdc.bdc_cate_tab_index;
}

int KLDataProc::getBDCFirstAreaIndex()
{
    return mSwitch.bdc.bdc_area_index;
}

int KLDataProc::getAlbumFirstIndex()
{
    return mSwitch.cate_tab_index;
}

int KLDataProc::getChipAudioThirdIndex()
{
    return -1;
}

int KLDataProc::getPlayThirdIndex(ChipItemModel *model)
{
    if (m_pChipItem == model)
    {
        if (m_pChipItem->equal(m_pChipItemPlay))
        {
            return mPlayPath.chip_item_index;
        }
        return -1;
    } else if (m_pChipItemPlay == model)
    {
        return mPlayPath.chip_item_index;
    } else
    {
        assert(0);
    }
}

void KLDataProc::playNext(bool isKeyPress)
{
    if (m_pChipItemPlay->isEmpty()
            || -1 == mPlayPath.chip_item_index)
    {
        GEN_Printf(LOG_DEBUG, "invalid play.");
        return;
    }

    int index = 1 + mPlayPath.chip_item_index;

    chipPlayThirdClick(index, isKeyPress);

    // 需要后台加载数据了
    if (index + 2 >= m_pChipItemPlay->size())
    {
        m_pChipItemPlay->loadNextPage(ChipItemUnion::LOAD_OVER_ALBUM_IN_PLAYVIEW);
    }
}

void KLDataProc::playPrev()
{
    if (m_pChipItemPlay->isEmpty()
            || -1 == mPlayPath.chip_item_index)
    {
        GEN_Printf(LOG_DEBUG, "invalid play.");
        return;
    }

    chipPlayThirdClick(mPlayPath.chip_item_index - 1);
}

void KLDataProc::currentIsCollect()
{
    if (!m_pChipItemPlay->isEmpty())
    {
        LocalDataProc::instance()->opCurCollect(&mPlayInfo);
    }
}

void KLDataProc::notifyCurIsCollect(bool isCollect)
{
    mCurrentIsCollect = isCollect;
    Q_EMIT m_pChipItemPlay->isCollectChanged(isCollect);
}

void KLDataProc::notifyBDCCollectChange(int index, bool isCollect)
{
    m_pBDCItem->isCollectItemContentChange(index, isCollect);
}

bool KLDataProc::getCurrentPlayInfo(ByteString &parentId, ByteString &id)
{
    int index2 = mPlayPath.chip_item_index;
    VectorTable<MusicChipItemUnion *> &vec2 = m_pChipItemPlay->vec();

    if (index2 < 0 || index2 >= vec2.size())
    {
        GEN_Printf(LOG_ERROR, "Index=%d Out of range=%d", index2, vec2.size());
        return false;
    }

    parentId = vec2[index2]->parentId;
    id       = vec2[index2]->chipId;
    return true;
}

void KLDataProc::localItemPlay(int type, int index, ChipItemUnion *pUnion)
{
    mSwitch.local.table_type = type;
    mSwitch.local.table_list_item_index = index;

    m_pChipItemPlay->chipLocalLoad(pUnion);

    mPlayPath = mSwitch;
    mPlayPath.current_play_source = type;

    chipPlayThirdClick(index);

    Q_EMIT m_pChipItemPlay->playingIndexChanged(index);
}

void KLDataProc::playDefaultItem(ChipItemUnion *pUnion)
{
    MusicChipItemUnion info;
    m_pCurPlayUnion = pUnion;
    mPlayPath.chip_item_sub_index = -1;
    if (pUnion->getUnionInfo(info, mPlayPath.chip_item_sub_index))
    {
        gInstance->setSourceUrl(info.playUrl.string());
    }
}

void KLDataProc::audioDetailLoadOver(MusicDetail &detail)
{
    m_pCurPlayUnion = nullptr;
    gInstance->setSourceUrl(detail.playUrl.string());
}

void KLDataProc::msgTipTimer(ATimer *that, void *ptr)
{
    GEN_Printf(LOG_DEBUG, "Msg Box delay show.");
    that->setRunning(false);
    ((KLDataProc *)ptr)->showDelayMsgBox();
}

void KLDataProc::showDelayMsgBox()
{
    switch (mCurrenObjectName)
    {
    case kl::OBJECT_ACTIVE_MANAGE:
    case kl::OBJECT_INIT_MANAGE:
         Q_EMIT gInstance->msgTipGlobal(KLUIProc::msgBufferring, "");
        break;    
    case kl::OBJECT_ALBUM_LIST:
    case kl::OBJECT_BDC_AREA_LIST:
    case kl::OBJECT_BDC_ITEM_DETAIL:
    case kl::OBJECT_BDC_ITEM_LIST:
    case kl::OBJECT_BDC_ITEM_PROGRAM:
    case kl::OBJECT_CATEGORY_ALL:
    case kl::OBJECT_CATEGORY_BDC:
    case kl::OBJECT_CATEGORY_SUB_LIST:
    case kl::OBJECT_CHIP_AUDIO_DETAIL:    
    case kl::OBJECT_CHIP_RADIO_DETAIL:
    case kl::OBJECT_CHIP_RADIO_LIST:
    case kl::OBJECT_OPERATE_LIST:
//    case kl::OBJECT_SUGGESTION_WORD:
//    case kl::OBJECT_VOICE_SEARCH_ALL:
    case kl::OBJECT_TYPERADIO_LIST:
        Q_EMIT gInstance->msgTipCateItem(KLUIProc::msgBufferring, "Data Loading...");
        break;
    case kl::OBJECT_ALBUM_DETAIL:
    case kl::OBJECT_CHIP_AUDIO_LIST:
        Q_EMIT gInstance->msgTipAudioList(KLUIProc::msgBufferring, "Data Loading...");
        break;
    default:
        GEN_Printf(LOG_ERROR, "Invalid objectName: %d", mCurrenObjectName);
        break;
    }
}

void KLDataProc::msgBoxLocalLoadOver(int objectName)
{
    switch (objectName)
    {
    case kl::OBJECT_ACTIVE_MANAGE:
    case kl::OBJECT_INIT_MANAGE:
         Q_EMIT gInstance->msgTipGlobal(KLUIProc::nullEmpty, "");
        break;
    case kl::OBJECT_ALBUM_LIST:
    case kl::OBJECT_BDC_AREA_LIST:
    case kl::OBJECT_BDC_ITEM_DETAIL:
    case kl::OBJECT_BDC_ITEM_LIST:
    case kl::OBJECT_BDC_ITEM_PROGRAM:
    case kl::OBJECT_CATEGORY_ALL:
    case kl::OBJECT_CATEGORY_BDC:
    case kl::OBJECT_CATEGORY_SUB_LIST:
    case kl::OBJECT_CHIP_AUDIO_DETAIL:
    case kl::OBJECT_CHIP_RADIO_DETAIL:
    case kl::OBJECT_CHIP_RADIO_LIST:
    case kl::OBJECT_OPERATE_LIST:
//        case kl::OBJECT_SUGGESTION_WORD:
//        case kl::OBJECT_VOICE_SEARCH_ALL:
    case kl::OBJECT_TYPERADIO_LIST:
        Q_EMIT gInstance->msgTipCateItem(KLUIProc::nullEmpty, "");
        break;
    case kl::OBJECT_ALBUM_DETAIL:
    case kl::OBJECT_CHIP_AUDIO_LIST:
        Q_EMIT gInstance->msgTipAudioList(KLUIProc::nullEmpty, "");
        break;
    default:
        GEN_Printf(LOG_ERROR, "Invalid objectName: %d", mCurrenObjectName);
        break;
    }
}

void KLDataProc::klObjectObtainState(bool state, int objectName)
{
    GEN_Printf(LOG_DEBUG, "objectName: %d, state=%d", objectName, state);
    if (state)
    {
        mCurrenObjectName = objectName;
        m_pMsgPopDelayTimer->restart(700);

        msgBoxLocalLoadOver(mCurrenObjectName);
    } else
    {
        GEN_Printf(LOG_ERROR, "objectName: %d obtain start failed.", objectName);
    }
}

void KLDataProc::klObjectObtainOver(int objectName)
{
    GEN_Printf(LOG_DEBUG, "ObjectName: %d load over.", objectName);
    if (m_pMsgPopDelayTimer->isRunning())
    {
        GEN_Printf(LOG_DEBUG, "Timer is running.");
        m_pMsgPopDelayTimer->stop();
        return;
    }
    // close pop box
    msgBoxLocalLoadOver(objectName);
}

void KLDataProc::klLoadDataExportEmpty(int objectName)
{
    if (m_pMsgPopDelayTimer->isRunning())
    {
        m_pMsgPopDelayTimer->stop();
    }

    int viewType = getCurrentShowView();
    GEN_Printf(LOG_DEBUG, "viewType: %d, objectName: %d load empty data.", viewType, objectName);
    switch (viewType)
    {
    case CURRENT_VIEW_IN_ALBUM_AUDIOLIST_AND_INFO:
        Q_EMIT gInstance->msgTipAudioList(KLUIProc::emptyData, "Load Empty Data.");
        break;
    case CURRENT_VIEW_IN_ALBUM_INFO_LIST:
        Q_EMIT gInstance->msgTipCateItem(KLUIProc::emptyData, "Load Empty Data.");
        break;
    case CURRENT_VIEW_IN_BROADCAST:
        Q_EMIT gInstance->msgTipBroadcast(KLUIProc::emptyData, "Load Empty Data.");
        break;
    case CURRENT_VIEW_IN_COLLECT:
    case CURRENT_VIEW_IN_DOWNLOAD:
    case CURRENT_VIEW_IN_HISTROY:
    case CURRENT_VIEW_IN_VOICE_GUIDANCE:
    case CURRENT_VIEW_IN_SETTING:
        break;
    default:
        break;
    }
}

void KLDataProc::klLoadDataPriserExcept(int objectName, const ByteString &str)
{
    if (m_pMsgPopDelayTimer->isRunning())
    {
        m_pMsgPopDelayTimer->stop();
    }
    int viewType = getCurrentShowView();
    GEN_Printf(LOG_DEBUG, "viewType: %d, objectName: %d data priser failed.", viewType, objectName);
    switch (viewType)
    {
    case CURRENT_VIEW_IN_ALBUM_AUDIOLIST_AND_INFO:
        Q_EMIT gInstance->msgTipAudioList(KLUIProc::failTip, QObject::tr(str.string()));
        break;
    case CURRENT_VIEW_IN_ALBUM_INFO_LIST:
        Q_EMIT gInstance->msgTipCateItem(KLUIProc::failTip, QObject::tr(str.string()));
        break;
    case CURRENT_VIEW_IN_BROADCAST:
        Q_EMIT gInstance->msgTipBroadcast(KLUIProc::failTip, QObject::tr(str.string()));
        break;
    case CURRENT_VIEW_IN_COLLECT:
    case CURRENT_VIEW_IN_DOWNLOAD:
    case CURRENT_VIEW_IN_HISTROY:
    case CURRENT_VIEW_IN_VOICE_GUIDANCE:
    case CURRENT_VIEW_IN_SETTING:
        break;
    default:
        break;
    }
}

void KLDataProc::sysNetLoadApiExcept(int objectName, int /*type*/, const char *str)
{
    int viewType = getCurrentShowView();
    GEN_Printf(LOG_DEBUG, "viewType: %d, objectName: %d load api except.", viewType, objectName);
    switch (viewType)
    {
    case CURRENT_VIEW_IN_ALBUM_AUDIOLIST_AND_INFO:
        Q_EMIT gInstance->msgTipAudioList(KLUIProc::failTip, QObject::tr(str));
        break;
    case CURRENT_VIEW_IN_ALBUM_INFO_LIST:
        Q_EMIT gInstance->msgTipCateItem(KLUIProc::failTip, QObject::tr(str));
        break;
    case CURRENT_VIEW_IN_BROADCAST:
        Q_EMIT gInstance->msgTipBroadcast(KLUIProc::failTip, QObject::tr(str));
        break;
    case CURRENT_VIEW_IN_COLLECT:
    case CURRENT_VIEW_IN_DOWNLOAD:
    case CURRENT_VIEW_IN_HISTROY:
    case CURRENT_VIEW_IN_VOICE_GUIDANCE:
    case CURRENT_VIEW_IN_SETTING:
        break;
    default:
        break;
    }
}

void KLDataProc::reloadErrObject()
{
    GEN_Printf(LOG_DEBUG, "reload error object");
    Application::instance()->postCmd(SIG_KL_RELOAD_ERR_OBJECT);
}

void KLDataProc::localDataRenderPlaying(ChipItemUnion *pUnion)
{
    m_pChipItemPlay->localDataChange(pUnion);
}

