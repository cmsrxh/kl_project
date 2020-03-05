#ifndef QQ_IP_POSITIONING_H
#define QQ_IP_POSITIONING_H

#include "app_common.h"
#include "net_util/net_url.h"
#include "net_util/load_item.h"

class QQIPPositioning : public LoadItem
{
public:
    QQIPPositioning();

    bool obtain();

    void allDataObtain(NetBuffer *data);

    void errorInfo(int type, const char *str);

    void profile();

    void clear()
    {
        mIp.clear();
        mNation.clear();
        mProvince.clear();
        mCity.clear();
        mDistrict.clear();
    }
private:
    NetUrl     mUrl;

    ByteString mIp;        //上网IP

    ByteString mNation;    //国家
    ByteString mProvince;  //省
    ByteString mCity;      //市
    ByteString mDistrict;  //区，可能为空字串

    int        mAdcode;    //行政区划代码
};

#endif // QQ_IP_POSITIONING_H
