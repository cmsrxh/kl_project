#include "events/common_log.h"
#include "net_util/net_common.h"
#include "config_local_info.h"
#include "kl_chip_radio_detail.h"

/*
http://open.kaolafm.com/v2/radio/detail?rid=1200000000162&appid=cn5290&openid=cn52902019122410012497&deviceid=69acf5ec77a42531d1748b7dfc6dcdb6&os=linux&packagename=com.edog.car.linuxwhhj&sign=81230f5ae8db203704271b5c87cc6fbc&channel=linuxwhhj&

{"result":{"id":1200000000162,"name":"搞笑电台","img":"http://img.kaolafm.net/mz/images/201906/a8110b25-d3b3-4039-a365-fed555257851/default.jpg","followedNum":2473,"isOnline":1,"listenNum":18879628,"desc":"全球首家音频喜剧连播电台，7x24x365不间断播音， 笑话段子轮番轰炸；情景喜剧爆笑热辣；热点吐槽新奇诙谐；更有好玩的相声、脱口秀等海量内容。放松心情，伴随收听，搞笑FM不容错过。","commentNum":45,"isSubscribe":0,"type":3,"host":[],"keyWords":["脱口秀","笑话","段子"]},"requestId":"cn52901577190495852120","serverTime":"1577190495862"}

*/
kl::ChipRadioDetail::ChipRadioDetail(const ByteString &rid)
    : ItemObject<RadioDetail>("http://open.kaolafm.com/v2/radio/detail", NetUrl::NET_HTTP_METHOD_GET)
    , mRID(rid)
{
}

NetUrl &kl::ChipRadioDetail::genQueryUrl()
{
    mUrl.appendChange("rid", mRID);

    mUrl.appendChange("openid", LocalConfig::instance()->openID());
    mUrl.appendChange("sign", SIGN_ChipRadioDetail);

    return mUrl;
}

void kl::ChipRadioDetail::profile()
{
    GEN_Printf(LOG_DUMP, "id: %s", mItem.id.string());
    GEN_Printf(LOG_DUMP, "name: %s", mItem.name.string());
    GEN_Printf(LOG_DUMP, "img: %s", mItem.img.string());
    GEN_Printf(LOG_DUMP, "followedNum: %s", mItem.followedNum.string());
    GEN_Printf(LOG_DUMP, "isOnline: %s", mItem.isOnline.string());
    GEN_Printf(LOG_DUMP, "listenNum: %s", mItem.listenNum.string());
    GEN_Printf(LOG_DUMP, "desc: %s", mItem.desc.string());
    GEN_Printf(LOG_DUMP, "commentNum: %s", mItem.commentNum.string());
    GEN_Printf(LOG_DUMP, "isSubscribe: %s", mItem.isSubscribe.string());
    GEN_Printf(LOG_DUMP, "type: %s", mItem.type.string());
}

void kl::ChipRadioDetail::genResult(NetBuffer *data)
{
    cJSON *root = cJSON_Parse((char *)data->buffer(), data->size());
    cJSON *result = cJSON_GetObjectItem(root, "result");
    if (result)
    {
        jsonGenTypeRadio(mItem, result);
        profile();
    } else
    {
        GEN_Printf(LOG_ERROR, "priser failed, size: %lu\n%s", data->size(), data->buffer());
    }

    cJSON_Delete(root);
}
