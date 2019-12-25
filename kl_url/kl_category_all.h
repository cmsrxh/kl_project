#ifndef KL_CATEGORY_ALL_H
#define KL_CATEGORY_ALL_H

#include "kl_save_object.h"

namespace kl
{
class CategoryAll : public SaveObject<CateMain>
{
public:
    CategoryAll();

    NetUrl &genQueryUrl();

    void profile();

    void genResult(const char *data, unsigned long size);
};
}

#endif // KL_CATEGORY_ALL_H
