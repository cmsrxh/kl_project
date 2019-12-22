#ifndef KL_IMAGE_H
#define KL_IMAGE_H

#include <stdio.h>
#include "app_common.h"
#include "net_util/net_url.h"
#include "net_util/load_item.h"

namespace kl
{
class KLImage : public LoadItem
{
public:
    KLImage(const ByteString &imgUrl);
    ~KLImage();

    void obtain();

    void oneFrameObtain(uint8_t *data, size_t len);

    void oneFrameObtainOver();

    void errorInfo(int type, const char *str);
private:
    NetUrl    mUrl;
};
}
#endif // KL_IMAGE_H
