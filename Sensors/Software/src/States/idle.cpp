
#include "idle.h"
#include "stateMachine.h"

#include <rnp_default_address.h>
#include <rnp_routingtable.h>


#include "flags.h"

Idle::Idle(stateMachine* sm):
State(sm,SYSTEM_FLAG::STATE_GROUNDSTATION_GATEWAY)
{};

void Idle::initialise(){
    State::initialise();
   
    
};

State* Idle::update(){
    _sm->loadcell0.update();
    _sm->ntctemp0.update();
    _sm->ntctemp1.update();
    _sm->ptap0.update();
    _sm->ptap1.update();
    
    return this;//loopy loop
};

void Idle::exitstate(){
    State::exitstate();
};