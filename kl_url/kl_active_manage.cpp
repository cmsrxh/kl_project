/**
 * @file:    kl_active_manage.cpp
 * @author:  陈猛
 * @em:      chen-meng@hongjing-wh.com
 * @date:    2019年12月20日 星期五 14时43分57秒
 * @details: 
 ************************************************************************/

#include "events/common_log.h"
#include "app_common.h"
#include <openssl/md5.h>
#include "kl_active_manage.h"

static char *genSign(NetUrl &url)
{
    unsigned char sign_bytes[16];
    static char sign[33];

    ByteString gen = url.genKLSign(ByteString(APPID, sizeof (APPID) - 1), ByteString(SECRETKEY, sizeof(SECRETKEY) - 1));

    GEN_Printf(LOG_DEBUG, "gen: %s", gen.string());

    MD5((unsigned char *)gen.string(), gen.size(), sign_bytes);

    for (int i = 0; i < 16; ++i)
    {
        unsigned char c = sign_bytes[i];
        sign[i * 2]     = STR_PATTERN[(c >> 4) & 0xF];
        sign[i * 2 + 1] = STR_PATTERN[(c)  & 0xF];
    }
    sign[32] = '\0';

    GEN_Printf(LOG_DEBUG, "sign: %s", sign);

    return sign;
}

kl::ActiveManage::ActiveManage()
    : url("http://open.kaolafm.com/v1/app/active", NetUrl::NET_HTTP_METHOD_POST)
{}

NetUrl &  kl::ActiveManage::genQueryUrl()
{   
//    url.append("deviceid", "akjdhfaksdfhkjdshfkjah");
    url.append("appid", APPID);
    url.append("sign", "359b410e439fbdb3098f488a0cb8f9d1");
//    url.append("sign", genSign(url));

    return url;
}
