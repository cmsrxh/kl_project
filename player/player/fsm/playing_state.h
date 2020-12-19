#ifndef PLAYING_STATE_H
#define PLAYING_STATE_H

#include "normal_state.h"

class PlayingState : public NormalState
{
public:
    PlayingState();

    int entry(const StateQEvt *);

    int exit(const StateQEvt *);

    int translate(const StateQEvt *);
};

#endif // PLAYING_STATE_H
