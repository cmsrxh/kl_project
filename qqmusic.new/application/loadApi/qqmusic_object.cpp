#include <events/common_log.h>
#include <openssl/md5.h>
#include <application/application.h>
#include "qqmusic_object.h"

CurlLoadArg qqmusic::Object::mCurlArgs = CurlLoadArg("Mozilla/5.0 (X11; Linux x86_64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/75.0.3770.100 Safari/537.36"
                                                     , "https://c.y.qq.com/");

qqmusic::Object::Object(const ByteString &baseUrl, int objectName, int methodType)
    : mUrl(baseUrl, methodType)
    , mLoad(true), m_pUINotify(nullptr)
    , mObjectName(objectName)
{
}

qqmusic::Object::~Object()
{
    if (mLoad.isLoading())
    {
        mLoad.cancel();
    }
}

/**
 * @brief qqmusic::Object::obtain
 * @details 启动下载，获取标签数据
 */
bool qqmusic::Object::obtain()
{
    if (mLoad.isLoading())
    {
        GEN_Printf(LOG_WARN, "is loading, need cancel loading.");
//        mLoad.cancel();
        return true;
    }

    return mLoad.setLoad(genQueryUrl(), loadStatus, (void *)this, &mCurlArgs);
}

char *qqmusic::Object::genSign(NetUrl &url)
{
    UNUSED(url);
    return nullptr;
}

void qqmusic::Object::loadStatus(int status, void *data, void *arg)
{
    switch (status)
    {
    case OP_CURL_STATUS_LOAD_ALL_OVER:      // 表示数据下载全部一次性返回了
    {
        int ret = static_cast<qqmusic::Object *>(arg)->loadData(static_cast<NetBuffer *>(data));
        static_cast<qqmusic::Object *>(arg)->loadOver();

        switch (ret)
        {
        case KL_DATA_PRISER_OK:          // 分析数据正确
            static_cast<qqmusic::Object *>(arg)->uiNotifyOver();
            break;
        case KL_DATA_PRISER_EMPTY:       // 分析数据正确，但是得到的数据是空
            static_cast<qqmusic::Object *>(arg)->uiNotifyErrorInfo(UINotifyIface::LOAD_EMPTY_DATA, ByteString());
            break;
        case KL_DATA_PRISER_JSOC_ERROR:  // 不能正确解析json数据
            static_cast<qqmusic::Object *>(arg)->uiNotifyErrorInfo(UINotifyIface::LOAD_PRISER_JSOC_ERROR,
                                                                   ByteString((char *)static_cast<NetBuffer *>(data)->buffer(),
                                                                              static_cast<NetBuffer *>(data)->size()));
            Application::instance()->postEvent(SIG_COLLECT_ERR_OBJECT, (long)arg);
            break;
        default:
            SF_ASSERT(0);
            break;
        }       
        break;
    }
    case OP_CURL_STATUS_LOAD_ONE_FRAME:     // 表示数据下载方式按照块来计算，有多少数据就来多少
        static_cast<qqmusic::Object *>(arg)->loadData(static_cast<NetBuffer *>(data));
        break;
    case OP_CURL_STATUS_LOAD_OVER:          // 表示数据下载完成了，与上一个枚举配合，表示下载已经完成，可以关闭文件。
        static_cast<qqmusic::Object *>(arg)->loadOver();
        static_cast<qqmusic::Object *>(arg)->uiNotifyOver();
        break;
    case OP_CURL_STATUS_ERROR_TYPE:         // 往下的枚举表示数据错误的定义
    default:
        static_cast<qqmusic::Object *>(arg)->loadOver();
        static_cast<qqmusic::Object *>(arg)->loadErrorInfo(status - OP_CURL_STATUS_ERROR_TYPE, (char *)data);

        static_cast<qqmusic::Object *>(arg)->uiNotifyErrorInfo(UINotifyIface::LOAD_SYS_API_FAILED, (char *)data);
        Application::instance()->postEvent(SIG_COLLECT_ERR_OBJECT, (long)arg);

        GEN_Printf(LOG_ERROR, "Load Error: %d. %s", status - OP_CURL_STATUS_ERROR_TYPE, (char *)data);
        // Application::instance()->postKlEvent(SIG_SYS_NET_LOAD_API_EXCEPT, status - OP_CURL_STATUS_ERROR_TYPE, (long)arg, (char *)data);
        break;
    }
}
