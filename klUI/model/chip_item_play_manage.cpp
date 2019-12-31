#include <events/common_log.h>
#include "chip_item_model.h"
#include "chip_item_union.h"
#include "kl_data_proc.h"
#include "chip_item_play_manage.h"

ChipPlayManage *gPlayInstance;

ChipPlayManage::ChipPlayManage()
{
    gPlayInstance = this;
}

void ChipPlayManage::setChipShow(ChipItemModel *chipShow)
{
    m_pChipShow = chipShow;
}

void ChipPlayManage::setPlayModel(ChipItemModel *playModel)
{
    m_pPlayModel = playModel;
}

void ChipPlayManage::loadError(int type, const QString &err_str)
{
    Q_EMIT m_pChipShow->loadError(type, err_str);
}

void ChipPlayManage::dataLoadOver(long ptr, bool loadAction)
{
    if (loadAction)
    { // 表示前台界面操作需要加载
        if (0 == m_pChipShow->size())
        {
            KLDataProc::instance()->enterAlbumView();
        }
        Q_EMIT m_pChipShow->itemCountChanged();
        Q_EMIT m_pChipShow->dataLoadOver(ptr);
    } else
    { // 表示播放列表播放到最后，需要列表数据
        Q_EMIT m_pPlayModel->dataLoadOver(ptr);
    }
}
