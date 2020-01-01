#ifndef CHIP_ITEM_PLAY_MANAGE_H
#define CHIP_ITEM_PLAY_MANAGE_H

#include <QObject>

class ChipItemModel;
/**
 * @brief The ChipPlayManage class
 * @details 单实例模式
 */
class ChipPlayManage
{
public:
    ChipPlayManage();

    // 设置专辑信息audio列表的展示
    void setChipShow(ChipItemModel *chipShow);
    // 设置播放的列表展示
    void setPlayModel(ChipItemModel *playModel);

    void loadError(bool loadAction, int type, QString const &err_str);
    void dataLoadOver(long ptr, bool loadAction);

private:
    ChipItemModel *m_pChipShow;
    ChipItemModel *m_pPlayModel;
};

extern ChipPlayManage *gPlayInstance;

#endif // CHIP_ITEM_PLAY_MANAGE_H
