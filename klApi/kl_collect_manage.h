#ifndef KL_COLLECT_MANAGE_H
#define KL_COLLECT_MANAGE_H

#include "kl_save_record.h"

namespace kl {
class CollectManage : public SaveRecord
{
    CollectManage();
    CollectManage(CollectManage &);
    CollectManage &operator=(CollectManage &);
public:
    static CollectManage *instance()
    {
        static CollectManage i;
        return &i;
    }

};
}

#endif // KL_COLLECT_MANAGE_H
