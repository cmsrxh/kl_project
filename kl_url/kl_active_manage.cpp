/**
 * @file:    kl_active_manage.cpp
 * @author:  陈猛
 * @em:      chen-meng@hongjing-wh.com
 * @date:    2019年12月20日 星期五 14时43分57秒
 * @details: 
 ************************************************************************/

#include "events/common_log.h"
#include "kl_common.h"
#include "net_util/net_common.h"
#include "config_local_info.h"
#include "kl_active_manage.h"

/*
POST /v2/app/active?
appid=cn5290&
deviceid=4961c879191af84ef352e215642d569c&
os=linux&
packagename=com.edog.car.linuxwhhj&
sign=72e3649e3fd530f41995b8ebc7249050&
channel=linuxwhhj&
HTTP/1.1
*/
kl::ActiveManage::ActiveManage()
    : KLObject ("http://open.kaolafm.com/v2/app/active", NetUrl::NET_HTTP_METHOD_POST)
{}

void kl::ActiveManage::obtain()
{   
    if (LocalConfig::instance()->openID().empty())
    {
        KLObject::obtain();
    } else
    {
        GEN_Printf(LOG_INFO, "openid: %s is exist", LocalConfig::instance()->openID().string());
    }
}

NetUrl &kl::ActiveManage::genQueryUrl()
{   
    mUrl.append("sign", SIGN_ActiveManage);

    return mUrl;
}

void kl::ActiveManage::loadData(uint8_t *data, unsigned long size)
{
    data[size] = '\0';
    GEN_Printf(LOG_INFO, "device active data: \n%s", (char *)data);
}
