#ifndef KL_ALBUM_AUDIO_DETAIL_H
#define KL_ALBUM_AUDIO_DETAIL_H

#include "kl_item_object.h"

namespace kl
{
class ChipAudioDetail : public ItemObject<AudioItem>
{
public:
    ChipAudioDetail(const ByteString &aid);
    ~ChipAudioDetail();

    NetUrl &genQueryUrl();

    void profile();

    void genResult(NetBuffer *data);

private:
    ByteString mAudioId;
};
}

#endif // KL_ALBUM_AUDIO_DETAIL_H
