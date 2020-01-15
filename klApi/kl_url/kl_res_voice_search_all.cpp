#include "events/common_log.h"
#include "net_util/net_common.h"
#include "net_util/http_util.h"
#include "config_local_info.h"
#include "kl_res_voice_search_all.h"

/*
http://open.kaolafm.com/v2/resource/voice/searchall?deviceid=69acf5ec77a42531d1748b7dfc6dcdb6&sign=955d3a8e24056e71297c51bb5cf98381&appid=cn5290&voicesource=1&appsource=1&qualitytype=1&q=%7B%22category%22%3A%22%22%2C%22albumName%22%3A%22%22%2C%22artist%22%3A%22%22%2C%22audioName%22%3A%22%22%2C%22field%22%3A2%2C%22tag%22%3A1%2C%22text%22%3A%22%E9%9F%B3%E4%B9%90%22%2C%22keyword%22%3A%22%E9%9F%B3%E4%B9%90%22%7D&appid=cn5290&openid=cn52902019122410012497&deviceid=69acf5ec77a42531d1748b7dfc6dcdb6&os=linux&packagename=com.edog.car.linuxwhhj&sign=81230f5ae8db203704271b5c87cc6fbc&channel=linuxwhhj&

{"result":
{"playType":1,"delayTime":0,"playIndex":0,"dataList":
[
{"id":1100000089210,"name":"音乐","img":"http://img.kaolafm.net/mz/images/201707/0993b5f5-0f6b-49e6-a27f-af751e6be123/default.jpg","type":0,"albumName":"音乐","source":2,"duration":0,"playUrl":"","oldId":0,"sourceName":"考拉主播","callback":"0_analyzer_01_1578125434590140","host":[],"listenNum":34490,"isShowRed":0,"isRequest":0,"originalDuration":0},
{"id":1000024878847,"name":"音乐","img":"http://img.kaolafm.net/mz/images/201906/813acf2c-9c92-477a-8821-842d2528fafc/default.jpg","type":1,"albumName":"莫扎特钢琴曲精选","source":1,"duration":24204,"playUrl":"http://image.kaolafm.net/mz/aac_64/201906/7ac5f3586edd57ecb607660e1a16c9e0.aac","oldId":0,"sourceName":"考拉喜马拉雅","callback":"1_analyzer_01_1578125434590331","host":[],"listenNum":180018,"isShowRed":1,"isRequest":1,"originalDuration":24139},
{"id":1100002130062,"name":"《夜色钢琴曲》","img":"http://img.kaolafm.net/mz/images/201905/2b3cd91c-5ea2-47e1-8cd5-a43736c044ae/default.jpg","type":0,"albumName":"《夜色钢琴曲》","source":1,"duration":0,"playUrl":"","oldId":291718,"sourceName":"考拉喜马拉雅","callback":"2_analyzer_01_1578125434590285","host":[],"listenNum":468520636,"isShowRed":0,"isRequest":0,"originalDuration":0},
{"id":1100002136232,"name":"李峙的不老歌","img":"http://img.kaolafm.net/mz/images/201906/e66b46d3-144c-4907-875c-f49b3a445533/default.jpg","type":0,"albumName":"李峙的不老歌","source":1,"duration":0,"playUrl":"","oldId":236268,"sourceName":"考拉喜马拉雅","callback":"3_analyzer_01_1578125434590672","host":[],"listenNum":245506090,"isShowRed":0,"isRequest":0,"originalDuration":0},
{"id":1100002135708,"name":"伤感听听","img":"http://img.kaolafm.net/mz/images/201906/bfa0cafa-a75d-450c-8eef-9b70bf3a3491/default.jpg","type":0,"albumName":"伤感听听","source":7,"duration":0,"playUrl":"","oldId":127566,"sourceName":"考拉未知","callback":"4_analyzer_01_1578125434590405","host":[],"listenNum":586896372,"isShowRed":0,"isRequest":0,"originalDuration":0},
{"id":1100002130065,"name":"民谣在路上","img":"http://img.kaolafm.net/mz/images/201904/5cb6df25-a4ae-4b85-97d8-707c1465dddf/default.jpg","type":0,"albumName":"民谣在路上","source":1,"duration":0,"playUrl":"","oldId":2947060,"sourceName":"考拉喜马拉雅","callback":"5_analyzer_01_1578125434590806","host":[],"listenNum":31237847,"isShowRed":0,"isRequest":0,"originalDuration":0},
{"id":1100002135683,"name":"小七的私房歌","img":"http://img.kaolafm.net/mz/images/201906/5b50ad06-0537-443a-b808-4773c74b17ab/default.jpg","type":0,"albumName":"小七的私房歌","source":1,"duration":0,"playUrl":"","oldId":237771,"sourceName":"考拉喜马拉雅","callback":"6_analyzer_01_1578125434590538","host":[],"listenNum":23712109,"isShowRed":0,"isRequest":0,"originalDuration":0},
{"id":1200000000548,"name":"流行音乐台","img":"http://img.kaolafm.net/mz/images/201906/20044826-7605-4583-b784-bbeb34476b5c/default.jpg","type":3,"albumName":"流行音乐台","source":1,"duration":0,"playUrl":"","oldId":0,"sourceName":"考拉喜马拉雅","callback":"7_analyzer_01_1578125434590001","host":[],"listenNum":3986842,"isShowRed":0,"isRequest":0,"originalDuration":0},
{"id":1200000000552,"name":"经典音乐台","img":"http://img.kaolafm.net/mz/images/201907/25b95ef0-cca9-4a3d-bba9-049f940eaf83/default.jpg","type":3,"albumName":"经典音乐台","source":1,"duration":0,"playUrl":"","oldId":0,"sourceName":"考拉喜马拉雅","callback":"8_analyzer_01_1578125434590323","host":[],"listenNum":3782015,"isShowRed":0,"isRequest":0,"originalDuration":0},
{"id":1100002130054,"name":"听一首歌·念一个人·忆一段情","img":"http://img.kaolafm.net/mz/images/201904/bcc9a7e9-574e-449b-a5e7-0912e9a75259/default.jpg","type":0,"albumName":"听一首歌·念一个人·忆一段情","source":1,"duration":0,"playUrl":"","oldId":249689,"sourceName":"考拉喜马拉雅","callback":"9_analyzer_01_1578125434590728","host":[],"listenNum":19614127,"isShowRed":0,"isRequest":0,"originalDuration":0},
{"id":1100002130076,"name":"哄你入睡的流行钢琴曲","img":"http://img.kaolafm.net/mz/images/201905/1ff9b433-9171-477a-89e9-f4dc25c9642a/default.jpg","type":0,"albumName":"哄你入睡的流行钢琴曲","source":1,"duration":0,"playUrl":"","oldId":20380455,"sourceName":"考拉喜马拉雅","callback":"10_analyzer_01_1578125434590368","host":[],"listenNum":17621925,"isShowRed":0,"isRequest":0,"originalDuration":0},
{"id":1100002135687,"name":"旧年华","img":"http://img.kaolafm.net/mz/images/201906/aa5e6421-237d-4fd8-8ab9-a64049dee3d9/default.jpg","type":0,"albumName":"旧年华","source":1,"duration":0,"playUrl":"","oldId":266276,"sourceName":"考拉喜马拉雅","callback":"11_analyzer_01_1578125434590511","host":[],"listenNum":5452888,"isShowRed":0,"isRequest":0,"originalDuration":0},
{"id":1100002135099,"name":"怀旧金曲","img":"http://img.kaolafm.net/mz/images/201906/18873caa-45fe-4866-95c6-c2ca83cb375b/default.jpg","type":0,"albumName":"怀旧金曲","source":1,"duration":0,"playUrl":"","oldId":268926,"sourceName":"考拉喜马拉雅","callback":"12_analyzer_01_1578125434590261","host":[],"listenNum":14100803,"isShowRed":0,"isRequest":0,"originalDuration":0},
{"id":1100002141906,"name":"张一的那些年","img":"http://img.kaolafm.net/mz/images/201908/1b0e2ed3-899c-404e-9633-92ec75cea2c5/default.jpg","type":0,"albumName":"张一的那些年","source":7,"duration":0,"playUrl":"","oldId":145616,"sourceName":"考拉未知","callback":"13_analyzer_01_1578125434590392","host":[{"name":"张一","des":"","img":"http://img.kaolafm.net/mz/images/201908/8f8982d9-930b-40d0-93df-59b61d85d521/default.jpg"}],"listenNum":14700449,"isShowRed":0,"isRequest":0,"originalDuration":0},
{"id":1100002130057,"name":"一段话 一首歌","img":"http://img.kaolafm.net/mz/images/201904/6368124c-1875-4d8b-af52-28567e3d6e6c/default.jpg","type":0,"albumName":"一段话 一首歌","source":1,"duration":0,"playUrl":"","oldId":261506,"sourceName":"考拉喜马拉雅","callback":"14_analyzer_01_1578125434590231","host":[],"listenNum":32981819,"isShowRed":0,"isRequest":0,"originalDuration":0},
{"id":1000025479085,"name":"音乐","img":"http://img.kaolafm.net/mz/images/201909/02e5ce3b-f61b-4260-b86d-903528d95c65/default.jpg","type":1,"albumName":"郎朗钢琴演奏精选集","source":1,"duration":30120,"playUrl":"http://image.kaolafm.net/mz/aac_64/201909/3e525702054b66fb05a674b72da61a0e.aac","oldId":0,"sourceName":"考拉喜马拉雅","callback":"15_analyzer_01_1578125434590623","host":[],"listenNum":12681,"isShowRed":1,"isRequest":1,"originalDuration":30120},
{"id":1100002130049,"name":"小南 老歌情怀","img":"http://img.kaolafm.net/mz/images/201905/039776d5-4f19-4365-958d-f5be2bae721b/default.jpg","type":0,"albumName":"小南 老歌情怀","source":1,"duration":0,"playUrl":"","oldId":230311,"sourceName":"考拉喜马拉雅","callback":"16_analyzer_01_1578125434590502","host":[],"listenNum":8014090,"isShowRed":0,"isRequest":0,"originalDuration":0},
{"id":1100002135757,"name":"周六·不老情歌","img":"http://img.kaolafm.net/mz/images/201906/2457020d-2ab0-4f36-9b23-20a87f7fd007/default.jpg","type":0,"albumName":"周六·不老情歌","source":1,"duration":0,"playUrl":"","oldId":3523943,"sourceName":"考拉喜马拉雅","callback":"17_analyzer_01_1578125434590574","host":[],"listenNum":47944490,"isShowRed":0,"isRequest":0,"originalDuration":0},
{"id":1100002135702,"name":"刘森的小情歌","img":"http://img.kaolafm.net/mz/images/201906/3188b0c0-f6a3-470c-a175-d803d57d5e24/default.jpg","type":0,"albumName":"刘森的小情歌","source":1,"duration":0,"playUrl":"","oldId":412197,"sourceName":"考拉喜马拉雅","callback":"18_analyzer_01_1578125434590412","host":[],"listenNum":5465405,"isShowRed":0,"isRequest":0,"originalDuration":0},
{"id":1100002130060,"name":"经典留声机","img":"http://img.kaolafm.net/mz/images/201905/7bea4e8c-1bf5-4d2e-baa1-e1c472cf578c/default.jpg","type":0,"albumName":"经典留声机","source":1,"duration":0,"playUrl":"","oldId":290996,"sourceName":"考拉喜马拉雅","callback":"19_analyzer_01_1578125434590518","host":[],"listenNum":16162777,"isShowRed":0,"isRequest":0,"originalDuration":0}
],"debugInfos":{}
},"serverTime":1578125434590,"requestId":"cn52901578125434445352"}

 */

