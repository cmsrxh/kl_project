#ifndef DETAIL_UNION_H
#define DETAIL_UNION_H

#include "ui_data_union.h"
#include "pop_tip_manage.h"

// AlbumDetail
// BroadcastItemDetail
// ChipAudioDetail
// ChipRadioDetail
class DetailUnion : public UINotifyIface
{
public:
    // 下载动作
    enum {
        LOAD_DETAIL_SHOW_IN_ALBUM_VIEW = PopTipManage::LOAD_DETAIL_SHOW_IN_ALBUM_VIEW,     // 显示在在专辑信息界面中的
        LOAD_DETAIL_AUDIO_PLAYING = PopTipManage::LOAD_DETAIL_AUDIO_PLAYING,          // 下载专辑碎片并进行播放
    };
    DetailUnion(int type);
    virtual ~DetailUnion();

    void loadDetail(const ByteString &id, int loadAction);

    void dataPrepare();

    void errorInfo(int, const ByteString &);

    void getDetail(MusicDetail &detail);

    int getLoadAction() const
    {
        return mLoadAction;
    }

    int getChipType() const
    {
        return mDetailType;
    }
private:
    int          mDetailType;
    int          mLoadAction;
    UIDetail    *m_pDetail;    
};

#endif // DETAIL_UNION_H
