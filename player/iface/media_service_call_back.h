#ifndef MEDIA_SERVICE_CALL_BACK_H
#define MEDIA_SERVICE_CALL_BACK_H

#include "media_iface_common.h"

//给HMI继承的类，用于接收服务发来的消息
class MediaServiceCallback
{
public:
    MediaServiceCallback() {}

    virtual void mediaNotify(int msg, int ext1, int ext2, const char *str) = 0;
};

#endif // MEDIA_SERVICE_CALL_BACK_H
