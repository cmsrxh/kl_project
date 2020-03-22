#include "app_common.h"
#include <events/common_log.h>
#include <events/app_timer.h>
#include "util/config_setting.h"
#include "model/kl_data_proc.h"
//#include "model/kl_local_data_proc.h"
#include "pop_tip_manage.h"
#include "current_backup.h"
#include "application.h"

Application::Application()
    : m_pKLInit(NULL), m_pKLActive(NULL)
{
}

void Application::initialize()
{    
    // 启动收数据线程，并连接播放服务端socket
    DataProc::instance()->initMedia();

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
            break;

        case SIG_SOCKET_CLIENT_MSG_EXIT:
            DataProc::instance()->initMedia();
            break;
        case SIG_KL_INIT_ERROR:
            klInitActiveManage((GeneralQEvt *)evt);
            break;

        case SIG_KL_CURRENT_IS_COLLECT:
//            kl::CollectManage::instance()->checkCurrentItem((kl::RecordItem *)(((GeneralQEvt *)evt)->wParam));
            break;

        case SIG_KL_CURRENT_OP_COLLECT:
//            kl::CollectManage::instance()->opCurrentItem((kl::RecordItem *)(((GeneralQEvt *)evt)->wParam));
            break;

        case SIG_KL_BDC_OP_COLLECT:
//            kl::CollectManage::instance()->opBDCItem((kl::RecordItem *)(((GeneralQEvt *)evt)->wParam), ((GeneralQEvt *)evt)->lParam);
            break;

        case SIG_KL_RECORD_CURRENT_PLAY:
//            kl::RecordManage::instance()->addHistoryItem((kl::RecordItem *)(((GeneralQEvt *)evt)->wParam));
            break;

        case SIG_KL_HISTORY_CLEAR_APP:
//            kl::RecordManage::instance()->historyClear();
            break;

        case SIG_KL_SEARCH_RESULT_PROC:
//            kl::SearchManage::instance()->onSearchResult((kl::VoiceSearchAll *)(((GeneralQEvt *)evt)->pHander),
//                                                         ((GeneralQEvt *)evt)->wParam);
            break;

        case SIG_KL_COLLECT_ERR_OBJECT:
            mErrObj.push_back(reinterpret_cast<kl::KLObject *> (((GeneralQEvt *)evt)->wParam));
            break;

        case SIG_KL_RELOAD_ERR_OBJECT:
        {
            ListTable<kl::KLObject *>::iterator it = mErrObj.begin();
            for ( ; it != mErrObj.end(); ++it)
            {
                bool ret = false;//(*it)->obtain();
                PopTipManage::instance()->errReloadStart(ret, 0/*(*it)->objectName()*/);
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

void Application::poweroff()
{
    GEN_Printf(LOG_DEBUG, "Save local record.");
//    kl::DownloadManage::instance()->saveNodesFile();
//    kl::CollectManage::instance()->saveNodesFile();
//    kl::RecordManage::instance()->saveNodesFile();
    CurrentBackup::instance()->saveCurrent();
}

void Application::klInitActiveManage(GeneralQEvt *evt)
{

}

void Application::klInitGetOpenId()
{

}

void Application::positioningManage(long qqPtr)
{

}
