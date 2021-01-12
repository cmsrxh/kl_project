/*************************************************************************
	> File Name:    application.h
	> Author:       陈猛
	> Mail:         chen-meng@hongjing-wh.com
    > Created Time: 2019年07月26日 星期四 10时48分29秒
    > Description:  所有App的写作模板
 ************************************************************************/
#ifndef _APPLICATION_H_
#define _APPLICATION_H_

#include <events/sf_mutex.h>
#include <events/sf_loop.h>
#include <events/sf_thread.h>
#include <util/set_table.h>
#include "app_common.h"

namespace qqmusic
{
class Object;
}

#define MOBJECT qqmusic::Object

class Application : public SFLoop, public SimpleThread
{
public:
    inline static Application *instance()
    {
        static Application i;
        return &i;
    }
    void initialize();

    void run();

    bool postEvent(int cmd, long ext1 = 0, long ext2 = 0, const char *str = 0);

    void collectObject(MOBJECT *obj)
    {
        mKlBack.push_back(obj);
    }

    void poweroff();

protected:
    void klInitActiveManage(GeneralQEvt *evt);

    void klInitGetOpenId();

    void positioningManage(long qqPtr);
private:
    Application();
    Application(Application &);
    Application &operator =(Application &);

    /**
     * @details 记录当前，在设备没有OpenID的时候那些下载操作，
     * @warning 注意这些下载对象不能释放，否则这里记录的指针就是非法
     * @brief mKlBack
     */
    SetTable<MOBJECT *>    mKlBack;

    /**
     * @brief mErrObj
     * @details 记录当前对象，发生网络错误的时候需要重新加载
     */
    SetTable<MOBJECT *>    mErrObj;


};

#define sfApp (Application::instance())

#endif // _APPLICATION_H_

