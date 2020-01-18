#include "events/common_log.h"
#include "net_util/net_common.h"
#include "config_local_info.h"
#include "kl_album_detail.h"

/*
http://open.kaolafm.com/v2/album/detail?ids=1100000048156&appid=cn5290&openid=cn52902019122410012497&deviceid=69acf5ec77a42531d1748b7dfc6dcdb6&os=linux&packagename=com.edog.car.linuxwhhj&sign=81230f5ae8db203704271b5c87cc6fbc&channel=linuxwhhj&

{"result":[{"id":1100000048156,"name":"考拉华语新歌推荐","img":"http://img.kaolafm.net/mz/images/201506/cdd592cc-017f-4e2f-828c-729d94d75e8a/default.jpg","followedNum":179114,"countNum":801,"isOnline":1,"desc":"华语新歌及时听，分分钟追流行。网络潮流最新单曲，引领潮流最新华语新曲，不定期更新中。","listenNum":95006633,"sortType":-1,"hasCopyright":1,"host":[{"name":"尹小山","des":"","img":""}],"produce":"","status":"更新中","updateDay":"不定期更新","copyrightLabel":"","keyWords":["音乐","歌单","华语","新歌","流行"],"commentNum":62,"lastCheckDate":1577154288000,"type":0,"isSubscribe":0}],"serverTime":"1577238526247","requestId":"cn52901577238526240638"}

*/
kl::AlbumDetail::AlbumDetail(const ByteString &albumId)
    : ItemObject<AlbDetail>("http://open.kaolafm.com/v2/album/detail", NetUrl::NET_HTTP_METHOD_GET)
    , mAlbumId(ByteString::allocString(albumId))
{
}

kl::AlbumDetail::~AlbumDetail()
{
    mAlbumId.clear();
}

NetUrl &kl::AlbumDetail::genQueryUrl()
{
    mUrl.appendChange("ids", mAlbumId);

    mUrl.appendChange("openid", LocalConfig::instance()->openID());
    mUrl.appendChange("sign", SIGN_AlbumDetail);

    return mUrl;
}

void kl::AlbumDetail::profile()
{
    GEN_Printf(LOG_DUMP, "id: %s", mItem.id.string());
    GEN_Printf(LOG_DUMP, "name: %s", mItem.name.string());
    GEN_Printf(LOG_DUMP, "img: %s", mItem.img.string());
    GEN_Printf(LOG_DUMP, "followedNum: %s", mItem.followedNum.string());
    GEN_Printf(LOG_DUMP, "countNum: %s", mItem.countNum.string());
    GEN_Printf(LOG_DUMP, "isOnline: %s", mItem.isOnline.string());
    GEN_Printf(LOG_DUMP, "desc: %s", mItem.desc.string());
    GEN_Printf(LOG_DUMP, "listenNum: %s", mItem.listenNum.string());
    GEN_Printf(LOG_DUMP, "sortType: %s", mItem.sortType.string());
    GEN_Printf(LOG_DUMP, "hasCopyright: %s", mItem.hasCopyright.string());
    GEN_Printf(LOG_DUMP, "host: %s", mItem.host.string());
    GEN_Printf(LOG_DUMP, "produce: %s", mItem.produce.string());
    GEN_Printf(LOG_DUMP, "status: %s", mItem.status.string());
    GEN_Printf(LOG_DUMP, "updateDay: %s", mItem.updateDay.string());
    GEN_Printf(LOG_DUMP, "copyrightLabel: %s", mItem.copyrightLabel.string());
    GEN_Printf(LOG_DUMP, "commentNum: %s", mItem.commentNum.string());
    GEN_Printf(LOG_DUMP, "lastCheckDate: %s", mItem.lastCheckDate.string());
    GEN_Printf(LOG_DUMP, "type: %s", mItem.type.string());
    GEN_Printf(LOG_DUMP, "isSubscribe: %s", mItem.isSubscribe.string());
}

int kl::AlbumDetail::genResult(NetBuffer *data)
{
    int ret = KL_DATA_PRISER_OK;
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

            cJSON *keyWord = cJSON_GetObjectItem(result, "keyWords");
            if (keyWord)
            {
                for (cJSON *item = keyWord->child; NULL != item; item = item->next )
                {
                    mItem.keyWords.push_back(item->valuestring);
                    item->valuestring = NULL;
                }
            }

            cJSON *host = cJSON_GetObjectItem(result, "host");
            if (host)
            {
                for (cJSON *item = host->child; NULL != item; item = item->next )
                {
                    mItem.host = item->valuestring;
                    item->valuestring = NULL;
                    break;
                }
            }

            profile();
        }
    } else
    {
        GEN_Printf(LOG_ERROR, "Priser result failed.");
        ret = KL_DATA_PRISER_JSOC_ERROR;
    }

    cJSON_Delete(root);
    return ret;
}
