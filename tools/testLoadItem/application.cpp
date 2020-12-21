#include <events/common_log.h>
#include "kl_ip_positioning.h"
#include "app_common.h"
#include "application.h"

Application::Application()
{

}

void Application::initialize()
{
    (new kl::IPPositioning)->obtain();

    SimpleThread::start();
}

void Application::run()
{
    const QEvt *evt;

    initThreadPrivate();

    GEN_Printf(LOG_DEBUG, "event loop is started");
    while(getMessage(evt))
    {
        switch (evt->sig)
        {        
        case SIG_IPC_NOTIFY_HANDLER_ATTACH:
        {
            long sockHandler = ((GeneralQEvt*)evt)->wParam;
                    
            break;
        }
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

bool Application::postEvent(int cmd, long ext1, long ext2, const char *str)
{
    GeneralQEvt *e = (GeneralQEvt *)newEvt(cmd, sizeof(GeneralQEvt));

    if (e)
    {
        e->wParam = ext1;
        e->lParam = ext2;
        e->pHander = (void *)str;
        return post(e);
    } else
    {
        GEN_Printf(LOG_ERROR, "Post Kl Cmd failed.");
        return false;
    }
}


