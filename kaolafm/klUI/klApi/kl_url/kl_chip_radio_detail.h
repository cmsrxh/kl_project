#ifndef KL_CHIP_RADIO_DETAIL_H
#define KL_CHIP_RADIO_DETAIL_H

#include "kl_item_object.h"

namespace kl
{
class ChipRadioDetail : public ItemObject<RadioDetail>
{
public:
    ChipRadioDetail(const ByteString &rid);

    NetUrl &genQueryUrl();

    void profile();

    int genResult(NetBuffer *data);

private:
    ByteString mRID;
};
}
#endif // KL_CHIP_RADIO_DETAIL_H
