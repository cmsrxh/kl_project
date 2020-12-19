#ifndef MEDIA_SERVICE_PROXY_H
#define MEDIA_SERVICE_PROXY_H

#include "media_client_call_back.h"

class MediaServiceProxy
{
public:
    static MediaServiceProxy *instace()
    {
        static MediaServiceProxy i;
        return &i;
    }

    /**
     * @brief mediaNotify
     * @param msg [in] 消息分类枚举
     * @param ext1 [in] 第一个消息int参数
     * @param ext2 [in] 第二个消息int参数
     * @param str [in] 第三个消息char *参数
     */
    void mediaNotify(int msg, int ext1 = 0, int ext2 = 0, const char *str = nullptr);


    //! 设置服务端的回调接口
    void setCallback(MediaClientCallback *value);

private:
    MediaServiceProxy();
};

#endif // MEDIA_SERVICE_PROXY_H
