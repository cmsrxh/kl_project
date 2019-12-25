#ifndef KL_TYPE_RADIO_LIST_H
#define KL_TYPE_RADIO_LIST_H

#include "kl_save_object.h"

namespace kl
{

class TypeRadioList : public SaveObject<TypeRadio>
{
public:
    /**
     * @brief TypeRadioList
     * @details 广播标签下面的智能电台标签
     */
    TypeRadioList(int cid = -1);
    ~TypeRadioList();

    NetUrl &genQueryUrl();

    void profile();

    void genResult(const char *data, unsigned long size);

private:
    ByteString mCID;
};
}
#endif // KL_TYPE_RADIO_LIST_H
