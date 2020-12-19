#ifndef MEDIA_SERVICE_CALL_BACK_H
#define MEDIA_SERVICE_CALL_BACK_H

#include "media_iface_common.h"

/**
 * @brief The MediaServiceCallback class
 * @note 给HMI继承的类，用于接收服务发来的消息
 */
class MediaServiceCallback
{
public:
    MediaServiceCallback() {}

    /**
     * @brief mediaNotify
     * @param msg [in] 消息分类枚举
     * @param ext1 [in] 第一个消息int参数
     * @param ext2 [in] 第二个消息int参数
     * @param str [in] 第三个消息char *参数
     */
    virtual void mediaNotify(int /*msg*/, int /*ext1*/, int /*ext2*/, const char */*str*/) {}
};

#endif // MEDIA_SERVICE_CALL_BACK_H
