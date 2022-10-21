#include <PTransducer/PTransducer.h>
#include <ADS1219/ADS1219.h>
#include <global_config.h>

#include <librrc/nrcremotesensorbase.h>
#include <rnp_networkmanager.h>

PTransducer::PTransducer(ADS1219 &ADS, float grad, float c, uint8_t ADCchannel, RnpNetworkManager& netman):
NRCRemoteSensorBase(netman),
_ADS(ADS),
_grad(grad),
_c(c),
_ADCchannel(ADCchannel)
{};


float PTransducer::calculate()
{
    Pressure = ((float) _ADS.readAdjusted(_ADCchannel) - (float)_c)/_grad;
    return Pressure;
}

void PTransducer::update(){
    updateSensorValue(calculate());
}