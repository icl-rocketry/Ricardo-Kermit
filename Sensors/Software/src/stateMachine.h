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
#include "MAX31856/MAX31856.h"

#include "SPI.h"
#include "Wire.h"

#include "LoadCell/LoadCell.h"
#include "NTCThermistor/NTCthermistor.h"
#include "PTransducer/PTransducer.h"

class stateMachine {
  
  public:
    stateMachine();// The constructor
    //functions
    void initialise(State* initStatePtr);
    void update();
    void changeState(State* newStatePtr);

   
    SPIClass vspi;
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

    LoadCell loadcell0;
    PTransducer ptap0;
    PTransducer ptap1;
    NTCThermistor ntctemp0;
    NTCThermistor ntctemp1;

    MAX31856 TC0;
    MAX31856 TC1;
    MAX31856 TC2;
    MAX31856 TC3;
    MAX31856 TC4;
    MAX31856 TC5;
    MAX31856 TC6;
    MAX31856 TC7;
    MAX31856 TC8;
    MAX31856 TC9;


  private:
    State* _currStatePtr;

    static constexpr int default_address = 120;
};

