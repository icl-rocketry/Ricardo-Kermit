/*
**********************
* PINS               *
**********************
 */
#pragma once

//I2C
static constexpr int _SDA = 21;
static constexpr int _SCL = 22;

//Chip Selects
static constexpr int TC_CS0 = 10;
static constexpr int TC_CS1 = 26;
static constexpr int TC_CS2 = 4;
static constexpr int TC_CS3 = 9;
static constexpr int TC_CS4 = 15;
static constexpr int TC_CS5 = 13;
static constexpr int TC_CS6 = 27;
static constexpr int TC_CS7 = 12;
static constexpr int TC_CS8 = 25;
static constexpr int TC_CS9 = 14;

//SPI
static constexpr int V_SCLK = 18;
static constexpr int V_MISO1 = 19;
static constexpr int V_MOSI1 = 23;

static constexpr int H_MISO = 12;
static constexpr int H_MOSI = 13;
static constexpr int H_SCLK = 14;

//CAN
static constexpr int TxCan = 32;
static constexpr int RxCan = 33;

//PINS kept for compatibility