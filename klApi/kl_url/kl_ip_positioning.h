#ifndef KL_IP_POSITIONING_H
#define KL_IP_POSITIONING_H

#include "kl_item_object.h"

class LocalPosition
{
public:
    void clear()
    {
        ip.clear();
        nation.clear();
        province.clear();
        city.clear();
        district.clear();
    }
    ByteString ip;        //上网IP

    ByteString nation;    //国家
    ByteString province;  //省
    ByteString city;      //市
    ByteString district;  //区，可能为空字串

    int        adcode;    //行政区划代码
};
namespace kl {
class IPPositioning : public ItemObject<LocalPosition>
{
public:
    IPPositioning();

    NetUrl &genQueryUrl();

    void profile();

    int genResult(NetBuffer *data);
};
}

#endif // KL_IP_POSITIONING_H
