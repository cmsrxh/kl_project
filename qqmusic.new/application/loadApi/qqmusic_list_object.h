#ifndef KL_SAVE_OBJECT_H
#define KL_SAVE_OBJECT_H

#include "qqmusic_object.h"
#include "util/list_table.h"

namespace qqmusic
{
template <typename item_node>
class ListObject : public Object
{
public:
    ListObject(const ByteString &baseUrl, int objectName, int methodType)
        : Object(baseUrl, objectName, methodType)
        , mSaveFile(nullptr)
    {}
    ~ListObject()
    {
        clearData();
    }

    void clearData()
    {
        typename ListTable<item_node>::iterator it;
        for (it = mNodes.pop_front(); it != mNodes.end(); it = mNodes.pop_front())
        {
            it->clear();
            mNodes.remove(it);
        }
    }

    int loadData (NetBuffer *data)
    {        
        int ret = genResult(data);

        if (mSaveFile)
        {
            int fd = open(mSaveFile, O_CREAT | O_TRUNC | O_WRONLY, 0664);
            if (fd)
            {
                int ret = write(fd, data->buffer(), data->size());
                if (ret != (int)data->size())
                {
                    GEN_Printf(LOG_WARN, "write failed, [%d, %lu]", ret, data->size());
                }

                close(fd);
            } else
            {
                GEN_Printf(LOG_ERROR, "%s open failed, %s", mSaveFile, strerror(errno));
            }
        }

        return ret;
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
                        char tmp[sizeof(NetBuffer)];
                        data[ret] = '\0';                        
                        genResult(NetBuffer::packBuffer(tmp, reinterpret_cast<uint8_t *>(data), (size_t)ret));
                    } else
                    {
                        GEN_Printf(LOG_ERROR, "read failed, %s", strerror(errno));
                    }
                    close(fd);
                    delete [] data;
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

    ListTable<item_node> &nodes()
    {
        return mNodes;
    }

    bool isNeedLoad()
    {
        return mNodes.empty();
    }
    bool isEmpty()
    {
        return mNodes.empty();
    }

    virtual int genResult(NetBuffer */*data*/) { return KL_DATA_PRISER_OK; }

    void setSaveFile(const char *saveFile)
    {
        mSaveFile = saveFile;
    }
protected:
    const char *mSaveFile;
    ListTable<item_node> mNodes;
};
}

#endif // KL_SAVE_OBJECT_H
