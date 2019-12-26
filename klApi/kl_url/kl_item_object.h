#ifndef KL_ITEM_OBJECT_H
#define KL_ITEM_OBJECT_H

#include "kl_object.h"
#include "kl_common.h"
#include "util/list_table.h"

namespace kl
{
template <typename item_node>
class ItemObject : public KLObject
{
public:
    ItemObject(const ByteString &baseUrl, int methodType)
        : KLObject(baseUrl, methodType)
    {}
    ~ItemObject()
    {
        clearData();
    }

    void clearData()
    {
        mItem.clear();
    }
    void loadData (uint8_t *data, unsigned long size)
    {
        data[size] = '\0';
        // GEN_Printf(LOG_INFO, "app init data: \n%s", (char *)data);

        genResult((char *)data, size);
    }

    virtual void genResult(const char */*data*/, unsigned long /*size*/) {}

    item_node &item()
    {
        return mItem;
    }
protected:
    item_node mItem;
};
}

#endif // KL_ITEM_OBJECT_H
