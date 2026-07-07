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
#include <cstdio>

#include "Loggers/TelemetryLogger/telemetrylogframe.h"


#ifdef CONFIG_IDF_TARGET_ESP32S3
static constexpr int VSPI_BUS_NUM = 0;
static constexpr int HSPI_BUS_NUM = 1;
#else
static constexpr int VSPI_BUS_NUM = VSPI;
static constexpr int HSPI_BUS_NUM = HSPI;
#endif

static constexpr uint8_t ICLR_LOGO_WIDTH = 32;
static constexpr uint8_t ICLR_LOGO_HEIGHT = 32;

static const uint8_t ICLR_LOGO_XBM[] = {
    0x00, 0xf0, 0x0f, 0x00, 0x00, 0xde, 0x7f, 0x00, 0x80, 0xfd, 0xdf, 0x01, 0xc0, 0x67, 0x66, 0x03,
    0xe0, 0x39, 0x82, 0x05, 0xf0, 0x2c, 0x3c, 0x0f, 0xf8, 0x2e, 0xcc, 0x16, 0x7c, 0x16, 0xf0, 0x3d,
    0x5c, 0x23, 0x38, 0x38, 0x6e, 0x31, 0x1c, 0x56, 0xbe, 0x38, 0xc4, 0x77, 0x56, 0x0e, 0xf0, 0x6f,
    0x07, 0x30, 0xfc, 0xe0, 0xf7, 0x07, 0x04, 0xec, 0x17, 0x3e, 0x9c, 0xff, 0xff, 0x39, 0x70, 0xe0,
    0x05, 0x20, 0x04, 0xe0, 0x07, 0x3e, 0x9c, 0xff, 0xfd, 0x39, 0x7c, 0xe0, 0x37, 0x00, 0xe0, 0xe7,
    0x84, 0x3f, 0xf4, 0x6f, 0xee, 0x2f, 0xc0, 0x57, 0xea, 0x81, 0x08, 0x70, 0x5c, 0x08, 0xb0, 0x2b,
    0x3c, 0x0c, 0x70, 0x3c, 0x78, 0x47, 0xe2, 0x1e, 0xf0, 0x62, 0x46, 0x0f, 0xe0, 0xf9, 0x9f, 0x07,
    0xc0, 0xc7, 0xe3, 0x03, 0x80, 0xfb, 0xff, 0x01, 0x00, 0xfe, 0x7b, 0x00, 0x00, 0xf0, 0x0f, 0x00
};

System::System() : RicCoreSystem(Commands::command_map, Commands::defaultEnabledCommands, Serial),
                   canbus(systemstatus, PinMap::TxCan, PinMap::RxCan, 3),
                   SDSPI(VSPI_BUS_NUM),
                   SNSRSPI(HSPI_BUS_NUM),
                   TC0(SNSRSPI, PinMap::TC0_Cs),
                   TC1(SNSRSPI, PinMap::TC1_Cs, MAX31856::TCType::TT),
                   ADC0(SNSRSPI, PinMap::ADC0_Cs, PinMap::ADC_CLK),
                   oled(U8G2_R0, PinMap::OLED_SCLK, PinMap::OLED_MOSI, PinMap::OLED_CS, PinMap::OLED_DC, PinMap::OLED_RST),
                   CPT0(networkmanager, 0),
                   CPT1(networkmanager, 1),
                   Mass(networkmanager, 0),
                   Thrust(networkmanager, 1),
                //    FS0(networkmanager, PCNT_UNIT_0, PCNT_CHANNEL_0, PinMap::FS0, 0.001146158078),
                   primarysd(SDSPI,PinMap::SdCs_1,SD_SCK_MHZ(20),false,&systemstatus){};

void System::systemSetup()
{

    Serial.setRxBufferSize(GeneralConfig::SerialRxSize);
    Serial.begin(GeneralConfig::SerialBaud);
    setupDisplay();
  

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

    ADC0.setOSR(ADS131M04::OSROPT::OSR8192);
    ADC0.setGain(0,ADS131M04::GAIN::GAIN1);
    ADC0.setGain(1,ADS131M04::GAIN::GAIN1);
    ADC0.setGain(2,ADS131M04::GAIN::GAIN1);
    ADC0.setGain(3,ADS131M04::GAIN::GAIN1);
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
    
    updateDisplay();

    remoteSensorUpdate();

    logReadings();
    // Serial.println((int)primarysd.getState());

    if((primarysd.getError() > 0) && !systemstatus.flagSet(SYSTEM_FLAG::ERROR_SD)){
        systemstatus.newFlag(SYSTEM_FLAG::ERROR_SD, "SD Card Failed with error: " + std::to_string(primarysd.getError()));
    };
};

