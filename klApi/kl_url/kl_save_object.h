#ifndef KL_SAVE_OBJECT_H
#define KL_SAVE_OBJECT_H

#include "kl_object.h"
#include "kl_common.h"
#include "util/list_table.h"

namespace kl
{
template <typename save_node>
class SaveObject : public KLObject
{
public:
    SaveObject(const ByteString &baseUrl, int methodType)
        : KLObject(baseUrl, methodType)
        , mSaveFile(nullptr)
    {}
    ~SaveObject()
    {
        clearData();
    }

    void clearData()
    {
        typename ListTable<save_node>::iterator it;
        for (it = mNodes.pop_front(); it != mNodes.end(); it = mNodes.pop_front())
        {
            it->clear();
            mNodes.remove(it);
        }
    }

    void loadData (uint8_t *data, unsigned long size)
    {
        data[size] = '\0';
        // GEN_Printf(LOG_INFO, "app init data: \n%s", (char *)data);

        genResult((char *)data, size);

        if (mSaveFile)
        {
            int fd = open(mSaveFile, O_CREAT | O_WRONLY, 0664);
            if (fd)
            {
                int ret = write(fd, data, size);
                if (ret != (int)size)
                {
                    GEN_Printf(LOG_WARN, "write failed, [%d, %lu]", ret, size);
                }

                close(fd);
            } else
            {
                GEN_Printf(LOG_ERROR, "%s open failed, %s", mSaveFile, strerror(errno));
            }
        }
    }

    void loadNodesFile()
    {
        if (mSaveFile)
        {
            struct stat st;

            if (stat(mSaveFile, &st) == 0 && st.st_size > 2)
            {
                int fd = open(mSaveFile, O_RDONLY);
                if (fd > 0)
                {
                    char *data = new char[st.st_size + 1];

                    int ret = read(fd, data, st.st_size);
                    if(ret > 0)
                    {
                        data[ret] = '\0';
                        genResult(data, ret);
                    } else
                    {
                        GEN_Printf(LOG_ERROR, "read failed, %s", strerror(errno));
                    }
                    close(fd);
                } else
                {
                    GEN_Printf(LOG_ERROR, "open failed, %s", strerror(errno));
                }
            } else
            {
                GEN_Printf(LOG_ERROR, "%s open failed, %s", mSaveFile, strerror(errno));
            }
        }
    }

    ListTable<save_node> &nodes()
    {
        return mNodes;
    }

    bool isNeedLoad()
    {
        return mNodes.empty();
    }

    virtual void genResult(const char */*data*/, unsigned long /*size*/) {}

    void setSaveFile(const char *saveFile)
    {
        mSaveFile = saveFile;
    }
protected:
    const char *mSaveFile;
    ListTable<save_node> mNodes;
};
}

#endif // KL_SAVE_OBJECT_H
