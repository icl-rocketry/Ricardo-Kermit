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


#include "Sensors/ADS131M06.h"
#include <stdint.h>

#include <librrc/nrcremotesensorbase.h>
#include <librnp/rnp_networkmanager.h>

class LoadCell : public NRCRemoteSensorBase<LoadCell>
{
public:
    LoadCell(ADS131M06 *ADS1, uint32_t zeroReading, uint8_t ADC1channel, RnpNetworkManager& netman, float localgval = 9.81);
    LoadCell(ADS131M06 *ADS1, ADS131M06 *ADS2, uint32_t zeroReading, uint8_t ADC1channel, uint8_t ADC2channel, RnpNetworkManager& netman, float localgval = 9.81);
    void setConversionFactor(float convfactor);
    float calculateWeight();
    float calculateMass();

    float getWeight(){return Weight;};
    float getMass(){return calculateMass();};

    void update();

private: 
    ADS131M06 *_ADS1; // reference to an ADS object
    ADS131M06 *_ADS2; // reference to an ADS object
    uint32_t zeroReading;
    float conversionfactor;
    float returnedconvfactor;
    uint8_t channel1;
    uint8_t channel2;
    float g;
    float Weight;
};