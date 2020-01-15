#ifndef KL_CHIP_RADIO_LIST_H
#define KL_CHIP_RADIO_LIST_H

#include "kl_save_object.h"

namespace kl
{
class ChipRadioList : public SaveObject<RadioItem>
{
public:
    /**
     * @brief ChipRadioList
     * @param rid [in] 智能电台ID号
     */
    ChipRadioList(const ByteString &rid);
    ~ChipRadioList();

    NetUrl &genQueryUrl();

    void profile();

    void genResult(NetBuffer *data);

    bool loadNextPage();
private:
    ByteString mRID;
};
}

#endif // KL_CHIP_RADIO_LIST_H
