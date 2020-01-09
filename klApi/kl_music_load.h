#ifndef KL_MUSIC_LOAD_H
#define KL_MUSIC_LOAD_H

#include <stdio.h>
#include "app_common.h"
#include "net_util/net_url.h"
#include "net_util/load_item.h"

namespace kl
{
class MusicLoad : public LoadItem
{
public:
    MusicLoad(const ByteString &url, const char *file);
    ~MusicLoad();

    void obtain();

    void setTotalSize(ByteString const &size);

    void oneFrameObtain(uint8_t *data, size_t len);

    void oneFrameObtainOver();

    void errorInfo(int type, const char *str);
private:
    NetUrl     mUrl;
    int        mFile;
    long       mCurrent;
    long       mTotalSize;
};
}

#endif // KL_MUSIC_LOAD_H
