#ifndef KL_BROADCAST_ITEM_DETAIL_H
#define KL_BROADCAST_ITEM_DETAIL_H

#include "kl_item_object.h"

namespace kl
{
class BroadcastItemDetail : public ItemObject<BDCastDetail>
{
public:
    /**
     * @brief BroadcastItemDetail
     * @param broadcastId [] 广播ID号码
     * @details 获取当前广播的详细信息
     */
    BroadcastItemDetail(ByteString const &mBroadcastId);
    ~BroadcastItemDetail();

    NetUrl &genQueryUrl();

    void profile();

    void genResult(const char *data, unsigned long size);

public:
    ByteString mBroadcastId;
};
}

#endif // KL_BROADCAST_ITEM_DETAIL_H
