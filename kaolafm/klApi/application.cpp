#include "events/common_log.h"
#include "app_common.h"
#include "config_local_info.h"
#include "net_util/load_item.h"
#include "net_util/http_util.h"
#include "kl_url/kl_active_manage.h"
#include "kl_url/kl_init_manage.h"
#include "kl_url/kl_broadcast_area_list.h"
#include "kl_url/kl_category_broadcast.h"
#include "kl_url/kl_category_sub_list.h"
#include "kl_url/kl_broadcast_item_list.h"
#include "kl_url/kl_broadcast_item_detail.h"
#include "kl_url/kl_broadcast_item_programlist.h"
#include "kl_url/kl_category_all.h"
#include "kl_url/kl_operate_list.h"
#include "kl_url/kl_type_radio_list.h"
#include "kl_url/kl_chip_audio_detail.h"
#include "kl_url/kl_chip_audio_list.h"
#include "kl_url/kl_chip_radio_detail.h"
#include "kl_url/kl_chip_radio_list.h"
#include "kl_url/kl_album_detail.h"
#include "kl_url/kl_album_list.h"
#include "kl_url/kl_res_suggestion_word.h"
#include "kl_url/kl_res_voice_search_all.h"
#include "kl_url/kl_ip_positioning.h"
#include "qq_ip_positioning.h"
#include "test_album_load.h"
#include "util/net_buffer.h"
#include "util/config_setting.h"
#include "application.h"

Application::Application()
{
}

void Application::initialize()
{
//    ByteString result;
//    HttpUtil().urlEncoding("陈猛", result);
//    GEN_Printf(LOG_INFO, "string: %s", result.string());

#if 0
    NetUrl url("www.baidu.com");

    url.append("a", "b");
    url.append("b", "b");
    url.append("c", "b");

    url.appendChange("a", "d");
    url.appendChange("b", "e");

    GEN_Printf(LOG_DEBUG, "%s", url.genUrl().string());
    exit(1);

#endif
#if 0
        ByteString queuy = ByteString::allocString("%7B%22category%22%3A%22%22%2C%22albumName%22%3A%22%22%2C%22artist%22%3A%22%22%2C%22audioName%22%3A%22%22%2C%22field%22%3A2%2C%22tag%22%3A1%2C%22text%22%3A%22%E9%9F%B3%E4%B9%90%22%2C%22keyword%22%3A%22%E9%9F%B3%E4%B9%90%22%7D");
        HttpUtil::urlDecoding(queuy);

        GEN_Printf(LOG_DEBUG, "%s", queuy);
        queuy.clear();
#endif

    if(!LocalConfig::instance()->init())
    {
        static  kl::InitManage init;

//        GEN_Printf(LOG_DEBUG, "deviceID: %s", LocalConfig::instance()->deviceID().string());
//        GEN_Printf(LOG_DEBUG, "appID: %s", LocalConfig::instance()->appID().string());
//        GEN_Printf(LOG_DEBUG, "secretKey: %s", LocalConfig::instance()->secretKey().string());
//        GEN_Printf(LOG_DEBUG, "openID: %s", LocalConfig::instance()->openID().string());

        init.obtain();
    } else
    {
//        static kl::ActiveManage act;
//        act.obtain();

//        static kl::BroadcastAreaList area;
//        area.obtain();
//        area.loadAreaFile();
//        area.profile();

//        static kl::CategoryBroadcast cate;
//        cate.obtain();

//        static kl::BroadcastItemList bdcitems(2, 16);
//        bdcitems.obtain();

//        static kl::BroadcastItemDetail bdcdetail("1600000000747");
//        bdcdetail.obtain();

//        static kl::BroadcastItemProgramlist program("1600000000510");
//        program.obtain();

//        static kl::CategoryAll all;
//        all.obtain();

//        static kl::CategorySublist sub("-1");
//        sub.obtain();

//        static kl::OperateList operate(1, 20);
//        operate.obtain();

//        static kl::TypeRadioList typeradio;
//        typeradio.obtain();

//        static kl::AlbumDetail albumDetail("1100000048156");
//        albumDetail.obtain();

//        static kl::AlbumList albumList(2085);
//        albumList.obtain();

//        static kl::ChipAudioDetail audioDe("1000013097452");
//        audioDe.obtain();

//        static kl::ChipAudioList audioList("1100000048156");
//        audioList.obtain();

//        static kl::ChipRadioDetail radioDe("1200000000162");
//        radioDe.obtain();

//        static kl::ChipRadioList radioList("1200000000162");
//        radioList.obtain();

//        static kl::VoiceSearchAll search("张学友");

//        search.obtain();

//        static kl::SuggestionWord suggest("chen");

//        suggest.obtain();

//        static TestAlbumLoad test;

//        test.obtain();

//        static kl::IPPositioning ipPosition;
//        ipPosition.obtain();

        static QQIPPositioning ipPostion;
        ipPostion.obtain();
    }
}

