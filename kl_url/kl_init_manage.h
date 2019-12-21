#ifndef KL_INIT_MANAGE_H
#define KL_INIT_MANAGE_H

#include "kl_object.h"

namespace kl
{

class InitManage : public KLObject
{
public:
    InitManage();

    NetUrl &genQueryUrl();

    void loadData (uint8_t *data, unsigned long size);
};

}

#endif // KL_INIT_MANAGE_H
