#include <LoadCell/LoadCell.h>
#include <ADS1219/ADS1219.h>
#include <global_config.h>

LoadCell::LoadCell(ADS1219* ADS):
_ADS(ADS)
{};

float getWeight(uint8_t ADCchannel) {


}

float getMass(float localgval, uint8_t ADCchannel) {
    return getWeight(ADCchannel)/localgval;
}