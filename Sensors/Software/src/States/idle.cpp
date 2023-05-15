
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
    if ((millis() - prevUpdateTime) > readDelta){
        prevUpdateTime = millis(); 
        switch (readIndex){
        case 0:
        {
            _sm->loadcell0.update();
            //Serial.println(_sm->ADS0.readAdjusted(10));
            readIndex ++;
            break;
        }
        case 1:
        {
            _sm->ntctemp0.update();
            //Serial.println(_sm->ADS0.readAdjusted(2));
            readIndex ++;
            break;
        }
        case 2:
        {
            _sm->ntctemp1.update();
            //Serial.println(_sm->ADS0.readAdjusted(3));
            readIndex++;
            //Serial.println("Update");
            break;
        }
        case 3:
        {
             _sm->ptap0.update();
             //Serial.println(_sm->ADS2.readAdjusted(1));
             readIndex++;
            break;
        }
        case 4:
        {
            _sm->ptap1.update();
            //Serial.println(_sm->ADS2.readAdjusted(0));
            readIndex = 0;
            break;
        }
        default:
        {
            break;
        }
          

        }
        
    }

    // _sm->TC0.update();
    // _sm->TC1.update();
    // _sm->TC2.update();
    // _sm->TC3.update();
    // _sm->TC4.update();
    // _sm->TC5.update();
    // _sm->TC6.update();
    // _sm->TC7.update();
    _sm->TC8.update();
    Serial.println(_sm->TC8.getTemp());
    delay(200);
    // _sm->TC9.update();
    
    return this;//loopy loop
};

void Idle::exitstate(){
    State::exitstate();
};