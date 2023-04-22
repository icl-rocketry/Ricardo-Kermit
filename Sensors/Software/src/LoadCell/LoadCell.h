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

#include <librrc/nrcremotesensorbase.h>
#include <rnp_networkmanager.h>
#include <global_config.h>

class LoadCell : public NRCRemoteSensorBase<LoadCell>
{
public:
    LoadCell(ADS1219 *ADS1, uint32_t zeroReading, uint8_t ADC1channel, RnpNetworkManager& netman, float localgval = 9.81);
    LoadCell(ADS1219 *ADS1, ADS1219 *ADS2, uint32_t zeroReading, uint8_t ADC1channel, uint8_t ADC2channel, RnpNetworkManager& netman, float localgval = 9.81);
    void setGradient(float convfactor);
    float calculateWeight();
    float calculateMass();
    float getGradient(float KnownMass);
    adsGain_t gain1;
    adsGain_t gain2;

    float getWeight(){return Weight;};
    float getMass(){return calculateMass();};

    void update();
    void zero(uint16_t Nsamples);
    

    

private: 
    ADS1219 *_ADS1; // reference to an ADS object
    ADS1219 *_ADS2; // reference to an ADS object
    uint32_t zeroReading;
    float Gradient;
    float returnedGrad;
    uint8_t channel1;
    uint8_t channel2;
    float g;
    float Weight;
    
    // void extendedCommandHandler_impl(const NRCPacket::NRC_COMMAND_ID commandID,packetptr_t packetptr){};
     
    
};