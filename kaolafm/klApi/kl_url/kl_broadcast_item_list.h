#ifndef KL_BROADCAST_ITEM_LIST_H
#define KL_BROADCAST_ITEM_LIST_H

#include "kl_save_object.h"

namespace kl
{
class BroadcastItemList : public SaveObject<BDCastItem>
{
public:

    /**
     * @brief BroadcastItemList
     * @param type 对应 CateBCast->mType
     * @param classfyid 分类id 1 国家台 2 省市台 3 网络台, 对应 CateBCast->mId
     * @param code  城市code
     * @param page 页数 默认为1, 表示需要第几页
     * @param pagesize 每页数据条数 默认为50 最大50
     */
    BroadcastItemList(int type, int classfyid, int code = 0, int page = 1, int pagesize = 50);

    ~BroadcastItemList();

    NetUrl &genQueryUrl();

    void profile();

    int genResult(NetBuffer *data);

    int getHaveNext() const
    {
        return haveNext;
    }

    int getPage() const
    {
        return page;
    }

    bool loadNextPage();

private:
    ByteString type;
    ByteString classfyid;
    ByteString code;
    ByteString needPage;
    ByteString needPageSize;

    int page;
    int pageSize;
    int count;
    int totalPages;
    int haveNext;
    int nextPage;
    int havePre;
    int prePage;
};
}

#endif // KL_BROADCAST_ITEM_LIST_H
