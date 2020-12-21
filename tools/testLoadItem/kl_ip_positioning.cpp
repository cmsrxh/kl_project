#include <events/common_log.h>
#include <openssl/md5.h>
#include <application.h>
#include "kl_ip_positioning.h"

#define Secret_key "6QgCuGQFVBJizbqTwoZ6eYYxab6ssJ"
#define QQ_API_KEY "EHGBZ-F7GKJ-4YEFC-FU3KK-7JHRK-B2BJM"
#define STR_PATTERN "0123456789abcdef"

/*
WebServiceAPI（GET方法）签名计算

通用概念：
1. 请求路径：调用接口时的路径，如：/ws/geocoder/v1，末尾是否带 / 均可，不做要求
2. SecretKey (SK)：在腾讯位置服务控制台 > Key配置中，勾选WebServiceAPI的 SN校验时自动生成的随机字串，用于计算签名（sig）
3. sig：签名计算结果

通过以下示例讲解（本例为调用逆地址解析请求的url）:
https://apis.map.qq.com/ws/geocoder/v1?location=28.7033487,115.8660847&key=5Q5BZ-5EVWJ-SN5F3-K6QBZ-B3FAO-*****

GET请求分为：域名，请求路径和参数三个部分，用于签名计算的有：
请求路径：/ws/geocoder/v1
请求参数：location=28.7033487,115.8660847&key=5Q5BZ-5EVWJ-SN5F3-K6QBZ-B3FAO-*****

1. 首先对参数进行排序：按参数名升序（本例结果为key在前，location在后）：
key=5Q5BZ-5EVWJ-SN5F3-K6QBZ-B3FAO-RVBWM&location=28.7033487,115.8660847

2. 签名计算(sig)：
请求路径+”?”+请求参数+SK进行拼接，并计算拼接后字符串md5值，即为签名(sig)：
要求：请求参数必须是未进行任何编码（如urlencode）的原始数据
md5("/ws/geocoder/v1?key=5Q5BZ-5EVWJ-SN5F3-*****&location=28.7033487,115.8660847SWvT26ypwq5Nwb5RvS8cLi6NSoH8HlJX")
本例计算得到结果为：90da272bfa19122547298e2b0bcc0e50


3. 生成最终请求：将计算得到的签名sig，放到请求中（参数名即为：sig）：
https://apis.map.qq.com/ws/geocoder/v1?key=5Q5BZ-5EVWJ-SN5F3-K6QBZ-B3FAO-*****&location=28.7033487,115.8660847&sig=90da272bfa19122547298e2b0bcc0e50
注意：计算 sig 要使用原始参数值，不要进行任何编码，但最终发送时的参数，是需要时行url编码的
*/
kl::IPPositioning::IPPositioning()
    : mUrl("https://apis.map.qq.com/ws/location/v1/ip/", NetUrl::NET_HTTP_METHOD_GET)
{
    unsigned char sign_bytes[16];
    char sign[33];

    mUrl.append("key", QQ_API_KEY);
    ByteString url = ByteString::allocString(mUrl.genUrl());

    ByteString gen = url + Secret_key;

    char *p = gen.string();
    while(*p)
    {
        if(*p == '/' && (( '0' <= *(p-1) && *(p-1) <= '9' )
                         || ( 'a' <= *(p-1) && *(p-1) <= 'z' )
                         || ( 'A' <= *(p-1) && *(p-1) <= 'Z' )) ) break;
        ++p;
    }

    MD5((unsigned char *)p, strlen(p), sign_bytes);

    for (int i = 0; i < 16; ++i)
    {
        unsigned char c = sign_bytes[i];
        sign[i * 2]     = STR_PATTERN[(c >> 4) & 0xF];
        sign[i * 2 + 1] = STR_PATTERN[(c)  & 0xF];
    }
    sign[32] = '\0';
    mSignature = ByteString::allocString(ByteString(sign, 32));

    mUrl.append("sig", mSignature);
}

kl::IPPositioning::~IPPositioning()
{
    clear();
}

bool kl::IPPositioning::obtain()
{
    return LoadItem::obtain(mUrl);
}

void kl::IPPositioning::allDataObtain(NetBuffer *data)
{
    GEN_Printf(LOG_DEBUG, "[%d]=%s", data->size(), data->buffer());
    //printf("%s\n", bytes);
    cJSON *result, *ad_info, *location;
    cJSON *root = cJSON_Parse((char *)data->buffer(), data->size());

    if (UNLIKELY(root == NULL))
    {
        GEN_Printf(LOG_ERROR, "cJSON_Parse error");
        goto end;
    }

    JSON_VALUETRING_SWAP_BYTESTRING(root, status, mStatus);
    JSON_VALUETRING_SWAP_BYTESTRING(root, message, mMessage);

    result = cJSON_GetObjectItem(root, "result");
    if(UNLIKELY(NULL == result))
    {
        GEN_Printf(LOG_WARN, "Result is empty.");
        goto end;
    }

    JSON_VALUETRING_SWAP_BYTESTRING(result, ip, mIp);
    location = cJSON_GetObjectItem(result, "location");
    if (LIKELY(location))
    {
        JSON_VALUETRING_SWAP_BYTESTRING(location, lat, mLat);
        JSON_VALUETRING_SWAP_BYTESTRING(location, lng, mLng);
    }

    ad_info = cJSON_GetObjectItem(result, "ad_info");
    if (LIKELY(ad_info))
    {
        JSON_VALUETRING_SWAP_BYTESTRING(ad_info, adcode, mAdcode);
        JSON_VALUETRING_SWAP_BYTESTRING(ad_info, nation, mNation);
        JSON_VALUETRING_SWAP_BYTESTRING(ad_info, province, mProvince);
        JSON_VALUETRING_SWAP_BYTESTRING(ad_info, city, mCity);
        JSON_VALUETRING_SWAP_BYTESTRING(ad_info, district, mDistrict);
    }

end:
    cJSON_Delete(root);

//    Application::instance()->postKlEvent(SIG_LOCATION_POSITIONING, (long)this);
}

void kl::IPPositioning::errorInfo(int type, const char *str)
{
    GEN_Printf(LOG_ERROR, "type=%d, error: %s", type, str);
//    Application::instance()->postKlEvent(SIG_LOCATION_POSITIONING, (long)this);
}

void kl::IPPositioning::profile()
{
    GEN_Printf(LOG_DUMP, "上网IP: %s", mIp.string());
    GEN_Printf(LOG_DUMP, "国家: %s", mNation.string());
    GEN_Printf(LOG_DUMP, "省: %s", mProvince.string());
    GEN_Printf(LOG_DUMP, "市: %s", mCity.string());
    GEN_Printf(LOG_DUMP, "区: %s", mDistrict.string());
    GEN_Printf(LOG_DUMP, "行政区划代码: %s", mAdcode.string());
    GEN_Printf(LOG_DUMP, "经纬度: (%s, %s)", mLat.string(), mLng.string());
    GEN_Printf(LOG_DUMP, "状态: %s", mStatus.string());
    GEN_Printf(LOG_DUMP, "信息: %s", mMessage.string());
}
