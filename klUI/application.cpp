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
    if (!LocalConfig::instance()->init())
    {
        static  kl::InitManage init;
        init.obtain();
    } else
    {

    }

    SimpleThread::start();
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



