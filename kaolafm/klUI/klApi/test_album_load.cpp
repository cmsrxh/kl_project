#include <events/common_log.h>
#include "application.h"
#include "test_album_load.h"

class SearchResult : public UINotifyIface
{
public:
    void dataPrepare()
    {
        Application::instance()->postKlEvent(Application::SIG_TEST_SEARCH_LOAD_OVER);
    }
    void errorInfo(int type, const ByteString &str)
    {
        GEN_Printf(LOG_ERROR, "ERROR: %d, %s", type, str);
    }
};

class AudioDetailResult: public UINotifyIface
{
public:
    void dataPrepare()
    {
        Application::instance()->postKlEvent(Application::SIG_TEST_AUDIO_DETAIL_LOAD_OVER);
    }
    void errorInfo(int type, const ByteString &str)
    {
        GEN_Printf(LOG_ERROR, "ERROR: %d, %s", type, str);
    }
};

static SearchResult         _gSearchResult;
static AudioDetailResult    _gAudioDetailResult;
TestAlbumLoad *gTestLoad;
TestAlbumLoad::TestAlbumLoad(const ByteString &keyword)
    : mCurrentDetailCount(0)
    , m_pSearch(new kl::VoiceSearchAll(keyword))
{
    m_pDownload = kl::DownloadManage::instance();
    gTestLoad   = this;
    m_pSearch->setUINotify(&_gSearchResult);
}

TestAlbumLoad::~TestAlbumLoad()
{
    delete m_pSearch;
}

void TestAlbumLoad::obtain()
{
#if 1
//    m_pSearch->obtain();
#else
    char buffer[1024];
    ListTable<kl::RecordItem> nodes = m_pDownload->nodes();
    ListTable<kl::RecordItem>::iterator it = nodes.begin();

    for (; it != nodes.end(); ++it)
    {
        sprintf(buffer, "/home/hongjing-cm/Music/%s.mp3", it->name.string());
        if (0 == access(buffer, F_OK))
        {
            sprintf(buffer, "/home/hongjing-cm/Music/%s-%s.mp3", it->name.string(), it->id.string());
        }
        kl::MusicLoad *load = new kl::MusicLoad(it->playUrl, buffer);

        load->setTotalSize(it->fileSize);

        load->obtain();

        mMusicList.push_back(load);
    }
#endif
}

void TestAlbumLoad::loadAllAlbumInfo()
{
    mCurrentDetailCount = 0;
    ListTable<kl::SearchItem> &node = m_pSearch->nodes();
    ListTable<kl::SearchItem>::iterator it = node.begin();
    for (; it != node.end(); ++it)
    {
        GEN_Printf(LOG_INFO, "id: %s, type: %s, album: %s, name: %s", it->id.string(), it->type.string(), it->albumName.string(), it->name.string());
        if (it->type == "1")
        {
            kl::ChipAudioDetail *audioList = new kl::ChipAudioDetail(it->id);
            audioList->setUINotify(&_gAudioDetailResult);
            mAudioList.push_back(audioList);
            audioList->obtain();
        }
    }
}

void TestAlbumLoad::totalLoadDeatailInfo()
{
    int total = mAudioList.size();
    ++mCurrentDetailCount;
    GEN_Printf(LOG_DEBUG, "load count=%d, total=%d", mCurrentDetailCount, total);
    if (mCurrentDetailCount == total)
    {
        Application::instance()->postKlEvent(Application::SIG_TEST_AUDIO_DETAIL_LIST_LOAD_OVER);
    }
}

void TestAlbumLoad::detailListLoadOver()
{
    ListTable<kl::ChipAudioDetail *>::iterator it = mAudioList.begin();
    for (; it != mAudioList.end(); ++it)
    {
        kl::ChipAudioDetail *detail = *it;
        GEN_Printf(LOG_INFO, "name: %s, size: %s, url: %s", detail->item().audioName.string(),
                   detail->item().mp3FileSize64.string(), detail->item().mp3PlayUrl64.string());

        kl::RecordItem *item = new kl::RecordItem;
        item->type       = 0;
        item->id         = ByteString::allocString(detail->item().audioId);
        item->parentId   = ByteString::allocString(detail->item().albumId);
        item->name       = ByteString::allocString(detail->item().audioName);
        item->parentName = ByteString::allocString(detail->item().albumName);
        item->image      = ByteString::allocString(detail->item().audioPic);
        item->playUrl    = ByteString::allocString(detail->item().mp3PlayUrl64);
        item->fileSize   = ByteString::allocString(detail->item().mp3FileSize64);

        m_pDownload->saveItem(item);
    }

    m_pDownload->saveNodesFile();
}
