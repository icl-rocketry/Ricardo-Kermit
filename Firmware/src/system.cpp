#include "system.h"

#include <memory>

#include <libriccore/riccoresystem.h>

#include <HardwareSerial.h>

#include "Config/systemflags_config.h"
#include "Config/commands_config.h"
#include "Config/pinmap_config.h"
#include "Config/general_config.h"

#include "Commands/commands.h"

#include "States/idle.h"

#ifdef CONFIG_IDF_TARGET_ESP32S3
static constexpr int VSPI_BUS_NUM = 0;
static constexpr int HSPI_BUS_NUM = 1;
#else
static constexpr int VSPI_BUS_NUM = VSPI;
static constexpr int HSPI_BUS_NUM = HSPI;
#endif

System::System():
RicCoreSystem(Commands::command_map,Commands::defaultEnabledCommands,Serial),
canbus(systemstatus,PinMap::TxCan,PinMap::RxCan,3),
SDSPI(VSPI_BUS_NUM),
SNSRSPI(HSPI_BUS_NUM),
TC0(SNSRSPI, PinMap::TC0_Cs),
TC1(SNSRSPI, PinMap::TC1_Cs),
TC2(SNSRSPI, PinMap::TC2_Cs),
TC3(SNSRSPI, PinMap::TC3_Cs),
ADC0(SNSRSPI, PinMap::ADC0_Cs, PinMap::ADC_CLK),//need clkout pin and channel
ADC1(SNSRSPI, PinMap::ADC1_Cs),
VPT0(networkmanager, ADC0, 5),
VPT1(networkmanager, ADC0, 4),
VPT2(networkmanager, ADC0, 3),
VPT3(networkmanager, ADC0, 2),
VPT4(networkmanager, ADC0, 1),
VPT5(networkmanager, ADC0, 0),
CPT0(networkmanager, ADC1, 5),
CPT1(networkmanager, ADC1, 4),
LC0( &ADC1, (uint32_t) 0, (uint8_t) 3, networkmanager),
LC1( &ADC1, (uint32_t) 0, (uint8_t) 2, networkmanager)
{};


void System::systemSetup(){
    
    Serial.setRxBufferSize(GeneralConfig::SerialRxSize);
    Serial.begin(GeneralConfig::SerialBaud);
   
    //intialize rnp message logger
    loggerhandler.retrieve_logger<RicCoreLoggingConfig::LOGGERS::SYS>().initialize(networkmanager);

    //initialize statemachine with idle state
    statemachine.initalize(std::make_unique<Idle>(systemstatus,commandhandler));
    
    networkmanager.setNodeType(NODETYPE::HUB);
    networkmanager.setNoRouteAction(NOROUTE_ACTION::BROADCAST,{1,3});
    
    networkmanager.addInterface(&canbus);
    
    //any other setup goes here
    SNSRSPI.begin(PinMap::SNSR_SCLK,PinMap::SNSR_MISO,PinMap::SNSR_MOSI);
    SNSRSPI.setFrequency(5000000);
    SNSRSPI.setBitOrder(MSBFIRST);
    SNSRSPI.setDataMode(SPI_MODE1);
    SNSRSPI.setClockDivider(SPI_CLOCK_DIV16);

    pinMode(PinMap::ADC0_Cs, OUTPUT);
    pinMode(PinMap::ADC1_Cs, OUTPUT);
    pinMode(PinMap::TC0_Cs, OUTPUT);
    pinMode(PinMap::TC1_Cs, OUTPUT);
    pinMode(PinMap::TC2_Cs, OUTPUT);
    pinMode(PinMap::TC3_Cs, OUTPUT);

    digitalWrite(PinMap::ADC0_Cs, HIGH);
    digitalWrite(PinMap::ADC1_Cs, HIGH);
    digitalWrite(PinMap::TC0_Cs, HIGH);
    digitalWrite(PinMap::TC1_Cs, HIGH);
    digitalWrite(PinMap::TC2_Cs, HIGH);
    digitalWrite(PinMap::TC3_Cs, HIGH);

    //Thermocouples:
    TC0.setup();
    TC1.setup();
    TC2.setup();
    TC3.setup();
    //ADC's:
    ADC0.setup();
    ADC1.setup();     


};


void System::systemUpdate(){

    TC0.update();
    TC1.update();
    TC2.update();
    TC3.update();

    ADC0.update();
    ADC1.update();
};