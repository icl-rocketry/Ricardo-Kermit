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

#include <cstdlib>

#include "Loggers/TelemetryLogger/telemetrylogframe.h"


#ifdef CONFIG_IDF_TARGET_ESP32S3
static constexpr int VSPI_BUS_NUM = 0;
static constexpr int HSPI_BUS_NUM = 1;
#else
static constexpr int VSPI_BUS_NUM = VSPI;
static constexpr int HSPI_BUS_NUM = HSPI;
#endif

System::System() : RicCoreSystem(Commands::command_map, Commands::defaultEnabledCommands, Serial),
                   canbus(systemstatus, PinMap::TxCan, PinMap::RxCan, 3),
                   SDSPI(VSPI_BUS_NUM),
                   SNSRSPI(HSPI_BUS_NUM),
                   TC0(SNSRSPI, PinMap::TC0_Cs),
                   TC1(SNSRSPI, PinMap::TC1_Cs),
                   TC2(SNSRSPI, PinMap::TC2_Cs),
                   TC3(SNSRSPI, PinMap::TC3_Cs),
                   ADC0(SNSRSPI, PinMap::ADC0_Cs, PinMap::ADC_CLK), // need clkout pin and channel
                   ADC1(SNSRSPI, PinMap::ADC1_Cs),
                   CPT0(networkmanager, 0, ADC1, 5),
                   CPT1(networkmanager, 1, ADC1, 4),
                   VPT0(networkmanager, 2, ADC1, 3),
                   VPT1(networkmanager, 3, ADC1, 2),
                   VPT2(networkmanager, 4, ADC1, 1),
                   VPT3(networkmanager, 5, ADC1, 0),
                   LC0(networkmanager, 0, ADC0, 5),
                   LC1(networkmanager, 1, ADC0, 4),
                   VPT4(networkmanager, 6, ADC0, 3),
                   VPT5(networkmanager, 7, ADC0, 2),
                   VPT6(networkmanager, 8, ADC0, 1),
                   VPT7(networkmanager, 9, ADC0, 0),
                   RLMPT0(networkmanager,0,-2,0),
                   RLMPT1(networkmanager,1,-2,0),
                   RLMTC0(networkmanager,2,-273,0),
                   RLMTC1(networkmanager,3,-273,0),
                   RLMTC2(networkmanager,4,-273,0),
                   RLMTC3(networkmanager,5,-273,0),
                   RLMGPT0(networkmanager,6,0,1),
                   RLMGPT1(networkmanager,7,0,1),
                   RLMGTC0(networkmanager,8,0,1,100e-3),
                   RLMGTC1(networkmanager,9,-2,1,100e-3),
                   RLMGTC2(networkmanager,10,-2,1,100e-3),
                   RLMGTC3(networkmanager,11,-2,1,100e-3),
                   GLOBALMON(networkmanager),
                   primarysd(SDSPI,PinMap::SdCs_1,SD_SCK_MHZ(20),false,&systemstatus){};

void System::systemSetup()
{

    Serial.setRxBufferSize(GeneralConfig::SerialRxSize);
    Serial.begin(GeneralConfig::SerialBaud);
  


    // initialize statemachine with idle state
    statemachine.initalize(std::make_unique<Idle>(systemstatus, commandhandler));

    canbus.setup();
    networkmanager.setNodeType(NODETYPE::HUB);
    networkmanager.setNoRouteAction(NOROUTE_ACTION::BROADCAST, {1, 3});

    networkmanager.addInterface(&canbus);

    // any other setup goes here

    pinMode(PinMap::SdCs_1, OUTPUT);
    pinMode(PinMap::ADC0_Cs, OUTPUT);
    pinMode(PinMap::ADC1_Cs, OUTPUT);
    pinMode(PinMap::TC0_Cs, OUTPUT);
    pinMode(PinMap::TC1_Cs, OUTPUT);
    pinMode(PinMap::TC2_Cs, OUTPUT);
    pinMode(PinMap::TC3_Cs, OUTPUT);

    digitalWrite(PinMap::SdCs_1, HIGH);
    digitalWrite(PinMap::ADC0_Cs, HIGH);
    digitalWrite(PinMap::ADC1_Cs, HIGH);
    digitalWrite(PinMap::TC0_Cs, HIGH);
    digitalWrite(PinMap::TC1_Cs, HIGH);
    digitalWrite(PinMap::TC2_Cs, HIGH);
    digitalWrite(PinMap::TC3_Cs, HIGH);

    setupSPI();

    // Thermocouples:
    TC0.setup();
    TC1.setup();
    TC2.setup();
    TC3.setup();
    // ADC's:
    ADC0.setup();
    ADC1.setup();

    ADC0.setOSR(ADS131M06::OSROPT::OSR16256);
    ADC1.setOSR(ADS131M06::OSROPT::OSR16256);

    serviceSetup();

    remoteSensorSetup();

    primarysd.setup();

    initializeLoggers();
};

