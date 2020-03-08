#include <events/common_log.h>
#include "image_cache_manage.h"
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
bool kl::KLImage::obtain()
{
    return (mFile > 0) ? LoadItem::obtain(mUrl) : false;
}

void kl::KLImage::oneFrameObtain(NetBuffer *data)
{
    int ret = write(mFile, data->buffer(), data->size());
    if ((size_t)ret != data->size())
    {
        GEN_Printf(LOG_ERROR, "write file except, need write=%lu, "
                              "actual write=%d, reson: %s",
                   data->size(), ret, strerror(errno));
    }
}

void kl::KLImage::oneFrameObtainOver()
{
    if (mFile > 0)
    {
        close(mFile);
        mFile = -1;

        ImageCacheManage::instance()->dataPrepare(notify);
    } else
    {
        ImageCacheManage::instance()->errorInfo(notify);
    }

    delete this;
}

void kl::KLImage::errorInfo(int , const char *)
{
    ImageCacheManage::instance()->errorInfo(notify);
    delete this;
}

