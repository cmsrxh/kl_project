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
#include "media_client_proxy.h"
#include "media_iface_common.h"
#include "media_service_call_back.h"
#include "qq_ip_positioning.h"
#include "application.h"
#include "current_backup.h"
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
    m_pCurPlayUnion = nullptr;
}

void KLDataProc::enterBroadcastView()
{
    GEN_Printf(LOG_DEBUG, "enter BDC view.");
    int index = 0;
    mSwitch.media_type = MEDIA_TYPE_BROADCAST;
    gInstance->viewAlbumBDCSwitch("bdc/KlInlineBroadcast.qml");

    if (-1 < mSwitch.bdc.bdc_cate_tab_index)
    {
        index = mSwitch.bdc.bdc_cate_tab_index;
    }

    if (mSwitch.bdc.bdc_cate_tab_index == index) mSwitch.bdc.bdc_cate_tab_index = -1;

    bdcFirstCateTabClick(index);
}

void KLDataProc::playSubItem(MusicChipItemUnion *chip)
{
    GEN_Printf(LOG_DEBUG, "play sub item,sub_type=%d", chip->sub_type);
    m_pCurPlayUnion = nullptr;
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
            // chip->subChips = chip_item;
            playDefaultItem(chip_item);
        } else
        {
            chip_item = new ChipItemUnion(chip->sub_type);
            // 也仅仅只是播放列表项中子列表
            // m_pChipItemPlay->setChipItemUnion(chip_item);
            // chip->subChips = chip_item;
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
        SF_ASSERT(0);
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
    case PLAY_CHIP_TYPE_PREV_PLAYING_RECORD:
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
    MediaClientProxy::instance()->setCallback(&i);
    MediaClientProxy::instance()->registerNotifyMsg();
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
            SF_ASSERT(0);
        }
    } else if (1 == mSwitch.mainTabIndex) //表示在我的界面
    {
        // GEN_Printf(LOG_DEBUG, "---self_tab_index=%d----", mSwitch.local.self_tab_index);
        switch (mSwitch.local.self_tab_index)
        {
        case 0: return CURRENT_VIEW_IN_COLLECT;
        case 1: return CURRENT_VIEW_IN_DOWNLOAD;
        case 2: return CURRENT_VIEW_IN_HISTROY;
        case 3: return CURRENT_VIEW_IN_VOICE_GUIDANCE;
        case 4: return CURRENT_VIEW_IN_SETTING;
        default:
            GEN_Printf(LOG_ERROR, "is Not invalid view tab");
            SF_ASSERT(0);
        }
    } else
    {
        GEN_Printf(LOG_ERROR, "is Not invalid view tab");
        SF_ASSERT(0);
    }
    return -1;
}

void KLDataProc::mainTabClick(int index)
{
    mSwitch.setMainTabIndex(index);
}

void KLDataProc::selfTabClick(int index)
{
    //GEN_Printf(LOG_DEBUG, "---SET: self_tab_index=%d----", mSwitch.local.self_tab_index);
    mSwitch.local.self_tab_index = index;
}

