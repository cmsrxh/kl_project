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

    if(!LocalConfig::instance()->init())
    {
        static  kl::InitManage init;

        GEN_Printf(LOG_DEBUG, "deviceID: %s", LocalConfig::instance()->deviceID().string());
        GEN_Printf(LOG_DEBUG, "appID: %s", LocalConfig::instance()->appID().string());
        GEN_Printf(LOG_DEBUG, "secretKey: %s", LocalConfig::instance()->secretKey().string());
        GEN_Printf(LOG_DEBUG, "openID: %s", LocalConfig::instance()->openID().string());

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

        static kl::AlbumDetail albumDetail("1100000048156");
        albumDetail.obtain();

        static kl::AlbumList albumList(2085);
        albumList.obtain();

        static kl::ChipAudioDetail audioDe("1000013097452");
        audioDe.obtain();

        static kl::ChipAudioList audioList("1100000048156");
        audioList.obtain();

        static kl::ChipRadioDetail radioDe("1200000000162");
        radioDe.obtain();

        static kl::ChipRadioList radioList("1200000000162");
        radioList.obtain();

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



