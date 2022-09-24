#include <NTCThermistor/NTCthermistor.h>
#include <ADS1219/ADS1219.h>
#include <global_config.h>

NTCThermistor::NTCThermistor(ADS1219* ADS):
_ADS(ADS)
{};


float NTCThermistor::getTemp(const uint32_t Rfixed, const float SHHA, const float SHHB, const float SHHC, uint8_t ADCchannel)
{
    Rtherm = Rfixed * ((16777215/_ADS->readAdjusted(ADCchannel))-1);
    Temp = 1 / (SHHA + SHHB * log(Rtherm) + SHHC * pow(log(Rtherm),3));
    return Temp;
}