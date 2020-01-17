#include "events/common_log.h"
#include "net_util/net_common.h"
#include "config_local_info.h"
#include "kl_category_sub_list.h"

/*
GET /v2/category/sublist?
cid=2085&
appid=cn5290&
openid=cn52902019122310006506&
deviceid=4961c879191af84ef352e215642d569c&
os=linux&
packagename=com.edog.car.linuxwhhj&
sign=0f12b0800b55013e2e9e60cdbcc1a5cd &
channel=linuxwhhj& HTTP/1.1

//
appid=cn5290&
deviceid=4961c879191af84ef352e215642d569c&
os=linux&
packagename=com.edog.car.linuxwhhj&
channel=linuxwhhj&
openid=cn52902019122310006506&
cid=2050&
sign=0f12b0800b55013e2e9e60cdbcc1a5cd
*/
/*
 {"result":
[
{"cid":2085,"name":"全部","hasSub":1,"img":""},
{"cid":1347,"name":"动感嗨曲","hasSub":0,"img":"http://img.kaolafm.net/mz/images/201906/5ca46cb3-77d3-4234-896a-22f17796187e/default.jpg"},
{"cid":1346,"name":"经典金曲","hasSub":0,"img":"http://img.kaolafm.net/mz/images/201906/d8734309-4d48-4269-9a99-2719530ff2fb/default.jpg"},
{"cid":1351,"name":"华语","hasSub":0,"img":"http://img.kaolafm.net/mz/images/201906/0d193bb4-39b6-4423-ba64-61b25206cc44/default.jpg"},
{"cid":1350,"name":"欧美","hasSub":0,"img":"http://img.kaolafm.net/mz/images/201906/d9960d6b-2fc7-431d-aa14-7665164a1cee/default.jpg"},
{"cid":1344,"name":"歌单","hasSub":0,"img":"http://img.kaolafm.net/mz/images/201906/4b35244b-d9cd-4acc-8865-6eed710c9ce1/default.jpg"},
{"cid":1345,"name":"音乐故事","hasSub":0,"img":"http://img.kaolafm.net/mz/images/201906/5b30de17-3163-4bee-b8c9-3cf96beee8dc/default.jpg"},
{"cid":1348,"name":"清新治愈","hasSub":0,"img":"http://img.kaolafm.net/mz/images/201906/f44ba5e0-f056-4609-b50b-eb4e340a18b4/default.jpg"},
{"cid":1349,"name":"轻音乐","hasSub":0,"img":"http://img.kaolafm.net/mz/images/201906/b0db9cf4-06bf-44b1-a613-a22561dc22b5/default.jpg"}
],"requestId":"cn52901577095816209571","serverTime":"1577095816230"}

*/
kl::CategorySublist::CategorySublist(int cid)
    : SaveObject<CateSub> ("http://open.kaolafm.com/v2/category/sublist", NetUrl::NET_HTTP_METHOD_GET)
    , mCid(ByteString::allocLong(cid))
{
}

kl::CategorySublist::CategorySublist(const ByteString &cid)
    : SaveObject<CateSub> ("http://open.kaolafm.com/v2/category/sublist", NetUrl::NET_HTTP_METHOD_GET)
    , mCid(ByteString::allocString(cid))
{
}

kl::CategorySublist::~CategorySublist()
{
    mCid.clear();
}

NetUrl &kl::CategorySublist::genQueryUrl()
{
    mUrl.appendChange("openid", LocalConfig::instance()->openID());
    mUrl.appendChange("cid", mCid);
    mUrl.appendChange("sign", SIGN_CategorySublist);

    return mUrl;
}

void kl::CategorySublist::profile()
{
    ListTable<CateSub>::iterator it = mNodes.begin();

    for (; it != mNodes.end(); ++it)
    {
        GEN_Printf(LOG_DUMP, "%s=%s", it->name.string(), it->cid.string());
    }
}

void kl::CategorySublist::genResult(NetBuffer *data)
{
    GEN_Printf(LOG_DEBUG, "size: %lu\n%s", data->size(), data->buffer());
    cJSON *root = cJSON_Parse((char *)data->buffer(), data->size());
    cJSON *result = cJSON_GetObjectItem(root, "result");
    if (result)
    {
        for (cJSON *item = result->child; NULL != item; item = item->next )
        {
            CateSub  sub;

            JSON_VALUETRING_SWAP_BYTESTRING(item, cid, sub.cid);
            JSON_VALUETRING_SWAP_BYTESTRING(item, name, sub.name);
            JSON_VALUETRING_SWAP_BYTESTRING(item, hasSub, sub.hasSub);
            JSON_VALUETRING_SWAP_BYTESTRING(item, img, sub.img);

            mNodes.push_back(sub);
        }

        if (mNodes.empty())
        {
            GEN_Printf(LOG_WARN, "load sub list is empty");
        }/* else
        {
            profile();
        }*/
    } else
    {
        GEN_Printf(LOG_ERROR, "priser failed, size: %lu\n%s", data->size(), data->buffer());
    }

    cJSON_Delete(root);
}

