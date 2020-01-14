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
#include "kl_local_data_proc.h"
#include "kl_download_manage.h"
#include "iface/media_service_i_face.h"
#include "iface/media_iface_common.h"
#include "iface/media_service_call_back.h"
#include "application.h"
#include "../klIface/kl_service_notify.h"

extern KLUIProc *gInstance;

class MpvMsgPriser : public MediaServiceCallback
{
public:
    void msgLoopExit()
    {
        GEN_Printf(LOG_ERROR, "msg loop exit, need restart");
        sleep(2);
        Application::instance()->postCmd(SIG_SOCKET_CLIENT_MSG_EXIT);
    }

    void mediaNotify(int msg, int ext1, int ext2, const char *str)
    {
        //GEN_Printf(LOG_DUMP, "msg: %d (%d, %d), %s", msg, ext1, ext2, str);
        Q_EMIT gInstance->recvNotify(msg, ext1, ext2, QString::fromUtf8(str));
    }
};

KLDataProc::KLDataProc()
    : mCurrentIsCollect(false)
    , m_pPlayManage(new ChipPlayManage)
    , m_pHistoryChip(new ChipItemUnion(PLAY_CHIP_TYPE_LOCAL_LOAD))
{    
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

void KLDataProc::setPlayInfo(MusicChipItemUnion &chip)
{
    mPlayInfo.isLocal   = 0;
    mPlayInfo.type      = chip.type;
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

    MapTable<ByteString, AlbunView>::iterator it3 = mChipMap.begin();
    for (; it3 != mChipMap.begin(); ++it3)
    {
        it3->key.clear();

        if (it3->value.chip_item)
        {
            delete it3->value.chip_item;
        }
        if (it3->value.datail_item)
        {
            delete it3->value.datail_item;
        }
    }

    delete m_pCateData;
    delete m_pBDCTabData;
    delete m_pBDCAreaData;
    delete m_pPlayManage;
}

void KLDataProc::initSockService()
{
    KLServiceNotify::instance();
}

bool KLDataProc::initMedia()
{
    static MpvMsgPriser i;
    return MediaServiceIFace::instance()->initClientIface(&i);
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

void KLDataProc::mainTabClick(int index)
{
    mSwitch.setMainTabIndex(index);
}

void KLDataProc::albumFirstClick(int index)
{
    ByteString cid = m_pCate->getCID(index);
    if (cid.empty())
    {
        qWarning() << "index is invalid" << index;
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

        qDebug() << "cid = " << cid.string() << cid_num;

        m_pCateItem->clear();

        CateItemUnion *& tmp = mCidMap[cid_num];
        if (tmp)
        {
            qDebug() << "CateItemUnion created.";
            m_pCateItem->setCateItemUnion(tmp);
            tmp->onLoadOver(m_pCateItem);
            m_pCateItem->resetAll();
        } else
        {
            qDebug() << "CateItemUnion new Create.";
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

    ByteString id = vec[index]->id;
    int type = vec[index]->type;

    GEN_Printf(LOG_DEBUG, "Load Type: %d, id=%s", type, id.string());
    mSwitch.setCateItemIndex(index);
    m_pChipItem->clear();

    DetailQobject::instance()->setDetailName(vec[index]->name);

    AlbunView &tmp = mChipMap[ByteString::allocString(id)];
    if (tmp.chip_item)
    {
        m_pChipItem->setChipItemUnion(tmp.chip_item);
        tmp.chip_item->onLoadOver(m_pChipItem);
        m_pChipItem->resetAll();

        DetailQobject::instance()->setDetailUnion(tmp.datail_item);
        DetailQobject::instance()->getCurrent();

        this->enterAlbumView();        
    } else
    {
        qDebug() << "type=" << type << "id=" << id.string();
        tmp.chip_item = new ChipItemUnion(type);
        m_pChipItem->setChipItemUnion(tmp.chip_item);
        tmp.chip_item->loadChipList(id);

        tmp.datail_item = new DetailUnion(type);
        DetailQobject::instance()->setDetailUnion(tmp.datail_item);
        tmp.datail_item->loadDetail(id);
    }
    mSwitch.setCurrentPlayList(tmp.chip_item);
}

void KLDataProc::chipAudioThirdChick(int index)
{
    VectorTable<MusicChipItemUnion *> &vec = m_pChipItem->vec();

    if (index < 0 || index >= vec.size())
    {
        GEN_Printf(LOG_ERROR, "Index=%d Out of range=%d", index, vec.size());
        return;
    }
    bool clickValid = true;
    // 在同样的播放列表中点击, 表明点击的播放列表与当前播放列表是相等的
    if (mSwitch.current_play_list == mPlayPath.current_play_list)
    {
        clickValid = (mPlayPath.chip_item_index == index) ? false : true;
    }

    if (clickValid)
    {
        mSwitch.setChipItemIndex(index);

        // start play music
        setPlayInfo(*(vec[index]));
//        GEN_Printf(LOG_DEBUG, "++++++++++++++++++++++++++++++++++++++++");
//        LocalDataProc::instance()->recordCurrentPlayItem(&mPlayInfo);
        gInstance->setSourceUrl(vec[index]->playUrl.string());
        // 把原来的标签切换，赋值给当前
        mPlayPath = mSwitch;
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
        mPlayPath.setChipItemIndex(index);
        // start play music
        setPlayInfo(*(vec[index]));
        gInstance->setSourceUrl(vec[index]->playUrl.string());
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
            tmp->onLoadOver(m_pBDCItem);
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

        ByteString id = vec[index]->id;
        int type = vec[index]->type;

        AlbunView &tmp = mChipMap[ByteString::allocString(id)];
        if (tmp.chip_item)
        {
            m_pChipItemPlay->clear();
            m_pChipItemPlay->setChipItemUnion(tmp.chip_item);
            tmp.chip_item->onLoadOver(m_pChipItemPlay);
            m_pChipItemPlay->resetAll();
        } else
        {
            qDebug() << "type=" << type << "id=" << id.string();
            tmp.chip_item = new ChipItemUnion(type);
            m_pChipItemPlay->setChipItemUnion(tmp.chip_item);
            tmp.chip_item->loadChipList(id, true, ChipItemUnion::LOAD_OVER_BDCPROGRAM_IN_PLAYVIEW);

            tmp.datail_item = NULL;
        }

        mSwitch.setCurrentPlayList(tmp.chip_item);
        mSwitch.bdc.bdc_item_index = index;

        mPlayPath = mSwitch;

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

    Q_EMIT m_pBDCItem->currenBDCIndexChanged(mSwitch.bdc.bdc_item_index);

    VectorTable<MusicChipItemUnion *> &vec = m_pChipItemPlay->vec();

    if (PLAY_CHIP_TYPE_RADIO_CHIP == mPlayPath.current_play_list->getChipType())
    {
        mPlayPath.chip_item_index = 0;
    } else
    {
        mPlayPath.chip_item_index = vec.size()-1;
    }

    if (mPlayPath.chip_item_index >= 0 && vec.size() > 0)
    {
        GEN_Printf(LOG_DEBUG, "Play: %d-%d, url: %s", mPlayPath.chip_item_index, vec.size(), vec[mPlayPath.chip_item_index]->playUrl.string());
        setPlayInfo(*(vec[mPlayPath.chip_item_index]));
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
    int cur = 0, dur = 0;
    int index = mPlayPath.chip_item_index;

    if (m_pChipItemPlay->checkUnion(mPlayPath.current_play_list))
    {
        m_pChipItemPlay->clean();
        m_pChipItemPlay->setChipItemUnion(mPlayPath.current_play_list);
        GEN_Printf(LOG_DEBUG, "On Load Data");
        m_pChipItemPlay->chipLoadOver((long)mPlayPath.current_play_list);
    }

    VectorTable<MusicChipItemUnion *> &vec = m_pChipItemPlay->vec();

    if (index < 0 || index >= vec.size())
    {
        GEN_Printf(LOG_ERROR, "Index=%d Out of range=%d, Can't show detail info.", index, vec.size());
        return;
    }

    m_pChipItemPlay->getSliderBase(cur, dur, index);

    gInstance->setSliderBase(cur, dur);

    Q_EMIT m_pChipItemPlay->playingIndexChanged(index);

    if ((mSwitch.current_play_list == mPlayPath.current_play_list)
             && (MEDIA_TYPE_ALBUMINFO == mPlayPath.media_type) )
    {
        Q_EMIT m_pChipItem->playingIndexChanged(index);
    }

    Q_EMIT gInstance->playingInfo(QStringFromByteString(vec[index]->name),
                                  QStringFromByteString(vec[index]->desc));

    LocalDataProc::instance()->recordCurrentPlayItem(&mPlayInfo);

    GEN_Printf(LOG_DEBUG, "Show Over, %d", vec.size());
}

void KLDataProc::enterAlbumView()
{
    gInstance->pushNew("qrc:/CateItemInfoView.qml");
}

int KLDataProc::getAlbumSecondIndex()
{
    return mSwitch.cate_item_index;
}

int KLDataProc::getBDCSecondIndex()
{
    if ((mSwitch.current_play_list == mPlayPath.current_play_list)
            && (mSwitch.bdc.bdc_cate_tab_index == mPlayPath.bdc.bdc_cate_tab_index)
            && (MEDIA_TYPE_BROADCAST == mPlayPath.media_type) )
    {
        // GEN_Printf(LOG_WARN, "second section: %d", mPlayPath.bdc.bdc_item_index);
        return mPlayPath.bdc.bdc_item_index;
    } else
    {
        return -1;
    }
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
    if ((mSwitch.current_play_list == mPlayPath.current_play_list)
             && (MEDIA_TYPE_ALBUMINFO == mPlayPath.media_type) )
    {
        return mPlayPath.chip_item_index;
    } else
    {
        return -1;
    }
}

int KLDataProc::getPlayThirdIndex()
{
    return mPlayPath.chip_item_index;
}

void KLDataProc::playNext()
{
    if (!mPlayPath.current_play_list
            || -1 == mPlayPath.chip_item_index)
    {
        GEN_Printf(LOG_DEBUG, "invalid play.");
        return;
    }

    int index = 1 + mPlayPath.chip_item_index;
    MusicChipItemUnion info;

    if (mPlayPath.current_play_list->getUnionInfoByIndex(info, index))
    {
        // start play music
        mPlayPath.setChipItemIndex(index);
        setPlayInfo(info);
        gInstance->setSourceUrl(info.playUrl.string());
    } else
    {
        GEN_Printf(LOG_WARN, "Play Next index: %d was out of range: %d.", index, m_pChipItemPlay->size());
    }

    // 需要后台加载数据了
    if (index + 2 >= m_pChipItemPlay->size())
    {
        mPlayPath.current_play_list->loadNextPage(ChipItemUnion::LOAD_OVER_ALBUM_IN_PLAYVIEW);
    }
}

void KLDataProc::playPrev()
{
    if (!mPlayPath.current_play_list
            || -1 == mPlayPath.chip_item_index)
    {
        GEN_Printf(LOG_DEBUG, "invalid play.");
        return;
    }

    int index = mPlayPath.chip_item_index - 1;
    MusicChipItemUnion info;

    if (mPlayPath.current_play_list->getUnionInfoByIndex(info, index))
    {
        // start play music
        mPlayPath.setChipItemIndex(index);
        setPlayInfo(info);
        gInstance->setSourceUrl(info.playUrl.string());
    } else
    {
        GEN_Printf(LOG_WARN, "Play Prev index: %d was out of range.", index);
    }
}

void KLDataProc::currentIsCollect()
{
    if (mPlayPath.current_play_list)
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

void KLDataProc::localItemTypeRadioPlay(int local_type, int main_index, const ByteString &parentId, const ByteString &id)
{
    AlbunView &tmp = mChipMap[ByteString::allocString(parentId)];

    m_pChipItemPlay->setDefaultId(id);
    mSwitch.local.table_type = local_type;
    mSwitch.local.table_list_main_index = main_index;

    if (tmp.chip_item)
    {
        m_pChipItemPlay->clear();
        m_pChipItemPlay->setChipItemUnion(tmp.chip_item);
        tmp.chip_item->onLoadOver(m_pChipItemPlay);
        m_pChipItemPlay->resetAll();

        mSwitch.setCurrentPlayList(tmp.chip_item);
        mPlayPath = mSwitch;

        playDefaultItem();
    } else
    {
        tmp.chip_item = new ChipItemUnion(PLAY_CHIP_TYPE_TYPE_RADIO);
        m_pChipItemPlay->setChipItemUnion(tmp.chip_item);
        tmp.chip_item->loadChipList(parentId, true, ChipItemUnion::LOAD_OVER_TYPE_RADIO__PLAY_CHOICE);

        tmp.datail_item = NULL;
        mSwitch.setCurrentPlayList(tmp.chip_item);
        mPlayPath = mSwitch;
    }
}

void KLDataProc::localItemBroadcastPlay(int local_type, int main_index, const ByteString &parentId, const ByteString &id)
{
    AlbunView &tmp = mChipMap[ByteString::allocString(parentId)];

    m_pChipItemPlay->setDefaultId(id);
    mSwitch.local.table_type = local_type;
    mSwitch.local.table_list_main_index = main_index;

    if (tmp.chip_item)
    {
        m_pChipItemPlay->clear();
        m_pChipItemPlay->setChipItemUnion(tmp.chip_item);
        tmp.chip_item->onLoadOver(m_pChipItemPlay);
        m_pChipItemPlay->resetAll();

        mSwitch.setCurrentPlayList(tmp.chip_item);
        mPlayPath = mSwitch;

        playDefaultItem();
    } else
    {
        tmp.chip_item = new ChipItemUnion(PLAY_CHIP_TYPE_BROADCAST);
        m_pChipItemPlay->setChipItemUnion(tmp.chip_item);
        tmp.chip_item->loadChipList(parentId, true, ChipItemUnion::LOAD_OVER_BROADCAST__PLAY_CHOICE);

        tmp.datail_item = NULL;
        mSwitch.setCurrentPlayList(tmp.chip_item);
        mPlayPath = mSwitch;
    }
}

void KLDataProc::localItemAlbumPlay(int local_type, int main_index, const ByteString &parentId, const ByteString &id)
{
    AlbunView &tmp = mChipMap[ByteString::allocString(parentId)];

    m_pChipItemPlay->setDefaultId(id);
    mSwitch.local.table_type = local_type;
    mSwitch.local.table_list_main_index = main_index;

    if (tmp.chip_item)
    {
        m_pChipItemPlay->clear();
        m_pChipItemPlay->setChipItemUnion(tmp.chip_item);
        tmp.chip_item->onLoadOver(m_pChipItemPlay);
        m_pChipItemPlay->resetAll();
        mSwitch.setCurrentPlayList(tmp.chip_item);
        mPlayPath = mSwitch;

        playDefaultItem();
    } else
    {
        tmp.chip_item = new ChipItemUnion(PLAY_CHIP_TYPE_ALBUM);
        m_pChipItemPlay->setChipItemUnion(tmp.chip_item);
        tmp.chip_item->loadChipList(parentId, true, ChipItemUnion::LOAD_OVER_ALBUM__PLAY_CHOICE);

        tmp.datail_item = NULL;

        mSwitch.setCurrentPlayList(tmp.chip_item);
        mPlayPath = mSwitch;
    }
}

void KLDataProc::localItemDownLoadPlay(int local_type, int main_index)
{
    mSwitch.local.table_type = local_type;
    mSwitch.local.table_list_main_index = main_index;

    m_pHistoryChip->setChipHandler(kl::DownloadManage::instance());
    m_pChipItemPlay->clear();
    mSwitch.setCurrentPlayList(m_pHistoryChip);

    m_pChipItemPlay->resetAll();
    mPlayPath = mSwitch;

    VectorTable<MusicChipItemUnion *> &vec = m_pChipItemPlay->vec();
    if (main_index >= 0 && main_index < vec.size())
    {
        mPlayPath.chip_item_index = main_index;
        setPlayInfo(*(vec[main_index]));
        gInstance->setSourceUrl(vec[main_index]->playUrl.string());
    }
}

void KLDataProc::playDefaultItem()
{
    int i = 0;
    VectorTable<MusicChipItemUnion *> &vec = m_pChipItemPlay->vec();

    for (; i < vec.size(); ++i)
    {
        if (vec[i]->chipId == m_pChipItemPlay->getDefaultId())
        {
            break;
        }
    }

    if (i == vec.size())
    {
        GEN_Printf(LOG_WARN, "Current Default item not in list.");
        --i;
    }

    if (i >= 0 && i < vec.size())
    {
        mPlayPath.chip_item_index = i;
        setPlayInfo(*(vec[i]));
        gInstance->setSourceUrl(vec[i]->playUrl.string());
    }
}

