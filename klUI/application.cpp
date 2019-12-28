#include <locale.h>
#include <stdarg.h>
#include "events/common_log.h"
#include "fsm/sf_state_mache.h"
#include "mpv_player_proc.h"
#include "iface/media_notify.h"

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
    : m_pKLInit(NULL), m_pKLActive(NULL)
{
    setlocale(LC_NUMERIC, "C");
}

void Application::initialize()
{    
    if (!LocalConfig::instance()->init())
    {
        m_pKLInit = new kl::InitManage;

        m_pKLInit->obtain();
    } else
    {
        stateMache->initialize();
    }

    SimpleThread::start();
}

void Application::runLoop()
{
    const QEvt *evt;

    setName("Events/Core");
    initThreadPrivate();

    GEN_Printf(LOG_DEBUG, "event loop is started");
    while(getMessage(evt))
    {
        switch (evt->sig)
        {
        case SIG_HAVE_OPEN_ID:
            klInitGetOpenId();
        case SIG_SURFACE_CREATED:
            stateMache->initialize();
            MPVPlayerProc::instance()->setSurface((int64_t)0);
            break;
        case SIG_SURFACE_CHANGED:
            GEN_Printf(LOG_DEBUG, "width: %ld, height: %ld",
                       ((GeneralQEvt *)(evt))->lParam,
                       ((GeneralQEvt *)(evt))->wParam);

            break;
        case SIG_SURFACE_DESTROYED:
            GEN_Printf(LOG_INFO, "destory");
            stateMache->postEvent(state::STATE_EVENT_EXIT_PLAYING);
            stateMache->exitState();
            MPVPlayerProc::instance()->setSurface((int64_t)0);
            break;

        case SIG_ON_MPV_EVENT:
            MPVPlayerProc::instance()->onMpvEvents();
            break;

        case SIG_MVP_NEED_RECREARE:
            MPVPlayerProc::instance()->reCreatePlayer();
            break;

        case SIG_SET_SOURCE_PREPARE:
            MediaNotify::instance()->notifyPrepare();
            break;

        case SIG_STATE_EVENT_PROC:
            stateMache->procEvt(evt);
            break;

        case SIG_KL_INIT_ERROR:
            klInitActiveManage((GeneralQEvt *)evt);
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

void Application::postKlEvent(int cmd, int ext1, int ext2, const char *str)
{
    GeneralQEvt *e = (GeneralQEvt *)newEvt(cmd, sizeof(GeneralQEvt));

    if (e)
    {
        e->wParam = ext1;
        e->lParam = ext2;
        e->pHander = (void *)str;
        post(e);
    } else
    {
        GEN_Printf(LOG_ERROR, "Post Kl Cmd failed.");
    }
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



