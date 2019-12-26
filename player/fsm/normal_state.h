#ifndef NORMAL_STATE_H
#define NORMAL_STATE_H

#include "events/sf_state.h"

class NormalState : public SFState
{
public:
    NormalState();

    int entry(const StateQEvt *);

    int exit(const StateQEvt *);

    int translate(const StateQEvt *);
};

#endif // NORMAL_STATE_H
