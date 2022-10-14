/* 
Inital State machine framework written by Daniele Bella <3
Code used to process states, and the transitions between them, contains parent class for states

Written by the Electronics team, Imperial College London Rocketry
*/

#include "stateMachine.h"
#include <string>
#include <vector>
#include <functional>

#include "global_config.h"
#include "ricardo_pins.h"

#include "States/state.h"


#include "Network/interfaces/usb.h"
#include "Network/interfaces/canbus.h"

#include "rnp_networkmanager.h"
#include "rnp_default_address.h"
#include "rnp_nvs_save.h"

#include "Storage/logController.h"
#include "Storage/systemstatus.h"

#include "SPI.h"
#include "Wire.h"

// 1000 psi values: 
// gradient = 49.07143; c = 295

//1600 psi values:
// gradient = 29.21429; c = 295

stateMachine::stateMachine() : 
    vspi(VSPI),
    I2C(0),
    usbserial(Serial,systemstatus,logcontroller),
    canbus(systemstatus,logcontroller,2),
    networkmanager(default_address,NODETYPE::HUB,true),
    commandhandler(this),
    logcontroller(networkmanager),
    systemstatus(&logcontroller),
    ADS0(I2C,D0addr),
    ADS1(I2C,D1addr),
    ADS2(I2C,D2addr),
    loadcell0(&ADS0,4254000,10,networkmanager),
    ptap0(ADS2,29.21429,295,1,networkmanager),
    ptap1(ADS1,49.07143,295,2,networkmanager),
    ntctemp0(ADS0,43000,22000,273.15+25,100000.0,3950.0,2,networkmanager),
    ntctemp1(ADS0,43000,22000,273.15+25,100000.0,3950.0,3,networkmanager)
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


  //Initialise ADC devices
  ADS0.begin(D0gain, D0drate, D0mode, D0vref, offsetD0);
  ADS1.begin(D1gain, D1drate, D1mode, D1vref, offsetD1);
  ADS2.begin(D2gain, D2drate, D2mode, D2vref, offsetD2);

  //open serial port on usb interface
  Serial.begin(Serial_baud);
  Serial.setRxBufferSize(SERIAL_SIZE_RX);

  //setup interfaces
  usbserial.setup();
  canbus.setup();

  //setup network manager so communication is running
  // add interfaces
  networkmanager.addInterface(&usbserial);
  networkmanager.addInterface(&canbus);

  networkmanager.enableAutoRouteGen(true);
  networkmanager.setNoRouteAction(NOROUTE_ACTION::BROADCAST,{1,2});

  //load default routing table
  RoutingTable routetable;
  routetable.setRoute((uint8_t)DEFAULT_ADDRESS::ROCKET,Route{2,1,{}});
  routetable.setRoute((uint8_t)DEFAULT_ADDRESS::GROUNDSTATION,Route{2,1,{}});

  networkmanager.setRoutingTable(routetable);
  networkmanager.updateBaseTable(); // save the new base table

  networkmanager.setAddress(default_address);

  networkmanager.enableAutoRouteGen(true); // enable route learning
  networkmanager.setNoRouteAction(NOROUTE_ACTION::BROADCAST,{1,2}); // enable broadcast over serial and radio only
  Serial.println(networkmanager.getAddress());
  
  logcontroller.setup();
  networkmanager.setLogCb([this](const std::string& message){return logcontroller.log(message);});

  //configure save function from network manager
  networkmanager.setSaveConfigImpl(RnpNvsSave::SaveToNVS);

  //try to load previous net config from nvs
  RnpNetworkManagerConfig savedNetworkConfig;
  if (!RnpNvsSave::ReadFromNVS(savedNetworkConfig))
  {
    logcontroller.log("loading saved config");
    networkmanager.loadconfig(savedNetworkConfig);
  }

  // command handler callback
  networkmanager.registerService(static_cast<uint8_t>(DEFAULT_SERVICES::COMMAND),commandhandler.getCallback()); 
  networkmanager.registerService(10,loadcell0.getThisNetworkCallback());
  networkmanager.registerService(12,ntctemp0.getThisNetworkCallback());
  networkmanager.registerService(13,ntctemp1.getThisNetworkCallback());
  networkmanager.registerService(16,ptap1.getThisNetworkCallback());
  networkmanager.registerService(19,ptap0.getThisNetworkCallback());
    
  //call setup state
  changeState(initStatePtr);
 
  
};

void stateMachine::update() {

  networkmanager.update();

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

