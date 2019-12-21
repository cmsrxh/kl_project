#include "events/common_log.h"
#include "app_common.h"
#include "net_util/load_item.h"
#include "net_util/http_util.h"
#include "kl_url/kl_active_manage.h"
#include "kl_url/kl_init_manage.h"
#include "application.h"

Application::Application()
{
}

void Application::initialize()
{
//    ByteString result;
//    HttpUtil().urlEncoding("陈猛", result);
//    GEN_Printf(LOG_INFO, "string: %s", result.string());

//    kl::ActiveManage act;

//    act.obtain();


    // kl::InitManage init;
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



