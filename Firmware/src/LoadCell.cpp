#include "LoadCell.h"

#include <librrc/nrcremotesensorbase.h>
#include <librnp/rnp_networkmanager.h>

LoadCell::LoadCell(ADS131M06 *ADS1, uint32_t zeroReading, uint8_t ADC1channel, RnpNetworkManager &netman, float localgval) : NRCRemoteSensorBase(netman),
                                                                                                                           _ADS1(ADS1),
                                                                                                                           _ADS2(nullptr),
                                                                                                                           zeroReading(zeroReading),
                                                                                                                           channel1(ADC1channel),
                                                                                                                           g(localgval){};

LoadCell::LoadCell(ADS131M06 *ADS1, ADS131M06 *ADS2, uint32_t zeroReading, uint8_t ADC1channel, uint8_t ADC2channel, RnpNetworkManager &netman, float localgval) : NRCRemoteSensorBase(netman),
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

float LoadCell::calculateWeight()
{
    //gain1 = _ADS1->gainout;
    //_ADS1->setGain(FOUR);
    if (!_ADS2)
    {
        Weight = (float)(_ADS1->getOutput(channel1) - (float) zeroReading) / (float)conversionfactor;
    }
    else
    {
        // gain2 = _ADS2->gainout;
        // _ADS2->setGain(FOUR);
        Weight = (float)(_ADS1->getOutput(channel1) - _ADS2->getOutput(channel2) - zeroReading) / (float)conversionfactor;
        // _ADS2->setGain(gain2);
    }
    // _ADS1->setGain(gain1);
    return Weight;

}

float LoadCell::calculateMass()
{
    return (float) Weight / (float) g;
}

void LoadCell::update(){
    updateSensorValue(calculateWeight());
}