#include <events/common_log.h>
#include <openssl/md5.h>
#include "config_local_info.h"
#include "application.h"
#include "kl_object.h"

kl::KLObject::KLObject(const ByteString &baseUrl, int methodType)
    : mUrl(baseUrl, methodType)
    , mLoad(true), m_pUINotify(nullptr)
    , mObjectName(-1)/*, mLoadStatus(STATUS_IDLE)*/
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
bool kl::KLObject::obtain()
{
    if (LocalConfig::instance()->openID().empty())
    {
        Application::instance()->collectObject(this);
        return false;
    }
    if (mLoad.isLoading())
    {
        GEN_Printf(LOG_WARN, "is loading, need cancel loading.");
//        mLoad.cancel();
        return true;
    }

    return mLoad.setLoad(genQueryUrl(), loadStatus, (void *)this);
/*
    switch (objectName()) {
    case kl::OBJECT_ACTIVE_MANAGE:
    case kl::OBJECT_INIT_MANAGE:
    case kl::OBJECT_SUGGESTION_WORD:
    case kl::OBJECT_VOICE_SEARCH_ALL:
        break;
    default:
        Application::instance()->postKlEvent(SIG_KL_OBJECT_OBTAIN_START, ret, (long)this);
        break;
    }
*/
}

#if 1

/*

**
 * @brief NetUrl::genKLSign
 * @param appid
 * @param secretkey
 * @return
 * @details kl 签名算法的字串生成
 *
ByteString NetUrl::genKLSign(const ByteString &appid, const ByteString &secretkey)
{
    const char *method = "get";
    if (mMethodType == NET_HTTP_METHOD_POST)
    {
        method = "post";
    }
    int urlLen = 5 + mBaseUrl.size() + appid.size() + secretkey.size();

    int   i = 0;
    char *urlStr = allocAddr(urlLen);

    strcpy(urlStr, method);
    i += strlen(method);

    memcpy(urlStr + i, mBaseUrl.string(), mBaseUrl.size());
    i += mBaseUrl.size();

    memcpy(urlStr + i, appid.string(), appid.size());
    i += appid.size();

    memcpy(urlStr + i, secretkey.string(), secretkey.size());
    i += secretkey.size();

    urlStr[i] = '\0';

    return ByteString(urlStr, i);
}

*/
char *kl::KLObject::genSign(NetUrl &url)
{
#if 0
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
#else
    UNUSED(url);
    return nullptr;
#endif
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

void kl::KLObject::loadStatus(int status, void *data, void *arg)
{
    switch (status)
    {
    case OP_CURL_STATUS_LOAD_ALL_OVER:      // 表示数据下载全部一次性返回了
    {
        int ret = static_cast<kl::KLObject *>(arg)->loadData(static_cast<NetBuffer *>(data));
        static_cast<kl::KLObject *>(arg)->loadOver();
        //GEN_Printf(LOG_DEBUG, "----ret = %d----", ret);
        switch (ret)
        {
        case KL_DATA_PRISER_OK:          // 分析数据正确
            static_cast<kl::KLObject *>(arg)->uiNotifyOver();
            break;
        case KL_DATA_PRISER_EMPTY:       // 分析数据正确，但是得到的数据是空
            static_cast<kl::KLObject *>(arg)->uiNotifyErrorInfo(UINotifyIface::LOAD_EMPTY_DATA, ByteString());
            break;
        case KL_DATA_PRISER_JSOC_ERROR:  // 不能正确解析json数据
            static_cast<kl::KLObject *>(arg)->uiNotifyErrorInfo(UINotifyIface::LOAD_PRISER_JSOC_ERROR,
                                                               ByteString((char *)static_cast<NetBuffer *>(data)->buffer(),
                                                                          static_cast<NetBuffer *>(data)->size()));
            Application::instance()->postKlEvent(SIG_KL_COLLECT_ERR_OBJECT, (long)arg);
            break;
        default:
            SF_ASSERT(0);
            break;
        }

        /*
        if (KL_DATA_PRISER_OK != ret)
        {
            NetBuffer *buf = NetBuffer::ref(static_cast<NetBuffer *>(data));
            if (!Application::instance()->postKlEvent(SIG_KL_LOAD_DATA_EXCEPT, ret, (long)arg,
                                                      reinterpret_cast<char *>(buf)))
            {
                NetBuffer::unref(buf);
            }
        } else
        {
            Application::instance()->postKlEvent(SIG_KL_OBJECT_OBTAIN_OVER, 0, (long)arg);
        }
        */
        break;
    }
    case OP_CURL_STATUS_LOAD_ONE_FRAME:     // 表示数据下载方式按照块来计算，有多少数据就来多少
        static_cast<kl::KLObject *>(arg)->loadData(static_cast<NetBuffer *>(data));
        break;
    case OP_CURL_STATUS_LOAD_OVER:          // 表示数据下载完成了，与上一个枚举配合，表示下载已经完成，可以关闭文件。
        static_cast<kl::KLObject *>(arg)->loadOver();
        static_cast<kl::KLObject *>(arg)->uiNotifyOver();
        break;
    case OP_CURL_STATUS_ERROR_TYPE:         // 往下的枚举表示数据错误的定义
    default:
        static_cast<kl::KLObject *>(arg)->loadOver();        
        static_cast<kl::KLObject *>(arg)->loadErrorInfo(status - OP_CURL_STATUS_ERROR_TYPE, (char *)data);

        static_cast<kl::KLObject *>(arg)->uiNotifyErrorInfo(UINotifyIface::LOAD_SYS_API_FAILED, (char *)data);
        Application::instance()->postKlEvent(SIG_KL_COLLECT_ERR_OBJECT, (long)arg);

        GEN_Printf(LOG_ERROR, "Load Error: %d. %s", status - OP_CURL_STATUS_ERROR_TYPE, (char *)data);
        // Application::instance()->postKlEvent(SIG_SYS_NET_LOAD_API_EXCEPT, status - OP_CURL_STATUS_ERROR_TYPE, (long)arg, (char *)data);
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
