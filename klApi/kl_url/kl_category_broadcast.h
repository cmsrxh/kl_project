#ifndef KL_CATEGORY_BROADCAST_H
#define KL_CATEGORY_BROADCAST_H

#include "kl_save_object.h"

namespace kl
{
class CategoryBroadcast : public SaveObject<CateBCast>
{
public:
    /* 国家台 省市台 ...  */
    CategoryBroadcast();

    NetUrl &genQueryUrl();

    void profile();

    void genResult(NetBuffer *data);
};
}

#endif // KL_CATEGORY_BROADCAST_H
