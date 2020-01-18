#include "events/common_log.h"
#include "net_util/net_common.h"
#include "app_common.h"
#include "config_local_info.h"
#include "kl_broadcast_item_list.h"

/*
GET /v2/broadcast/list?

type=2&
classifyid=16&
pagenum=1&
pagesize=50&
area=0&

appid=cn5290&
openid=cn52902019122310006506&
deviceid=4961c879191af84ef352e215642d569c&
os=linux&packagename=com.edog.car.linuxwhhj&
sign=0f12b0800b55013e2e9e60cdbcc1a5cd&
channel=linuxwhhj& HTTP/1.1

 http://open.kaolafm.com/v2/broadcast/list?
appid=cn5290&
deviceid=4961c879191af84ef352e215642d569c&os=linux&packagename=com.edog.car.linuxwhhj&channel=linuxwhhj&
type=2&
area=0&
classifyid=16&
pagenum=1&
pagesize=50&
sign=0f12b0800b55013e2e9e60cdbcc1a5cd
*/

/*
{"result":
{"haveNext":0,
"nextPage":1,
"havePre":0,
"prePage":1,
"currentPage":1,
"count":14,
"sumPage":1,
"pageSize":30,
"dataList":
[
{"broadcastId":1600000000594,"name":"湖南旅游频道年代音乐台","img":"http://img.kaolafm.net/mz/images/201612/d8db844f-ddcf-4b0a-8d32-d86bcbd4fd24/default.jpg","currentProgramTitle":"","status":1,"onLineNum":18391,"likedNum":150,"isSubscribe":0,"playUrl":"http://play.c.l.kaolafm.net/ugc/1600000000594/playlist.m3u8","freq":"FM106.9"},
{"broadcastId":1600000000747,"name":"广西旅游广播","img":"http://img.kaolafm.net/mz/images/201612/667cdc5f-9954-4047-be33-5a0e5357c5c8/default.jpg","currentProgramTitle":"","status":1,"onLineNum":6379,"likedNum":30,"isSubscribe":0,"playUrl":"http://play.c.l.kaolafm.net/ugc/1600000000747/playlist.m3u8","freq":"FM104.0"},
...
*/

kl::BroadcastItemList::BroadcastItemList(int type, int classfyid, int code, int page, int pagesize)
    : SaveObject<BDCastItem> ("http://open.kaolafm.com/v2/broadcast/list", NetUrl::NET_HTTP_METHOD_GET)
    , type(ByteString::allocLong(type))
    , classfyid(ByteString::allocLong(classfyid))
    , code(ByteString::allocLong(code))
    , needPage(ByteString::allocLong(page))
    , needPageSize(ByteString::allocLong(pagesize))
{
//    GEN_Printf(LOG_DEBUG, "[%s] size=%d", this->code.string(), this->code.size());
}

kl::BroadcastItemList::~BroadcastItemList()
{
    type.clear();
    classfyid.clear();
    code.clear();
    needPage.clear();
    needPageSize.clear();
}

NetUrl &kl::BroadcastItemList::genQueryUrl()
{
    mUrl.appendChange("type", type);
    mUrl.appendChange("area", code);
    mUrl.appendChange("classifyid", classfyid);
    mUrl.appendChange("pagenum", needPage);
    mUrl.appendChange("pagesize", needPageSize);
    mUrl.appendChange("openid", LocalConfig::instance()->openID());
    mUrl.appendChange("sign", SIGN_BroadcastItemList);
    return mUrl;
}

void kl::BroadcastItemList::profile()
{
    GEN_Printf(LOG_DUMP, "haveNext=%d", haveNext);
    GEN_Printf(LOG_DUMP, "nextPage=%d", nextPage);
    GEN_Printf(LOG_DUMP, "havePre=%d", havePre);
    GEN_Printf(LOG_DUMP, "prePage=%d", prePage);
    GEN_Printf(LOG_DUMP, "page=%d", page);
    GEN_Printf(LOG_DUMP, "count=%d", count);
    GEN_Printf(LOG_DUMP, "totalPages=%d", totalPages);
    GEN_Printf(LOG_DUMP, "pageSize=%d", pageSize);
    ListTable<BDCastItem>::iterator it = mNodes.begin();

    for (; it != mNodes.end(); ++it)
    {
        GEN_Printf(LOG_DUMP, "%s=%s", it->name.string(), it->broadcastId.string());
    }
}

