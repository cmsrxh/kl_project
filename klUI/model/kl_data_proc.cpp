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
}

void KLDataProc::chipItemChick(int index)
{
    VectorTable<MusicChipItemUnion *> &vec = m_pChipItem->vec();

    if (index < 0 || index >= vec.size())
    {
        GEN_Printf(LOG_ERROR, "Index=%d Out of range=%d", index, vec.size());
        return;
    }

    if (mSwitch.chip_item_index != index)
    {
        mSwitch.setChipItemIndex(index);

        gInstance->setSourceUrl(vec[index]->playUrl.string());
    } else
    {
        GEN_Printf(LOG_DEBUG, "index: %d was setted.", index);
    }
}

void KLDataProc::showPlayingInfo()
{
    int index = mSwitch.chip_item_index;
    VectorTable<MusicChipItemUnion *> &vec = m_pChipItem->vec();

    if (index < 0 || index >= vec.size())
    {
        GEN_Printf(LOG_ERROR, "Index=%d Out of range=%d, Can't show detail info.", index, vec.size());
        return;
    }
    mPlayPath = mSwitch;
    m_pChipItem->setPlayingIndex(index);
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
