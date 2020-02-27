#ifndef CURRENT_BACKUP_H
#define CURRENT_BACKUP_H

#include "model/kl_ui_data_union.h"
#include "kl_save_record.h"

class SaveCurrentPlayingList : public kl::SaveRecord
{
public:

};

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


private:
    CurrentBackup();

    SaveCurrentPlayingList mPlayList;
};

#endif // CURRENT_BACKUP_H
