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

void KLDataProc::init(CategoryModel *cate, CateItemModel *cateItem, ChipItemModel *chip, ChipItemModel *player)
{
    m_pCate         = cate;
    m_pCateItem     = cateItem;
    m_pChipItem     = chip;
    m_pChipItemPlay = player;

    mMainCate       = new CategoryUnion(CategoryUnion::MAIN_CATE, cate);

    m_pPlayManage->setChipShow(chip);
    m_pPlayManage->setPlayModel(player);

    cate->setCateUnion(mMainCate);

    mMainCate->loadCategory(CategoryUnion::MAIN_CATE, 0);
}

void KLDataProc::mainTabClick(int index)
{
    mSwitchAudio.setMainTabIndex(index);
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
        mSwitchAudio.setCateTabIndex(index);
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
    mSwitchAudio.setCateItemIndex(index);
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
    mSwitchAudio.setCurrentPlayList(tmp.chip_item);
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
    if (mSwitchAudio.current_play_list == mPlayPath.current_play_list)
    {
        clickValid = (mPlayPath.chip_item_index == index) ? false : true;
    }

    if (clickValid)
    {
        mSwitchAudio.setChipItemIndex(index);

        // start play music
        gInstance->setSourceUrl(vec[index]->playUrl.string());
        // 把原来的标签切换，赋值给当前
        mPlayPath = mSwitchAudio;
    } else
    {
        GEN_Printf(LOG_DEBUG, "index: %d was setted.", index);
    }
}

void KLDataProc::chipPlayItemClick(int index)
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

    if (mSwitchAudio.current_play_list == mPlayPath.current_play_list)
    {
        Q_EMIT m_pChipItem->playingIndexChanged(index);
    }

    Q_EMIT gInstance->playingInfo(QStringFromByteString(vec[index]->name),
                                  QStringFromByteString(vec[index]->desc));

    GEN_Printf(LOG_DEBUG, "Show Over, %d", vec.size());
}

void KLDataProc::enterAlbumView()
{
    gInstance->pushNew("CateItemInfoView.qml");
}

int KLDataProc::getCateTabIndex()
{
    return mSwitchAudio.cate_tab_index;
}

int KLDataProc::getCurChipIndex()
{
    if (mSwitchAudio.current_play_list == mPlayPath.current_play_list)
    {
        return mPlayPath.chip_item_index;
    } else
    {
        return -1;
    }
}

int KLDataProc::getCurPlayIndex()
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
        mPlayPath.current_play_list->loadNextPage(false);
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