void Application::runLoop()
{
    const QEvt *evt;

    initThreadPrivate();

    GEN_Printf(LOG_DEBUG, "event loop is started");
    while(getMessage(evt))
    {
        switch (evt->sig)
        {
        case SIG_TEST_SEARCH_LOAD_OVER:
            gTestLoad->loadAllAlbumInfo();
            break;
        case SIG_TEST_AUDIO_DETAIL_LOAD_OVER:
            gTestLoad->totalLoadDeatailInfo();
            break;
        case SIG_TEST_AUDIO_DETAIL_LIST_LOAD_OVER:
            gTestLoad->detailListLoadOver();
            break;
        case SIG_HAVE_OPEN_ID:
            klInitGetOpenId();
            break;
        case SIG_KL_INIT_ERROR:
            klInitActiveManage((GeneralQEvt *)evt);
            break;
        case SIG_KL_LOAD_DATA_EXCEPT:
            klLoadDataExceptProc((GeneralQEvt *)evt);
            break;
        case SIG_USER_UNUSED:
        default:
            GEN_Printf(LOG_WARN, "[%d] is UNKOWN.", evt->sig);
            break;
        }


        recycleEvent(evt);
    }

    GEN_Printf(LOG_WARN, "Event Loop is Exit.");
    exit(EXIT_SUCCESS);
}

bool Application::postKlEvent(int cmd, int ext1, int ext2, const char *str)
{
    bool ret = false;
    GeneralQEvt *e = (GeneralQEvt *)newEvt(cmd, sizeof(GeneralQEvt));

    if (e)
    {
        e->wParam = ext1;
        e->lParam = ext2;
        e->pHander = (void *)str;
        ret = post(e);
    } else
    {
        GEN_Printf(LOG_ERROR, "Post Kl Cmd failed.");
    }
    return ret;
}

void Application::klInitActiveManage(GeneralQEvt *evt)
{
    switch (evt->wParam)
    {
    case 1: // 考拉调用init，加载OpenId信息的json解析有错
        // 有可能需要调用Active，来激活设备
        if (!m_pKLActive)
        {
            m_pKLActive = new kl::ActiveManage;
        }
        m_pKLActive->obtain();

        break;
    case 2: // 考拉调用init，加载数据失败
    case 3: // 考拉调用active，加载数据失败
        GEN_Printf(LOG_ERROR, "init or active failed, %s", (char *)evt->pHander);
        break;

    default:
        break;
    }
}

void Application::klInitGetOpenId()
{
    ListTable<kl::KLObject *>::iterator it = mKlBack.begin();
    for ( ; it != mKlBack.end(); ++it)
    {
        (*it)->obtain();
    }
}

void Application::klLoadDataExceptProc(GeneralQEvt *evt)
{
    NetBuffer *buf = reinterpret_cast<NetBuffer *>(evt->pHander);
    switch (evt->wParam) {
    case kl::KL_DATA_PRISER_EMPTY:
        break;
    case kl::KL_DATA_PRISER_JSOC_ERROR:
        break;
    default:
        break;
    }
    GEN_Printf(LOG_DEBUG, "%s", buf->buffer());
    NetBuffer::unref(buf);
}



