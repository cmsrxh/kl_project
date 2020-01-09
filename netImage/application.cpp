#include <locale.h>
#include <stdarg.h>
#include "events/common_log.h"
#include "events/app_timer.h"

#include "app_common.h"
#include "config_local_info.h"
#include "kl_url/kl_active_manage.h"
#include "kl_url/kl_init_manage.h"
#include "util/config_setting.h"
#include "kl_collect_manage.h"
#include "kl_download_manage.h"
#include "kl_record_manage.h"
#include "application.h"

Application::Application()
    : m_pKLInit(NULL), m_pKLActive(NULL)
    , m_pIpcSockLanuch(new ATimer)
{
    setlocale(LC_NUMERIC, "C");

    m_pIpcSockLanuch->setRepeat(false);
    m_pIpcSockLanuch->setHandler(Application::ipcSockLanuchTimer, nullptr);

}

void Application::initialize()
{    


    SimpleThread::start();
}

void Application::runLoop()
{
    const QEvt *evt;

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

bool Application::postKlEvent(int cmd, long ext1, long ext2, const char *str)
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

void Application::klInitActiveManage(GeneralQEvt *evt)
{
    UNUSED(evt);
}

void Application::klInitGetOpenId()
{
}

void Application::ipcSockLanuchTimer(ATimer *, void *)
{
    Application::instance()->postCmd(SIG_SOCKET_CLIENT_MSG_EXIT);
}



