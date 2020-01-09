/*************************************************************************
	> File Name:    application.h
	> Author:       陈猛
	> Mail:         chen-meng@hongjing-wh.com
    > Created Time: 2019年07月26日 星期四 10时48分29秒
    > Description:  所有App的写作模板
 ************************************************************************/
#ifndef _APPLICATION_H_
#define _APPLICATION_H_

#include "events/sf_mutex.h"
#include "events/sf_loop.h"
#include "app_common.h"
#include "events/sf_thread.h"
#include "util/list_table.h"

namespace kl
{
class InitManage;
class ActiveManage;
class KLObject;
}
class Application : public SFLoop
        #ifdef PLATFORM_HMI
        , public SimpleThread
        #endif
{
public:
    enum {
        SIG_TEST_SEARCH_LOAD_OVER = 2000,
        SIG_TEST_AUDIO_DETAIL_LOAD_OVER,
        SIG_TEST_AUDIO_DETAIL_LIST_LOAD_OVER,
    };

    inline static Application *instance()
    {
        static Application i;
        return &i;
    }

    void    initialize();

    void    runLoop() NO_RETUEN;

#ifdef PLATFORM_HMI
    void run()
    {
        runLoop();
    }
#endif
    void postKlEvent(int cmd, int ext1 = 0, int ext2 = 0, const char *str = 0);

    void collectObject(kl::KLObject *obj)
    {
        mKlBack.push_back(obj);
    }
protected:
    void klInitActiveManage(GeneralQEvt *evt);
    void klInitGetOpenId();
private:
    Application();
    Application(Application &);
    Application &operator =(Application &);

    kl::InitManage              *m_pKLInit;
    kl::ActiveManage            *m_pKLActive;
    /**
     * @details 记录当前，在设备没有OpenID的时候那些下载操作，
     * @warning 注意这些下载对象不能释放，否则这里记录的指针就是非法
     * @brief mKlBack
     */
    ListTable<kl::KLObject *>    mKlBack;
};

#define sfApp (Application::instance())

#endif // _APPLICATION_H_

