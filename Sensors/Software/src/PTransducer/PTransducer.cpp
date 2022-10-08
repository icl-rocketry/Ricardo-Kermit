#include <PTransducer/PTransducer.h>
#include <ADS1219/ADS1219.h>
#include <global_config.h>

PTransducer::PTransducer(ADS1219 &ADS):
_ADS(ADS)
{};


float PTransducer::getPressure(const float grad, const float c, uint8_t ADCchannel)
{
    Pressure = ((3.308* _ADS.readAdjusted(ADCchannel) / 16777215) - c )/grad;
    return Pressure;
}