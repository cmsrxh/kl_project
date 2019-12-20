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

kl::ActiveManage::ActiveManage()
    : mBaseUrl("http://open.kaolafm.com/v1/app/active")
{
}

ByteString kl::ActiveManage::genQueryUrl()
{
    int size = mBaseUrl.size() + 10 + sizeof(APPID) + sizeof(SECRETKEY) + 33 + 64;
    char *sign_url = new char [size];
    unsigned char sign_bytes[16];
    char sign[33];

    int len = snprintf(sign_url, size, "get%s" APPID SECRETKEY, mBaseUrl.string());

    GEN_Printf(LOG_DEBUG, "sign url: %s", sign_url);

    unsigned char *tmp = MD5((unsigned char *)sign_url, len, sign_bytes);

    GEN_Printf(LOG_DEBUG, "%p - %p", tmp, sign_bytes);

    for (int i = 0; i < 16; ++i)
    {
        unsigned char c = sign_bytes[i];
        sign[i * 2]     = STR_PATTERN[(c >> 4) & 0xF];
        sign[i * 2 + 1] = STR_PATTERN[(c)  & 0xF];
    }
    sign[32] = '\0';

    len = snprintf(sign_url, size, "%s?deviceid=%s&appid=%s&sign=%s",
                   mBaseUrl.string(), "akjdhfaksdfhkjdshfkjah", APPID,
                   "a6309ffd3ba9e291bdbd0a64a1b62a35");

    return ByteString(sign_url, len);
}
