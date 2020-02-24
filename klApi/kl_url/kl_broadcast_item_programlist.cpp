#include "events/common_log.h"
#include "net_util/net_common.h"
#include "app_common.h"
#include "config_local_info.h"
#include "kl_broadcast_item_programlist.h"

/*
/v2/broadcast/programlist?
bid=1600000000510&
date=&appid=cn5290&
openid=cn52902019122410012497&
deviceid=69acf5ec77a42531d1748b7dfc6dcdb6&
os=linux&packagename=com.edog.car.linuxwhhj&
sign=81230f5ae8db203704271b5c87cc6fbc&channel=linuxwhhj&

http://open.kaolafm.com/v2/broadcast/programlist?
appid=cn5290&
deviceid=69acf5ec77a42531d1748b7dfc6dcdb6&
os=linux&packagename=com.edog.car.linuxwhhj&channel=linuxwhhj&
bid=1600000000747&date=&openid=cn52902019122410012497&
sign=81230f5ae8db203704271b5c87cc6fbc
*/
/*
[default][DEBUG][genResult.49]
{"result":
[
{"programId":35245367,"broadcastId":1600000000510,"nextProgramId":-1,"preProgramId":-1,"title":"千里共良宵","backLiveUrl":"http://pb.x.l.kaolafm.net/play.m3u8?broadcastId=1600000000510&stime=20191224000000&etime=20191224015959","playUrl":"http://play.x.l.kaolafm.net/live/1600000000510/index.m3u8","comperes":"无","beginTime":"00:00:00","endTime":"01:59:59","startTime":1577116800000,"finishTime":1577123999000,"status":2,"isSubscribe":0,"desc":"千里共良宵\n","broadcastDesc":"中国唯一覆盖全国的24小时新闻直播频率。","broadcastName":"央广中国之声","broadcastImg":"http://img.kaolafm.net/mz/images/201608/617b7348-1d42-4fc1-820b-b8ce30f26999/default.jpg","icon":null},
{"programId":35245368,"broadcastId":1600000000510,"nextProgramId":-1,"preProgramId":-1,"title":"昨日新闻重现","backLiveUrl":"http://pb.x.l.kaolafm.net/play.m3u8?broadcastId=1600000000510&stime=20191224020000&etime=20191224035959","playUrl":"http://play.x.l.kaolafm.net/live/1600000000510/index.m3u8","comperes":"姗姗 雨亭 苏扬","beginTime":"02:00:00","endTime":"03:59:59","startTime":1577124000000,"finishTime":1577131199000,"status":2,"isSubscribe":0,"desc":"昨日新闻重现\n","broadcastDesc":"中国唯一覆盖全国的24小时新闻直播频率。","broadcastName":"央广中国之声","broadcastImg":"http://img.kaolafm.net/mz/images/201608/617b7348-1d42-4fc1-820b-b8ce30f26999/default.jpg","icon":null},
....
*/
kl::BroadcastItemProgramlist::BroadcastItemProgramlist(const ByteString &bid)
    : SaveObject<BDCastProgramItem> ("http://open.kaolafm.com/v2/broadcast/programlist", NetUrl::NET_HTTP_METHOD_GET)
    , mBID(bid)
{
    setObjectName(OBJECT_BDC_ITEM_PROGRAM);
}

kl::BroadcastItemProgramlist::~BroadcastItemProgramlist()
{
    mBID.clear();
}

NetUrl &kl::BroadcastItemProgramlist::genQueryUrl()
{
    mUrl.appendChange("bid", mBID);
    mUrl.appendChange("date", "");
    mUrl.appendChange("openid", LocalConfig::instance()->openID());
    mUrl.appendChange("sign", SIGN_BroadcastItemProgramlist);
    return mUrl;
}

void kl::BroadcastItemProgramlist::profile()
{
    ListTable<BDCastProgramItem>::iterator it = mNodes.begin();
    int count = 0;

    for (; it != mNodes.end(); ++it, ++count)
    {
        GEN_Printf(LOG_DUMP, "%s=%s", it->title.string(), it->programId.string());
    }
    GEN_Printf(LOG_DEBUG, "Load broadcast program size: %d", count);
}

int kl::BroadcastItemProgramlist::genResult(NetBuffer *data)
{
    int ret = KL_DATA_PRISER_OK;
//    GEN_Printf(LOG_DEBUG, "%s", data);
    cJSON *root = cJSON_Parse((char *)data->buffer(), data->size());
    cJSON *result = cJSON_GetObjectItem(root, "result");
    if (result)
    {
        for (cJSON *item = result->child; NULL != item; item = item->next )
        {
            BDCastProgramItem program;

            JSON_VALUETRING_SWAP_BYTESTRING(item, programId, program.programId);
            JSON_VALUETRING_SWAP_BYTESTRING(item, broadcastId, program.broadcastId);
            JSON_VALUETRING_SWAP_BYTESTRING(item, nextProgramId, program.nextProgramId);
            JSON_VALUETRING_SWAP_BYTESTRING(item, preProgramId, program.preProgramId);
            JSON_VALUETRING_SWAP_BYTESTRING(item, title, program.title);
            JSON_VALUETRING_SWAP_BYTESTRING(item, backLiveUrl, program.backLiveUrl);
            JSON_VALUETRING_SWAP_BYTESTRING(item, stime, program.stime);
            JSON_VALUETRING_SWAP_BYTESTRING(item, etime, program.etime);
            JSON_VALUETRING_SWAP_BYTESTRING(item, playUrl, program.playUrl);
            JSON_VALUETRING_SWAP_BYTESTRING(item, comperes, program.comperes);
            JSON_VALUETRING_SWAP_BYTESTRING(item, endTime, program.endTime);
            JSON_VALUETRING_SWAP_BYTESTRING(item, startTime, program.startTime);
            JSON_VALUETRING_SWAP_BYTESTRING(item, finishTime, program.finishTime);
            JSON_VALUETRING_SWAP_BYTESTRING(item, status, program.status);
            JSON_VALUETRING_SWAP_BYTESTRING(item, isSubscribe, program.isSubscribe);
            JSON_VALUETRING_SWAP_BYTESTRING(item, desc, program.desc);
            JSON_VALUETRING_SWAP_BYTESTRING(item, broadcastDesc, program.broadcastDesc);
            JSON_VALUETRING_SWAP_BYTESTRING(item, broadcastName, program.broadcastName);
            JSON_VALUETRING_SWAP_BYTESTRING(item, broadcastImg, program.broadcastImg);

            mNodes.push_back(program);
        }

        if (mNodes.empty())
        {
            GEN_Printf(LOG_WARN, "load broadcast program list is empty.");
            ret = KL_DATA_PRISER_EMPTY;
        }/* else
        {
            profile();
        }*/
    } else
    {
        GEN_Printf(LOG_ERROR, "priser failed, size: %lu\n%s", data->size(), data->buffer());
        ret = KL_DATA_PRISER_JSOC_ERROR;
    }

    cJSON_Delete(root);
    return ret;
}
