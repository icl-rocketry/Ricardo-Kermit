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
#include <U8g2lib.h>

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
        SPIClass SDSPI;     //SPI for the SD card
        SPIClass SNSRSPI;   //SPI for the sensors
        //2 thermocouples:
        MAX31856 TC0;
        MAX31856 TC1;
        //1 4-channel ADC:
        ADS131M04 ADC0;

        // SPI OLED Display used to show live ADC Channel values.
        U8G2_SSD1322_ZJY_256X64_F_4W_SW_SPI oled;
    
        NRCRemotePTap CPT0;
        NRCRemotePTap CPT1;
        // NRCRemotePTap CPT2;
        // NRCRemotePTap CPT3;
        NRCRemoteLoadcell Mass;
        NRCRemoteLoadcell Thrust;

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
        void setupDisplay();
        void drawStartupLogo();
        void updateDisplay();


        const std::string log_path = "/Logs";
        const std::string config_path = "/Config";

        uint32_t telemetry_log_delta = 1000;
        uint32_t prev_telemetry_log_time;
        uint64_t display_boot_start_time = 0;
        uint64_t prev_display_update_time = 0;

        static constexpr uint64_t display_logo_duration_us = 3000000ULL; // 3 seconds
        static constexpr uint64_t display_update_delta = 200000ULL;      // 0.2 seconds




};
