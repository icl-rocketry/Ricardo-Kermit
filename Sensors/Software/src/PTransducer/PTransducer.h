#pragma once
/**
 * @file PTransducer.h
 * @author Andrei Paduraru
 * @brief Sensor class for pressure transducers.
 * 
 * @version 0.1
 * @date 2022-09-24
 *
 * @copyright Copyright (c) 2022
 */
#include <Arduino.h>
#include "ADS1219/ADS1219.h"

class PTransducer {

private:
    ADS1219* _ADS; //reference to an ADS object
    float Pressure;
public:
	PTransducer(ADS1219* ADS);
    float getPressure(const float grad, const float c, uint8_t ADCchannel);

//protected:
};