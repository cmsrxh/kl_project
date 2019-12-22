#include <events/common_log.h>
#include <openssl/md5.h>
#include "kl_object.h"

kl::KLObject::KLObject(const ByteString &baseUrl, int methodType)
    : mUrl(baseUrl, methodType)
    , mLoad(true)
{
}

kl::KLObject::~KLObject()
{
    if (mLoad.isLoading())
    {
        mLoad.cancel();
    }
}

/**
 * @brief kl::KLObject::obtain
 * @details 启动下载，获取标签数据
 */
void kl::KLObject::obtain()
{
    if (mLoad.isLoading())
    {
        GEN_Printf(LOG_WARN, "is loading, need cancel loading.");
        mLoad.cancel();
    }

    mLoad.setLoad(genQueryUrl(), loadStatus, (void *)this);
}

char *kl::KLObject::genSign(NetUrl &url)
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

void kl::KLObject::loadStatus(int status, uint8_t *data, size_t len, void *arg)
{
    switch (status)
    {
    case OP_CURL_STATUS_LOAD_ALL_OVER:      // 表示数据下载全部一次性返回了
        static_cast<kl::KLObject *>(arg)->loadData(data, len);
        static_cast<kl::KLObject *>(arg)->loadOver();
        break;
    case OP_CURL_STATUS_LOAD_ONE_FRAME:     // 表示数据下载方式按照块来计算，有多少数据就来多少
        break;
    case OP_CURL_STATUS_LOAD_OVER:          // 表示数据下载完成了，与上一个枚举配合，表示下载已经完成，可以关闭文件。
        static_cast<kl::KLObject *>(arg)->loadOver();
        break;
    case OP_CURL_STATUS_ERROR_TYPE:         // 往下的枚举表示数据错误的定义
    default:
        static_cast<kl::KLObject *>(arg)->loadOver();
        GEN_Printf(LOG_ERROR, "Load Error: %d. %s", status - OP_CURL_STATUS_ERROR_TYPE, (char *)data);
        break;
    }
}

