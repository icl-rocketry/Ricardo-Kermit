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
static constexpr int SCLK1 = 18;
static constexpr int MISO1 = 19;
static constexpr int MOSI1 = 23;

//CAN
static constexpr int CAN_TX = 32;
static constexpr int CAN_RX = 33;
