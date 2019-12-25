#ifndef KL_ALBUM_AUDIO_LIST_H
#define KL_ALBUM_AUDIO_LIST_H

#include "kl_save_object.h"

namespace kl
{
class ChipAudioList : public SaveObject<AudioItem>
{
public:
    /**
     * @brief ChipAudioList
     * @param aid [in] 碎片ID long long type
     * @param sorttype [in] 排序方式, 1是正序，0时反序
     * @param pagenum [in] 第几页，1为开始
     * @param pagesize [in]
     */
    ChipAudioList(const ByteString &aid, int sorttype = 1, int pagenum = 1, int pagesize = 20);
    ~ChipAudioList();

    NetUrl &genQueryUrl();

    void profile();

    void genResult(const char *data, unsigned long size);

private:
    ByteString mAudioId;
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
#endif // KL_ALBUM_AUDIO_LIST_H
