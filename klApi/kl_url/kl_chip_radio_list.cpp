#include "events/common_log.h"
#include "net_util/net_common.h"
#include "config_local_info.h"
#include "kl_chip_radio_list.h"
/*
http://open.kaolafm.com/v2/radio/list?rid=1200000000162&clockid=&appid=cn5290&openid=cn52902019122410012497&deviceid=69acf5ec77a42531d1748b7dfc6dcdb6&os=linux&packagename=com.edog.car.linuxwhhj&sign=81230f5ae8db203704271b5c87cc6fbc&channel=linuxwhhj&

*/
/*
{"result":
[
{"audioId":1000024928458101409,"audioName":"搞笑电台","audioPic":"http://img.kaolafm.net/mz/images/201906/eb01b107-d009-43be-a4bb-dbebc8496082/default.jpg","audioDes":"","albumId":1100002136734,"albumName":"笑中有意 热辣无穷","albumPic":"http://img.kaolafm.net/mz/images/201906/eb01b107-d009-43be-a4bb-dbebc8496082/default.jpg","orderNum":1,"mp3PlayUrl32":"http://image.kaolafm.net/mz/mp3_32/201906/0e0ada63-b774-4712-a7de-8193535481bc.mp3","mp3PlayUrl64":"http://image.kaolafm.net/mz/mp3_64/201906/0e0ada63-b774-4712-a7de-8193535481bc.mp3","aacPlayUrl":"http://image.kaolafm.net/mz/aac_64/201906/0e0ada63-b774-4712-a7de-8193535481bc.aac","aacPlayUrl32":"http://image.kaolafm.net/mz/aac_32/201906/0e0ada63-b774-4712-a7de-8193535481bc.aac","aacPlayUrl64":"http://image.kaolafm.net/mz/aac_64/201906/0e0ada63-b774-4712-a7de-8193535481bc.aac","aacPlayUrl128":"http://image.kaolafm.net/mz/aac_128/201906/0e0ada63-b774-4712-a7de-8193535481bc.aac","aacPlayUrl320":"http://image.kaolafm.net/mz/aac_320/201906/0e0ada63-b774-4712-a7de-8193535481bc.aac","host":[],"aacFileSize":675062,"mp3FileSize32":675062,"mp3FileSize64":675062,"updateTime":1561549356,"clockId":"1","duration":16849,"originalDuration":16849,"listenNum":4512,"likedNum":0,"hasCopyright":1,"commentNum":0,"trailerStart":0,"trailerEnd":0,"categoryId":131,"source":0,"isListened":0,"icon":null,"isThirdParty":0,"callBack":"","hasNextPage":1,"singer":"","contentType":2,"contentTypeName":"台宣","mainTitleName":"搞笑电台","subheadName":"笑中有意 热辣无穷","launchType":null,"launchName":null},
{"audioId":1000025911182,"audioName":"你觉得智能是不是真的可以改变生活","audioPic":"http://img.kaolafm.net/mz/images/201903/3d49f65d-7738-4d3a-927e-d7276b46baae/default.png","audioDes":"无论你问什么样的问题，大龙都能给你一个逗乐你的答案 只有你问不出的问题，没有我答不出来的答案。","albumId":1100002127272,"albumName":"大龙大声说","albumPic":"http://img.kaolafm.net/mz/images/201903/3d49f65d-7738-4d3a-927e-d7276b46baae/default.png","orderNum":283,"mp3PlayUrl32":"http://image.kaolafm.net/mz/mp3_32/201912/d8bf62d8-1df0-41d6-ac71-4594677359f0.mp3","mp3PlayUrl64":"http://image.kaolafm.net/mz/mp3_64/201912/d8bf62d8-1df0-41d6-ac71-4594677359f0.mp3","aacPlayUrl":"http://image.kaolafm.net/mz/aac_64/201912/d8bf62d8-1df0-41d6-ac71-4594677359f0.aac","aacPlayUrl32":"http://image.kaolafm.net/mz/aac_32/201912/d8bf62d8-1df0-41d6-ac71-4594677359f0.aac","aacPlayUrl64":"http://image.kaolafm.net/mz/aac_64/201912/d8bf62d8-1df0-41d6-ac71-4594677359f0.aac","aacPlayUrl128":"http://image.kaolafm.net/mz/aac_128/201912/d8bf62d8-1df0-41d6-ac71-4594677359f0.aac","aacPlayUrl320":"http://image.kaolafm.net/mz/aac_320/201912/d8bf62d8-1df0-41d6-ac71-4594677359f0.aac","host":[],"aacFileSize":18305644,"mp3FileSize32":18305644,"mp3FileSize64":18305644,"updateTime":1577178000,"clockId":"1","duration":457613,"originalDuration":457613,"listenNum":32,"likedNum":0,"hasCopyright":1,"commentNum":0,"trailerStart":0,"trailerEnd":0,"categoryId":143,"source":2,"isListened":0,"icon":null,"isThirdParty":0,"callBack":"","hasNextPage":1,"singer":"","contentType":0,"contentTypeName":"","mainTitleName":"你觉得智能是不是真的可以改变生活","subheadName":"大龙大声说","launchType":null,"launchName":null},
{"audioId":1000023308475121409,"audioName":"Ryan Adams-Gimme Something Good","audioPic":"http://img.kaolafm.net/mz/images/201808/c65a54bc-86a0-4840-95b1-b0227587aca7/default.jpg","audioDes":".","albumId":1100002115416,"albumName":"Music","albumPic":"http://img.kaolafm.net/mz/images/201808/c65a54bc-86a0-4840-95b1-b0227587aca7/default.jpg","orderNum":1,"mp3PlayUrl32":"http://image.kaolafm.net/mz/mp3_32/201808/59ea7656-24f6-410a-8e48-a38a02b20e29.mp3","mp3PlayUrl64":"http://image.kaolafm.net/mz/mp3_64/201808/59ea7656-24f6-410a-8e48-a38a02b20e29.mp3","aacPlayUrl":"http://image.kaolafm.net/mz/aac_64/201808/59ea7656-24f6-410a-8e48-a38a02b20e29.aac","aacPlayUrl32":"http://image.kaolafm.net/mz/aac_32/201808/59ea7656-24f6-410a-8e48-a38a02b20e29.aac","aacPlayUrl64":"http://image.kaolafm.net/mz/aac_64/201808/59ea7656-24f6-410a-8e48-a38a02b20e29.aac","aacPlayUrl128":"http://image.kaolafm.net/mz/aac_128/201808/59ea7656-24f6-410a-8e48-a38a02b20e29.aac","aacPlayUrl320":"","host":[],"aacFileSize":931566,"mp3FileSize32":931566,"mp3FileSize64":931566,"updateTime":1535617781,"clockId":"1","duration":232986,"originalDuration":232986,"listenNum":1512,"likedNum":0,"hasCopyright":1,"commentNum":0,"trailerStart":0,"trailerEnd":0,"categoryId":153,"source":0,"isListened":0,"icon":null,"isThirdParty":0,"callBack":"","hasNextPage":1,"singer":"","contentType":4,"contentTypeName":"Music","mainTitleName":"Ryan Adams-Gimme Something Good","subheadName":"","launchType":null,"launchName":null},
...
],"serverTime":"1577190671409","requestId":"cn52901577190671277832"}
*/

