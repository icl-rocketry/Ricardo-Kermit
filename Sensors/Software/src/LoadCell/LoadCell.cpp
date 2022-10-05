#include "LoadCell.h"
#include <ADS1219/ADS1219.h>
#include <global_config.h>

LoadCell::LoadCell(ADS1219 *ADS1, uint32_t zeroMassreading, float localgval, uint8_t ADC1channel) : _ADS1(ADS1)
{
    zeroReading = zeroMassreading;
    channel1 = ADC1channel;
    g = localgval;
    multipledevices = 0;
};

LoadCell::LoadCell(ADS1219 *ADS1, ADS1219 *ADS2, uint32_t zeroMassreading, float localgval, uint8_t ADC1channel, uint8_t ADC2channel) : _ADS1(ADS1),
                                                                                                                                                          _ADS2(ADS2)
{
    zeroReading = zeroMassreading;
    channel1 = ADC1channel;
    channel2 = ADC2channel;
    g = localgval;
    multipledevices = 1;
};

void setConversionFactor(float convfactor){
    conversionfactor = convfactor;
}

float getWeight()
{
    if (multipledevices == 0)
    {
        Weight = (_ADS1->readAdjusted(channel1) - zeroReading) / conversionfactor;
    }
    else
    {
        Weight = (_ADS1->readAdjusted(channel1) - _ADS2->readAdjusted(channel2) - zeroReading) / conversionfactor;
    }
    return Weight;
}

float getMass()
{
    return getWeight() / localgval;
}

float getConversionFactor(float KnownMass)
{
    if (multipledevices == 0)
    {
        returnedconvfactor = (_ADS1->readAdjusted(channel1) - zeroMassreading) / (KnownMass * localgval);
    }
    else
    {
        returnedconvfactor = (_ADS1->readAdjusted(channel1) - _ADS2->readAdjusted(channel2) - zeroMassreading) / (KnownMass * localgval);
    }
    return returnedconvfactor;
}