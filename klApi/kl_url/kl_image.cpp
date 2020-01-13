#include <events/common_log.h>
#include "kl_image.h"

kl::KLImage::KLImage(const ByteString &imgUrl, const char *file)
    : LoadItem(false), mUrl(imgUrl, NetUrl::NET_HTTP_METHOD_GET)
    , notify(nullptr)
{
    mFile = open(file, O_WRONLY | O_CREAT, 0664);
    if (mFile < 0)
    {
        GEN_Printf(LOG_ERROR, "Open image load file error, %s", strerror(errno));
    }
}

kl::KLImage::~KLImage()
{
    if (isLoading())
    {
        // cancel();
    }

    if (mFile > 0)
    {
        close(mFile);
    }
}

/**
 * @brief kl::KLImage::obtain
 * @details 启动下载，获取标签数据
 */
void kl::KLImage::obtain()
{
    if (mFile > 0)
    {
        LoadItem::obtain(mUrl);
    } else
    {
        GEN_Printf(LOG_ERROR, "File Create failed.");
        delete this;
    }
}

void kl::KLImage::oneFrameObtain(uint8_t *data, size_t len)
{
    int ret = write(mFile, data, len);
    if ((size_t)ret != len)
    {
        GEN_Printf(LOG_ERROR, "write file except, need write=%d, "
                              "actual write=%d, reson: %s",
                   len, ret, strerror(errno));
    }
}

void kl::KLImage::oneFrameObtainOver()
{
    if (mFile > 0)
    {
        close(mFile);
        mFile = -1;
        if (notify) notify->dataPrepare();
    } else
    {
        if (notify) notify->errorInfo(0, "file open error.");
    }

    delete this;
}

void kl::KLImage::errorInfo(int type, const char *str)
{
    // GEN_Printf(LOG_ERROR, "type[%d] = %s", type, str);
    if (notify) notify->errorInfo(type, str);

    delete this;
}

