#pragma once

/**
 * @file LoadCell.h
 * @author Andrei Paduraru
 * @brief Class to read an analog load cell and convert it to a mass or weight (configurable)
 * @version 0.1
 * @date 2022-09-30
 *
 * @copyright Copyright (c) 2022
 *
 */

#include <global_config.h>
#include <ADS1219/ADS1219.h>
#include <stdint.h>

class LoadCell
{
public:
    LoadCell(ADS1219 *ADS1, uint32_t zeroReading, uint8_t ADC1channel, float localgval = 9.81);
    LoadCell(ADS1219 *ADS1, ADS1219 *ADS2, uint32_t zeroReading, uint8_t ADC1channel, uint8_t ADC2channel, float localgval = 9.81);
    void setConversionFactor(float convfactor);
    float getWeight();
    float getMass();
    float getConversionFactor(float KnownMass);

private:
    ADS1219 *_ADS1; // reference to an ADS object
    ADS1219 *_ADS2; // reference to an ADS object
    uint32_t zeroReading;
    float conversionfactor;
    float returnedconvfactor;
    uint8_t channel1;
    uint8_t channel2;
    float g;
    float Weight;
};