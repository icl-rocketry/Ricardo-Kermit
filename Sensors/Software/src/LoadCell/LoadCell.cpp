#include "LoadCell.h"

#include <Preferences.h>
#include <librrc/nrcremotesensorbase.h>
#include <rnp_networkmanager.h>

LoadCell::LoadCell(ADS1219 *ADS1, uint32_t zeroReading, uint8_t ADC1channel, RnpNetworkManager &netman, float localgval, uint8_t LCindex) : NRCRemoteSensorBase(netman),
                                                                                                                                            _ADS1(ADS1),
                                                                                                                                            _ADS2(nullptr),
                                                                                                                                            zeroReading(zeroReading),
                                                                                                                                            channel1(ADC1channel),
                                                                                                                                            g(localgval),
                                                                                                                                            LoadCellInd(LCindex){};

LoadCell::LoadCell(ADS1219 *ADS1, ADS1219 *ADS2, uint32_t zeroReading, uint8_t ADC1channel, uint8_t ADC2channel, RnpNetworkManager &netman, float localgval, uint8_t LCindex) : NRCRemoteSensorBase(netman),
                                                                                                                                                                                _ADS1(ADS1),
                                                                                                                                                                                _ADS2(ADS2),
                                                                                                                                                                                zeroReading(zeroReading),
                                                                                                                                                                                channel1(ADC1channel),
                                                                                                                                                                                channel2(ADC2channel),
                                                                                                                                                                                g(localgval),
                                                                                                                                                                                LoadCellInd(LCindex){};

void LoadCell::setup()
{
    loadConsts();
}

void LoadCell::loadConsts()
{

    Preferences pref;
    auto NVSIndName = "LC" + LoadCellInd;

    pref.begin(NVSIndName);
    Gradient = pref.getFloat("m", 0);
    zeroReading = pref.getUInt("zero", 0);
    g = pref.getFloat("g", 9.81);
}

void LoadCell::setGradient(float grad)
{
    Preferences pref;
    auto NVSIndName = "LC" + LoadCellInd;
    pref.begin(NVSIndName);

    Gradient = grad;
    pref.putFloat("m", 0);
}

void LoadCell::setg(float gravconst)
{
    Preferences pref;
    auto NVSIndName = "LC" + LoadCellInd;
    pref.begin(NVSIndName);

    g = gravconst;
    pref.putFloat("g", gravconst);
}

float LoadCell::calculateWeight()
{
    gain1 = _ADS1->gainout;
    _ADS1->setGain(FOUR);
    if (!_ADS2)
    {
        Weight = (float)(_ADS1->readAdjusted(channel1) - (float)zeroReading) / (float)Gradient;
        // Serial.println((float)Weight/(float)g);
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
    return (float)Weight / (float)g;
}

void LoadCell::zero(uint16_t Nsamples)
{
    Preferences pref;
    auto NVSIndName = "LC" + LoadCellInd;
    pref.begin(NVSIndName);

    uint32_t meanval = 0;

    gain1 = _ADS1->gainout;
    _ADS1->setGain(FOUR);
    if (Nsamples == 0)
    {
        meanval = _ADS1->readAdjusted(channel1);
    }
    else
    {
        for (uint16_t iter = 0; iter < Nsamples; iter++)
        {
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
        meanval = (float)meanval / (float)Nsamples;
    }

    zeroReading = meanval;
    pref.putUInt("zero", meanval);
}

void LoadCell::extendedCommandHandler_impl(const NRCPacket::NRC_COMMAND_ID commandID, packetptr_t packetptr)
{
    SimpleCommandPacket receivedPacket(*packetptr);
    switch (commandID)
    {
    case (NRCPacket::NRC_COMMAND_ID::ZERO):
        zero(receivedPacket.arg);
        break;
    default:
        break;
    }
}

void LoadCell::update()
{
    updateSensorValue(calculateWeight());
}