#include "events/common_log.h"
#include "net_util/net_common.h"
#include "app_common.h"
#include "config_local_info.h"
#include "kl_broadcast_item_detail.h"

/*
 /v2/broadcast/detail?broadcastid=1600000000510&appid=cn5290&openid=cn52902019122410012497&deviceid=69acf5ec77a42531d1748b7dfc6dcdb6&os=linux&packagename=com.edog.car.linuxwhhj&sign=81230f5ae8db203704271b5c87cc6fbc&channel=linuxwhhj&
 /v2/broadcast/detail?broadcastid=1600000000510&appid=cn5290&openid=cn52902019122410012497&deviceid=69acf5ec77a42531d1748b7dfc6dcdb6&os=linux&packagename=com.edog.car.linuxwhhj&
sign=81230f5ae8db203704271b5c87cc6fbc&channel=linuxwhhj&

 http://open.kaolafm.com/v2/broadcast/detail?appid=cn5290&deviceid=4961c879191af84ef352e215642d569c&os=linux&packagename=com.edog.car.linuxwhhj&channel=linuxwhhj&broadcastid=1600000000747&openid=cn52902019122310006506&sign=81230f5ae8db203704271b5c87cc6fbc

{"result":{"broadcastId":1600000000747,"name":"广西旅游广播","img":"http://img.kaolafm.net/mz/images/201612/667cdc5f-9954-4047-be33-5a0e5357c5c8/default.jpg","classifyName":"省市台","isSubscribe":0,"playUrl":"http://play.c.l.kaolafm.net/ugc/1600000000747/playlist.m3u8","onLineNum":0,"likedNum":30,"status":1,"classifyId":2,"roomId":7899778,"freq":"FM104.0","icon":null,"areaCode":450000},"requestId":"cn52901577166214636900","serverTime":"1577166214642"}

*/
kl::BroadcastItemDetail::BroadcastItemDetail(ByteString const &broadcastId)
    : ItemObject<BDCastDetail>("http://open.kaolafm.com/v2/broadcast/detail", NetUrl::NET_HTTP_METHOD_GET)
    , mBroadcastId(broadcastId)
{
}

kl::BroadcastItemDetail::~BroadcastItemDetail()
{
    mItem.mBroadcastId.clear();
    mItem.mName.clear();
    mItem.mImg.clear();
    mItem.mClassifyName.clear();
    mItem.mPlayUrl.clear();
    mItem.mClassifyId.clear();
    mItem.mAreaCode.clear();
}

NetUrl &kl::BroadcastItemDetail::genQueryUrl()
{
    mUrl.appendChange("broadcastid", mBroadcastId);

    mUrl.appendChange("openid", LocalConfig::instance()->openID());
    mUrl.appendChange("sign", SIGN_BroadcastItemDetail);

    return mUrl;
}

void kl::BroadcastItemDetail::profile()
{
    GEN_Printf(LOG_DUMP, "name: %s", mItem.mName.string());
    GEN_Printf(LOG_DUMP, "img: %s", mItem.mImg.string());
    GEN_Printf(LOG_DUMP, "classifyName: %s", mItem.mClassifyName.string());
    GEN_Printf(LOG_DUMP, "playUrl: %s", mItem.mPlayUrl.string());
    GEN_Printf(LOG_DUMP, "classifyId: %s", mItem.mClassifyId.string());
    GEN_Printf(LOG_DUMP, "areaCode: %s", mItem.mAreaCode.string());
}

int kl::BroadcastItemDetail::genResult(NetBuffer *data)
{
    int ret = KL_DATA_PRISER_OK;
    cJSON *root = cJSON_Parse((char *)data->buffer(), data->size());
    cJSON *result = cJSON_GetObjectItem(root, "result");

    if (result)
    {
        JSON_VALUETRING_SWAP_BYTESTRING(result, broadcastId, mItem.mBroadcastId);
        JSON_VALUETRING_SWAP_BYTESTRING(result, name, mItem.mName);
        JSON_VALUETRING_SWAP_BYTESTRING(result, img, mItem.mImg);
        JSON_VALUETRING_SWAP_BYTESTRING(result, classifyName, mItem.mClassifyName);
        JSON_VALUETRING_SWAP_BYTESTRING(result, playUrl, mItem.mPlayUrl);
        JSON_VALUETRING_SWAP_BYTESTRING(result, classifyId, mItem.mClassifyId);
        JSON_VALUETRING_SWAP_BYTESTRING(result, areaCode, mItem.mAreaCode);

        profile();
    } else
    {
        GEN_Printf(LOG_ERROR, "Priser result failed.");
        ret = KL_DATA_PRISER_JSOC_ERROR;
    }

    cJSON_Delete(root);
    return ret;
}
