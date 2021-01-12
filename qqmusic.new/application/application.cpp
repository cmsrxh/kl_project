#include "app_common.h"
#include <events/common_log.h>
#include <events/app_timer.h>
#include "util/config_setting.h"

#include "loadApi/qqmusic_object.h"
#include "loadApi/qqmusic_singer_list.h"
#include "loadApi/qqmusic_category.h"
#include "loadApi/qqmusic_category_playlist.h"
#include "application.h"

Application::Application()
{
}

//OK
char string1[] = "http://c.y.qq.com/splcloud/fcgi-bin/fcg_get_diss_by_tag.fcg?picmid=1&rnd=0.9214369031553655&g_tk=5381&jsonpCallback=getPlaylist&loginUin=0&hostUin=0&format=jsonp&inCharset=utf8&outCharset=utf-8&notice=0&platform=yqq&needNewCode=0&categoryId=10000000&sortId=5&sin=0&ein=29";
//ERROR
char string2[] = "http://c.y.qq.com/splcloud/fcgi-bin/fcg_get_diss_by_tag.fcg?picmid=1?rnd=0.9214369031553655&g_tk=5381&jsonpCallback=getPlaylist&loginUin=0&hostUin=0&format=jsonp&inCharset=utf8&outCharset=utf-8&notice=0&platform=yqq&needNewCode=0&categoryId=10000000&sortId=5&sin=0&ein=29";


void Application::initialize()
{
    srand(time(NULL));
//    for (int i = 0; i < sizeof (string1); ++i)
//    {
//        if (string1[i] != string2[i])
//        {
//            GEN_Printf(LOG_DEBUG, "[%d] %s", i, string1 + i);
//            break;
//        }
//    }
//    GEN_Printf(LOG_DEBUG, "Result=%d", strcmp(string1, string2));
//    static qqmusic::SingerList singer;
//    singer.obtain();

//    static qqmusic::Category cate;
//    cate.obtain();
//    sleep(1);
//    static qqmusic::CategoryPlaylist pl(10000000);
//    pl.obtain();


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
            GEN_Printf(LOG_DEBUG, "Ipc New[%ld] Instance connect.", sockHandler);
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

void Application::poweroff()
{
    GEN_Printf(LOG_DEBUG, "Save local record.");
}