void System::serviceSetup()
{
    networkmanager.registerService(10, CPT0.getThisNetworkCallback());
    networkmanager.registerService(11, CPT1.getThisNetworkCallback());
    networkmanager.registerService(12, Thrust.getThisNetworkCallback());
    networkmanager.registerService(13, Mass.getThisNetworkCallback());
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
    std::unique_ptr<WrappedFile> telemetrylogfile = primarysd.open(log_directory_path + "/telemetrylog.csv", static_cast<FILE_MODE>(O_WRITE | O_CREAT | O_AT_END),100);

    // intialize sys logger
    loggerhandler.retrieve_logger<RicCoreLoggingConfig::LOGGERS::SYS>().initialize(std::move(syslogfile), networkmanager);

    // initialize telemetry logger
    std::string file_header = "ch0sens,ch1sens,ch2sens,ch3sens,tc0(C),tc1(C),time(ms)";
    loggerhandler.retrieve_logger<RicCoreLoggingConfig::LOGGERS::TELEMETRY>().initialize(std::move(telemetrylogfile),file_header,[](std::string_view msg){RicCoreLogging::log<RicCoreLoggingConfig::LOGGERS::SYS>(msg);});
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

    CPT0.update(ADC0.getOutput(0));
    CPT1.update(ADC0.getOutput(1));
    Thrust.update(ADC0.getOutput(2));
    Mass.update(ADC0.getOutput(3));
}

void System::logReadings()
{
    if (millis() - prev_telemetry_log_time > telemetry_log_delta)
    {
        TelemetryLogframe logframe;

        logframe.ch0sens = CPT0.getPressure();
        logframe.ch1sens = CPT1.getPressure();
        logframe.ch2sens = Thrust.getWeight();
        logframe.ch3sens = Mass.getMass();

        logframe.temp0 = TC0.getTemp();
        logframe.temp1 = TC1.getTemp();

        logframe.timestamp = millis();
        prev_telemetry_log_time = millis();

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
    Thrust.setup();
    Mass.setup();
}

void System::setupDisplay()
{
    oled.begin();

    display_boot_start_time = millis();
    prev_display_update_time = display_boot_start_time;

    drawStartupLogo();
}

void System::drawStartupLogo()
{
    oled.clearBuffer();
    oled.setDrawColor(1);

    const uint8_t x = (256 - ICLR_LOGO_WIDTH) / 2;
    const uint8_t y = (64 - ICLR_LOGO_HEIGHT) / 2;

    oled.drawXBMP(x, y, ICLR_LOGO_WIDTH, ICLR_LOGO_HEIGHT, ICLR_LOGO_XBM);
    oled.sendBuffer();
}

void System::updateDisplay()
{ 
    const uint32_t now = millis();

    // Keep the ICLR logo on screen for the first 3 seconds after display init.
    if (now - display_boot_start_time < display_logo_duration_ms) {
        return;
    }

    if (now - prev_display_update_time < display_update_delta_ms) {
        return;
    }

    prev_display_update_time = now;

    char line[32];

    oled.clearBuffer();
    oled.setFont(u8g2_font_9x15_tf);

    oled.drawStr(0, 14, "KERMIT | DAQ");

    snprintf(line, sizeof(line), "ADC1:%04ld", static_cast<long>(ADC0.getOutput(0)));
    oled.drawStr(0, 34, line);

    snprintf(line, sizeof(line), "ADC2:%04ld", static_cast<long>(ADC0.getOutput(1)));
    oled.drawStr(0, 54, line);

    snprintf(line, sizeof(line), "ADC3:%04ld", static_cast<long>(ADC0.getOutput(2)));
    oled.drawStr(128, 34, line);

    snprintf(line, sizeof(line), "ADC4:%04ld", static_cast<long>(ADC0.getOutput(3)));
    oled.drawStr(128, 54, line);

    oled.sendBuffer();
}
