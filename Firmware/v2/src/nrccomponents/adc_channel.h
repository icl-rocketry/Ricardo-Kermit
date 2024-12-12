/**
 * @file adc_channel.h
 * @author Andrei Paduraru
 * @brief This class manages individual ADC channels on (for now) ADS131M06 adc chips. It deals with 
 * calibration of channel settings, as well the type of sensor physically attached to the channel and
 * its calibration. 
 * @version 0.1
 * @date 2024-01-18
 * 
 * @copyright Copyright (c) 2024
 * 
 */

#pragma once
#include "Sensors/ADS131M06.h"
#include <stdint.h>

#include <librrc/remote/nrcremotesensorbase.h>
#include <librnp/rnp_networkmanager.h>

#include <librrc/Helpers/nvsstore.h>
#include <librrc/packets/loadcellcalibrationpacket.h>

class adc_channel : public NRCRemoteSensorBase<adc_channel>
{
public:

    adc_channel(RnpNetworkManager& netman,  uint8_t channelIndex, ADS131M06& ADS, uint8_t ADCchannel):
        NRCRemoteSensorBase(netman),
        m_channelInd(channelIndex),
        m_ADS(ADS),
        m_ADSchInd(ADCchannel){};

    float getValue(){return m_value;}

    void calibrate_impl(packetptr_t packetptr);
    
    bool setup(){loadCalibration();};

    void update(){
        m_ADS.getOutput(m_channelInd);

        updateSensorValue(m_value);
    };

private:

    uint8_t m_channelInd;
    ADS131M06& m_ADS;
    uint8_t m_ADSchInd;

    float m_value;

    void loadCalibration();
};