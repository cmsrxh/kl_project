#ifndef KL_ITEM_OBJECT_H
#define KL_ITEM_OBJECT_H

#include "qqmusic_object.h"
#include "util/list_table.h"

namespace qqmusic
{
template <typename item_node>
class ItemObject : public Object
{
public:
    ItemObject(const ByteString &baseUrl, int objectName, int methodType)
        : Object(baseUrl, objectName, methodType)
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
