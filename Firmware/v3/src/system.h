#pragma once

#include <libriccore/riccoresystem.h>

#include "Config/systemflags_config.h"
#include "Config/commands_config.h"
#include "Config/pinmap_config.h"

#include <libriccore/networkinterfaces/can/canbus.h>

#include "Sensors/MAX31856.h"
#include "Sensors/ADS131M04.h"
#include <librrc/Remote/nrcremoteptap.h>
#include <librrc/Remote/nrcremoteloadcell.h>
#include <librrc/Remote/nrcremoteflowsensor.h>

#include "Commands/commands.h"

#include "Storage/sdfat_store.h"
#include "Sensors/adc_channel.h"

/* System class: a class which encapsulates all the classes used to describe components on
    the board, creating the board system. Interacting with the board involves interacting
    the system */

class System : public RicCoreSystem<System,SYSTEM_FLAG,Commands::ID>
{
    public:

        System();
        
        void systemSetup();

        void systemUpdate();

        CanBus<SYSTEM_FLAG> canbus;

        //2 seperate SPI lines:
        SPIClass SDSPI;     //SPI for the SD card
        SPIClass SNSRSPI;   //SPI for the sensors
        //2 thermocouples:
        MAX31856 TC0;
        MAX31856 TC1;
        //1 4-channel ADC:
        ADS131M04 ADC0;

        adc_channel SEN0;
        adc_channel SEN1;
        adc_channel SEN2;
        adc_channel SEN3;

        // NRCRemoteFlowSensor FS0;

        SdFat_Store primarysd;

    private:

        void setupSPI();
        void serviceSetup();
        void deviceUpdate();
        void remoteSensorUpdate();
        void initializeLoggers();
        void logReadings();
        void remoteSensorSetup();

        const std::string log_path = "/Logs";
        const std::string config_path = "/Config";

        uint32_t telemetry_log_delta = 5000;
        uint32_t prev_telemetry_log_time;


};