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
                   TC1(SNSRSPI, PinMap::TC1_Cs, MAX31856::TCType::TT),
                   ADC0(SNSRSPI, PinMap::ADC0_Cs, PinMap::ADC_CLK),
                   SEN0(networkmanager, ADC0, 0, 0),
                   SEN1(networkmanager, ADC0, 0, 0),
                   SEN2(networkmanager, ADC0, 0, 0),
                   SEN3(networkmanager, ADC0, 0, 0),
                   //    FS0(networkmanager, PCNT_UNIT_0, PCNT_CHANNEL_0, PinMap::FS0, 0.001146158078),
                   primarysd(SDSPI, PinMap::SdCs_1, SD_SCK_MHZ(20), false, &systemstatus) {};

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
    pinMode(PinMap::TC0_Cs, OUTPUT);
    pinMode(PinMap::TC1_Cs, OUTPUT);
    pinMode(PinMap::SD_EN, OUTPUT);

    digitalWrite(PinMap::SdCs_1, HIGH);
    digitalWrite(PinMap::ADC0_Cs, HIGH);
    digitalWrite(PinMap::TC0_Cs, HIGH);
    digitalWrite(PinMap::TC1_Cs, HIGH);
    digitalWrite(PinMap::SD_EN, LOW);

    setupSPI();

    // Thermocouples:
    TC0.setup();
    TC1.setup();
    // ADCs:
    ADC0.setup();
    // Turbine Flow Sensor:
    // FS0.setup();

    ADC0.setOSR(ADS131M04::OSROPT::OSR16256);
    // ADC0.setGain(5, ADS131M06::GAIN::GAIN64);
    // ADC0.setGain(5, ADS131M06::GAIN::GAIN64);

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

    if ((primarysd.getError() > 0) && !systemstatus.flagSet(SYSTEM_FLAG::ERROR_SD))
    {
        systemstatus.newFlag(SYSTEM_FLAG::ERROR_SD, "SD Card Failed with error: " + std::to_string(primarysd.getError()));
    };
};

void System::serviceSetup()
{
    networkmanager.registerService(10, SEN0.getThisNetworkCallback());
    networkmanager.registerService(11, SEN1.getThisNetworkCallback());
    networkmanager.registerService(12, SEN2.getThisNetworkCallback());
    networkmanager.registerService(13, SEN3.getThisNetworkCallback());
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
    std::unique_ptr<WrappedFile> telemetrylogfile = primarysd.open(log_directory_path + "/telemetrylog.txt", static_cast<FILE_MODE>(O_WRITE | O_CREAT | O_AT_END), 100);

    // intialize sys logger
    loggerhandler.retrieve_logger<RicCoreLoggingConfig::LOGGERS::SYS>().initialize(std::move(syslogfile), networkmanager);

    // initialize telemetry logger
    loggerhandler.retrieve_logger<RicCoreLoggingConfig::LOGGERS::TELEMETRY>().initialize(std::move(telemetrylogfile), [](std::string_view msg)
                                                                                         { RicCoreLogging::log<RicCoreLoggingConfig::LOGGERS::SYS>(msg); });
}

void System::deviceUpdate()
{

    ADC0.update();

    TC0.update();
    TC1.update();

    // FS0.update();
}

void System::remoteSensorUpdate()
{
    SEN0.update();
    SEN1.update();
    SEN2.update();
    SEN3.update();
}

void System::logReadings()
{
    if (micros() - prev_telemetry_log_time > telemetry_log_delta)
    {
        TelemetryLogframe logframe;

        logframe.ch0sens = SEN0.getValueProcessed();
        logframe.ch1sens = SEN1.getValueProcessed();
        logframe.ch2sens = SEN2.getValueProcessed();
        logframe.ch3sens = SEN3.getValueProcessed();

        logframe.temp0 = TC0.getTemp();
        logframe.temp1 = TC1.getTemp();

        logframe.timestamp = esp_timer_get_time();

        RicCoreLogging::log<RicCoreLoggingConfig::LOGGERS::TELEMETRY>(logframe);

        prev_telemetry_log_time = esp_timer_get_time();
    }
}

void System::setupSPI()
{
    SDSPI.begin(PinMap::SD_SCLK, PinMap::SD_MISO, PinMap::SD_MOSI);
    SDSPI.setFrequency(20e6);
    SDSPI.setBitOrder(MSBFIRST);
    SDSPI.setDataMode(SPI_MODE0);

    SNSRSPI.begin(PinMap::SNSR_SCLK, PinMap::SNSR_MISO, PinMap::SNSR_MOSI);
    SNSRSPI.setFrequency(5000000);
    SNSRSPI.setBitOrder(MSBFIRST);
    SNSRSPI.setDataMode(SPI_MODE1);
}

void System::remoteSensorSetup()
{
    SEN0.setup();
    SEN1.setup();
    SEN2.setup();
    SEN3.setup();
}