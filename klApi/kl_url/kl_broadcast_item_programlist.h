#ifndef KL_BROADCAST_ITEM_PROGRAMLIST_H
#define KL_BROADCAST_ITEM_PROGRAMLIST_H

#include "kl_save_object.h"
//81230f5ae8db203704271b5c87cc6fbc
namespace kl
{
class BroadcastItemProgramlist : public SaveObject<BDCastProgramItem>
{
public:
    /**
     * @brief BroadcastItemProgramlist
     * @param bid [in] 广播ID号码
     * @details 根据ID号，获取当前广播的播放列表
     */
    BroadcastItemProgramlist(ByteString const &bid);
    ~BroadcastItemProgramlist();

    NetUrl &genQueryUrl();

    void profile();

    void genResult(NetBuffer *data);

private:
    ByteString mBID;
};
}

#endif // KL_BROADCAST_ITEM_PROGRAMLIST_H
