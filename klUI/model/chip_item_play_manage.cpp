#include <events/common_log.h>
#include "chip_item_model.h"
#include "chip_item_union.h"
#include "kl_download_manage.h"
#include "kl_data_proc.h"
#include "chip_item_play_manage.h"

ChipPlayManage *gPlayInstance;

ChipPlayManage::ChipPlayManage()
{
    gPlayInstance = this;

    connect(this, SIGNAL(dataLoadOver(long,int)), this, SLOT(onDataLoadOver(long,int)));
}

void ChipPlayManage::setChipShow(ChipItemModel *chipShow)
{
    m_pChipShow = chipShow;
}

void ChipPlayManage::setPlayModel(ChipItemModel *playModel)
{
    m_pPlayModel = playModel;
}

void ChipPlayManage::loadError(int loadAction, int type, const QString &err_str)
{
    switch (loadAction) {
    case ChipItemUnion::LOAD_OVER_ALBUM_NORMAL_SHOW:
        Q_EMIT m_pChipShow->loadError(type, err_str);
        break;
    case ChipItemUnion::LOAD_OVER_ALBUM_IN_PLAYVIEW:
        // 正常加载的数据，显示在节目列表界面中，用于专辑当前正在播放的列表,发生在当前播放列表已经放完需要后台加载。
    case ChipItemUnion::LOAD_OVER_BDCPROGRAM_IN_PLAYVIEW:
        // 显示广播节目，在节目列表界面上, 然后要自动播放最新的电台节目
    case ChipItemUnion::LOAD_OVER_TYPE_RADIO__PLAY_CHOICE:
        // 加载智能电台，并播放当前选择的节目
    case ChipItemUnion::LOAD_OVER_BROADCAST__PLAY_CHOICE:
        // 加载传统电台，并播放当前选择的节目
    case ChipItemUnion::LOAD_OVER_ALBUM__PLAY_CHOICE:
        // 加载专辑，并播放当前选择的碎片
    default:
        Q_EMIT m_pPlayModel->loadError(type, err_str);
        break;
    }

}

void ChipPlayManage::onDataLoadOver(long ptr, int loadAction)
{
    GEN_Printf(LOG_DEBUG, "chip load over, loadAction=%d.", loadAction);

    switch (loadAction)
    {
    case ChipItemUnion::LOAD_OVER_ALBUM_NORMAL_SHOW:
        // 表示前台界面操作需要加载
        if (0 == m_pChipShow->size())
        {
            KLDataProc::instance()->enterAlbumView();
        }
        Q_EMIT m_pChipShow->itemCountChanged();
        m_pChipShow->chipLoadOver(ptr);
        break;
    case ChipItemUnion::LOAD_OVER_ALBUM_IN_PLAYVIEW:
        // 表示播放列表播放到最后，需要列表数据
        m_pPlayModel->chipLoadOver(ptr);
        break;
    case ChipItemUnion::LOAD_OVER_BDCPROGRAM_IN_PLAYVIEW:
        m_pPlayModel->clear();
        m_pPlayModel->chipLoadOver(ptr);
        KLDataProc::instance()->bdcProgramListAction();
        break;
    case ChipItemUnion::LOAD_OVER_TYPE_RADIO__PLAY_CHOICE:
        // 加载智能电台，并播放当前选择的节目
        m_pPlayModel->clear();
        m_pPlayModel->chipLoadOver(ptr);
        KLDataProc::instance()->playDefaultItem();
        break;
    case ChipItemUnion::LOAD_OVER_BROADCAST__PLAY_CHOICE:
        // 加载传统电台，并播放当前选择的节目
        m_pPlayModel->clear();
        m_pPlayModel->chipLoadOver(ptr);
        KLDataProc::instance()->playDefaultItem();
        break;
    case ChipItemUnion::LOAD_OVER_ALBUM__PLAY_CHOICE:
        // 加载专辑，并播放当前选择的碎片
        m_pPlayModel->clear();
        m_pPlayModel->chipLoadOver(ptr);
        KLDataProc::instance()->playDefaultItem();
        break;
    default:
        break;
    }
}

