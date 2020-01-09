#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <events/common_log.h>
#include "kl_music_load.h"

kl::MusicLoad::MusicLoad(const ByteString &url, const char *file)
    : LoadItem(false), mUrl(url, NetUrl::NET_HTTP_METHOD_GET)
{
    mFile = open(file, O_WRONLY | O_CREAT, 0664);
    if (mFile < 0)
    {
        GEN_Printf(LOG_ERROR, "Open load file error, %s", strerror(errno));
    }
}

kl::MusicLoad::~MusicLoad()
{
    if (isLoading())
    {
        cancel();
    }
    if (mFile > 0)
    {
        close(mFile);
    }
}

void kl::MusicLoad::obtain()
{
    mCurrent = 0;
    LoadItem::obtain(mUrl);
}

void kl::MusicLoad::setTotalSize(const ByteString &size)
{
    if (size.empty())
    {
        mTotalSize = 0;
    } else
    {
        mTotalSize = strtol(size.string(), NULL, 10);
    }
}

void kl::MusicLoad::oneFrameObtain(uint8_t *data, size_t len)
{
    mCurrent += len;

    // GEN_Printf(LOG_DEBUG, "file=%d, (%ld ~ %ld)", mFile, mCurrent, mTotalSize);
    int ret = write(mFile, data, len);
    if ((size_t)ret != len)
    {
        GEN_Printf(LOG_ERROR, "write file except, need write=%d, "
                              "actual write=%d, reson: %s",
                   len, ret, strerror(errno));
    }
}

void kl::MusicLoad::oneFrameObtainOver()
{
    GEN_Printf(LOG_DEBUG, "write file=%d over, size = %0.02gMB.", mFile, (double)(mCurrent) / 1000000.0);
    if (mFile > 0)
    {
        close(mFile);
        mFile = -1;
    }
}

void kl::MusicLoad::errorInfo(int type, const char *str)
{
    GEN_Printf(LOG_ERROR, "load music file=%d error, type[%d], reason=%s", mFile, type, str);
}
