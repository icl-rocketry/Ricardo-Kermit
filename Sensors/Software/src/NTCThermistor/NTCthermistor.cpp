#include <NTCThermistor/NTCthermistor.h>
#include <ADS1219/ADS1219.h>
#include <global_config.h>

#include <librrc/nrcremotesensorbase.h>
#include <rnp_networkmanager.h>
#include <cstdlib>

/*
NTCThermistor::NTCThermistor(ADS1219 &ADS, const uint32_t Rfixed, const uint32_t Rseriesextra, const float A, const float B, const float C, uint8_t ADCchannel,RnpNetworkManager& netman) : 
NRCRemoteSensorBase(netman),
_ADS(ADS),
SHHA(A),
SHHB(B),
SHHC(C),
RFixedPD(Rfixed),
Rseries(Rseriesextra),
channel(ADCchannel)
{}; */

NTCThermistor::NTCThermistor(ADS1219 &ADS, const uint32_t Rfixed, const uint32_t Rseriesextra, const float T1, const float R1, const float Beta, uint8_t ADCchannel,RnpNetworkManager& netman) : 
NRCRemoteSensorBase(netman),
_ADS(ADS),
RFixedPD(Rfixed),
Rseries(Rseriesextra),
channel(ADCchannel),
Beta(Beta),
T1(T1),
R1(R1)
{};

float NTCThermistor::getTemp()
{
    Rtherm = RFixedPD * ((ADCMax / abs(_ADS.readAdjusted(channel))) - 1) - Rseries;
    Temp = 1 / (SHHA + SHHB * log(Rtherm) + SHHC * pow(log(Rtherm), 3));
    return Temp;
}

float NTCThermistor::getTempLinear()
{
    Rtherm = RFixedPD * (((float)ADCMax) / ((float)_ADS.readAdjusted(channel) - 1.0)) - Rseries;
    //Rtherm = ((float) ADCMax / (float) _ADS.readAdjusted(channel)) * (1 - ((float) _ADS.readAdjusted(channel) / (float) ADCMax)) * (Rseries + RFixedPD) - Rseries; 
    //debug stuff
    // Serial.println("ADC val");
    // Serial.println(_ADS.readAdjusted(channel));
    // Serial.println("Therm resistance");
    // Serial.println(Rtherm);

    Temp = 1.0 / ((1.0/T1) - 1.0/Beta * (log(R1/Rtherm)));
    return Temp;
}

void NTCThermistor::update()
{
    updateSensorValue(getTempLinear());
}