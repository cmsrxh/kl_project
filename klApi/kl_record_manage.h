#ifndef KL_RECORD_MANAGE_H
#define KL_RECORD_MANAGE_H

#include "kl_save_record.h"

namespace kl
{
class RecordManage : public SaveRecord
{
    RecordManage();
    RecordManage(RecordManage &);
    RecordManage &operator=(RecordManage &);
public:
    static RecordManage *instance()
    {
        static RecordManage i;
        return &i;
    }

    void addHistoryItem(RecordItem *item);

    void historyClear();
};
}

#endif // KL_RECORD_MANAGE_H
