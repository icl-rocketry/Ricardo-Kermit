#include <NTCThermistor/NTCthermistor.h>
#include <ADS1219/ADS1219.h>
#include <global_config.h>

#include <librrc/nrcremotesensorbase.h>
#include <rnp_networkmanager.h>

NTCThermistor::NTCThermistor(ADS1219 &ADS, const uint32_t Rfixed, const uint32_t Rseriesextra, const float A, const float B, const float C, uint8_t ADCchannel,RnpNetworkManager& netman) : 
NRCRemoteSensorBase(netman),
_ADS(ADS),
SHHA(A),
SHHB(B),
SHHC(C),
RFixedPD(Rfixed),
Rseries(Rseriesextra),
channel(ADCchannel)
{};

NTCThermistor::NTCThermistor(ADS1219 &ADS, const uint32_t Rfixed, const uint32_t Rseriesextra, const float gradient, const float Cconstant, uint8_t ADCchannel,RnpNetworkManager& netman) : 
NRCRemoteSensorBase(netman),
_ADS(ADS),
RFixedPD(Rfixed),
Rseries(Rseriesextra),
channel(ADCchannel),
grad(gradient),
constant(Cconstant)
{};

float NTCThermistor::getTemp()
{
    Rtherm = RFixedPD * ((ADCMax / _ADS.readAdjusted(channel)) - 1) - Rseries;
    Temp = 1 / (SHHA + SHHB * log(Rtherm) + SHHC * pow(log(Rtherm), 3));
    return Temp;
}

float NTCThermistor::getTempLinear()
{
    Rtherm = RFixedPD * ((ADCMax / _ADS.readAdjusted(channel)) - 1) - Rseries;
    Temp = grad * Rtherm + constant;
    return Temp;
}

void NTCThermistor::update()
{
    updateSensorValue(getTempLinear());
}