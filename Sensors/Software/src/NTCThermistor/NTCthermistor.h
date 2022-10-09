#pragma once 

/**
 * @file NTCthermistor.h
 * @author Andrei Paduraru
 * @brief Sensor class for NTC thermistors. 
 * Returns temperature value as a float, calculated
 * using the Steinhart-Hart equation:
 * https://en.wikipedia.org/wiki/Steinhart%E2%80%93Hart_equation
 * 
 * @version 0.1
 * @date 2022-09-22
 *
 * @copyright Copyright (c) 2022
 */
#include <ricardo_pins.h>
#include <global_config.h>
#include "ADS1219/ADS1219.h"
#include <Arduino.h>

class NTCThermistor{

private:
    ADS1219 &_ADS; //reference to an ADS object
    float Temp;
    float Rtherm;
    float SHHA;
    float SHHB;
    float SHHC;
    uint32_t RFixedPD;
    uint32_t Rseries;
    uint8_t channel;
    float grad;
    float constant;
    static constexpr uint32_t ADCMax = 16777215;

public:
	NTCThermistor(ADS1219 &ADS, const uint32_t Rfixed, const uint32_t Rseriesextra, const float A, const float B, const float C, uint8_t ADCchannel);
    NTCThermistor(ADS1219 &ADS, const uint32_t Rfixed, const uint32_t Rseriesextra, const float gradient, const float Cconstant, uint8_t ADCchannel);
    float getTemp();
    float getTempLinear();
//protected:
};