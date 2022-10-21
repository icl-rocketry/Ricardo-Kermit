
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
    if (millis() - prevUpdateTime > readDelta){
        prevUpdateTime = millis(); 
        switch (readIndex){
        case 0:
        {
            _sm->loadcell0.update();
            readIndex ++;
            break;
        }
        case 1:
        {
            _sm->ntctemp0.update();
            readIndex ++;
            break;
        }
        case 2:
        {
            _sm->ntctemp1.update();
            readIndex++;
            //Serial.println("Update");
            break;
        }
        case 3:
        {
             _sm->ptap0.update();
             readIndex++;
            break;
        }
        case 4:
        {
            _sm->ptap1.update();
            readIndex = 0;
            break;
        }
        default:
        {
            break;
        }
          

        }
        
    }
    
    return this;//loopy loop
};

void Idle::exitstate(){
    State::exitstate();
};