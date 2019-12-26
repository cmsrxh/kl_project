#include <events/common_log.h>
#include <openssl/md5.h>
#include "config_local_info.h"
#include "kl_object.h"

kl::KLObject::KLObject(const ByteString &baseUrl, int methodType)
    : mUrl(baseUrl, methodType)
    , mLoad(true), m_pUINotify(nullptr)
{
    mUrl.append("appid", LocalConfig::instance()->appID());
    mUrl.append("deviceid", LocalConfig::instance()->deviceID());

    mUrl.append("os", "linux");
    mUrl.append("packagename", "com.edog.car.linuxwhhj");

    mUrl.append("channel", "linuxwhhj");
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

#if 1
char *kl::KLObject::genSign(NetUrl &url)
{
    unsigned char sign_bytes[16];
    static char sign[33];

    ByteString gen = url.genKLSign(LocalConfig::instance()->appID(),
                                   LocalConfig::instance()->secretKey());

    GEN_Printf(LOG_DEBUG, "gen: %s, size: %d", gen.string(), gen.size());

    MD5((unsigned char *)gen.string(), gen.size(), sign_bytes);

    for (int i = 0; i < 16; ++i)
    {
        unsigned char c = sign_bytes[i];
        sign[i * 2]     = STR_PATTERN[(c >> 4) & 0xF];
        sign[i * 2 + 1] = STR_PATTERN[(c)  & 0xF];
    }
    sign[32] = '\0';

    GEN_Printf(LOG_DEBUG, "false sign: %s", sign);
    GEN_Printf(LOG_DEBUG, "true  sign: %s", SIGN_ActiveManage);

    return sign;
}
#else
char *kl::KLObject::genSign(NetUrl &url)
{
    unsigned char md[16] = {0};
    static char sign[33] = {0};
    MD5_CTX ctx;

    // ByteString gen = url.genKLSign(ByteString(APPID, sizeof (APPID) - 1), ByteString(SECRETKEY, sizeof(SECRETKEY) - 1));
    ByteString gen("gethttp://open.kaolafm.com/v2/app/initcn5290d673b807770b49b4077cb3bfa0d6aedd");

    GEN_Printf(LOG_DEBUG, "gen: %s", gen.string());

    MD5_Init(&ctx);

    MD5_Update(&ctx, (unsigned char *)gen.string(), gen.size());

    MD5_Final(md, &ctx);

    for (int i = 0; i < 16; ++i)
    {
        unsigned char c = md[i];
        sign[i * 2]     = STR_PATTERN[(c >> 4) & 0xF];
        sign[i * 2 + 1] = STR_PATTERN[(c)  & 0xF];
    }
    sign[32] = '\0';

//    char  tmp[3]={ '\0' };

//    for (int i=0; i<16; i++ )
//    {
//        sprintf (tmp, "%02X" ,md[i]);
//        strcat (sign,tmp);
//    }
    GEN_Printf(LOG_DEBUG, "sign: %s", sign);
    GEN_Printf(LOG_DEBUG, "sign: 72e3649e3fd530f41995b8ebc7249050");

    return sign;
}
#endif

void kl::KLObject::loadStatus(int status, uint8_t *data, size_t len, void *arg)
{
    switch (status)
    {
    case OP_CURL_STATUS_LOAD_ALL_OVER:      // 表示数据下载全部一次性返回了
        static_cast<kl::KLObject *>(arg)->loadData(data, len);
        static_cast<kl::KLObject *>(arg)->loadOver();
        static_cast<kl::KLObject *>(arg)->uiNotifyOver();
        break;
    case OP_CURL_STATUS_LOAD_ONE_FRAME:     // 表示数据下载方式按照块来计算，有多少数据就来多少
        static_cast<kl::KLObject *>(arg)->loadData(data, len);
        break;
    case OP_CURL_STATUS_LOAD_OVER:          // 表示数据下载完成了，与上一个枚举配合，表示下载已经完成，可以关闭文件。
        static_cast<kl::KLObject *>(arg)->loadOver();
        static_cast<kl::KLObject *>(arg)->uiNotifyOver();
        break;
    case OP_CURL_STATUS_ERROR_TYPE:         // 往下的枚举表示数据错误的定义
    default:
        static_cast<kl::KLObject *>(arg)->loadOver();
        static_cast<kl::KLObject *>(arg)->loadErrorInfo(status - OP_CURL_STATUS_ERROR_TYPE, (char *)data);
        static_cast<kl::KLObject *>(arg)->uiNotifyErrorInfo(status - OP_CURL_STATUS_ERROR_TYPE, (char *)data);
        GEN_Printf(LOG_ERROR, "Load Error: %d. %s", status - OP_CURL_STATUS_ERROR_TYPE, (char *)data);
        break;
    }
}

void kl::KLObject::jsonGenAudioItem(kl::AudioItem &item, cJSON *result)
{
    JSON_VALUETRING_SWAP_BYTESTRING(result, audioId, item.audioId);
    JSON_VALUETRING_SWAP_BYTESTRING(result, audioName, item.audioName);
    JSON_VALUETRING_SWAP_BYTESTRING(result, audioPic, item.audioPic);
    JSON_VALUETRING_SWAP_BYTESTRING(result, audioDes, item.audioDes);
    JSON_VALUETRING_SWAP_BYTESTRING(result, albumId, item.albumId);
    JSON_VALUETRING_SWAP_BYTESTRING(result, albumName, item.albumPic);
    JSON_VALUETRING_SWAP_BYTESTRING(result, orderNum, item.orderNum);
    JSON_VALUETRING_SWAP_BYTESTRING(result, mp3PlayUrl32, item.mp3PlayUrl32);
    JSON_VALUETRING_SWAP_BYTESTRING(result, mp3PlayUrl64, item.mp3PlayUrl64);
    JSON_VALUETRING_SWAP_BYTESTRING(result, aacPlayUrl, item.aacPlayUrl);
    JSON_VALUETRING_SWAP_BYTESTRING(result, aacPlayUrl32, item.aacPlayUrl32);
    JSON_VALUETRING_SWAP_BYTESTRING(result, aacPlayUrl64, item.aacPlayUrl64);
    JSON_VALUETRING_SWAP_BYTESTRING(result, aacPlayUrl128, item.aacPlayUrl128);
    JSON_VALUETRING_SWAP_BYTESTRING(result, aacPlayUrl320, item.aacPlayUrl320);
    JSON_VALUETRING_SWAP_BYTESTRING(result, aacFileSize, item.aacFileSize);
    JSON_VALUETRING_SWAP_BYTESTRING(result, mp3FileSize32, item.mp3FileSize32);
    JSON_VALUETRING_SWAP_BYTESTRING(result, mp3FileSize64, item.mp3FileSize64);
    JSON_VALUETRING_SWAP_BYTESTRING(result, updateTime, item.updateTime);
    JSON_VALUETRING_SWAP_BYTESTRING(result, clockId, item.clockId);
    JSON_VALUETRING_SWAP_BYTESTRING(result, duration, item.duration);
    JSON_VALUETRING_SWAP_BYTESTRING(result, originalDuration, item.originalDuration);
    JSON_VALUETRING_SWAP_BYTESTRING(result, listenNum, item.listenNum);
    JSON_VALUETRING_SWAP_BYTESTRING(result, likedNum, item.likedNum);
    JSON_VALUETRING_SWAP_BYTESTRING(result, hasCopyright, item.hasCopyright);
    JSON_VALUETRING_SWAP_BYTESTRING(result, commentNum, item.commentNum);
    JSON_VALUETRING_SWAP_BYTESTRING(result, trailerStart, item.trailerStart);
    JSON_VALUETRING_SWAP_BYTESTRING(result, trailerEnd, item.trailerEnd);
    JSON_VALUETRING_SWAP_BYTESTRING(result, categoryId, item.categoryId);
    JSON_VALUETRING_SWAP_BYTESTRING(result, source, item.source);
    JSON_VALUETRING_SWAP_BYTESTRING(result, isListened, item.isListened);
    JSON_VALUETRING_SWAP_BYTESTRING(result, isThirdParty, item.isThirdParty);
    JSON_VALUETRING_SWAP_BYTESTRING(result, hasNextPage, item.hasNextPage);

}

void kl::KLObject::jsonGenTypeRadio(kl::TypeRadio &tmp, cJSON *result)
{
    JSON_VALUETRING_SWAP_BYTESTRING(result, id, tmp.id);
    JSON_VALUETRING_SWAP_BYTESTRING(result, name, tmp.name);
    JSON_VALUETRING_SWAP_BYTESTRING(result, img, tmp.img);
    JSON_VALUETRING_SWAP_BYTESTRING(result, followedNum, tmp.followedNum);
    JSON_VALUETRING_SWAP_BYTESTRING(result, isOnline, tmp.isOnline);
    JSON_VALUETRING_SWAP_BYTESTRING(result, listenNum, tmp.listenNum);
    JSON_VALUETRING_SWAP_BYTESTRING(result, desc, tmp.desc);
    JSON_VALUETRING_SWAP_BYTESTRING(result, commentNum, tmp.commentNum);
    JSON_VALUETRING_SWAP_BYTESTRING(result, isSubscribe, tmp.isSubscribe);
    JSON_VALUETRING_SWAP_BYTESTRING(result, type, tmp.type);

    cJSON *keyWord = cJSON_GetObjectItem(result, "keyWords");
    if (keyWord)
    {
        for (cJSON *item = keyWord->child; NULL != item; item = item->next )
        {
            tmp.keyWords.push_back(item->valuestring);
            item->valuestring = NULL;
        }
    }
}

