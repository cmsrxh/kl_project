#include "events/common_log.h"
#include "image_cache_manage.h"

#define TMP_DIR   "/tmp/image-kl/"
#define VALID_LEN 20
#define TEMP_STR  "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ"

ImageCacheManage::ImageCacheManage()
{
    mkdir(TMP_DIR, 0777);
}

ImageCacheManage::~ImageCacheManage()
{
    ListTable<ByteString>::iterator it = mList.begin();
    for (; it != mList.end(); ++it)
    {
        it->clear();
    }
}

ByteString ImageCacheManage::loadImage(const ByteString &imgUrl, ImageStatus *notify)
{
    ByteString filePath = imgUrlEncrypt(imgUrl);
    if (filePath.empty())
    {
        GEN_Printf(LOG_ERROR, "general image url is empty");
        return filePath;
    }

    mList.push_back(filePath);

    GEN_Printf(LOG_DEBUG, "Save path: %s", filePath.string());

    struct stat st;

    if (0 == stat(filePath.string(), &st)
            && st.st_size > 512)
    {
        if (notify) notify->dataPrepare();
    } else
    {
        kl::KLImage *img = new kl::KLImage(imgUrl, filePath.string());
        img->setUINotify(notify);
        img->obtain();
    }

    return filePath;
}

ByteString ImageCacheManage::imgUrlEncrypt(ByteString const &in)
{
    ByteString ret;
    // static char buffer[VALID_LEN + 12 + sizeof(TMP_DIR)];
    static char tamp[] = "hongjing-lantonda-1906";
    char tear[VALID_LEN] = {0};
    char *src = in.string();
    int  size = in.size();

    //GEN_Printf(LOG_DEBUG, "Data In Length: %u", size);

    if (size > 0)
    {
        int offset = size % VALID_LEN;
        int times  = size / VALID_LEN;
        char *des = new char [VALID_LEN + 12 + sizeof(TMP_DIR)];
        //char *des = buffer;

        memcpy(tear, src + times * VALID_LEN, offset);
        //GEN_Printf(LOG_DEBUG, "offset: %d, times: %d", offset, times);

        ret = ByteString(des, VALID_LEN + 4 + sizeof(TMP_DIR));
        strcpy(des, TMP_DIR);
        des += sizeof(TMP_DIR) - 1;

        for (int i = 0; i < VALID_LEN; ++i)
        {
            int val = tamp[i] + tear[i];
            for (int j = 0; j < times; ++j)
            {
                val += *(src + j * VALID_LEN + i);
            }

            if (i && ((i % 4) == 0))
            {
                *des++ = '-';
            }
            *des++ = ((char *)TEMP_STR)[val % 36];
        }
        *des = '\0';
    } else
    {
        GEN_Printf(LOG_ERROR, "size: %d is invalid", size);
    }
    return ret;
}
