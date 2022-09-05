#pragma once
#include <ADS1219/ADS1219.h>

/* ADS1219 Config
Device 0 - U13
Device 1 - U14
Device 2 - U15 */

//I2C Addresses
static constexpr int D0addr = 0b1000000;
static constexpr int D1addr = 0b1000100;
static constexpr int D2addr = 0b1000101;

//Gain
static constexpr adsGain_t D0gain = ONE;
static constexpr adsGain_t D1gain = ONE;
static constexpr adsGain_t D2gain = ONE;

//DataRate
static constexpr int D0gain = 20;
static constexpr int D1gain = 20;
static constexpr int D2gain = 20;

//Conversion Mode
static constexpr adsMode_t D0mode = CONTINUOUS;
static constexpr adsMode_t D1mode = CONTINUOUS;
static constexpr adsMode_t D2mode = CONTINUOUS;

//VREF
static constexpr adsRef_t D0vref = REF_EXTERNAL;
static constexpr adsRef_t D1vref = REF_EXTERNAL;
static constexpr adsRef_t D2vref = REF_EXTERNAL;

//Offsets
static constexpr int offsetD0ch0 = 0;
static constexpr int offsetD0ch1 = 0;
static constexpr int offsetD0ch2 = 0;
static constexpr int offsetD0ch3 = 0;
static constexpr int offsetD1ch0 = 0;
static constexpr int offsetD1ch1 = 0;
static constexpr int offsetD1ch2 = 0;
static constexpr int offsetD1ch3 = 0;
static constexpr int offsetD2ch0 = 0;
static constexpr int offsetD2ch1 = 0;
