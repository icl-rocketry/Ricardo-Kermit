/* 
state for groundstation use as reciever
*/
#pragma once

#include "state.h"

#include "rnp_routingtable.h"

class Idle: public State {
    public:
        Idle(stateMachine* sm);
        void initialise();
        State* update();
        void exitstate();
    private:
        uint32_t prevUpdateTime;
        static constexpr int readDelta = 100;
        uint8_t readIndex = 0;


};