int kl::BroadcastItemList::genResult(NetBuffer *data)
{
    int ret = KL_DATA_PRISER_OK;
//    GEN_Printf(LOG_DEBUG, "%s", data);
    cJSON *root = cJSON_Parse((char *)data->buffer(), data->size());
    cJSON *result = cJSON_GetObjectItem(root, "result");
    if (result)
    {
        cJSON *obj;
        cJSON *dataList;

        haveNext    = (obj = cJSON_GetObjectItem(result, "haveNext")) ? atoi(obj->valuestring) : 0;
        nextPage    = (obj = cJSON_GetObjectItem(result, "nextPage")) ? atoi(obj->valuestring) : 0;
        havePre     = (obj = cJSON_GetObjectItem(result, "havePre")) ? atoi(obj->valuestring) : 0;
        prePage     = (obj = cJSON_GetObjectItem(result, "prePage")) ? atoi(obj->valuestring) : 0;
        page        = (obj = cJSON_GetObjectItem(result, "currentPage")) ? atoi(obj->valuestring) : 0;
        count       = (obj = cJSON_GetObjectItem(result, "count")) ? atoi(obj->valuestring) : 0;
        totalPages  = (obj = cJSON_GetObjectItem(result, "sumPage")) ? atoi(obj->valuestring) : 0;
        pageSize    = (obj = cJSON_GetObjectItem(result, "pageSize")) ? atoi(obj->valuestring) : 0;

        if ((dataList = cJSON_GetObjectItem(result, "dataList")))
        {
            for (cJSON *item = dataList->child; NULL != item; item = item->next )
            {
                BDCastItem bdc;

                JSON_VALUETRING_SWAP_BYTESTRING(item, broadcastId, bdc.broadcastId);
                JSON_VALUETRING_SWAP_BYTESTRING(item, playUrl, bdc.playUrl);
                JSON_VALUETRING_SWAP_BYTESTRING(item, name, bdc.name);
                JSON_VALUETRING_SWAP_BYTESTRING(item, img, bdc.image);
                JSON_VALUETRING_SWAP_BYTESTRING(item, currentProgramTitle, bdc.currentProgramTitle);
                JSON_VALUETRING_SWAP_BYTESTRING(item, status, bdc.status);
                JSON_VALUETRING_SWAP_BYTESTRING(item, onLineNum, bdc.onLineNum);
                JSON_VALUETRING_SWAP_BYTESTRING(item, likedNum, bdc.likedNum);
                JSON_VALUETRING_SWAP_BYTESTRING(item, freq, bdc.freq);
                JSON_VALUETRING_SWAP_BYTESTRING(item, isSubscribe, bdc.isSubscribe);

                mNodes.push_back(bdc);
            }
        }
        if (mNodes.empty())
        {
            GEN_Printf(LOG_WARN, "load broadcast item list is empty.");
            ret = KL_DATA_PRISER_EMPTY;
        } /*else
        {
            profile();
        }*/
    }else
    {
        GEN_Printf(LOG_ERROR, "priser failed, size: %lu\n%s", data->size(), data->buffer());
        ret = KL_DATA_PRISER_JSOC_ERROR;
    }

    cJSON_Delete(root);
    return ret;
}

bool kl::BroadcastItemList::loadNextPage()
{
    if (haveNext)
    {
        needPage.clear();
        needPage = ByteString::allocLong(nextPage);

        obtain();
        return true;
    } else
    {
        GEN_Printf(LOG_DEBUG, "Nothing next page.");
        return false;
    }
}
