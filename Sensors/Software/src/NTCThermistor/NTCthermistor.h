#pragma once 

/**
 * @file NTCthermistor.h
 * @author Andrei Paduraru
 * @brief Sensor class for NTC thermistors. 
 * Returns temperature value as a double precision float, calculated
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
#include <Wire.h>

class NTCThermistor{

private:
    ADS1219* _ADS; //reference to an ADS object
    double Temp;
    double Rtherm;
public:
	NTCThermistor(ADS1219* ADS);
    double getTemp(const int Rfixed, const double SHHA, const double SHHB, const double SHHC, uint8_t ADCchannel);

//protected:
};