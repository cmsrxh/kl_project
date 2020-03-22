#ifndef CURRENT_BACKUP_H
#define CURRENT_BACKUP_H

#include "model/ui_data_union.h"

class ChipItemUnion;
/**
 * @brief The CurrentBackup class
 * @details 保存当前界面UI信息，包括当前界面所处的位置，
 * 以及当前播 放的信息（包含播放进度，歌曲ID，名字，描述等信息）
 */
class CurrentBackup
{
    CurrentBackup(CurrentBackup &);
    CurrentBackup &operator=(CurrentBackup &);
public:
    static CurrentBackup *instance()
    {
        static CurrentBackup i;
        return &i;
    }
    ~CurrentBackup();

    void saveCurrent();

    void recoveryCurrent();

    bool empty();

    void pushRecPlayInto(VectorTable<MusicChipItemUnion *> &);

    /**
     * @brief playPrevRecordInfo
     * @details 播放上一次播放记录的歌曲信息
     */
    void playPrevRecordInfo();

    ByteString getRecPlayId()
    {
        return mRecPlay.id;
    }
private:
    CurrentBackup();

    int             mCurPosition;
    ChipItemUnion  *m_pPrevRecChip;
    CollectNode     mRecPlay;

    char *genCurrentInfo();
    void  priserCurrentInfo(char *data, int len);
};

#endif // CURRENT_BACKUP_H