void KLDataProc::albumFirstClick(int index)
{
    GEN_Printf(LOG_DEBUG, "album first click index=%d", index);
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
            mSwitch.setCateTabIndex(index);
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
                PopTipManage::instance()->closeMsgBox(kl::OBJECT_ALBUM_LIST);
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
            PopTipManage::instance()->closeMsgBox(kl::OBJECT_CHIP_AUDIO_LIST);
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

void KLDataProc::chipPlayThirdClick(int index)
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
        case PLAY_CHIP_TYPE_PREV_PLAYING_RECORD: // 表示上一次程序退出时记录的播放信息
        {
            playSubItem(vec[index]);
            break;
        }
        default:
            GEN_Printf(LOG_ERROR, "play type: %d out of range", vec[index]->type);
            SF_ASSERT(0);
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
    GEN_Printf(LOG_DEBUG, "-------bdc cate click: %d-----", index);
    if (mSwitch.bdc.bdc_cate_tab_index != index)
    {
        VectorTable<MusicCateUnion *> &vec = m_pBDCTab->vec();
        if (index < 0 || index > vec.size())
        {
            GEN_Printf(LOG_WARN, "index=%d is invalid", index);
            return;
        }

        MusicCateUnion *bdcCate = vec[index];

        if (bdcCate->cid.empty())
        {
            GEN_Printf(LOG_WARN, "ID num is empty, %s.", bdcCate->hasSub.string());
            if (0 == index) // 需要定位当前位置
            {
                GEN_Printf(LOG_DEBUG, "----------------------");
                (new QQIPPositioning)->obtain();
            }
            return;
        }

        int cid_type   = CateItemUnion::CATE_ITEM_BDCAST;
        int cid_num    = bdcCate->cid.toInt();
        int bdc_type   = bdcCate->hasSub.toInt();
        int area_code  = 0;
        bool isAreaLab = false;

        if (0 == index)  // "id":2,"name":"省市台","type":1,
        {
            area_code = cid_num;
            bdc_type  = 1;
            cid_num = 2;
        } else if (-1 == cid_num) //type radio "id":-1,"name":"智能台","type":-1
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
                    area_code = areaVec[mSwitch.bdc.bdc_area_index]->cid.toInt();
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
                PopTipManage::instance()->closeMsgBox(kl::OBJECT_BDC_ITEM_LIST);
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

            int cid_num    = bdcCate->cid.toInt();
            int bdc_type   = bdcCate->hasSub.toInt();
            int area_code  = vec[index]->cid.toInt();

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
                    PopTipManage::instance()->closeMsgBox(kl::OBJECT_BDC_ITEM_LIST);
                }
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
        if (chip_item->isEmpty())
        {
            chip_item->loadChipList(id, true, ChipItemUnion::LOAD_OVER_BDCPROGRAM_IN_PLAYVIEW);
        } else if (mSwitch.bdc.bdc_item_index != index)
        {
            m_pChipItemPlay->chipLocalLoad(chip_item);
            bdcProgramListAction();
        }

        id.clear();
    } else
    {
        chip_item = new ChipItemUnion(type);
        m_pChipItemPlay->setChipItemUnion(chip_item);
        chip_item->loadChipList(id, true, ChipItemUnion::LOAD_OVER_BDCPROGRAM_IN_PLAYVIEW);
    }

    mSwitch.bdc.bdc_item_index = index;
    mPlayPath = mSwitch;
    mPlayPath.chip_item_index = index;
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

    // 表示操作的当前项是否正在播放中
    int arg = (mPlayInfo.parentId == tmp->parentId) ? 1 : 0;

    LocalDataProc::instance()->bdcTypeRadioCollect((arg << 16) | index, tmp);
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
        if (m_pCurPlayUnion && mPlayPath.chip_item_sub_index >= 0)
        {
            int cur = 0, dur = 0;
            m_pCurPlayUnion->getUnionSlideBase(cur, dur, mPlayPath.chip_item_sub_index);
            gInstance->setSliderBase(cur, dur);
        }
        break;
    case PLAY_CHIP_TYPE_HISTROY_RECORD:   // 加载历史记录
        needRecord = false;
        if (m_pCurPlayUnion && mPlayPath.chip_item_sub_index >= 0)
        {
            int cur = 0, dur = 0;
            m_pCurPlayUnion->getUnionSlideBase(cur, dur, mPlayPath.chip_item_sub_index);
            gInstance->setSliderBase(cur, dur);
        }
        break;
    case PLAY_CHIP_TYPE_SEARCH_LOAD:      // 加载搜索列表
        if (m_pCurPlayUnion && mPlayPath.chip_item_sub_index >= 0)
        {
            int cur = 0, dur = 0;
            m_pCurPlayUnion->getUnionSlideBase(cur, dur, mPlayPath.chip_item_sub_index);
            gInstance->setSliderBase(cur, dur);
        }
        break;
    case PLAY_CHIP_TYPE_PREV_PLAYING_RECORD:
        if (m_pCurPlayUnion && mPlayPath.chip_item_sub_index >= 0)
        {
            int cur = 0, dur = 0;
            m_pCurPlayUnion->getUnionSlideBase(cur, dur, mPlayPath.chip_item_sub_index);
            gInstance->setSliderBase(cur, dur);
            GEN_Printf(LOG_DEBUG, "cur=%d, dur=%d", cur, dur);
        }
        break;
    default:
        GEN_Printf(LOG_ERROR, "type: %d out of range", vec[index]->type);
        SF_ASSERT(0);
        break;
    }

    GEN_Printf(LOG_INFO, "view: %d, type: %d, (%p, %d)", viewType, vec[index]->type,
               m_pCurPlayUnion, mPlayPath.chip_item_sub_index);

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
        //GEN_Printf(LOG_DEBUG, "----%d-----", mPlayPath.bdc.bdc_item_index);
        Q_EMIT m_pBDCItem->currenIndexChanged(mPlayPath.bdc.bdc_item_index);
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
        SF_ASSERT(0);
    }

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

