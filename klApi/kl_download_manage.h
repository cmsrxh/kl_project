#ifndef KL_DOWNLOAD_MANAGE_H
#define KL_DOWNLOAD_MANAGE_H

#include "kl_save_record.h"

namespace kl
{
class MusicLoad;
class DownloadManage : public SaveRecord
{
    DownloadManage();
    DownloadManage(DownloadManage &);
    DownloadManage *operator=(DownloadManage &);
public:
    static DownloadManage *instance()
    {
        static DownloadManage i;
        return &i;
    }

    void saveItem(RecordItem *item);

private:
    ListTable<kl::MusicLoad *> mList;
};
}

#endif // KL_DOWNLOAD_MANAGE_H
