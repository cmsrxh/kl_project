#include "events/common_log.h"
#include "net_util/net_common.h"
#include "config_local_info.h"
#include "kl_category_broadcast.h"

/*
{"result":
[
{"id":1,"name":"国家台","type":1,"img":"http://pic.kaolafm.net/mz/images/201511/1f82c756-01d9-4880-84de-f0a1383258dd/%E5%9B%BD%E5%AE%B6%E5%8F%B0.jpg"},
{"id":2,"name":"省市台","type":1,"img":"http://pic.kaolafm.net/mz/images/201511/1f82c756-01d9-4880-84de-f0a1383258dd/%E7%9C%81%E5%B8%82%E5%8F%B0.jpg"},
{"id":3,"name":"网络台","type":1,"img":"http://pic.kaolafm.net/mz/images/201511/1f82c756-01d9-4880-84de-f0a1383258dd/%E7%BD%91%E7%BB%9C%E5%8F%B0.jpg"},
{"id":-1,"name":"智能台","type":-1,"img":"http://pic.kaolafm.net/mz/images/201511/1f82c756-01d9-4880-84de-f0a1383258dd/%E6%99%BA%E8%83%BD%E5%8F%B0.jpg"},
{"id":6,"name":"交通台","type":2,"img":"http://img.kaolafm.net/mz/images/201511/8c369f91-d025-4ccd-a557-a2cdac86c690/default.jpg"},
{"id":7,"name":"经济台","type":2,"img":"http://img.kaolafm.net/mz/images/201511/024c4c40-efc8-4b5b-be32-79cf15ce3d9f/default.jpg"},
{"id":8,"name":"新闻台","type":2,"img":"http://img.kaolafm.net/mz/images/201511/cc063657-0a06-4dd9-bc73-39c86aaf0abb/default.jpg"},
{"id":9,"name":"音乐台","type":2,"img":"http://img.kaolafm.net/mz/images/201511/5781a5b7-46e1-44e4-a918-5f533f14644e/default.jpg"},
{"id":10,"name":"校园台","type":2,"img":"http://img.kaolafm.net/mz/images/201511/37213b19-7888-4c5c-8f64-210d1ed80b2c/default.jpg"},
{"id":11,"name":"娱乐台","type":2,"img":"http://img.kaolafm.net/mz/images/201511/17d5f65d-09f1-43f3-8148-354f4ff90468/default.jpg"},
{"id":12,"name":"方言台","type":2,"img":"http://img.kaolafm.net/mz/images/201511/31f63343-ec4a-4688-a529-8ad7d51efb2b/default.jpg"},
{"id":13,"name":"曲艺台","type":2,"img":"http://img.kaolafm.net/mz/images/201511/f9a62426-65c1-4d2c-8d51-61951d9f4b17/default.jpg"},
{"id":14,"name":"外语台","type":2,"img":"http://img.kaolafm.net/mz/images/201511/fcd725ac-3941-42b8-96a5-be958064cecc/default.jpg"},
{"id":15,"name":"文艺台","type":2,"img":"http://img.kaolafm.net/mz/images/201511/7d61d97e-f0a1-416b-aab3-51272230e512/default.jpg"},
{"id":16,"name":"旅游台","type":2,"img":"http://img.kaolafm.net/mz/images/201511/66405d85-2aba-49a6-8319-2733f7044d17/default.jpg"},
{"id":17,"name":"体育台","type":2,"img":"http://img.kaolafm.net/mz/images/201511/54fc067f-ab07-4cd5-9287-75962605561c/default.jpg"},
{"id":18,"name":"生活台","type":2,"img":"http://img.kaolafm.net/mz/images/201511/99afcd9d-2282-4970-8811-74038fede699/default.jpg"},
{"id":19,"name":"都市台","type":2,"img":"http://img.kaolafm.net/mz/images/201511/6b2f3f01-b749-4882-8414-95e533d20674/default.jpg"},
{"id":20,"name":"综合台","type":2,"img":"http://img.kaolafm.net/mz/images/201511/7cfbe9bd-11f3-4792-9cf0-6b2797fd82cc/default.jpg"},
{"id":21,"name":"民族台","type":2,"img":"http://pic.kaolafm.net/mz/images/201511/1f82c756-01d9-4880-84de-f0a1383258dd/%E6%B0%91%E6%97%8F%E5%8F%B0.jpg"}
],
"requestId":"cn5290157709193886233",
"serverTime":"1577091938868"
}
*/
kl::CategoryBroadcast::CategoryBroadcast()
    : SaveObject<CateBCast> ("http://open.kaolafm.com/v2/category/broadcast", NetUrl::NET_HTTP_METHOD_GET)
{
    setSaveFile(NET_KL_BDC_CATE_LIST_FILE);
}

NetUrl &kl::CategoryBroadcast::genQueryUrl()
{
    // mUrl.appendChange("sign", genSign(mUrl));
    mUrl.appendChange("openid", LocalConfig::instance()->openID());

    mUrl.appendChange("sign", SIGN_CategoryBroadcast);

    return mUrl;
}

void kl::CategoryBroadcast::profile()
{
    ListTable<CateBCast>::iterator it = mNodes.begin();

    for (; it != mNodes.end(); ++it)
    {
        GEN_Printf(LOG_DUMP, "%s=%d, type=%d", it->name.string(), it->id, it->type);
    }
}

void kl::CategoryBroadcast::genResult(const char *data, unsigned long size)
{
    cJSON *root = cJSON_Parse((char *)data, size);
    cJSON *result = cJSON_GetObjectItem(root, "result");
    if (result)
    {

        for (cJSON *item = result->child; NULL != item; item = item->next )
        {
            CateBCast sub;

            JSON_VALUETRING_SWAP_BYTESTRING(item, id, sub.id);
            JSON_VALUETRING_SWAP_BYTESTRING(item, name, sub.name);
            JSON_VALUETRING_SWAP_BYTESTRING(item, type, sub.type);
            JSON_VALUETRING_SWAP_BYTESTRING(item, img, sub.img);

            mNodes.push_back(sub);
        }

        if (mNodes.empty())
        {
            GEN_Printf(LOG_WARN, "load category broadcast list is empty");
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