//    if (m_pBDCItem->getCateType() == CateItemUnion::CATE_ITEM_TYPE_RADIO)
//    {
//        return index;
//    }
//    GEN_Printf(LOG_DEBUG, "index: %d, %d", index, index2);
    if (index2 < 0 || index2 >= vec2.size())
    {
//        GEN_Printf(LOG_ERROR, "Index=%d Out of range=%d", index2, vec2.size());
        return -1;
    }

    if (index >= 0 && index < vec.size())
    {
//        GEN_Printf(LOG_DEBUG, "type: %d, id: %s, parentId: %s", vec[index]->type, vec[index]->id.string(), vec2[index2]->parentId.string());

        if (vec[index]->id == vec2[index2]->parentId)
        {
            return index;
        }
    }

    // 需要通知电台列表，当前播放id，使之显示播放标志
    for (int i = 0; i < vec.size(); ++i)
    {
//        GEN_Printf(LOG_DEBUG, "id: %s, parentId: %s", vec[i]->id.string(), vec2[index2]->parentId.string());
        if (vec[i]->id == vec2[index2]->parentId)
        {
            mPlayPath.bdc.bdc_item_index = i;
            return i;
        }
    }

    // GEN_Printf(LOG_DEBUG, "---------get bdc index end.---------");
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

int KLDataProc::cateItemCurIndex(CateItemModel *that)
{
    if (that == m_pCateItem) //album model item
    {
        return getAlbumSecondIndex();;
    } else if (that == m_pBDCItem) //broadcast model item
    {
        return getBDCSecondIndex();
    }
    SF_ASSERT(0);
    return -1;
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
//    GEN_Printf(LOG_DEBUG, "----------%d----------", mSwitch.cate_tab_index);
//    if (4 == mSwitch.cate_tab_index) enterBroadcastView();
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
        SF_ASSERT(0);
    }
    return -1;
}

void KLDataProc::playNext()
{    
    if (m_pChipItemPlay->isEmpty()
            || -1 == mPlayPath.chip_item_index)
    {
        GEN_Printf(LOG_DEBUG, "invalid play.");
        return;
    }

    chipPlayThirdClick(1 + mPlayPath.chip_item_index);
}

