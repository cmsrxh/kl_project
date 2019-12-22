#include "events/common_log.h"
#include "kl_init_manage.h"

kl::InitManage::InitManage()
    : KLObject ("http://open.kaolafm.com/v1/app/init", NetUrl::NET_HTTP_METHOD_POST)
{
}

NetUrl &kl::InitManage::genQueryUrl()
{
    mUrl.appendContent("deviceid", "akjdhfaksdfhkjdshfkjah");
    mUrl.appendContent("devicetype", "0");
    mUrl.appendContent("osversion", "28");
    mUrl.appendContent("network", "-1");
//    url.appendContent("imsi", "");


    mUrl.append("appid", APPID);
//    url.append("sign", "359b410e439fbdb3098f488a0cb8f9d1");
    mUrl.append("sign", genSign(mUrl));

    return mUrl;
}

void kl::InitManage::loadData(uint8_t *data, unsigned long size)
{
    data[size] = '\0';
    GEN_Printf(LOG_INFO, "app init data: \n%s", (char *)data);
}