/*
{"category":"","albumName":"","artist":"","audioName":"","field":2,"tag":1,"text":"音乐","keyword":"音乐"}
 == %7B%22category%22%3A%22%22%2C%22albumName%22%3A%22%22%2C%22artist%22%3A%22%22%2C%22audioName%22%3A%22%22%2C%22field%22%3A2%2C%22tag%22%3A1%2C%22text%22%3A%22%E9%9F%B3%E4%B9%90%22%2C%22keyword%22%3A%22%E9%9F%B3%E4%B9%90%22%7D

*/

#define QUERY_BOX "{\"category\":\"\",\"albumName\":\"\",\"artist\":\"\",\"audioName\":\"\",\"field\":2,\"tag\":1,\"text\":\"\",\"keyword\":\"%s\"}"

kl::VoiceSearchAll::VoiceSearchAll(const ByteString &query)
    : SaveObject<SearchItem> ("http://open.kaolafm.com/v2/resource/voice/searchall", NetUrl::NET_HTTP_METHOD_GET)
    , m_pData(nullptr)
{
    char buffer[sizeof(QUERY_BOX) + query.size() * 2];
    int  len = sprintf(buffer, QUERY_BOX, query.string()/*, query.string()*/);

    ByteString box(buffer, len);

    HttpUtil::urlEncoding(box, mQuery);

    // GEN_Printf(LOG_DEBUG, "%s", genQueryUrl().genUrl().string());
}

