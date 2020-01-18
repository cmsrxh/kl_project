#include "events/common_log.h"
#include "net_util/net_common.h"
#include "config_local_info.h"
#include "kl_type_radio_list.h"
/*
http://open.kaolafm.com/v2/typeradio/list?
cid=-1&
appid=cn5290&openid=cn52902019122410012497&
deviceid=69acf5ec77a42531d1748b7dfc6dcdb6&os=linux&packagename=com.edog.car.linuxwhhj&
sign=81230f5ae8db203704271b5c87cc6fbc&channel=linuxwhhj&
*/

/*
{"result":
[
{"id":1200000000099,"name":"新闻电台","img":"http://img.kaolafm.net/mz/images/201906/1c1ab70b-e8da-4365-af0e-26293222647c/default.jpg","followedNum":2632,"isOnline":1,"listenNum":48566967,"desc":"有趣事、新鲜事，独家研发编排串播系统，人工筛选与大数据推荐无缝链接，让你听得更流畅更舒服，摆脱广播电台定时播放规定节目的束缚。\r\n","commentNum":54,"isSubscribe":0,"type":3,"host":[],"keyWords":["科技","财经","热点"]},
{"id":1200000000162,"name":"搞笑电台","img":"http://img.kaolafm.net/mz/images/201906/a8110b25-d3b3-4039-a365-fed555257851/default.jpg","followedNum":2473,"isOnline":1,"listenNum":18864967,"desc":"全球首家音频喜剧连播电台，7x24x365不间断播音， 笑话段子轮番轰炸；情景喜剧爆笑热辣；热点吐槽新奇诙谐；更有好玩的相声、脱口秀等海量内容。放松心情，伴随收听，搞笑FM不容错过。","commentNum":45,"isSubscribe":0,"type":3,"host":[],"keyWords":["脱口秀","笑话","段子"]},
{"id":1200000000164,"name":"情感电台","img":"http://img.kaolafm.net/mz/images/201906/a34c2eeb-1eac-4a07-8b7f-3667a8355066/default.jpg","followedNum":1065,"isOnline":1,"listenNum":5380172,"desc":"全国第一家智能化情感电台，可以为你轻松治愈答疑解惑，也可以和你眉来眼去性感聊骚。\r\n独家研发编排串播系统，人工筛选与大数据推荐无缝链接，让你听得更流畅更舒服，摆脱广播电台定时播放规定节目的束缚。","commentNum":34,"isSubscribe":0,"type":3,"host":[],"keyWords":["治愈","解惑","两性"]},
....
*/
kl::TypeRadioList::TypeRadioList(int cid)
    : SaveObject<TypeRadio> ("http://open.kaolafm.com/v2/typeradio/list", NetUrl::NET_HTTP_METHOD_GET)
    , mCID(ByteString::allocLong(cid))
{
}

kl::TypeRadioList::~TypeRadioList()
{
    mCID.clear();
}

NetUrl &kl::TypeRadioList::genQueryUrl()
{
    mUrl.appendChange("cid", mCID);

    mUrl.appendChange("openid", LocalConfig::instance()->openID());
    mUrl.appendChange("sign", SIGN_CategoryAll);

    return mUrl;
}

void kl::TypeRadioList::profile()
{
    ListTable<TypeRadio>::iterator it = mNodes.begin();

    for (; it != mNodes.end(); ++it)
    {
        GEN_Printf(LOG_DUMP, "%s=%s", it->name.string(), it->id.string());
    }
}

int kl::TypeRadioList::genResult(NetBuffer *data)
{
    int ret = KL_DATA_PRISER_OK;
    cJSON *root = cJSON_Parse((char *)data->buffer(), data->size());
    cJSON *result = cJSON_GetObjectItem(root, "result");
    if (result)
    {
        for (cJSON *item = result->child; NULL != item; item = item->next )
        {
            TypeRadio tmp;

            jsonGenTypeRadio(tmp, item);
            mNodes.push_back(tmp);
        }

        if (mNodes.empty())
        {
            GEN_Printf(LOG_WARN, "load categoryall list is empty.");
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

bool kl::TypeRadioList::loadNextPage()
{
    GEN_Printf(LOG_DEBUG, "Have Next, But can't really.");
    return false;
}
