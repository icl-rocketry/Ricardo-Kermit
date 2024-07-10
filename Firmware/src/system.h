#pragma once

#include <libriccore/riccoresystem.h>

#include "Config/systemflags_config.h"
#include "Config/commands_config.h"
#include "Config/pinmap_config.h"

#include <libriccore/networkinterfaces/can/canbus.h>

#include "Sensors/MAX31856.h"
#include "Sensors/ADS131M06.h"
#include "nrccomponents/nrcremoteloadcell.h"
#include "nrccomponents/valvehelper.h"
#include "nrccomponents/nrcremoteredline.h"
#include "nrccomponents/nrcremoteptap.h"

#include "librrc/Remote/nrcremoteflowsensor.h"
#include "Commands/commands.h"

#include "Storage/sdfat_store.h"

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
        SPIClass SDSPI; //SPI for the SD card
        SPIClass SNSRSPI; //SPI for the sensors
        //4 thermocouples:
        MAX31856 TC0;
        MAX31856 TC1;
        MAX31856 TC2;
        MAX31856 TC3;
        //2 ADC's:
        ADS131M06 ADC0;
        ADS131M06 ADC1;

        NRCRemotePTap CPT0;
        NRCRemotePTap CPT1;
        NRCRemotePTap VPT0;
        NRCRemotePTap VPT1;
        NRCRemotePTap VPT2;
        
        NRCRemotePTap VPT3;
        NRCRemoteLoadcell LC0;
        NRCRemoteLoadcell LC1;
        NRCRemotePTap VPT4;
        NRCRemotePTap VPT5;
        NRCRemotePTap VPT6;
        NRCRemotePTap VPT7;

        NRCRemoteRedline RLMPT0;
        NRCRemoteRedline RLMPT1;

        NRCRemoteRedline RLMTC0;
        NRCRemoteRedline RLMTC1;
        NRCRemoteRedline RLMTC2;
        NRCRemoteRedline RLMTC3;

        //valves to operate
        valvehelper SOL0;
        valvehelper SOL1;
        valvehelper SOL2;
        valvehelper SOL3;

        valvehelper SERVO0;
        valvehelper SERVO1;

        SdFat_Store primarysd;


    private:

        void setupSPI();
        void serviceSetup();
        void deviceUpdate();
        void remoteSensorUpdate();
        void initializeLoggers();
        void logReadings();
        void remoteSensorSetup();
        void setvalves(NRCRemoteRedline& obj);

        const std::string log_path = "/Logs";

        uint32_t telemetry_log_delta = 5000;
        uint32_t prev_telemetry_log_time;
        uint32_t prev_flow_sensr_update;


};