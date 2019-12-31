#include "events/common_log.h"
#include "net_util/net_common.h"
#include "config_local_info.h"
#include "kl_category_all.h"

/*
/v2/category?appid=cn5290&openid=cn52902019122410012497&deviceid=69acf5ec77a42531d1748b7dfc6dcdb6&os=linux&packagename=com.edog.car.linuxwhhj&
sign=81230f5ae8db203704271b5c87cc6fbc&channel=linuxwhhj&
*/
/*
{"result":
[
{"cid":-1,"name":"推荐","hasSub":0,"img":""},
{"cid":1332,"name":"头条","hasSub":1,"img":"http://img.kaolafm.net/mz/images/201906/035a404c-e8a8-4f50-9c1a-fd50ced5f2b3/default.jpg"},
....
*/
kl::CategoryAll::CategoryAll()
    : SaveObject<CateMain> ("http://open.kaolafm.com/v2/category", NetUrl::NET_HTTP_METHOD_GET)
{
    setSaveFile(NET_KL_CATE_ALL_LIST_FILE);
}

NetUrl &kl::CategoryAll::genQueryUrl()
{
    mUrl.appendChange("openid", LocalConfig::instance()->openID());
    mUrl.appendChange("sign", SIGN_CategoryAll);

    return mUrl;
}

void kl::CategoryAll::profile()
{
    ListTable<CateMain>::iterator it = mNodes.begin();

    for (; it != mNodes.end(); ++it)
    {
        GEN_Printf(LOG_DUMP, "%s=%s, hasSub=%s", it->name.string(), it->cid.string(), it->hasSub.string());
    }
}

void kl::CategoryAll::genResult(const char *data, unsigned long size)
{
//    GEN_Printf(LOG_DEBUG, "size: %lu\n%s", size, data);
    cJSON *root = cJSON_Parse((char *)data, size);
    cJSON *result = cJSON_GetObjectItem(root, "result");
    if (result)
    {
        for (cJSON *item = result->child; NULL != item; item = item->next )
        {
            CateMain tmp;

            JSON_VALUETRING_SWAP_BYTESTRING(item, cid, tmp.cid);
            JSON_VALUETRING_SWAP_BYTESTRING(item, name, tmp.name);
            JSON_VALUETRING_SWAP_BYTESTRING(item, hasSub, tmp.hasSub);
            JSON_VALUETRING_SWAP_BYTESTRING(item, img, tmp.img);

            mNodes.push_back(tmp);
        }

        if (mNodes.empty())
        {
            GEN_Printf(LOG_WARN, "load categoryall list is empty.");
        } else
        {
             profile();
        }
    } else
    {
        GEN_Printf(LOG_ERROR, "priser failed, size: %lu\n%s", size, data);
    }

    cJSON_Delete(root);
}
