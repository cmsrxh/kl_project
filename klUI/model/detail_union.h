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
        DETAIL_TYPE_ALBUM       = 1,    // 专辑二级标签的详情
        DETAIL_TYPE_BROADCAST   = 2,    // 电台二级标签的详情
        DETAIL_TYPE_AUDIO_CHIP  = 3,    // 专辑音乐节目碎片三级标签的详情
        DETAIL_TYPE_RADIO_CHIP  = 4,    // 智能电台节目碎片三级标签的详情
        DETAIL_TYPE_BDC_PROGRAM_CHIP,   // 电台节目碎片三级标签的详情
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
