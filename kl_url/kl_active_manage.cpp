/**
 * @file:    kl_active_manage.cpp
 * @author:  陈猛
 * @em:      chen-meng@hongjing-wh.com
 * @date:    2019年12月20日 星期五 14时43分57秒
 * @details: 
 ************************************************************************/

#include "events/common_log.h"
#include "kl_active_manage.h"

kl::ActiveManage::ActiveManage()
    : KLObject ("http://open.kaolafm.com/v1/app/active", NetUrl::NET_HTTP_METHOD_POST)
{}

NetUrl &kl::ActiveManage::genQueryUrl()
{   
    mUrl.appendContent("deviceid", "akjdhfaksdfhkjdshfkjah");
    mUrl.append("appid", APPID);
//    url.append("sign", "359b410e439fbdb3098f488a0cb8f9d1");
    mUrl.append("sign", genSign(mUrl));

    return mUrl;
}

void kl::ActiveManage::loadData(uint8_t *data, unsigned long size)
{
    data[size] = '\0';
    GEN_Printf(LOG_INFO, "device active data: \n%s", (char *)data);
}
