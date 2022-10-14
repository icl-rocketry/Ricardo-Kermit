#include "LoadCell.h"

#include <librrc/nrcremotesensorbase.h>
#include <rnp_networkmanager.h>

LoadCell::LoadCell(ADS1219 *ADS1, uint32_t zeroReading, uint8_t ADC1channel, RnpNetworkManager &netman, float localgval) : NRCRemoteSensorBase(netman),
                                                                                                                           _ADS1(ADS1),
                                                                                                                           _ADS2(nullptr),
                                                                                                                           zeroReading(zeroReading),
                                                                                                                           channel1(ADC1channel),
                                                                                                                           g(localgval){};

LoadCell::LoadCell(ADS1219 *ADS1, ADS1219 *ADS2, uint32_t zeroReading, uint8_t ADC1channel, uint8_t ADC2channel, RnpNetworkManager &netman, float localgval) : NRCRemoteSensorBase(netman),
                                                                                                                                                               _ADS1(ADS1),
                                                                                                                                                               _ADS2(ADS2),
                                                                                                                                                               zeroReading(zeroReading),
                                                                                                                                                               channel1(ADC1channel),
                                                                                                                                                               channel2(ADC2channel),
                                                                                                                                                               g(localgval){};

void LoadCell::setConversionFactor(float convfactor)
{
    conversionfactor = convfactor;
}

float LoadCell::getWeight()
{
    if (!_ADS2)
    {
        Weight = (float)(_ADS1->readAdjusted(channel1) - zeroReading) / (float)conversionfactor;
    }
    else
    {
        Weight = (float)(_ADS1->readAdjusted(channel1) - _ADS2->readAdjusted(channel2) - zeroReading) / (float)conversionfactor;
    }
    return Weight;
}

float LoadCell::getMass()
{
    return getWeight() / g;
}

float LoadCell::getConversionFactor(float KnownMass)
{
    if (!_ADS2)
    {
        returnedconvfactor =(float) (_ADS1->readAdjusted(channel1) - zeroReading) / (float) (KnownMass * g);
    }
    else
    {
        returnedconvfactor = (float) (_ADS1->readAdjusted(channel1) - _ADS2->readAdjusted(channel2) - zeroReading) /(float) (KnownMass * g);
    }
    return returnedconvfactor;
}

void LoadCell::update(){
    updateSensorValue(getWeight());
}