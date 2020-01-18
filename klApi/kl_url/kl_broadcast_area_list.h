#ifndef KL_BROADCAST_AREA_LIST_H
#define KL_BROADCAST_AREA_LIST_H

#include "kl_save_object.h"

namespace kl
{
class BroadcastAreaList : public SaveObject<AreaItem>
{
public:
    BroadcastAreaList();

    NetUrl &genQueryUrl();

    void profile();

    int genResult(NetBuffer *data);
};
}

#endif // KL_BROADCAST_AREA_LIST_H