void System::systemUpdate()
{
    deviceUpdate();

    remoteSensorUpdate();

    logReadings();
    // Serial.println((int)primarysd.getState());

    if((primarysd.getError() > 0) && !systemstatus.flagSet(SYSTEM_FLAG::ERROR_SD)){
        systemstatus.newFlag(SYSTEM_FLAG::ERROR_SD, "SD Card Failed with error: " + std::to_string(primarysd.getError()));
    };
};

void System::serviceSetup()
{
    //remote sensors
    networkmanager.registerService(10, CPT0.getThisNetworkCallback());
    networkmanager.registerService(11, CPT1.getThisNetworkCallback());
    networkmanager.registerService(12, VPT0.getThisNetworkCallback());
    networkmanager.registerService(13, VPT1.getThisNetworkCallback());
    networkmanager.registerService(14, VPT2.getThisNetworkCallback());
    networkmanager.registerService(15, VPT3.getThisNetworkCallback());
    networkmanager.registerService(16, LC0.getThisNetworkCallback());
    networkmanager.registerService(17, LC1.getThisNetworkCallback());
    networkmanager.registerService(18, VPT4.getThisNetworkCallback());
    networkmanager.registerService(19, VPT5.getThisNetworkCallback());
    networkmanager.registerService(20, VPT6.getThisNetworkCallback());
    networkmanager.registerService(21, VPT7.getThisNetworkCallback());

    //redline monitors
    networkmanager.registerService(30, RLMPT0.getThisNetworkCallback());
    networkmanager.registerService(31, RLMPT1.getThisNetworkCallback());
    networkmanager.registerService(32, RLMTC0.getThisNetworkCallback());
    networkmanager.registerService(33, RLMTC1.getThisNetworkCallback());
    networkmanager.registerService(34, RLMTC2.getThisNetworkCallback());
    networkmanager.registerService(35, RLMTC3.getThisNetworkCallback());
    networkmanager.registerService(36, RLMGPT0.getThisNetworkCallback());
    networkmanager.registerService(37, RLMGPT1.getThisNetworkCallback());
    networkmanager.registerService(38, RLMGTC0.getThisNetworkCallback());
    networkmanager.registerService(39, RLMGTC1.getThisNetworkCallback());
    networkmanager.registerService(40, RLMGTC2.getThisNetworkCallback());
    networkmanager.registerService(41, RLMGTC3.getThisNetworkCallback());

    //global monitor
    networkmanager.registerService(42, GLOBALMON.getThisNetworkCallback());

}

void System::initializeLoggers()
{
    // check if sd card is mounted
    if (primarysd.getState() != StoreBase::STATE::NOMINAL)
    {
        loggerhandler.retrieve_logger<RicCoreLoggingConfig::LOGGERS::SYS>().initialize(nullptr, networkmanager);

        return;
    }

    // open log files
    // get unique directory for logs
    std::string log_directory_path = primarysd.generateUniquePath(log_path, "");
    // make new directory
    primarysd.mkdir(log_directory_path);

    std::unique_ptr<WrappedFile> syslogfile = primarysd.open(log_directory_path + "/syslog.txt", static_cast<FILE_MODE>(O_WRITE | O_CREAT | O_AT_END));
    std::unique_ptr<WrappedFile> telemetrylogfile = primarysd.open(log_directory_path + "/telemetrylog.txt", static_cast<FILE_MODE>(O_WRITE | O_CREAT | O_AT_END),100);

    // intialize sys logger
    loggerhandler.retrieve_logger<RicCoreLoggingConfig::LOGGERS::SYS>().initialize(std::move(syslogfile), networkmanager);

    // initialize telemetry logger
    loggerhandler.retrieve_logger<RicCoreLoggingConfig::LOGGERS::TELEMETRY>().initialize(std::move(telemetrylogfile),[](std::string_view msg){RicCoreLogging::log<RicCoreLoggingConfig::LOGGERS::SYS>(msg);});
}

uint32_t updated = 0;
void System::deviceUpdate()
{

    ADC0.update();
    ADC1.update();

    TC0.update();
    TC1.update();
    TC2.update();
    TC3.update();

}

