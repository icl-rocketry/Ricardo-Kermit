/* 
Inital State machine framework written by Daniele Bella <3
Code used to process states, and the transitions between them, contains parent class for states

Written by the Electronics team, Imperial College London Rocketry
*/

#include "stateMachine.h"
#include <string>
#include <vector>
#include <functional>

#include "board_config.h"
#include "sensor_pins.h"

#include "States/state.h"

#include "Storage/systemstatus.h"

#include "Network/interfaces/usb.h"
#include "Network/interfaces/radio.h"
#include "Network/interfaces/canbus.h"

#include "rnp_networkmanager.h"
#include "rnp_default_address.h"

#include "SPI.h"
#include "Wire.h"

#include <ArduinoJson.h>



stateMachine::stateMachine() : 
    vspi(VSPI),
    hspi(HSPI),
    I2C(0),
    systemstatus(logcontroller),
    usbserial(Serial,systemstatus,logcontroller),
    radio(hspi,systemstatus,logcontroller),
    canbus(systemstatus,logcontroller,3),
    networkmanager(static_cast<uint8_t>(DEFAULT_ADDRESS::ROCKET),NODETYPE::HUB,true),
    commandhandler(this),
{};


void stateMachine::initialise(State* initStatePtr) {

  //internal io initilization must happen here so io buses setup for sensor initialzation
  //intialize i2c interface
  I2C.begin(_SDA,_SCL,I2C_FREQUENCY);
  //initalize spi interface
  vspi.begin();
  vspi.setFrequency(1000000);
  vspi.setBitOrder(MSBFIRST);
  vspi.setDataMode(SPI_MODE0);

  hspi.begin();
  hspi.setFrequency(8000000);
  hspi.setBitOrder(MSBFIRST);
  hspi.setDataMode(SPI_MODE0);

  //setup cs pins
  //initialise output variables as output
  //pinMode(LoraCs, OUTPUT);

  //initialise outputs as high
  //digitalWrite(LoraCs, HIGH);

  //open serial port on usb interface
  Serial.begin(Serial_baud);
  Serial.setRxBufferSize(SERIAL_SIZE_RX);

  //setup network interfaces
  usbserial.setup();
  radio.setup();
  canbus.setup();

  //setup network manager so communication is running
  // add interfaces
  networkmanager.addInterface(&usbserial);
  networkmanager.addInterface(&radio);
  networkmanager.addInterface(&canbus);

  networkmanager.enableAutoRouteGen(false);
  networkmanager.setNoRouteAction(NOROUTE_ACTION::DUMP,{});

  // networkmanager.setNoRouteAction(NOROUTE_ACTION::BROADCAST,{1}); // broadcasting back to usbserial for debugging

   // command handler callback
  networkmanager.registerService(static_cast<uint8_t>(DEFAULT_SERVICES::COMMAND),commandhandler.getCallback()); 
  
  networkmanager.setLogCb([this](const std::string& message){return logcontroller.log(message);});

  //register deployment and engine handler services
  networkmanager.registerService(deploymentHandlerServiceID,deploymenthandler.getThisNetworkCallback());  
  networkmanager.registerService(engineHandlerServiceID,enginehandler.getThisNetworkCallback());  

  //call setup state
  changeState(initStatePtr);
 
  
};

void stateMachine::update() {



  //check for new packets and process

  networkmanager.update();

 
  

  //call update on state after new information has been processed
  State* newStatePtr = _currStatePtr->update();

  if (newStatePtr != _currStatePtr) {
    changeState(newStatePtr);
  }

  
};

void stateMachine::changeState(State* newStatePtr) {
  // Delete old state instance and change to new one
  if (_currStatePtr != NULL){
    //if not null pointer call exitstate method
    _currStatePtr->exitstate();

  };

  delete _currStatePtr;
  
  _currStatePtr = newStatePtr;
  _currStatePtr->initialise();



};

