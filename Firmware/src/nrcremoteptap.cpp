#include "nrcremoteptap.h"
#include <Preferences.h>

uint32_t NRCRemotePTap::getValue()
{
    return (((float) _ADS.getOutput(_adc_channel) - (float)_c)/_grad);
}
