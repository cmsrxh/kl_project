#ifndef IMAGE_CACHE_MANAGE_H
#define IMAGE_CACHE_MANAGE_H

#include "kl_url/kl_image.h"
#include "util/hash_container.h"

class ImageCacheManage
{
    ImageCacheManage();
    ImageCacheManage(ImageCacheManage &);
    ImageCacheManage &operator=(ImageCacheManage &);
public:
    static ImageCacheManage *instance()
    {
        static ImageCacheManage i;
        return &i;
    }
    ~ImageCacheManage();

//    void push
    ByteString loadImage(const ByteString &imgUrl, ImageStatus *notify);

private:
    //保存tmp中的图片数据<url, file>
    HashContainer<ByteString, ByteString> mCaches;
    ListTable<ByteString>                 mList;

    ByteString imgUrlEncrypt(const ByteString &in);
};

#endif // IMAGE_CACHE_MANAGE_H
