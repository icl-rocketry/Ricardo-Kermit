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

// ADS1219 Device setting enums
static constexpr uint8_t GAIN_ONE = 0x00;
static constexpr uint8_t GAIN_FOUR = 0x10;

static constexpr uint8_t MODE_SINGLE_SHOT = 0x00;
static constexpr uint8_t MODE_CONTINUOUS = 0x02;

static constexpr uint8_t VREF_INTERNAL = 0x00;
static constexpr uint8_t VREF_EXTERNAL = 0x01;

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

// General Board Config
static constexpr uint32_t I2C_FREQUENCY = 400000;
static constexpr uint Serial_baud = 115200;
static constexpr uint SERIAL_SIZE_RX = 256;

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
static constexpr uint D0drate = 20;
static constexpr uint D1drate = 20;
static constexpr uint D2drate = 20;

// Conversion Mode
static constexpr adsMode_t D0mode = CONTINUOUS;
static constexpr adsMode_t D1mode = CONTINUOUS;
static constexpr adsMode_t D2mode = CONTINUOUS;

// VREF
static constexpr adsRef_t D0vref = REF_EXTERNAL;
static constexpr adsRef_t D1vref = REF_EXTERNAL;
static constexpr adsRef_t D2vref = REF_EXTERNAL;

// Offsets
static constexpr int offsetD0 = 13;
static constexpr int offsetD1 = 12;
static constexpr int offsetD2 = 12;
