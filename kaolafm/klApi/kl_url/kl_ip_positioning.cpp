#include "kl_ip_positioning.h"

#define Secret_key "6QgCuGQFVBJizbqTwoZ6eYYxab6ssJ"
#define QQ_API_KEY "EHGBZ-F7GKJ-4YEFC-FU3KK-7JHRK-B2BJM"


kl::IPPositioning::IPPositioning()
    : ItemObject<LocalPosition>("https://apis.map.qq.com/ws/location/v1/ip/", NetUrl::NET_HTTP_METHOD_GET)
{
}

NetUrl &kl::IPPositioning::genQueryUrl()
{
    mUrl.appendChange("key", QQ_API_KEY);
    return mUrl;
}

void kl::IPPositioning::profile()
{
    GEN_Printf(LOG_DUMP, "上网IP: %s", mItem.ip.string());
    GEN_Printf(LOG_DUMP, "国家: %s", mItem.nation.string());
    GEN_Printf(LOG_DUMP, "省: %s", mItem.province.string());
    GEN_Printf(LOG_DUMP, "市: %s", mItem.city.string());
    GEN_Printf(LOG_DUMP, "区: %s", mItem.district.string());
    GEN_Printf(LOG_DUMP, "行政区划代码: %d", mItem.adcode);
}

int kl::IPPositioning::genResult(NetBuffer *data)
{
    int ret = KL_DATA_PRISER_OK;
    GEN_Printf(LOG_DEBUG, "[%d]=%s", data->size(), data->buffer());
    /**
    cJSON *root = cJSON_Parse((char *)data->buffer(), data->size());
    cJSON *resultArray = cJSON_GetObjectItem(root, "result");

    if (resultArray)
    {
        for (cJSON *result = resultArray->child; NULL != result; result = result->next )
        {
            JSON_VALUETRING_SWAP_BYTESTRING(result, id, mItem.id);
            JSON_VALUETRING_SWAP_BYTESTRING(result, name, mItem.name);
            JSON_VALUETRING_SWAP_BYTESTRING(result, img, mItem.img);
            JSON_VALUETRING_SWAP_BYTESTRING(result, followedNum, mItem.followedNum);
            JSON_VALUETRING_SWAP_BYTESTRING(result, countNum, mItem.countNum);
            JSON_VALUETRING_SWAP_BYTESTRING(result, isOnline, mItem.isOnline);
            JSON_VALUETRING_SWAP_BYTESTRING(result, desc, mItem.desc);
            JSON_VALUETRING_SWAP_BYTESTRING(result, listenNum, mItem.listenNum);
            JSON_VALUETRING_SWAP_BYTESTRING(result, sortType, mItem.sortType);
            JSON_VALUETRING_SWAP_BYTESTRING(result, hasCopyright, mItem.hasCopyright);
            JSON_VALUETRING_SWAP_BYTESTRING(result, produce, mItem.produce);
            JSON_VALUETRING_SWAP_BYTESTRING(result, status, mItem.status);
            JSON_VALUETRING_SWAP_BYTESTRING(result, updateDay, mItem.copyrightLabel);
            JSON_VALUETRING_SWAP_BYTESTRING(result, commentNum, mItem.commentNum);
            JSON_VALUETRING_SWAP_BYTESTRING(result, lastCheckDate, mItem.lastCheckDate);
            JSON_VALUETRING_SWAP_BYTESTRING(result, type, mItem.type);
            JSON_VALUETRING_SWAP_BYTESTRING(result, isSubscribe, mItem.isSubscribe);


            profile();
        }
    } else
    {
        GEN_Printf(LOG_ERROR, "Priser result failed.");
        ret = KL_DATA_PRISER_JSOC_ERROR;
    }

    cJSON_Delete(root);

    */
    return ret;
}
