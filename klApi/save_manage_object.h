#ifndef SAVE_MANAGE_OBJECT_H
#define SAVE_MANAGE_OBJECT_H

#include "util/list_table.h"

template <typename save_node>
class SaveFileObject
{
public:
    SaveFileObject()
        : mSaveFile(nullptr)
    {}
    ~SaveFileObject()
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

    bool saveNodesFile()
    {
        if (mSaveFile)
        {
            int fd = open(mSaveFile, O_WRONLY);
            if (fd > 0)
            {
                char *data = nullptr;
                unsigned long len = 0;

                if (genSaveString(data, len))
                {
                    int ret = write(fd, data, len);
                    if (ret != len)
                    {
                        GEN_Printf(LOG_ERROR, "[%d == %d] write failed, %s", ret, len, strerror(errno));
                    }

                }

                close(fd);
                if (data)
                {
                    delete [] data;
                }
            } else
            {
                GEN_Printf(LOG_ERROR, "open write failed, %s", strerror(errno));
            }
            return true;
        }
        return false;
    }

    ListTable<save_node> &nodes()
    {
        return mNodes;
    }

    bool isNeedLoad()
    {
        return mNodes.empty();
    }

    /**
     * @brief genResult
     * @details 解析json格式的数据，数据来源于本地文件
     */
    virtual void genResult(const char */*data*/, unsigned long /*size*/) {}

    /**
     * @brief genSaveString
     * @return 成功生成有效数据，就返回真
     * @details 把当前nodes生成json格式的字串然后保存起来
     */
    virtual bool genSaveString(char *& /*data*/, unsigned long &/*len*/) { return false; }

    void setSaveFile(const char *saveFile)
    {
        mSaveFile = saveFile;
    }
protected:
    const char          *mSaveFile;
    ListTable<save_node> mNodes;
};

#endif // SAVE_MANAGE_OBJECT_H
