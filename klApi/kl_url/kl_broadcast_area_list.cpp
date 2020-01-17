#include "events/common_log.h"
#include "net_util/net_common.h"
#include "app_common.h"
#include "config_local_info.h"
#include "kl_broadcast_area_list.h"

/*
GET /v2/broadcast/arealist?
appid=cn5290&openid=cn52902019122310006506&
deviceid=4961c879191af84ef352e215642d569c&
os=linux&
packagename=com.edog.car.linuxwhhj&
sign=0f12b0800b55013e2e9e60cdbcc1a5cd&
channel=linuxwhhj& HTTP/1.1

 /v2/broadcast/arealist?
appid=cn5290&
deviceid=4961c879191af84ef352e215642d569c&
os=linux&
packagename=com.edog.car.linuxwhhj&
channel=linuxwhhj&
sign=0f12b0800b55013e2e9e60cdbcc1a5cd
*/
/*
{"result":[{"name":"北京","id":1},{"name":"重庆","id":20},{"name":"上海","id":28},{"name":"天津","id":2},{"name":"安徽","id":12},{"name":"福建","id":25},{"name":"广东","id":26},{"name":"甘肃","id":17},{"name":"广西","id":27},{"name":"贵州","id":23},{"name":"河北","id":7},{"name":"湖北","id":21},{"name":"河南","id":8},{"name":"湖南","id":22},{"name":"海南","id":31},{"name":"黑龙江","id":6},{"name":"吉林","id":3},{"name":"江苏","id":13},{"name":"江西","id":11},{"name":"辽宁","id":29},{"name":"内蒙","id":4},{"name":"宁夏","id":18},{"name":"青海","id":30},{"name":"四川","id":19},{"name":"山东","id":10},{"name":"山西","id":9},{"name":"陕西","id":5},{"name":"云南","id":24},{"name":"浙江","id":15},{"name":"新疆","id":14},{"name":"西藏","id":16}],"requestId":"cn52901577087045987280","serverTime":"1577087045991"}
*/
kl::BroadcastAreaList::BroadcastAreaList()
    : SaveObject<AreaItem> ("http://open.kaolafm.com/v2/broadcast/arealist", NetUrl::NET_HTTP_METHOD_GET)
{
    setSaveFile(NET_KL_BDC_AREA_LIST_FILE);
}

NetUrl &kl::BroadcastAreaList::genQueryUrl()
{
    mUrl.appendChange("openid", LocalConfig::instance()->openID());
    mUrl.appendChange("sign", SIGN_BroadcastAreaList);

    return mUrl;
}

void kl::BroadcastAreaList::profile()
{
    ListTable<AreaItem>::iterator it = mNodes.begin();

    for (; it != mNodes.end(); ++it)
    {
        GEN_Printf(LOG_DUMP, "%s=%s", it->name.string(), it->id.string());
    }
}

void kl::BroadcastAreaList::genResult(NetBuffer *data)
{
    cJSON *root = cJSON_Parse((char *)data->buffer(), data->size());
    cJSON *result = cJSON_GetObjectItem(root, "result");
    if (result)
    {
        for (cJSON *item = result->child; NULL != item; item = item->next )
        {           
            AreaItem tmp;

            JSON_VALUETRING_SWAP_BYTESTRING(item, name, tmp.name);
            JSON_VALUETRING_SWAP_BYTESTRING(item, id, tmp.id);

            mNodes.push_back(tmp);
        }
        if (mNodes.empty())
        {
            GEN_Printf(LOG_WARN, "load broadcast area list is empty.");
        }/* else
        {
            profile();
        }*/
    }else
    {
        GEN_Printf(LOG_ERROR, "priser failed, size: %lu\n%s", data->size(), data->buffer());
    }

    cJSON_Delete(root);
}