void System::remoteSensorUpdate()
{

    CPT0.update();
    CPT1.update();
    VPT0.update();
    VPT1.update();
    VPT2.update();
    VPT3.update();
    LC0.update();
    LC1.update();
    VPT4.update();
    VPT5.update();
    VPT6.update();
    VPT7.update();

    RLMPT0.update(CPT0.getValue());
    RLMPT1.update(CPT1.getValue());
    RLMTC0.update(TC0.getTemp());
    RLMTC1.update(TC1.getTemp());
    RLMTC2.update(TC2.getTemp());
    RLMTC3.update(TC3.getTemp());

    RLMGPT0.update(CPT0.getValue());
    RLMGPT1.update(CPT1.getValue());
    RLMGTC0.update(TC0.getTemp());
    RLMGTC1.update(TC1.getTemp());
    RLMGTC2.update(TC2.getTemp());
    RLMGTC3.update(TC3.getTemp());

    GLOBALMON.update();
}

void System::logReadings()
{
    if (micros() - prev_telemetry_log_time > telemetry_log_delta)
    {
        // FS0.update();
        TelemetryLogframe logframe;

        logframe.ch0sens = CPT0.getPressure();
        logframe.ch1sens = CPT1.getPressure();
        logframe.ch2sens = VPT0.getPressure();
        logframe.ch3sens = VPT1.getPressure();
        logframe.ch4sens = VPT2.getPressure();
        logframe.ch5sens = VPT3.getPressure();
        logframe.ch6sens = LC0.getWeight();
        logframe.ch7sens = LC1.getWeight();
        logframe.ch8sens = VPT4.getPressure();
        logframe.ch9sens = VPT5.getPressure();
        logframe.ch10sens = VPT6.getPressure();
        logframe.ch11sens = VPT7.getPressure();

        logframe.temp0 = TC0.getTemp();
        logframe.temp1 = TC1.getTemp();
        logframe.temp2 = TC2.getTemp();
        logframe.temp2 = TC2.getTemp();
        logframe.temp3 = TC3.getTemp();

        logframe.timestamp = esp_timer_get_time();
        prev_telemetry_log_time = micros();

        RicCoreLogging::log<RicCoreLoggingConfig::LOGGERS::TELEMETRY>(logframe);

        
    }
}

void System::setupSPI(){
    SDSPI.begin(PinMap::SD_SCLK,PinMap::SD_MISO,PinMap::SD_MOSI);
    SDSPI.setFrequency(20e6);
    SDSPI.setBitOrder(MSBFIRST);
    SDSPI.setDataMode(SPI_MODE0);

    SNSRSPI.begin(PinMap::SNSR_SCLK, PinMap::SNSR_MISO, PinMap::SNSR_MOSI);
    SNSRSPI.setFrequency(5000000);
    SNSRSPI.setBitOrder(MSBFIRST);
    SNSRSPI.setDataMode(SPI_MODE1);
}

void System::remoteSensorSetup(){
    CPT0.setup();
    CPT1.setup();
    VPT0.setup();
    VPT1.setup();
    VPT2.setup();
    VPT3.setup();
    LC0.setup();
    LC1.setup();
    VPT4.setup();
    VPT5.setup();
    VPT6.setup();
    VPT7.setup();

    RLMPT0.setup();
    RLMPT1.setup();
    RLMTC0.setup();
    RLMTC1.setup();
    RLMTC2.setup();
    RLMTC3.setup();

    RLMGPT0.setup();
    RLMGPT1.setup();
    RLMGTC0.setup();
    RLMGTC1.setup();
    RLMGTC2.setup();
    RLMGTC3.setup();

    globalMonSetup();
}

void System::globalMonSetup(){
    GLOBALMON.redlinesVect.push_back(RLMPT0);
    GLOBALMON.redlinesVect.push_back(RLMPT1);
    GLOBALMON.redlinesVect.push_back(RLMTC0);
    GLOBALMON.redlinesVect.push_back(RLMTC1);
    GLOBALMON.redlinesVect.push_back(RLMTC2);
    GLOBALMON.redlinesVect.push_back(RLMTC3);
    GLOBALMON.redlinesVect.push_back(RLMGPT0);
    GLOBALMON.redlinesVect.push_back(RLMGPT1);
    GLOBALMON.redlinesVect.push_back(RLMGTC0);
    GLOBALMON.redlinesVect.push_back(RLMGTC1);
    GLOBALMON.redlinesVect.push_back(RLMGTC2);
    GLOBALMON.redlinesVect.push_back(RLMGTC3);
}