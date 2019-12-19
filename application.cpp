#include "events/common_log.h"
#include "app_common.h"
#include "net_util/load_item.h"
#include "application.h"
#include "net_util/http_util.h"

Application::Application()
{

}


void Application::initialize()
{

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



