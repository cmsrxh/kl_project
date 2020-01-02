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

extern KLUIProc *gInstance;

KLDataProc::KLDataProc()
    : m_pPlayManage(new ChipPlayManage)
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
            gInstance->viewAlbumBDCSwitch("CateItemListView.qml");
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
    int type = atoi(vec[index]->type.string());

    GEN_Printf(LOG_DEBUG, "Load Type: %d, id=%s", type, id.string());
    mSwitch.setCateItemIndex(index);
    m_pChipItem->clear();

    DetailQobject::instance()->setDetailName(vec[index]->name);

    AlbunView &tmp = mChipMap[id];
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
        int type = atoi(vec[index]->type.string());

        m_pChipItemPlay->clear();
        AlbunView &tmp = mChipMap[id];
        if (tmp.chip_item)
        {
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
}

void KLDataProc::bdcThirdProgramLoadOver()
{
    GEN_Printf(LOG_DEBUG, "broadcast load over, play index=%d", mSwitch.bdc.bdc_item_index);

    Q_EMIT m_pBDCItem->currenBDCIndexChanged(mSwitch.bdc.bdc_item_index);

    VectorTable<MusicCateItemUnion *> &vec = m_pBDCItem->vec();

    if (ChipItemUnion::CHIP_ITEM_RADIO == mPlayPath.current_play_list->getChipType())
    {
        mPlayPath.chip_item_index = 0;
    } else
    {
        mPlayPath.chip_item_index = vec.size()-1;
    }

    if (mPlayPath.chip_item_index >= 0 && vec.size() > 0)
    {
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
    int index = mPlayPath.chip_item_index;

    if (m_pChipItemPlay->checkUnion(mPlayPath.current_play_list))
    {
        m_pChipItemPlay->clean();
        m_pChipItemPlay->setChipItemUnion(mPlayPath.current_play_list);
        GEN_Printf(LOG_DEBUG, "On Load Data");
        m_pChipItemPlay->onLoadOver((long)mPlayPath.current_play_list);
    }

    VectorTable<MusicChipItemUnion *> &vec = m_pChipItemPlay->vec();

    if (index < 0 || index >= vec.size())
    {
        GEN_Printf(LOG_ERROR, "Index=%d Out of range=%d, Can't show detail info.", index, vec.size());
        return;
    }

    Q_EMIT m_pChipItemPlay->playingIndexChanged(index);

    if ((mSwitch.current_play_list == mPlayPath.current_play_list)
             && (MEDIA_TYPE_ALBUMINFO == mPlayPath.media_type) )
    {
        Q_EMIT m_pChipItem->playingIndexChanged(index);
    }

    Q_EMIT gInstance->playingInfo(QStringFromByteString(vec[index]->name),
                                  QStringFromByteString(vec[index]->desc));

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
             && (MEDIA_TYPE_BROADCAST == mPlayPath.media_type) )
    {
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
        gInstance->setSourceUrl(info.playUrl.string());
    } else
    {
        GEN_Printf(LOG_WARN, "Play Prev index: %d was out of range.", index);
    }
}

bool KLDataProc::isBroadcastPlay()
{
    return ChipItemUnion::CHIP_ITEM_BDC_PROGRAM == mPlayPath.current_play_list->getChipType();
}
