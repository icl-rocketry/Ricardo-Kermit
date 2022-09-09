#pragma once

#include <Arduino.h>

enum class SYSTEM_FLAG:uint32_t{
    //state flags
    STATE_GROUNDSTATION_GATEWAY = (1 << 0), 
    //flags
    DEBUG = (1 << 7),
    //critical messages 
    ERROR_SPI = (1 << 8),
    ERROR_I2C = (1 << 9),
    ERROR_SERIAL = (1 << 10),
    ERROR_LORA = (1 << 11),
    ERROR_BARO = (1 << 12),
    ERROR_BATT = (1 << 13),
    ERROR_CAN = (1 << 21)
    
};


