#include "Arduino.h"

#include "setup.h"
#include "stateMachine.h"

#include "flags.h"


Setup::Setup(stateMachine* sm):
State(sm,SYSTEM_FLAG::STATE_SETUP)
{};

void Setup::initialise(){
    State::initialise(); 


};


State* Setup::update(){
    //transtion to preflight state
    State* _preflight_ptr = new Preflight(_sm);
    return _preflight_ptr;
};

void Setup::exitstate(){
    State::exitstate();
};