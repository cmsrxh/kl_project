#ifndef QQ_IP_POSITIONING_H
#define QQ_IP_POSITIONING_H

#include "net_util/load_item.h"

class QQIPPositioning : public LoadItem
{
public:
    explicit QQIPPositioning();
    ~QQIPPositioning();

    bool obtain();

    void allDataObtain(NetBuffer *data);

    void errorInfo(int type, const char *str);

    void profile();

    bool locationValid()
    {
        return 0 == mStatus.compare("0");
    }

    ByteString message() const
    {
        return mMessage;
    }

    ByteString city() const
    {
        return mCity;
    }

    ByteString province() const
    {
        return mProvince;
    }

    void clear()
    {
        mIp.clear();
        mNation.clear();
        mProvince.clear();
        mCity.clear();
        mDistrict.clear();
        mLat.clear();
        mLng.clear();
        mStatus.clear();
        mMessage.clear();
        mSignature.clear();
    }
private:
    NetUrl     mUrl;
    ByteString mSignature;

    ByteString mIp;        //上网IP

    ByteString mNation;    //国家
    ByteString mProvince;  //省
    ByteString mCity;      //市
    ByteString mDistrict;  //区，可能为空字串
    ByteString mAdcode;    //行政区划代码

    ByteString mLat;       // 纬度
    ByteString mLng;       // 经度

    ByteString mStatus;    // 获取状态（如果是0,表示正确）
    ByteString mMessage;   // 获取错误的信息
};

#endif // QQ_IP_POSITIONING_H
