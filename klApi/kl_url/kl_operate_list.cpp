#include "events/common_log.h"
#include "net_util/net_common.h"
#include "config_local_info.h"
#include "kl_operate_list.h"

/*
/v2/operate?
pagenum=1&pagesize=20&lon=&lat=&
appid=cn5290&openid=cn52902019122410012497&deviceid=69acf5ec77a42531d1748b7dfc6dcdb6&os=linux&packagename=com.edog.car.linuxwhhj&
sign=81230f5ae8db203704271b5c87cc6fbc&channel=linuxwhhj&

*/

/*
{"result":
{
"haveNext":1,
"nextPage":2,
"havePre":0,
"prePage":1,
"currentPage":1,
"count":56,
"sumPage":3,
"pageSize":20,
"dataList":[
{"rid":1200000000099,"rtype":3,"rname":"新闻电台","image":"http://img.kaolafm.net/mz/images/201906/527f6a28-f2ea-4dce-a9fe-10a807ffa94b/550_550.jpg","cornerMark":null,"rvalue":"1200000000099","listenNum":48604994,"desc":"有趣事、新鲜事，独家研发编排串播系统，人工筛选与大数据推荐无缝链接，让你听得更流畅更舒服，摆脱广播电台定时播放规定节目的束缚。\r\n","optType":1,"isOpt":0,"index":-1,"isMusic":0,"output":[],"category":null,"displayForm":0,"subrtype":null,"qqCategoryType":null},
{"rid":1100002146430,"rtype":0,"rname":"绝密档案","image":"http://img.kaolafm.net/mz/images/201910/babb05ce-30d1-4742-96d9-3d0cc46af017/550_550.jpg","cornerMark":null,"rvalue":"1100002146430","listenNum":613675154,"desc":"还原事实，探索真相。","optType":1,"isOpt":0,"index":-1,"isMusic":0,"output":[],"category":null,"displayForm":0,"subrtype":null,"qqCategoryType":null},
{"rid":1100002144762,"rtype":0,"rname":"他们再也没新歌","image":"http://img.kaolafm.net/mz/images/201909/e9231a46-ef15-453f-a48e-18ce440e6abd/550_550.png","cornerMark":null,"rvalue":"1100002144762","listenNum":315378,"desc":"我们再也听不到他们的新声。","optType":1,"isOpt":0,"index":-1,"isMusic":0,"output":[],"category":null,"displayForm":0,"subrtype":null,"qqCategoryType":null},
....
*/
kl::OperateList::OperateList(int pagenum, int pagesize)
    : SaveObject<Operate> ("http://open.kaolafm.com/v2/operate", NetUrl::NET_HTTP_METHOD_GET)
    , mPageNum(ByteString::allocLong(pagenum))
    , mPageSize(ByteString::allocLong(pagesize))
{
}

kl::OperateList::~OperateList()
{
    mPageNum.clear();
    mPageSize.clear();
}

NetUrl &kl::OperateList::genQueryUrl()
{
    mUrl.appendChange("pagenum", mPageNum);
    mUrl.appendChange("pagesize", mPageSize);
    mUrl.appendChange("lon", "");
    mUrl.appendChange("lat", "");

    mUrl.appendChange("openid", LocalConfig::instance()->openID());
    mUrl.appendChange("sign", SIGN_OperateList);

    return mUrl;
}

void kl::OperateList::profile()
{
    GEN_Printf(LOG_DUMP, "haveNext=%d", haveNext);
    GEN_Printf(LOG_DUMP, "nextPage=%d", nextPage);
    GEN_Printf(LOG_DUMP, "havePre=%d", havePre);
    GEN_Printf(LOG_DUMP, "prePage=%d", prePage);
    GEN_Printf(LOG_DUMP, "page=%d", page);
    GEN_Printf(LOG_DUMP, "count=%d", count);
    GEN_Printf(LOG_DUMP, "totalPages=%d", totalPages);
    GEN_Printf(LOG_DUMP, "pageSize=%d", pageSize);

    ListTable<Operate>::iterator it = mNodes.begin();

    for (; it != mNodes.end(); ++it)
    {
        GEN_Printf(LOG_DUMP, "%s=%s", it->rname.string(), it->rid.string());
    }
}

int kl::OperateList::genResult(NetBuffer *data)
{
    int ret = KL_DATA_PRISER_OK;
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
                Operate tmp;

                JSON_VALUETRING_SWAP_BYTESTRING(item, rid, tmp.rid);
                JSON_VALUETRING_SWAP_BYTESTRING(item, rtype, tmp.rtype);
                JSON_VALUETRING_SWAP_BYTESTRING(item, rname, tmp.rname);
                JSON_VALUETRING_SWAP_BYTESTRING(item, image, tmp.image);
                JSON_VALUETRING_SWAP_BYTESTRING(item, rvalue, tmp.rvalue);
                JSON_VALUETRING_SWAP_BYTESTRING(item, listenNum, tmp.listenNum);
                JSON_VALUETRING_SWAP_BYTESTRING(item, desc, tmp.desc);
                JSON_VALUETRING_SWAP_BYTESTRING(item, optType, tmp.optType);
                JSON_VALUETRING_SWAP_BYTESTRING(item, isMusic, tmp.isMusic);
                JSON_VALUETRING_SWAP_BYTESTRING(item, isOpt, tmp.isOpt);
                JSON_VALUETRING_SWAP_BYTESTRING(item, index, tmp.index);

                mNodes.push_back(tmp);
            }
        }
        if (mNodes.empty())
        {
            GEN_Printf(LOG_WARN, "load broadcast item list is empty.");
            ret = KL_DATA_PRISER_EMPTY;
        }/* else
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

bool kl::OperateList::loadNextPage()
{
    if (haveNext)
    {
        mPageNum.clear();
        mPageNum = ByteString::allocLong(nextPage);

        obtain();
        return true;
    } else
    {
        GEN_Printf(LOG_DEBUG, "Nothing next page.");
        return false;
    }
}
