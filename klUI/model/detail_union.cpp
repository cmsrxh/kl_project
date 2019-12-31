#include "events/common_log.h"
#include "kl_url/kl_album_detail.h"
#include "kl_url/kl_broadcast_item_detail.h"
#include "kl_url/kl_chip_audio_detail.h"
#include "kl_url/kl_chip_radio_detail.h"
#include "detail_union.h"
#include "detail_qobject.h"
#include "kl_data_proc.h"
#include "kl_ui_proc.h"

extern KLUIProc *gInstance;

DetailUnion::DetailUnion(int type)
    : mDetailType(type)
    , m_pDetail(NULL)
{
    if (0 == type)
    {
        mDetailType = DETAIL_TYPE_ALBUM;
    } else if (3 == type)
    {
        mDetailType = DETAIL_TYPE_RADIO_CHIP;
    } else
    {
        GEN_Printf(LOG_ERROR, "Media Type = %d is not invalid.", type);
        assert(0);
    }
}

void DetailUnion::loadDetail(const ByteString &id)
{
    switch (mDetailType)
    {
    case DETAIL_TYPE_ALBUM:
    {
        kl::AlbumDetail *album;
        if (m_pDetail)
        {
            album = ((kl::AlbumDetail *)m_pDetail);
        } else
        {
            album = new kl::AlbumDetail(id);
            m_pDetail = album;
        }
        album->setUINotify(this);
        album->obtain();
        break;
    }
    case DETAIL_TYPE_BROADCAST:
    {
        ((kl::BroadcastItemDetail *)m_pDetail);
        GEN_Printf(LOG_WARN, "Not come true");
        break;
    }
    case DETAIL_TYPE_AUDIO_CHIP:
    {
        kl::ChipAudioDetail *audio;
        if (m_pDetail)
        {
            audio = ((kl::ChipAudioDetail *)m_pDetail);
        } else
        {
            audio = new kl::ChipAudioDetail(id);
            m_pDetail = audio;
        }

        audio->setUINotify(this);
        audio->obtain();
        break;
    }
    case DETAIL_TYPE_RADIO_CHIP:
    {
        kl::ChipRadioDetail *radio;
        if (m_pDetail)
        {
            radio = ((kl::ChipRadioDetail *)m_pDetail);
        } else
        {
            radio = new kl::ChipRadioDetail(id);
            m_pDetail = radio;
        }

        radio->setUINotify(this);
        radio->obtain();
        break;
    }
    default:
        break;
    }
}

void DetailUnion::dataPrepare()
{
    GEN_Printf(LOG_DEBUG, "data prepared, mDetailType=%d", mDetailType);
    Q_EMIT DetailQobject::instance()->dataLoadOver((long)this);
}

void DetailUnion::errorInfo(int type, const char *err_str)
{
    // GEN_Printf(LOG_DEBUG, "Cate Item List Error, %s", err_str);
    Q_EMIT DetailQobject::instance()->loadError(type, QStringFromCString(err_str));
}

void DetailUnion::getDetail(MusicDetail &detail)
{
    GEN_Printf(LOG_DEBUG, "Get Detail info: %d", mDetailType);
    switch (mDetailType)
    {
    case DETAIL_TYPE_ALBUM:
    {
        kl::AlbDetail &item = ((kl::AlbumDetail *)m_pDetail)->item();

        detail.id = item.id;
        detail.hostName = item.host;
        detail.name = item.name;
        detail.image = item.img;
        detail.desc = item.desc;
        detail.keywords = &item.keyWords;
        break;
    }
    case DETAIL_TYPE_BROADCAST:
    {
        kl::BDCastDetail &item = ((kl::BroadcastItemDetail *)m_pDetail)->item();

        detail.id = item.mBroadcastId;
//        detail.hostName = ByteString();
        detail.name = item.mName;
        detail.image = item.mImg;
//        detail.desc = item.desc;
//        detail.keywords = item.keyWords;
        break;
    }
    case DETAIL_TYPE_AUDIO_CHIP:
    {
        kl::AudioItem &item = ((kl::ChipAudioDetail *)m_pDetail)->item();

        detail.id = item.audioId;
//        detail.hostName = item.host;
        detail.name = item.audioName;
        detail.image = item.audioPic;
        detail.desc = item.audioDes;
//        detail.keywords = item.keyWords;
        break;
    }
    case DETAIL_TYPE_RADIO_CHIP:
    {
        kl::RadioDetail &item = ((kl::ChipRadioDetail *)m_pDetail)->item();
        detail.id = item.id;
        detail.hostName = item.host;
        detail.name = item.name;
        detail.image = item.img;
        detail.desc = item.desc;
        detail.keywords = &item.keyWords;
        break;
    }
    default:
        break;
    }
}

/*
    switch (mDetailType)
    {
    case DETAIL_TYPE_ALBUM:
        ((kl::AlbumDetail *)m_pDetail);
        break;
    case DETAIL_TYPE_BROADCAST:
        ((kl::BroadcastItemDetail *)m_pDetail);
        break;
    case DETAIL_TYPE_AUDIO_CHIP:
        ((kl::ChipAudioDetail *)m_pDetail);
        break;
    case DETAIL_TYPE_RADIO_CHIP:
        ((kl::ChipRadioDetail *)m_pDetail);
        break;
    default:
        break;
    }
*/
