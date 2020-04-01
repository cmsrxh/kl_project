#include "qqmusic_category.h"

/*
"http://c.y.qq.com/splcloud/fcgi-bin/fcg_get_diss_tag_conf.fcg?g_tk=5381&notice=0&inCharset=utf8&outCharset=utf-8&platform=yqq&needNewCode=0"

*/
qqmusic::Category::Category()
    : ListObject<CateNode> ("http://c.y.qq.com/splcloud/fcgi-bin/fcg_get_diss_tag_conf.fcg?g_tk=5381&notice=0&inCharset=utf8&outCharset=utf-8&platform=yqq&needNewCode=0"
                            , OBJECT_NAME_PLAY_LIST_CATEGORY, NetUrl::NET_HTTP_METHOD_GET)
{

}

NetUrl &qqmusic::Category::genQueryUrl()
{
    return mUrl;
}

int qqmusic::Category::genResult(NetBuffer *data)
{
    int ret = KL_DATA_PRISER_OK;
    GEN_Printf(LOG_DEBUG, "size: %lu\n%s", data->size(), (char *)data->buffer());

    return ret;
}

void qqmusic::Category::profile()
{

}