kl::VoiceSearchAll::~VoiceSearchAll()
{
    NetBuffer::unref(m_pData);
    mQuery.clear();
}

NetUrl &kl::VoiceSearchAll::genQueryUrl()
{
    mUrl.appendChange("voicesource", "1");
    mUrl.appendChange("appsource", "1");
    mUrl.appendChange("qualitytype", "1");
    mUrl.appendChange("q", mQuery);

    mUrl.appendChange("openid", LocalConfig::instance()->openID());
    mUrl.appendChange("sign", SIGN_VoiceSearchAll);

    return mUrl;
}

void kl::VoiceSearchAll::profile()
{
    ListTable<SearchItem>::iterator it = mNodes.begin();

    for (; it != mNodes.end(); ++it)
    {
        GEN_Printf(LOG_DUMP, "type: %s, %s=%s", it->type.string(), it->name.string(), it->id.string());
    }
}

void kl::VoiceSearchAll::search(const ByteString &query)
{
    char buffer[sizeof(QUERY_BOX) + query.size() * 2];
    int  len = sprintf(buffer, QUERY_BOX, query.string()/*, query.string()*/);

    ByteString box(buffer, len);

    HttpUtil::urlEncoding(box, mQuery);

    obtain();
}

void kl::VoiceSearchAll::genResult(NetBuffer *data)
{
    cJSON *root = cJSON_Parse((char *)data->buffer(), data->size());
    cJSON *result = cJSON_GetObjectItem(root, "result");

    if (m_pData)
    {
        NetBuffer::unref(m_pData);
        m_pData = nullptr;
    }

    if (result)
    {
        cJSON *dataList;

        if ((dataList = cJSON_GetObjectItem(result, "dataList")))
        {
            for (cJSON *item = dataList->child; NULL != item; item = item->next )
            {
                SearchItem tmp;

                JSON_VALUETRING_SWAP_BYTESTRING(item, id, tmp.id);
                JSON_VALUETRING_SWAP_BYTESTRING(item, name, tmp.name);
                JSON_VALUETRING_SWAP_BYTESTRING(item, img, tmp.img);
                JSON_VALUETRING_SWAP_BYTESTRING(item, type, tmp.type);
                JSON_VALUETRING_SWAP_BYTESTRING(item, albumName, tmp.albumName);
                JSON_VALUETRING_SWAP_BYTESTRING(item, source, tmp.source);
                JSON_VALUETRING_SWAP_BYTESTRING(item, playUrl, tmp.playUrl);
                JSON_VALUETRING_SWAP_BYTESTRING(item, sourceName, tmp.sourceName);
                JSON_VALUETRING_SWAP_BYTESTRING(item, oldId, tmp.oldId);
                JSON_VALUETRING_SWAP_BYTESTRING(item, callback, tmp.callback);
                JSON_VALUETRING_SWAP_BYTESTRING(item, listenNum, tmp.listenNum);
                JSON_VALUETRING_SWAP_BYTESTRING(item, isShowRed, tmp.isShowRed);
                JSON_VALUETRING_SWAP_BYTESTRING(item, isRequest, tmp.isRequest);
                JSON_VALUETRING_SWAP_BYTESTRING(item, originalDuration, tmp.originalDuration);

                mNodes.push_back(tmp);
            }
        }
        if (mNodes.empty())
        {
            nofity(this, false);
            GEN_Printf(LOG_WARN, "load voice search list is empty.");
        } else
        {            
            m_pData = NetBuffer::ref(data);
            //profile();
            nofity(this, true);
        }
    }else
    {
        nofity(this, false);
        GEN_Printf(LOG_ERROR, "priser failed, size: %lu\n%s", data->size(), data->buffer());
    }

    cJSON_Delete(root);
}

void kl::VoiceSearchAll::loadErrorInfo(int type, const char *str)
{
    GEN_Printf(LOG_ERROR, "load error, type: %d, error: %s", type, str);
    if (m_pData)
    {
        NetBuffer::unref(m_pData);
        m_pData = nullptr;
    }
    nofity(this, false);
}


