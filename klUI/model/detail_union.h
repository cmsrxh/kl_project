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
    DetailUnion(int type);
    virtual ~DetailUnion();

    void loadDetail(const ByteString &id);

    void dataPrepare();

    void errorInfo(int, const char *);

    void getDetail(MusicDetail &detail);

private:
    int          mDetailType;
    UIDetail    *m_pDetail;    
};

#endif // DETAIL_UNION_H
