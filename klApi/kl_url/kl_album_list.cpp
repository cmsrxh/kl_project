#include "events/common_log.h"
#include "net_util/net_common.h"
#include "config_local_info.h"
#include "kl_album_list.h"

/*
http://open.kaolafm.com/v2/album/list?cid=2085&sorttype=1&pagenum=1&pagesize=20&appid=cn5290&openid=cn52902019122410012497&deviceid=69acf5ec77a42531d1748b7dfc6dcdb6&os=linux&packagename=com.edog.car.linuxwhhj&sign=81230f5ae8db203704271b5c87cc6fbc&channel=linuxwhhj&
*/

/*
{"result":
{"haveNext":1,
"nextPage":2,
"havePre":0,
"prePage":1,
"currentPage":1,
"count":138,
"sumPage":7,
"pageSize":20,
"dataList":
[
{"id":1100002144762,"name":"逝者的歌声","img":"http://img.kaolafm.net/mz/images/201909/e9231a46-ef15-453f-a48e-18ce440e6abd/550_550.png","listenNum":315926,"source":1,"sourceName":"听伴","oldId":null},
{"id":1100002137915,"name":"让人年轻10岁的英文歌","img":"http://img.kaolafm.net/mz/images/201907/17cf3a73-b24c-438f-afff-59943cb890e5/550_550.jpg","listenNum":260162,"source":1,"sourceName":"听伴","oldId":null},
{"id":1100000043055,"name":"给80后的歌","img":"http://img.kaolafm.net/mz/images/201910/df4a2b33-0d6b-447c-93b3-5eaf1928c585/550_550.jpg","listenNum":20441462,"source":1,"sourceName":"听伴","oldId":null},
.....
*/
kl::AlbumList::AlbumList(int cid, int sorttype, int pagenum, int pagesize)
    : SaveObject<AlbumItem> ("http://open.kaolafm.com/v2/album/list", NetUrl::NET_HTTP_METHOD_GET)
    , mCID(ByteString::allocLong(cid))
    , mSortType(ByteString::allocLong(sorttype))
    , needPage(ByteString::allocLong(pagenum))
    , needPageSize(ByteString::allocLong(pagesize))
{
}

kl::AlbumList::AlbumList(const ByteString &cid, int sorttype, int pagenum, int pagesize)
    : SaveObject<AlbumItem> ("http://open.kaolafm.com/v2/album/list", NetUrl::NET_HTTP_METHOD_GET)
    , mCID(ByteString::allocString(cid))
    , mSortType(ByteString::allocLong(sorttype))
    , needPage(ByteString::allocLong(pagenum))
    , needPageSize(ByteString::allocLong(pagesize))
{
}

kl::AlbumList::~AlbumList()
{
    mCID.clear();
    mSortType.clear();
    needPage.clear();
    needPageSize.clear();
}

NetUrl &kl::AlbumList::genQueryUrl()
{
    mUrl.appendChange("cid", mCID);
    mUrl.appendChange("sorttype", mSortType);
    mUrl.appendChange("pagenum", needPage);
    mUrl.appendChange("pagesize", needPageSize);

    mUrl.appendChange("openid", LocalConfig::instance()->openID());
    mUrl.appendChange("sign", SIGN_AlbumList);

    return mUrl;
}

void kl::AlbumList::profile()
{
    GEN_Printf(LOG_DUMP, "haveNext=%d", haveNext);
    GEN_Printf(LOG_DUMP, "nextPage=%d", nextPage);
    GEN_Printf(LOG_DUMP, "havePre=%d", havePre);
    GEN_Printf(LOG_DUMP, "prePage=%d", prePage);
    GEN_Printf(LOG_DUMP, "page=%d", page);
    GEN_Printf(LOG_DUMP, "count=%d", count);
    GEN_Printf(LOG_DUMP, "totalPages=%d", totalPages);
    GEN_Printf(LOG_DUMP, "pageSize=%d", pageSize);

    ListTable<AlbumItem>::iterator it = mNodes.begin();

    for (; it != mNodes.end(); ++it)
    {
        GEN_Printf(LOG_DUMP, "%s=%s", it->name.string(), it->id.string());
    }
}

void kl::AlbumList::genResult(const char *data, unsigned long size)
{
    // GEN_Printf(LOG_DEBUG, "size: %lu\n%s", size, data);
    cJSON *root = cJSON_Parse((char *)data, size);
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
                AlbumItem tmp;

                JSON_VALUETRING_SWAP_BYTESTRING(item, id, tmp.id);
                JSON_VALUETRING_SWAP_BYTESTRING(item, name, tmp.name);
                JSON_VALUETRING_SWAP_BYTESTRING(item, img, tmp.img);
                JSON_VALUETRING_SWAP_BYTESTRING(item, listenNum, tmp.listenNum);
                JSON_VALUETRING_SWAP_BYTESTRING(item, source, tmp.source);
                JSON_VALUETRING_SWAP_BYTESTRING(item, sourceName, tmp.sourceName);

                mNodes.push_back(tmp);
            }
        }
        if (mNodes.empty())
        {
            GEN_Printf(LOG_WARN, "load AlbumList is empty.");
        } else
        {
            profile();
        }
    }else
    {
        GEN_Printf(LOG_ERROR, "priser failed, size: %lu\n%s", size, data);
    }

    cJSON_Delete(root);
}

bool kl::AlbumList::loadNextPage()
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
