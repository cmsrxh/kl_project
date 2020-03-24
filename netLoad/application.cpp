#include "events/common_log.h"
#include "app_common.h"
#include "net_util/http_util.h"

#include "application.h"

Application::Application()
{

}


static void test1()
{
    //歌手列表
    ByteString uri = ByteString::allocString("http://u.y.qq.com/cgi-bin/musicu.fcg?g_tk=5381&loginUin=0&hostUin=0&format=jsonp&inCharset=utf8&outCharset=utf-8&notice=0&platform=yqq&needNewCode=0&data=%7B%22comm%22%3A%7B%22ct%22%3A24%2C%22cv%22%3A10000%7D%2C%22singerList%22%3A%7B%22module%22%3A%22Music.SingerListServer%22%2C%22method%22%3A%22get_singer_list%22%2C%22param%22%3A%7B%22area%22%3A-100%2C%22sex%22%3A-100%2C%22genre%22%3A-100%2C%22index%22%3A-100%2C%22sin%22%3A0%2C%22cur_page%22%3A1%7D%7D%7D");
    HttpUtil::urlDecoding(uri);

    //                                        http://u.y.qq.com/cgi-bin/musicu.fcg?g_tk=5381&loginUin=0&hostUin=0&format=jsonp&inCharset=utf8&outCharset=utf-8&notice=0&platform=yqq&needNewCode=0&data={"comm":{"ct":24,"cv":10000},"singerList":{"module":"Music.SingerListServer","method":"get_singer_list","param":{"area":-100,"sex":-100,"genre":-100,"index":-100,"sin":0,"cur_page":1}}}
    //GEN_Printf(LOG_DEBUG, "%s", uri.string());

    ByteString result;
    HttpUtil::urlEncoding(uri, result);

    GEN_Printf(LOG_DEBUG, "%s", result.string());

}

void Application::initialize()
{
    test1();
//    ByteString uri;
//    HttpUtil::urlEncoding("!~ +=46>.://=&?<-A-陈猛-dgdsz.+-*/+-", uri);
//    GEN_Printf(LOG_DEBUG, "%s", uri.string());
//    uri.clear();

    exit(1);
#ifdef PLATFORM_HMI
    SimpleThread::start();
#endif
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



