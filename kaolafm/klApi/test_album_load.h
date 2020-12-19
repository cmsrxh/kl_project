#ifndef TEST_ALBUM_LOAD_H
#define TEST_ALBUM_LOAD_H

#include "kl_url/kl_common.h"
#include "kl_url/kl_chip_audio_detail.h"
#include "kl_url/kl_chip_audio_list.h"
#include "kl_url/kl_chip_radio_detail.h"
#include "kl_url/kl_chip_radio_list.h"
#include "kl_url/kl_album_detail.h"
#include "kl_url/kl_album_list.h"
#include "kl_url/kl_res_suggestion_word.h"
#include "kl_url/kl_res_voice_search_all.h"
#include "kl_download_manage.h"
#include "kl_music_load.h"

class TestAlbumLoad
{
public:
    TestAlbumLoad(const ByteString &keyword = "张学友");
    ~TestAlbumLoad();

    void obtain();

    void loadAllAlbumInfo();

    void totalLoadDeatailInfo();

    void detailListLoadOver();
private:
    int                                mCurrentDetailCount;
    kl::VoiceSearchAll                *m_pSearch;
    kl::DownloadManage                *m_pDownload;
    ListTable<kl::ChipAudioDetail *>   mAudioList;
    ListTable<kl::MusicLoad *>         mMusicList;
};

extern TestAlbumLoad *gTestLoad;

#endif // TEST_ALBUM_LOAD_H