kl::ChipRadioList::ChipRadioList(const ByteString &rid)
    : SaveObject<RadioItem> ("http://open.kaolafm.com/v2/radio/list", NetUrl::NET_HTTP_METHOD_GET)
    , mRID(ByteString::allocString(rid))
{
}

kl::ChipRadioList::~ChipRadioList()
{
    mRID.clear();
}

NetUrl &kl::ChipRadioList::genQueryUrl()
{
    mUrl.append("rid", mRID);
    mUrl.append("clockid", "");;

    mUrl.append("openid", LocalConfig::instance()->openID());
    mUrl.append("sign", SIGN_ChipRadioList);

    return mUrl;
}

void kl::ChipRadioList::profile()
{
    ListTable<RadioItem>::iterator it = mNodes.begin();

    for (; it != mNodes.end(); ++it)
    {
        GEN_Printf(LOG_DUMP, "%s=%s", it->audioName.string(), it->audioId.string());
    }
}

void kl::ChipRadioList::genResult(const char *data, unsigned long size)
{
    // GEN_Printf(LOG_DEBUG, "size: %lu\n%s", size, data);
    cJSON *root = cJSON_Parse((char *)data, size);
    cJSON *result = cJSON_GetObjectItem(root, "result");
    if (result)
    {
        for (cJSON *item = result->child; NULL != item; item = item->next )
        {
            RadioItem tmp;

            jsonGenAudioItem(tmp, item);

            mNodes.push_back(tmp);
        }

        if (mNodes.empty())
        {
            GEN_Printf(LOG_WARN, "load broadcast item list is empty.");
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
