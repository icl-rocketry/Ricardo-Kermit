// Global Config
#pragma once
#include <stdint.h>

// uncomment to show all debug messages
#define VERBOSE

// Serial max timeout
#define MAX_SERIAL_TIMEOUT 5
// SPI speed - 2MHz
#define SPI_CLOCK_DIV8 0x004c1001

// define max packet length on any interface in the network
#define MAX_PACKET_LENGTH 256

// ArduinoJson
#define ARDUINOJSON_ENABLE_COMMENTS 1

// ADS1219 Device setting enums, (you shouldn't have to touch these)
static constexpr uint8_t GAIN_ONE = 0b00000000;
static constexpr uint8_t GAIN_FOUR = 0b0001000;

static constexpr uint8_t DATA_RATE_20 = 0b00000000;
static constexpr uint8_t DATA_RATE_90 = 0b00000100;
static constexpr uint8_t DATA_RATE_330 = 0b00001000;
static constexpr uint8_t DATA_RATE_1000 = 0b00110000;

static constexpr uint8_t MODE_SINGLE_SHOT = 0b00000000;
static constexpr uint8_t MODE_CONTINUOUS = 0b00000010;

static constexpr uint8_t VREF_INTERNAL = 0b00000000;
static constexpr uint8_t VREF_EXTERNAL = 0b00000001;

typedef enum
{
    ONE = GAIN_ONE,
    FOUR = GAIN_FOUR
} adsGain_t;

typedef enum
{
    SINGLE_SHOT = MODE_SINGLE_SHOT,
    CONTINUOUS = MODE_CONTINUOUS
} adsMode_t;

typedef enum
{
    REF_INTERNAL = VREF_INTERNAL,
    REF_EXTERNAL = VREF_EXTERNAL
} adsRef_t;

enum class ADCDevices : uint8_t
{
    D0 = 0,
    D1 = 1,
    D2 = 2,
};

enum class ADSDatarates : uint16_t
	{
		DataRate_20 = 20,
		DataRate_90 = 90,
		DataRate_330 = 330,
		DataRate_1000 = 1000
	};

// General Board Config
static constexpr uint32_t I2C_FREQUENCY = 400000;
static constexpr unsigned int Serial_baud = 115200;
static constexpr unsigned int SERIAL_SIZE_RX = 256;

/* ADS1219 Config
Device 0 - U13
Device 1 - U14
Device 2 - U15 */

// I2C Addresses
static constexpr uint8_t D0addr = 0b1000000;
static constexpr uint8_t D1addr = 0b1000100;
static constexpr uint8_t D2addr = 0b1000101;

// Gain
static constexpr adsGain_t D0gain = ONE;
static constexpr adsGain_t D1gain = ONE;
static constexpr adsGain_t D2gain = ONE;

// DataRate
static constexpr ADSDatarates D0drate = ADSDatarates::DataRate_20;
static constexpr ADSDatarates D1drate = ADSDatarates::DataRate_20;
static constexpr ADSDatarates D2drate = ADSDatarates::DataRate_20;

// Conversion Mode
static constexpr adsMode_t D0mode = CONTINUOUS;
static constexpr adsMode_t D1mode = CONTINUOUS;
static constexpr adsMode_t D2mode = CONTINUOUS;

// VREF
static constexpr adsRef_t D0vref = REF_EXTERNAL;
static constexpr adsRef_t D1vref = REF_EXTERNAL;
static constexpr adsRef_t D2vref = REF_EXTERNAL;

// Offsets (feed system board)
//static constexpr int32_t offsetD0 = 372;
//static constexpr int32_t offsetD1 = 367;
//static constexpr int32_t offsetD2 = 3336;

// Offsets (GND system board)
static constexpr int32_t offsetD0 = 191;
static constexpr int32_t offsetD1 = 265;
static constexpr int32_t offsetD2 = 57;

//offsets (test board)
// static constexpr int32_t offsetD0 = 214;
// static constexpr int32_t offsetD1 = 214;
// static constexpr int32_t offsetD2 = 214;