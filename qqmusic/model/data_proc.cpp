#include <events/common_log.h>
#include "data_proc.h"
#include "ui_proc.h"
#include "events/app_timer.h"
#include "local_data_proc.h"
#include "iface/media_service_i_face.h"
#include "iface/media_iface_common.h"
#include "iface/media_service_call_back.h"

#include "application.h"
#include "current_backup.h"
#include "../klIface/kl_service_notify.h"
#include "../klIface/kl_service_priser.h"

extern UIProc *gInstance;

class MpvMsgPriser : public MediaServiceCallback
{
public:
    void mediaNotify(int msg, int ext1, int ext2, const char *str)
    {
        //GEN_Printf(LOG_DUMP, "msg: %d (%d, %d), %s", msg, ext1, ext2, str);
        Q_EMIT gInstance->recvNotify(msg, ext1, ext2, QString::fromUtf8(str));
    }
};

DataProc::DataProc()
{    
}

void DataProc::enterBroadcastView()
{
    GEN_Printf(LOG_DEBUG, "enter BDC view.");
}

void DataProc::playSubItem(MusicChipItemUnion *chip)
{
    GEN_Printf(LOG_DEBUG, "play sub item,sub_type=%d", chip->sub_type);
}

bool DataProc::playCurSubItemSubNext(MusicChipItemUnion *)
{

    return false;
}

void DataProc::setPlayInfo(MusicChipItemUnion &chip)
{
}

DataProc::~DataProc()
{

}

void DataProc::initMedia()
{
    static MpvMsgPriser i;
    MediaServiceIFace::instance()->initClientIface(&i);
}

void DataProc::initAlbum(CategoryModel *cate, CateItemModel *cateItem, ChipItemModel *chip, ChipItemModel *player)
{
    m_pCate         = cate;
    m_pCateItem     = cateItem;
    m_pChipItem     = chip;
    m_pChipItemPlay = player;

}

void DataProc::initBroadcast(CategoryModel *bdcTab, CategoryModel *bdcArea, CateItemModel *bdcItem)
{
    m_pBDCTab   = bdcTab;
    m_pBDCArea  = bdcArea;
    m_pBDCItem  = bdcItem;
}

void DataProc::detailLoadAlbumInfo()
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

int DataProc::getCurrentShowView()
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
            assert(0);
        }
    } else
    {
        GEN_Printf(LOG_ERROR, "is Not invalid view tab");
        assert(0);
    }
    return -1;
}

void DataProc::mainTabClick(int index)
{
    mSwitch.setMainTabIndex(index);
}

void DataProc::selfTabClick(int index)
{
    //GEN_Printf(LOG_DEBUG, "---SET: self_tab_index=%d----", mSwitch.local.self_tab_index);
    mSwitch.local.self_tab_index = index;
}

void DataProc::albumFirstClick(int index)
{
    GEN_Printf(LOG_DEBUG, "album first click index=%d", index);

}

void DataProc::albumSecondClick(int index)
{
}

void DataProc::chipAudioThirdChick(int index)
{

}

void DataProc::chipPlayThirdClick(int index)
{

}

#define AREA_TAB_ID 2
void DataProc::bdcFirstCateTabClick(int index)
{
    GEN_Printf(LOG_DEBUG, "-------bdc cate click: %d-----", index);

}

void DataProc::bdcFirstAreaTabClick(int index)
{

}

void DataProc::bdcSecondItemClick(int index, bool /*isInArea*/)
{
}

void DataProc::bdcSecondItemCollectClick(int index, bool isCollect)
{
    GEN_Printf(LOG_DEBUG, "index=%d collect.", index, isCollect);

}

void DataProc::bdcProgramListAction()
{

}

/**
 * @brief KLDataProc::showPlayingInfo
 * @details 表示当前已经进行播放准备了
 */
void DataProc::showPlayingInfo()
{
}

//void KLDataProc::enterAlbumView()
//{
//    gInstance->pushNew("qrc:/CateItemInfoView.qml");
//}

int DataProc::getAlbumSecondIndex()
{

}

int DataProc::getBDCSecondIndex()
{

}

int DataProc::cateItemCurIndex(CateItemModel *that)
{

}

int DataProc::getBDCFirstTabIndex()
{
    return mSwitch.bdc.bdc_cate_tab_index;
}

int DataProc::getBDCFirstAreaIndex()
{
    return mSwitch.bdc.bdc_area_index;
}

int DataProc::getAlbumFirstIndex()
{
    return mSwitch.cate_tab_index;
}

int DataProc::getChipAudioThirdIndex()
{
    return -1;
}

int DataProc::getPlayThirdIndex(ChipItemModel *model)
{
    if (m_pChipItem == model)
    {

        return -1;
    } else if (m_pChipItemPlay == model)
    {
        return mPlayPath.chip_item_index;
    } else
    {
        assert(0);
    }
}

void DataProc::playNext()
{    
}

void DataProc::autoPlayNext()
{
}

void DataProc::playPrev()
{
}

bool DataProc::getCurrentPlayInfo(ByteString &parentId, ByteString &id)
{

    return true;
}

void DataProc::localItemPlay(int type, int index, ChipItemUnion *pUnion)
{
}

void DataProc::playDefaultItem(ChipItemUnion *pUnion)
{    
}

void DataProc::reloadErrObject()
{
    GEN_Printf(LOG_DEBUG, "reload error object");
    Application::instance()->postCmd(SIG_KL_RELOAD_ERR_OBJECT);
}

void DataProc::localDataRenderPlaying(ChipItemUnion *pUnion)
{

}

void DataProc::setViewSwitchInfo(char *data)
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

const CollectNode *DataProc::getPlayInfoIfPlaying() const
{
    return &mPlayInfo;
}

void DataProc::currentIsCollect()
{

}

void DataProc::notifyCurIsCollect(bool isCollect)
{

}

void DataProc::bdcNotifyCurIsCollect(CollectNode *item, bool isCollect)
{

}

void DataProc::notifyBDCCollectChange(int index, bool isCollect)
{

}

bool DataProc::deleteCurrentInfo(int chipType)
{

    return false;
}

void DataProc::collectItemDelete()
{

}

void DataProc::notifyCurrentCollectChange(CollectNode *node, bool isCollect)
{

}

//bool KLDataProc::locationConfirm(const ByteString &province, kl::AreaItem &area)
//{
//    GEN_Printf(LOG_DEBUG, "Locationing province=%s", province.string());

//    return false;
//}

//void KLDataProc::notifyLocationChange(kl::AreaItem *area)
//{

//}

void DataProc::notifyLocationFailed()
{

}

