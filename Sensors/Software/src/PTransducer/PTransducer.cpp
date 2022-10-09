#include <PTransducer/PTransducer.h>
#include <ADS1219/ADS1219.h>
#include <global_config.h>

PTransducer::PTransducer(ADS1219 &ADS):
_ADS(ADS)
{};


float PTransducer::getPressure(const float grad, const float c, uint8_t ADCchannel)
{
    Pressure = ((VMax * _ADS.readAdjusted(ADCchannel) / ADCMax) - c )/grad;
    return Pressure;
}