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

    int genResult(NetBuffer *data);

    /**
     * @brief checkFile
     * @details 核对文件是否过期, 过期就要重新下载
     */
    bool checkFile();
};
}

#endif // KL_CATEGORY_ALL_H
