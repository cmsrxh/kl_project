#ifndef KL_CATEGORY_SUB_LIST_H
#define KL_CATEGORY_SUB_LIST_H

#include "kl_save_object.h"

namespace kl
{
class CategorySublist : public SaveObject<CateSub>
{
public:
    /**
     * @brief CategorySublist
     * @param cid [in] 分类id（不填输出应用所有分类）
     */
    CategorySublist(int cid);
    CategorySublist(ByteString const &cid);
    ~CategorySublist();

    NetUrl &genQueryUrl();

    void profile();

    void genResult(NetBuffer *data);

private:
    ByteString mCid;
};
}

#endif // KL_CATEGORY_SUB_LIST_H
