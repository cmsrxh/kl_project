#include <events/common_log.h>
#include "kl_image.h"

kl::KLImage::KLImage(const ByteString &imgUrl)
    : LoadItem(false), mUrl(imgUrl, NetUrl::NET_HTTP_METHOD_GET)
{
}

kl::KLImage::~KLImage()
{
    if (isLoading())
    {
        cancel();
    }
}

/**
 * @brief kl::KLImage::obtain
 * @details 启动下载，获取标签数据
 */
void kl::KLImage::obtain()
{
    LoadItem::obtain(mUrl);
}

void kl::KLImage::oneFrameObtain(uint8_t *data, size_t len)
{
    UNUSED(data);
    UNUSED(len);
}

void kl::KLImage::oneFrameObtainOver()
{
}

void kl::KLImage::errorInfo(int type, const char *str)
{
    GEN_Printf(LOG_ERROR, "type[%d] = %s", type, str);
}
