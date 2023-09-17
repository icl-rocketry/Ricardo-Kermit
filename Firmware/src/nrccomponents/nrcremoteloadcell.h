#pragma once

/**
 * @file LoadCell.h
 * @author Andrei Paduraru
 * @brief Class to read an analog load cell and convert it to a mass or weight (configurable)
 * @version 0.1
 * @date 2022-09-30
 *
 * @copyright Copyright (c) 2022
 *
 */


#include "Sensors/ADS131M06.h"
#include <stdint.h>

#include <librrc/nrcremotesensorbase.h>
#include <librnp/rnp_networkmanager.h>

#include <librrc/Helpers/nvsstore.h>
#include <librrc/packets/loadcellcalibrationpacket.h>

class NRCRemoteLoadcell : public NRCRemoteSensorBase<NRCRemoteLoadcell>
{
public:

    NRCRemoteLoadcell(RnpNetworkManager& netman,  uint8_t loadCellIndex, ADS131M06& ADS, uint8_t ADCchannel, uint32_t zeroReading = 0, float grad = 1, float localgval = 9.81);

    float getWeight(){return _Weight;};
    float getMass(){return (float)(_Weight)/(float)_g;};

    void calibrate_impl(packetptr_t packetptr);
    
    void setup(){loadCalibration();};
    void update();

private: 
    uint8_t _LCindex;
    ADS131M06& _ADS;
    int32_t _const;
    float _gradient;
    uint8_t _adc_channel;
    float _g;
    float _Weight;

    void loadCalibration();
};