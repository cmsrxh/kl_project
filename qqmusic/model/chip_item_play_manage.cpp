#include <events/common_log.h>
#include "chip_item_model.h"
#include "chip_item_union.h"
#include "data_proc.h"
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

void ChipPlayManage::onDataLoadOver(long ptr, int loadAction)
{
    GEN_Printf(LOG_DEBUG, "chip load over, loadAction=%d.", loadAction);

    switch (loadAction)
    {
    case ChipItemUnion::LOAD_OVER_ALBUM_NORMAL_SHOW:
        // 表示前台界面操作需要加载
        if (0 == m_pChipShow->size())
        {
            DataProc::instance()->enterAlbumView();
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
        DataProc::instance()->bdcProgramListAction();
        break;    
    case ChipItemUnion::LOAD_OVER_BACK_PLAY_OP:
        DataProc::instance()->playDefaultItem(reinterpret_cast<ChipItemUnion *>(ptr));
        break;
    default:
        break;
    }
}

