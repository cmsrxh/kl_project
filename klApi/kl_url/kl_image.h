#ifndef KL_IMAGE_H
#define KL_IMAGE_H

#include <stdio.h>
#include "app_common.h"
#include "net_util/net_url.h"
#include "net_util/load_item.h"
#include "kl_url/kl_common.h"

namespace kl
{
class KLImage : public LoadItem
{
public:
    KLImage(const ByteString &imgUrl, const char *file);
    ~KLImage();

    bool obtain();

    void oneFrameObtain(NetBuffer *data);

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