void KLDataProc::autoPlayNext()
{
    VectorTable<MusicChipItemUnion *> &vec = m_pChipItemPlay->vec();
    int index = mPlayPath.chip_item_index;
    int size = vec.size();
    if (m_pChipItemPlay->isEmpty()
            || -1 == index)
    {
        GEN_Printf(LOG_DEBUG, "invalid play.");
        return;
    }
    if (index < 0 || index >= size)
    {
        GEN_Printf(LOG_ERROR, "Index=%d Out of range=%d", index, size);
        return;
    }

    int chipType = m_pChipItemPlay->getChipType();

    if (chipType == PLAY_CHIP_TYPE_COLLECT_RECORD   // 加载收藏
            || chipType == PLAY_CHIP_TYPE_HISTROY_RECORD   // 加载历史记录
            || chipType == PLAY_CHIP_TYPE_SEARCH_LOAD      // 加载搜索列表
            || chipType == PLAY_CHIP_TYPE_PREV_PLAYING_RECORD) // 表示上一次程序退出时记录的播放信息
    {
        if (!playCurSubItemSubNext(vec[index]))
        {
            if ((index + 1) < size)
            {
                playSubItem(vec[index + 1]);
                mPlayPath.setChipItemIndex(index + 1);
            } else
            {
                GEN_Printf(LOG_WARN, "Auto Play Next, out of range.");
            }
        }

        if (m_pCurPlayUnion)
        {
            switch (vec[index]->sub_type) {
            case PLAY_CHIP_TYPE_ALBUM        :    // 专辑二级标签item
            case PLAY_CHIP_TYPE_BROADCAST    :    // 电台二级标签item
            case PLAY_CHIP_TYPE_TYPE_RADIO   :    // 智能电台二级标签item
            {
                break;
            }
            default:
                return;
            }

            if (mPlayPath.chip_item_sub_index + 2 < m_pCurPlayUnion->itemCount())
            {
                m_pCurPlayUnion->loadNextPage(ChipItemUnion::LOAD_OVER_ALBUM_IN_PLAYVIEW);
            }
        }
    } else
    {
        chipPlayThirdClick(index + 1);

        // 需要后台加载数据了
        if (index + 2 >= m_pChipItemPlay->size())
        {
            m_pChipItemPlay->loadNextPage(ChipItemUnion::LOAD_OVER_ALBUM_IN_PLAYVIEW);
        }
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

bool KLDataProc::getCurrentPlayInfo(ByteString &parentId, ByteString &id)
{
    int index2 = mPlayPath.chip_item_index;
    VectorTable<MusicChipItemUnion *> &vec2 = m_pChipItemPlay->vec();

    if (index2 < 0 || index2 >= vec2.size())
    {
        //GEN_Printf(LOG_ERROR, "Index=%d Out of range=%d", index2, vec2.size());
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

    chipPlayThirdClick(index);

    Q_EMIT m_pChipItemPlay->playingIndexChanged(index);
}

void KLDataProc::playDefaultItem(ChipItemUnion *pUnion)
{    
    GEN_Printf(LOG_DEBUG, "Play default item: chipType=%d", m_pChipItemPlay->getChipType());

    MusicChipItemUnion info;
    m_pCurPlayUnion = pUnion;
    mPlayPath.chip_item_sub_index = -1;
    if (pUnion->getUnionInfo(info, mPlayPath.chip_item_sub_index))
    {
        gInstance->setSourceUrl(info.playUrl.string());
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

void KLDataProc::setViewSwitchInfo(char *data)
{
    mSwitch = *((SwitchPath *)data);
    mSwitch.bdc.bdc_item_index = -1;
    if (mSwitch.local.self_tab_index < 0 || mSwitch.local.self_tab_index > 4)
    {
        mSwitch.local.self_tab_index = 0;
    }
//    GEN_Printf(LOG_DEBUG, "mSwitch: sec=%d, fir=%d, firArea=%d", mSwitch.bdc.bdc_item_index, mSwitch.bdc.bdc_cate_tab_index, mSwitch.bdc.bdc_area_index);
//    GEN_Printf(LOG_DEBUG, "mPlayPath: sec=%d, fir=%d, firArea=%d", mPlayPath.bdc.bdc_item_index, mPlayPath.bdc.bdc_cate_tab_index, mPlayPath.bdc.bdc_area_index);
}

const CollectNode *KLDataProc::getPlayInfoIfPlaying() const
{
    return (m_pChipItemPlay->isEmpty()) ? nullptr : &mPlayInfo;
}

void KLDataProc::currentIsCollect()
{
    if (!m_pChipItemPlay->isEmpty() && !(mPlayInfo.id.empty() && mPlayInfo.parentId.empty()))
    {
        LocalDataProc::instance()->opCurCollect(&mPlayInfo);
    }
}

void KLDataProc::notifyCurIsCollect(bool isCollect)
{
    mCurrentIsCollect = isCollect;
    Q_EMIT m_pChipItemPlay->isCollectChanged(isCollect);
}

void KLDataProc::bdcNotifyCurIsCollect(CollectNode *item, bool isCollect)
{
    if ((mPlayInfo.parentId == item->parentId) &&
            (item->type == PLAY_CHIP_TYPE_BROADCAST
             || item->type == PLAY_CHIP_TYPE_TYPE_RADIO
             || item->type == PLAY_CHIP_TYPE_RADIO_CHIP
             || item->type == PLAY_CHIP_TYPE_BDC_PROGRAM_CHIP
             || item->type == PLAY_CHIP_TYPE_ALBUM
             || item->id == mPlayInfo.id))
    {
        notifyCurIsCollect(isCollect);
    }
}

void KLDataProc::notifyBDCCollectChange(int index, bool isCollect)
{
    //GEN_Printf(LOG_DEBUG, "++++++++++++index=%d", index);
    m_pBDCItem->isCollectItemContentChange(index, isCollect);
}

bool KLDataProc::deleteCurrentInfo(int chipType)
{
    if (chipType == m_pChipItemPlay->getChipType())
    {
        mPlayInfo.isLocal    = 0; // 表明当前媒体时候在本地
        mPlayInfo.type       = 0;    // PLAY_CHIP_TYPE*
        mPlayInfo.id         = ByteString();
        mPlayInfo.parentId   = ByteString();
        mPlayInfo.name       = ByteString();
        mPlayInfo.parentName = ByteString();
        mPlayInfo.image      = ByteString();
        mPlayInfo.playUrl    = ByteString();
        mPlayInfo.fileSize   = ByteString();
        return true;
    }
    return false;
}

void KLDataProc::collectItemDelete()
{
    if (PLAY_CHIP_TYPE_COLLECT_RECORD == m_pChipItemPlay->getChipType())
    {
        int index = mPlayPath.chip_item_index;
        mPlayPath.chip_item_index = -1;
        chipPlayThirdClick(index);

        if (m_pChipItemPlay->isEmpty())
        {
            gInstance->qmlReset();
        }
    }
}

void KLDataProc::notifyCurrentCollectChange(CollectNode *node, bool isCollect)
{
    GEN_Printf(LOG_DEBUG, "Current Collect Item change.%s,%s, %s", node->id.string(), node->parentId.string(), node->name.string());
    VectorTable<MusicCateItemUnion *> &vec = m_pBDCItem->vec();
    for (int i = 0; i < vec.size(); ++i)
    {
        //GEN_Printf(LOG_DEBUG, "%s, %s", vec[i]->id.string(), vec[i]->name.string());

        if (vec[i]->id == node->parentId)
        {
            m_pBDCItem->isCollectItemContentChange(i, isCollect);
            break;
        }
    }
}

bool KLDataProc::locationConfirm(const ByteString &province, kl::AreaItem &area)
{
    VectorTable<MusicCateUnion *> &vec = m_pBDCArea->vec();
    GEN_Printf(LOG_DEBUG, "Locationing province=%s", province.string());

    for (int i = 1; i < vec.size(); ++i)
    {
        MusicCateUnion *addr = vec[i];
        // GEN_Printf(LOG_DEBUG, "id=%s, name=%s", addr->cid.string(), addr->name.string());
        if (0 == strncmp(addr->name.string(), province.string(), addr->name.size()))
        {
            area.id = addr->cid;
            area.name = addr->name;
            return true;
        }
    }
    return false;
}

void KLDataProc::notifyLocationChange(kl::AreaItem *area)
{
    VectorTable<MusicCateUnion *> &vec = m_pBDCTab->vec();

    GEN_Printf(LOG_DEBUG, "Location Change: %s, %s", area->id.string(), area->name.string());
    if (!vec.empty())
    {
        vec[0]->cid  = area->id;
        vec[0]->name = area->name;
        // GEN_Printf(LOG_DEBUG, "notify data change.");
        Q_EMIT m_pBDCTab->dataChanged(m_pBDCTab->index(0), m_pBDCTab->index(0));

        // 此处是事件循环子线程调用这个偏向主线程调用的函数，有一定的概率会发生错误（同步和异步）
        // 最好使用消息的方式通知主线程，由主线程调用(bdcFirstCateTabClick)这个函数
        if (m_pBDCItem->empty())
        {
            // 发送到主线程中调用
            Q_EMIT gInstance->mainThreadProc(1, 0);
            // bdcFirstCateTabClick(0);
        }
    }
}

void KLDataProc::notifyLocationFailed()
{
    VectorTable<MusicCateUnion *> &vec = m_pBDCTab->vec();

    if (!vec.empty())
    {
        vec[0]->name = "定位失败";
        Q_EMIT m_pBDCTab->dataChanged(m_pBDCTab->index(0), m_pBDCTab->index(0));
    }
}

