#include "events/common_log.h"
#include "application.h"
#include "normal_state.h"
#include "playing_state.h"
#include "mpv_player_proc.h"
#include "sf_state_mache.h"

using namespace state;

SFStateMache::SFStateMache()
    : StateMache(STATE_MAX_DESC, STATE_NORMAL)
{
    registStates(new NormalState, "NormalState", 0);
    registStates(new PlayingState, "PlayingState", 0);
}

SFStateMache::~SFStateMache()
{

}

void SFStateMache::postEvent(int type, long arg1, long arg2, const char *arg3)
{
    StateQEvt *evt = (StateQEvt *)Application::newEvt(SIG_STATE_EVENT_PROC, sizeof(StateQEvt));

    if(evt)
    {
        evt->stateMsg = type;
        evt->wParam   = arg1;
        evt->lParam   = arg2;
        evt->pHander  = arg3 ? strdup(arg3) : NULL;

        Application::instance()->post(evt);
    } else
    {
        GEN_Printf(LOG_ERROR, "Event Queue is full.");
    }
}

bool SFStateMache::test_event(int evt)
{
    bool ret = false;
    switch (evt)
    {
    case state::STATE_EVENT_SET_FILE:
        ret = true;
        break;
    default:
        GEN_Printf(LOG_WARN, "test_event: %d is not test", evt);
        break;
    }
    return ret;
}

bool SFStateMache::test_state(int st)
{
    bool ret = false;
    switch (st)
    {
    case state::STATE_PLAYING:
        ret = !MPVPlayerProc::instance()->isIdle();
        break;

    default:
        GEN_Printf(LOG_WARN, "test_state: %d is not test", st);
        break;
    }
    return ret;
}


