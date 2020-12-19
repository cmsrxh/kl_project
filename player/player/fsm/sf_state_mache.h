#ifndef SF_STATE_MACHE_H
#define SF_STATE_MACHE_H

#include "events/sf_mutex.h"
#include "state_common.h"
#include "events/event_def.h"
#include "events/state_mache.h"
/* ***************************************************************************
 * 状态就是一个执行过程（流程)，事件是瞬间（或在一个极快的操作流程中）完成的，分为同步或异步
 * 事件:
 *      USB中的插拔、按键按压、互联手机界面上的操作等
 * 状态:
 *      正常状态、本地音乐播放、倒车过程、蓝牙来电、蓝牙音乐、本地电台播放、MCU升级
 *****************************************************************************/
class SFStateMache : public StateMache
{
public:
    static SFStateMache *instance()
    {
        static SFStateMache i;
        return &i;
    }
    ~SFStateMache();

    void postEvent(int type, long arg1 = 0, long arg2 = 0, const char *arg3 = nullptr);

    void procEvt(const QEvt *e)
    {
        procState((StateQEvt *)e);
    }
protected:
    bool test_event(int evt);
    bool test_state(int state);

private:
    SFStateMache();
    SFStateMache(SFStateMache &);
    SFStateMache &operator= (SFStateMache &);   
};

#define stateMache (SFStateMache::instance())

#endif // SF_STATE_MACHE_H
