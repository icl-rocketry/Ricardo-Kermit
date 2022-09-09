/* 
state for groundstation use as reciever
*/
#pragma once

#include "state.h"

#include "rnp_routingtable.h"

class GroundstationGateway: public State {
    public:
        GroundstationGateway(stateMachine* sm);
        void initialise();
        State* update();
        void exitstate();
    private:


};
