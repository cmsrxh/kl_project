#include "cate_item_union.h"
#include "cate_item_model.h"
#include "category_union.h"
#include "category_model.h"
#include "chip_item_model.h"
#include "chip_item_union.h"
#include "kl_data_proc.h"
#include "kl_ui_proc.h"
#include "qml_view_switch_stack.h"

extern KLUIProc *gInstance;

KLDataProc::KLDataProc()
{

}

void KLDataProc::init(CategoryModel *cate, CateItemModel *cateItem, ChipItemModel *chip)
{
    m_pCate     = cate;
    m_pCateItem = cateItem;
    m_pChipItem = chip;

    mMainCate   = new CategoryUnion(CategoryUnion::MAIN_CATE, cate);

    cate->setCateUnion(mMainCate);

    mMainCate->loadCategory(CategoryUnion::MAIN_CATE, 0);
}

void KLDataProc::cateTabClick(int index)
{
    ByteString cid = m_pCate->getCID(index);
    if (cid.empty())
    {
        qWarning() << "index is invalid" << index;
    } else
    {
        int cid_num = atoi(cid.string());

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
            tmp = new CateItemUnion(cid_num, m_pCateItem);

            m_pCateItem->setCateItemUnion(tmp);
            tmp->loadCateItem(cid_num);
        }
        mSwitch.setCateTabIndex(index);
    }
}

void KLDataProc::cateItemClick(int index)
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

    ChipItemUnion *&tmp = mChipMap[id];
    if (tmp)
    {
        m_pChipItem->setChipItemUnion(tmp);
        tmp->onLoadOver();
        m_pChipItem->resetAll();
        gInstance->pushNew("CateItemInfoView.qml");
    } else
    {
        tmp = new ChipItemUnion(type, m_pChipItem);
        m_pChipItem->setChipItemUnion(tmp);
        tmp->loadChipList(id);
    }
    mSwitch.setCurrentPlayList(tmp);
}

void KLDataProc::chipItemChick(int index)
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

/**
 * @brief KLDataProc::showPlayingInfo
 * @details 表示当前已经进行播放准备了
 */
void KLDataProc::showPlayingInfo()
{
    int index = mPlayPath.chip_item_index;
    VectorTable<MusicChipItemUnion *> &vec = m_pChipItem->vec();

    if (index < 0 || index >= vec.size())
    {
        GEN_Printf(LOG_ERROR, "Index=%d Out of range=%d, Can't show detail info.", index, vec.size());
        return;
    }

    if (mSwitch.current_play_list == mPlayPath.current_play_list)
    {
        Q_EMIT m_pChipItem->playingIndexChanged(index);
    } else
    {

    }
    Q_EMIT gInstance->playingInfo(vec[index]->name.string(),
                                  vec[index]->desc.string());
}

void KLDataProc::enterAlbumView()
{
    gInstance->pushNew("CateItemInfoView.qml");
}

int KLDataProc::getCateTabIndex()
{
    return mSwitch.cate_tab_index;
}

int KLDataProc::getCurChipIndex()
{
    if (mSwitch.current_play_list == mPlayPath.current_play_list)
    {
        return mPlayPath.chip_item_index;
    } else
    {
        return -1;
    }
}

void KLDataProc::playNext()
{
    if (!mPlayPath.current_play_list
            || -1 == mPlayPath.chip_item_index)
    {
        GEN_Printf(LOG_DEBUG, "invalid play.");
        return;
    }

    int index = ++mPlayPath.chip_item_index;
    MusicChipItemUnion info;

    if (mPlayPath.current_play_list->getUnionInfoByIndex(info, index))
    {
        // start play music
        gInstance->setSourceUrl(info.playUrl.string());
    } else
    {
        GEN_Printf(LOG_WARN, "Play Next index: %d was out of range.", index);
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

    int index = --mPlayPath.chip_item_index;
    MusicChipItemUnion info;

    if (mPlayPath.current_play_list->getUnionInfoByIndex(info, index))
    {
        // start play music
        gInstance->setSourceUrl(info.playUrl.string());
    } else
    {
        GEN_Printf(LOG_WARN, "Play Prev index: %d was out of range.", index);
    }
}
