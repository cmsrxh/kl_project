#include "events/common_log.h"
#include "net_util/net_common.h"
#include "config_local_info.h"
#include "kl_chip_audio_list.h"

/*
http://open.kaolafm.com/v2/audio/list?aid=1100000048156&sorttype=1&pagesize=20&pagenum=1&appid=cn5290&openid=cn52902019122410012497&deviceid=69acf5ec77a42531d1748b7dfc6dcdb6&os=linux&packagename=com.edog.car.linuxwhhj&sign=81230f5ae8db203704271b5c87cc6fbc&channel=linuxwhhj&
*/
/*
{"result":
{
"haveNext":1,
"nextPage":2,
"havePre":0,
"prePage":1,
"currentPage":1,
"count":801,
"sumPage":801,
"pageSize":1,
"dataList":
[
{"audioId":1000025911439,"audioName":"肖战 - 沧海一声笑（《新笑傲江湖》手游主题曲）","audioPic":"http://img.kaolafm.net/mz/images/201912/dfce481f-5327-4722-a6b9-c391d9dca3a1/default.jpg","audioDes":"肖战演绎的《沧海一声笑》国韵版，由国内民乐顶尖工作室新乐府录制、知名民乐老师配乐。肖战的演唱，更让歌中带有几分江湖少年的不羁。\r\n作为《新笑傲江湖》手游的主题曲，歌曲也已在游戏中首发，不如来金庸经典的武侠世界中，伴随着悠扬的歌声和肖战一起闯荡江湖吧。","albumId":1100000048156,"albumName":"考拉华语新歌推荐","albumPic":"http://img.kaolafm.net/mz/images/201506/cdd592cc-017f-4e2f-828c-729d94d75e8a/default.jpg","orderNum":863,"mp3PlayUrl32":"http://image.kaolafm.net/mz/mp3_32/201912/7a2ccb3e-1518-4070-ab43-0d4dc6158e43.mp3","mp3PlayUrl64":"http://image.kaolafm.net/mz/mp3_64/201912/7a2ccb3e-1518-4070-ab43-0d4dc6158e43.mp3","aacPlayUrl":"http://image.kaolafm.net/mz/aac_32/201912/7a2ccb3e-1518-4070-ab43-0d4dc6158e43.aac","aacPlayUrl32":"http://image.kaolafm.net/mz/aac_32/201912/7a2ccb3e-1518-4070-ab43-0d4dc6158e43.aac","aacPlayUrl64":"http://image.kaolafm.net/mz/aac_64/201912/7a2ccb3e-1518-4070-ab43-0d4dc6158e43.aac","aacPlayUrl128":"http://image.kaolafm.net/mz/aac_128/201912/7a2ccb3e-1518-4070-ab43-0d4dc6158e43.aac","aacPlayUrl320":"http://image.kaolafm.net/mz/aac_320/201912/7a2ccb3e-1518-4070-ab43-0d4dc6158e43.aac","host":[],"aacFileSize":2200217,"mp3FileSize32":1060171,"mp3FileSize64":2119933,"updateTime":1577154288000,"clockId":"","duration":264960,"originalDuration":264986,"listenNum":3378,"likedNum":0,"hasCopyright":1,"commentNum":0,"trailerStart":0,"trailerEnd":0,"categoryId":141,"source":2,"isListened":0,"icon":null,"isThirdParty":0,"callBack":"","hasNextPage":1,"singer":"","contentType":0,"contentTypeName":"","mainTitleName":"","subheadName":"","launchType":null,"launchName":null}
]
},"requestId":"cn5290157718856774470","serverTime":"1577188567758"}
*/

kl::ChipAudioList::ChipAudioList(const ByteString &aid, int sorttype, int pagenum, int pagesize)
    : SaveObject<AudioItem> ("http://open.kaolafm.com/v2/audio/list", NetUrl::NET_HTTP_METHOD_GET)
    , mAudioId(ByteString::allocString(aid))
    , mSortType(ByteString::allocLong(sorttype))
    , needPage(ByteString::allocLong(pagenum))
    , needPageSize(ByteString::allocLong(pagesize))
{
}

kl::ChipAudioList::~ChipAudioList()
{
    mAudioId.clear();
    mSortType.clear();
    needPage.clear();
    needPageSize.clear();
}

NetUrl &kl::ChipAudioList::genQueryUrl()
{
    mUrl.appendChange("aid", mAudioId);
    mUrl.appendChange("sorttype", mSortType);
    mUrl.appendChange("pagesize", needPageSize);
    mUrl.appendChange("pagenum", needPage);

    mUrl.appendChange("openid", LocalConfig::instance()->openID());
    mUrl.appendChange("sign", SIGN_ChipAudioList);

    return mUrl;
}

void kl::ChipAudioList::profile()
{
    GEN_Printf(LOG_DUMP, "haveNext=%d", haveNext);
    GEN_Printf(LOG_DUMP, "nextPage=%d", nextPage);
    GEN_Printf(LOG_DUMP, "havePre=%d", havePre);
    GEN_Printf(LOG_DUMP, "prePage=%d", prePage);
    GEN_Printf(LOG_DUMP, "page=%d", page);
    GEN_Printf(LOG_DUMP, "count=%d", count);
    GEN_Printf(LOG_DUMP, "totalPages=%d", totalPages);
    GEN_Printf(LOG_DUMP, "pageSize=%d", pageSize);

    ListTable<AudioItem>::iterator it = mNodes.begin();

    for (; it != mNodes.end(); ++it)
    {
        GEN_Printf(LOG_DUMP, "%s=%s", it->audioName.string(), it->audioId.string());
    }
}

void kl::ChipAudioList::genResult(NetBuffer *data)
{
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
                RadioItem tmp;

                jsonGenAudioItem(tmp, item);

                mNodes.push_back(tmp);
            }
        }
        if (mNodes.empty())
        {
            GEN_Printf(LOG_WARN, "load ChipAudioList is empty.");
        } else
        {
            profile();
        }
    }else
    {
        GEN_Printf(LOG_ERROR, "priser failed, size: %lu\n%s", data->size(), data->buffer());
    }

    cJSON_Delete(root);
}

bool kl::ChipAudioList::loadNextPage()
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
