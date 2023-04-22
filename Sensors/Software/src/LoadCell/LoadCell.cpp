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

void LoadCell::setGradient(float grad)
{
    Gradient = grad;
}

float LoadCell::calculateWeight()
{
    gain1 = _ADS1->gainout;
    _ADS1->setGain(FOUR);
    if (!_ADS2)
    {
        Weight = (float)(_ADS1->readAdjusted(channel1) - (float) zeroReading) / (float)Gradient;
        //Serial.println((float)Weight/(float)g);
    }
    else
    {
        gain2 = _ADS2->gainout;
        _ADS2->setGain(FOUR);
        Weight = (float)(_ADS1->readAdjusted(channel1) - _ADS2->readAdjusted(channel2) - zeroReading) / (float)Gradient;
        _ADS2->setGain(gain2);
    }
    _ADS1->setGain(gain1);
    return Weight;

}

float LoadCell::calculateMass()
{
    return (float) Weight / (float) g;
}

float LoadCell::getGradient(float KnownMass)
{
    if (!_ADS2)
    {
        returnedGrad =(float) (_ADS1->readAdjusted(channel1) - zeroReading) / (float) (KnownMass * g);
    }
    else
    {
        returnedGrad = (float) (_ADS1->readAdjusted(channel1) - _ADS2->readAdjusted(channel2) - zeroReading) /(float) (KnownMass * g);
    }
    return returnedGrad;
}

void LoadCell::zero(uint16_t Nsamples){
    
    uint32_t meanval = 0;

    gain1 = _ADS1->gainout;
    _ADS1->setGain(FOUR);
    if (Nsamples == 0){
        meanval = _ADS1->readAdjusted(channel1);
    }
    else{
    for (uint16_t iter = 0; iter < Nsamples; iter++ ){
        if (!_ADS2)
        {
            meanval += _ADS1->readAdjusted(channel1);
        }
        else
        {
            gain2 = _ADS2->gainout;
            _ADS2->setGain(FOUR);
            meanval += _ADS1->readAdjusted(channel1) - _ADS2->readAdjusted(channel2);
            _ADS2->setGain(gain2);
        }

    _ADS1->setGain(gain1);
    }
    
    meanval = meanval / Nsamples;
    }

    zeroReading = meanval;
    
    }

// void LoadCell::extendedCommandHandler_impl(const NRCPacket::NRC_COMMAND_ID commandID,packetptr_t packetptr){

    
// }

void LoadCell::update(){
    updateSensorValue(calculateWeight());
}