#pragma once

#include <libriccore/riccoresystem.h>

#include "Config/systemflags_config.h"
#include "Config/commands_config.h"
#include "Config/pinmap_config.h"

#include <libriccore/networkinterfaces/can/canbus.h>

#include "Sensors/MAX31856.h"
#include "Sensors/ADS131M06.h"
#include "LoadCell.h"
#include "nrcremoteptap.h"

#include "Commands/commands.h"

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

        NRCRemotePTap VPT0;
        NRCRemotePTap VPT1;
        NRCRemotePTap VPT2;
        NRCRemotePTap VPT3;
        NRCRemotePTap VPT4;
        NRCRemotePTap VPT5;
        NRCRemotePTap CPT0;
        NRCRemotePTap CPT1;

        LoadCell LC0;
        LoadCell LC1;

    // private:


};