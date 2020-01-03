#include <events/common_log.h>
#include "chip_item_model.h"
#include "chip_item_union.h"
#include "kl_data_proc.h"
#include "chip_item_play_manage.h"

ChipPlayManage *gPlayInstance;

ChipPlayManage::ChipPlayManage()
{
    gPlayInstance = this;

    connect(this, SIGNAL(programListOver()), this, SLOT(onBDCProgramListAction()), Qt::QueuedConnection);
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
    if (ChipItemUnion::LOAD_OVER_ALBUM_NORMAL_SHOW == loadAction)
    {
        Q_EMIT m_pChipShow->loadError(type, err_str);
    } else
    {
        Q_EMIT m_pPlayModel->loadError(type, err_str);
    }
}

void ChipPlayManage::dataLoadOver(long ptr, int loadAction)
{
    GEN_Printf(LOG_DEBUG, "chip load over, loadAction=%d.", loadAction);

    if (ChipItemUnion::LOAD_OVER_ALBUM_NORMAL_SHOW == loadAction)
    { // 表示前台界面操作需要加载
        if (0 == m_pChipShow->size())
        {
            KLDataProc::instance()->enterAlbumView();
        }
        Q_EMIT m_pChipShow->itemCountChanged();
        Q_EMIT m_pChipShow->dataLoadOver(ptr);
    } else if (ChipItemUnion::LOAD_OVER_ALBUM_IN_PLAYVIEW == loadAction)
    { // 表示播放列表播放到最后，需要列表数据
        Q_EMIT m_pPlayModel->dataLoadOver(ptr);
    } else if (ChipItemUnion::LOAD_OVER_BDCPROGRAM_IN_PLAYVIEW == loadAction)
    {
        Q_EMIT m_pPlayModel->dataLoadOver(ptr);
        Q_EMIT programListOver();
    }
}

void ChipPlayManage::onBDCProgramListAction()
{
    KLDataProc::instance()->bdcProgramListAction();
}
