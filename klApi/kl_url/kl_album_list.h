#ifndef KL_ALBUM_LIST_H
#define KL_ALBUM_LIST_H

#include "kl_save_object.h"

namespace kl
{
class AlbumList : public SaveObject<AlbumItem>
{
public:
    /**
     * @brief AlbumList
     * @param cid [in] 专辑ID
     * @param sorttype [in]  排序方式, 1是正序，0时反序
     * @param pagenum [in] 第几页，1为开始
     * @param pagesize [in]
     */
    AlbumList(int cid, int sorttype = 1, int pagenum = 1, int pagesize = 20);
    AlbumList(const ByteString &cid, int sorttype = 1, int pagenum = 1, int pagesize = 20);
    ~AlbumList();

    NetUrl &genQueryUrl();

    void profile();

    void genResult(const char *data, unsigned long size);

    int getHaveNext() const
    {
        return haveNext;
    }

    int getPage() const
    {
        return page;
    }

private:
    ByteString mCID;
    ByteString mSortType;
    ByteString needPage;
    ByteString needPageSize;


    int page;
    int pageSize;
    int count;
    int totalPages;
    int haveNext;
    int nextPage;
    int havePre;
    int prePage;
};
}
#endif // KL_ALBUM_LIST_H
