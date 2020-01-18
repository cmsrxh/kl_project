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
    int loadData (NetBuffer *data)
    {        
        return genResult(data);
    }

    virtual int genResult(NetBuffer */*data*/) { return KL_DATA_PRISER_OK; }

    item_node &item()
    {
        return mItem;
    }
protected:
    item_node mItem;
};
}

#endif // KL_ITEM_OBJECT_H
