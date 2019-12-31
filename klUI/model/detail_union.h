#ifndef DETAIL_UNION_H
#define DETAIL_UNION_H

#include "kl_ui_data_union.h"

// AlbumDetail
// BroadcastItemDetail
// ChipAudioDetail
// ChipRadioDetail
class DetailUnion : public kl::UINotifyIface
{
public:
    enum
    {
        DETAIL_TYPE_ALBUM       = 1,
        DETAIL_TYPE_BROADCAST   = 2,
        DETAIL_TYPE_AUDIO_CHIP  = 3,
        DETAIL_TYPE_RADIO_CHIP  = 4,
    };
    DetailUnion(int type);

    void loadDetail(const ByteString &id);

    void dataPrepare();

    void errorInfo(int, const char *);

    void getDetail(MusicDetail &detail);

private:
    int          mDetailType;
    UIDetail    *m_pDetail;

};

#endif // DETAIL_UNION_H
