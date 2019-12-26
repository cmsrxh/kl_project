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

namespace kl
{
class InitManage;
class ActiveManage;
}

class Application : public SFLoop, public SimpleThread
{
public:
    inline static Application *instance()
    {
        static Application i;
        return &i;
    }
    void    initialize();

    void    runLoop() NO_RETUEN;

    void run()
    {
        runLoop();
    }
    void postKlEvent(int cmd, int ext1 = 0, int ext2 = 0, const char *str = 0);

protected:
    void klInitActiveManage(GeneralQEvt *evt);

private:
    Application();
    Application(Application &);
    Application &operator =(Application &);

    kl::InitManage      *m_pKLInit;
    kl::ActiveManage    *m_pKLActive;
};

#define sfApp (Application::instance())

#endif // _APPLICATION_H_

