/* 
Code used to process states, and the transitions between them, contains parent class for states

Written by the Electronics team, Imperial College London Rocketry
*/

#pragma once

#include "Arduino.h"

#include "States/state.h"

#include "Storage/systemstatus.h"
#include "Storage/logController.h"

#include "Network/interfaces/usb.h"
#include "Network/interfaces/canbus.h"

#include "rnp_networkmanager.h"
#include "rnp_routingtable.h"

#include "Commands/commandHandler.h"
#include "Sensors/SensorStructs.h"
#include "ADS1219/ADS1219.h"

#include "SPI.h"
#include "Wire.h"


class stateMachine {
  
  public:
    stateMachine();// The constructor
    //functions
    void initialise(State* initStatePtr);
    void update();
    void changeState(State* newStatePtr);

   
    SPIClass vspi;
    SPIClass hspi;
    TwoWire I2C;

    USB usbserial;
    CanBus canbus;
    RnpNetworkManager networkmanager;
    CommandHandler commandhandler;

    LogController logcontroller;
    SystemStatus systemstatus;

    ADS1219 ADS0;
    ADS1219 ADS1;
    ADS1219 ADS2;

  private:
    State* _currStatePtr;

    static constexpr int default_address = 120;
};

