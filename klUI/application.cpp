#include <locale.h>
#include <stdarg.h>
#include "events/common_log.h"
#include "events/app_timer.h"
#ifdef USE_MPV_API_INTERFACE
#include "fsm/sf_state_mache.h"
#include "mpv_player_proc.h"
#include "iface/media_notify.h"
#endif
#include "app_common.h"
#include "config_local_info.h"
#include "kl_url/kl_active_manage.h"
#include "kl_url/kl_init_manage.h"
#include "util/config_setting.h"
#include "model/kl_data_proc.h"
//#include "model/kl_local_data_proc.h"
#include "pop_tip_manage.h"
#include "kl_collect_manage.h"
#include "kl_download_manage.h"
#include "kl_record_manage.h"
#include "kl_search_manage.h"
#include "current_backup.h"
#include "qq_ip_positioning.h"
#include "application.h"

Application::Application()
    : m_pKLInit(NULL), m_pKLActive(NULL)
    , m_pPositionArea(new kl::AreaItem)
{
#ifdef USE_MPV_API_INTERFACE
    setlocale(LC_NUMERIC, "C");
#endif
}

void Application::initialize()
{    
    if (!LocalConfig::instance()->init())
    {
        m_pKLInit = new kl::InitManage;

        m_pKLInit->obtain();
    } else
    {
#ifdef USE_MPV_API_INTERFACE
        stateMache->initialize();
#endif
    }
    //KLDataProc::instance()->initSockService();

    // 启动收数据线程，并连接播放服务端socket
    //postCmd(SIG_SOCKET_CLIENT_MSG_EXIT);

    SimpleThread::start();

    m_pPositionArea->id   = LocalConfig::instance()->getValue("location", "id");
    m_pPositionArea->name = LocalConfig::instance()->getValue("location", "name");
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
#ifdef USE_MPV_API_INTERFACE
            stateMache->initialize();
#endif
            break;
#ifdef USE_MPV_API_INTERFACE
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
#endif
        case SIG_SOCKET_CLIENT_MSG_EXIT:
            KLDataProc::instance()->initMedia();
            break;
        case SIG_KL_INIT_ERROR:
            klInitActiveManage((GeneralQEvt *)evt);
            break;

        case SIG_KL_CURRENT_IS_COLLECT:
            kl::CollectManage::instance()->checkCurrentItem((kl::RecordItem *)(((GeneralQEvt *)evt)->wParam));
            break;

        case SIG_KL_CURRENT_OP_COLLECT:
            kl::CollectManage::instance()->opCurrentItem((kl::RecordItem *)(((GeneralQEvt *)evt)->wParam));
            break;

        case SIG_KL_BDC_OP_COLLECT:
            kl::CollectManage::instance()->opBDCItem((kl::RecordItem *)(((GeneralQEvt *)evt)->wParam), ((GeneralQEvt *)evt)->lParam);
            break;

        case SIG_KL_RECORD_CURRENT_PLAY:
            kl::RecordManage::instance()->addHistoryItem((kl::RecordItem *)(((GeneralQEvt *)evt)->wParam));
            break;

        case SIG_KL_HISTORY_CLEAR_APP:
            kl::RecordManage::instance()->historyClear();
            break;

        case SIG_KL_SEARCH_RESULT_PROC:
            kl::SearchManage::instance()->onSearchResult((kl::VoiceSearchAll *)(((GeneralQEvt *)evt)->pHander),
                                                         ((GeneralQEvt *)evt)->wParam);
            break;

        case SIG_KL_COLLECT_ERR_OBJECT:
            mErrObj.push_back(reinterpret_cast<kl::KLObject *> (((GeneralQEvt *)evt)->wParam));
            break;

        case SIG_KL_RELOAD_ERR_OBJECT:
        {
            ListTable<kl::KLObject *>::iterator it = mErrObj.begin();
            for ( ; it != mErrObj.end(); ++it)
            {
                bool ret = (*it)->obtain();
                PopTipManage::instance()->errReloadStart(ret, (*it)->objectName());
            }
            mErrObj.clear();
            break;
        }

        case SIG_LOCATION_POSITIONING:
            positioningManage(((GeneralQEvt *)evt)->wParam);
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

void Application::poweroff()
{
    GEN_Printf(LOG_DEBUG, "Save local record.");
    kl::DownloadManage::instance()->saveNodesFile();
    kl::CollectManage::instance()->saveNodesFile();
    kl::RecordManage::instance()->saveNodesFile();
    CurrentBackup::instance()->saveCurrent();
}

void Application::klInitActiveManage(GeneralQEvt *evt)
{
    GEN_Printf(LOG_DEBUG, "klInitActiveManage type: %ld", evt->wParam);
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
    mKlBack.clear();
}

void Application::positioningManage(long qqPtr)
{
    kl::AreaItem    area;
    QQIPPositioning *ipPos = reinterpret_cast<QQIPPositioning *>(qqPtr);

    if (ipPos->locationValid())
    {
        if (KLDataProc::instance()->locationConfirm(ipPos->province(), area)
                && !(m_pPositionArea->id == area.id
                     && m_pPositionArea->name == area.name))
        {
            LocalConfig::instance()->setValue("location", "id", area.id);
            LocalConfig::instance()->setValue("location", "name", area.name);
            LocalConfig::instance()->save();
            m_pPositionArea->id   = LocalConfig::instance()->getValue("location", "id");
            m_pPositionArea->name = LocalConfig::instance()->getValue("location", "name");

            KLDataProc::instance()->notifyLocationChange(m_pPositionArea);
        }
    } else
    {
        GEN_Printf(LOG_ERROR, "Locationing reason: %s!!!", ipPos->message().string());
        KLDataProc::instance()->notifyLocationFailed();
    }

    delete ipPos;
}
