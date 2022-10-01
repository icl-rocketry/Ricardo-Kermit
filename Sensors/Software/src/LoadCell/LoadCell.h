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

class LoadCell{
    private:
    ADS1219* _ADS; //reference to an ADS object
    float Weight;

    public:
    LoadCell(ADS1219* ADS);
    float getMass(uint8_t ADCchannel);
    float getWeight(uint8_t ADCchannel);

};