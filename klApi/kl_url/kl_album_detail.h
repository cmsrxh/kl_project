#ifndef KL_ALBUM_DETAIL_H
#define KL_ALBUM_DETAIL_H

#include "kl_item_object.h"

namespace kl
{
class AlbumDetail : public ItemObject<AlbDetail>
{
public:
    /**
     * @brief AlbumDetail
     * @param albumId [in] 专辑ID long long
     * @details 根据专辑的ID号码，获取它的详细情况
     */
    AlbumDetail(const ByteString &albumId);
    ~AlbumDetail();

    NetUrl &genQueryUrl();

    void profile();

    void genResult(const char *data, unsigned long size);

public:
    ByteString mAlbumId;
};
}

#endif // KL_ALBUM_DETAIL_H
