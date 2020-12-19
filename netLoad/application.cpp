#include "events/common_log.h"
#include "app_common.h"
#include "net_util/http_util.h"
#include "net_util/test_data_load.h"
#include "application.h"

Application::Application()
{

}

#define TEST1 "http://u.y.qq.com/cgi-bin/musicu.fcg?g_tk=5381&loginUin=0&hostUin=0&format=jsonp&inCharset=utf8&outCharset=utf-8&notice=0&platform=yqq&needNewCode=0&data=%7B%22comm%22%3A%7B%22ct%22%3A24%2C%22cv%22%3A10000%7D%2C%22singerList%22%3A%7B%22module%22%3A%22Music.SingerListServer%22%2C%22method%22%3A%22get_singer_list%22%2C%22param%22%3A%7B%22area%22%3A-100%2C%22sex%22%3A-100%2C%22genre%22%3A-100%2C%22index%22%3A-100%2C%22sin%22%3A80%2C%22cur_page%22%3A2%7D%7D%7D"

// getCategory
#define TEST2 "http://c.y.qq.com/splcloud/fcgi-bin/fcg_get_diss_tag_conf.fcg?g_tk=5381&notice=0&inCharset=utf8&outCharset=utf-8&platform=yqq&needNewCode=0"

//playList
#define TEST3 "http://c.y.qq.com/splcloud/fcgi-bin/fcg_get_diss_by_tag.fcg?picmid=1&rnd=0.5451140774871057&g_tk=5381&jsonpCallback=getPlaylist&loginUin=0&hostUin=0&format=jsonp&inCharset=utf8&outCharset=utf-8&notice=0&platform=yqq&needNewCode=0&categoryId=10000000&sortId=5&sin=0&ein=29"
//            "http://c.y.qq.com/splcloud/fcgi-bin/fcg_get_diss_by_tag.fcg?picmid=1&rnd=${Math.random()}&g_tk=5381&jsonpCallback=getPlaylist&loginUin=0&hostUin=0&format=jsonp&inCharset=utf8&outCharset=utf-8&notice=0&platform=yqq&needNewCode=0&categoryId=${categoryId}&sortId=5&sin=${(page - 1) * 30}&ein=${page * 30 - 1}"


static void test1()
{
    //歌手列表
    ByteString uri = ByteString::allocString(TEST1);
    HttpUtil::urlDecoding(uri);
    GEN_Printf(LOG_DEBUG, "%s", uri.string());
}
static void test2()
{
    //歌手列表
    ByteString uri = ByteString::allocString(TEST2);
    HttpUtil::urlDecoding(uri);
    GEN_Printf(LOG_DEBUG, "%s", uri.string());
}
static void test3()
{
    //歌手列表
    ByteString uri = ByteString::allocString(TEST3);
    HttpUtil::urlDecoding(uri);
    GEN_Printf(LOG_DEBUG, "%s", uri.string());
}

void Application::initialize()
{
//    test1();
//    test2();
//    test3();

//    ByteString uri;
//    HttpUtil::urlEncoding("!~ +=46>.://=&?<-A-陈猛-dgdsz.+-*/+-", uri);
//    GEN_Printf(LOG_DEBUG, "%s", uri.string());
//    uri.clear();

    TestDataLoad *n = new  TestDataLoad;
    LoadItem *i = n;
    GEN_Printf(LOG_DEBUG, "----------------------------");
    delete i;

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



