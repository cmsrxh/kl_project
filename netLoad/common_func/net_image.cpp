#include <events/common_log.h>
#include "image_cache_manage.h"
#include "net_image.h"

NetImage::NetImage(const ByteString &imgUrl, const char *file)
    : LoadItem(false), mUrl(imgUrl, NetUrl::NET_HTTP_METHOD_GET)
    , notify(nullptr)
{
    mFile = open(file, O_WRONLY | O_CREAT, 0664);
    if (mFile < 0)
    {
        GEN_Printf(LOG_ERROR, "Open image load file error, %s", strerror(errno));
    }
}

NetImage::~NetImage()
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
 * @brief KLImage::obtain
 * @details 启动下载，获取标签数据
 */
bool NetImage::obtain()
{
    return (mFile > 0) ? LoadItem::obtain(mUrl) : false;
}

void NetImage::oneFrameObtain(NetBuffer *data)
{
    int ret = write(mFile, data->buffer(), data->size());
    if ((size_t)ret != data->size())
    {
        GEN_Printf(LOG_ERROR, "write file except, need write=%lu, "
                              "actual write=%d, reson: %s",
                   data->size(), ret, strerror(errno));
    }
}

void NetImage::oneFrameObtainOver()
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

void NetImage::errorInfo(int , const char *)
{
    ImageCacheManage::instance()->errorInfo(notify);
    delete this;
}

