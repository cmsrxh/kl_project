#ifndef KL_IMAGE_H
#define KL_IMAGE_H

#include <stdio.h>
#include "app_common.h"
#include "net_util/net_url.h"
#include "net_util/load_item.h"

class ImageStatus
{
public:
    virtual void dataPrepare() = 0;
    virtual void errorInfo(int , const char *) {}
};

namespace kl
{
class KLImage : public LoadItem
{
public:
    KLImage(const ByteString &imgUrl, const char *file);
    ~KLImage();

    void obtain();

    void oneFrameObtain(uint8_t *data, size_t len);

    void oneFrameObtainOver();

    void errorInfo(int type, const char *str);

    void setUINotify(ImageStatus *value)
    {
        notify = value;
    }
private:
    NetUrl       mUrl;
    ImageStatus *notify;
    int          mFile;
};
}
#endif // KL_IMAGE_H
