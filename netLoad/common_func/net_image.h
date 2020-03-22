#ifndef KL_IMAGE_H
#define KL_IMAGE_H

#include <stdio.h>
#include "common_func.h"
#include "../net_util/net_url.h"
#include "../net_util/load_item.h"

class NetImage : public LoadItem
{
public:
    NetImage(const ByteString &imgUrl, const char *file);
    ~NetImage();

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

#endif // KL_IMAGE_H
