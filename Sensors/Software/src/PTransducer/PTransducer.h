#pragma once
/**
 * @file PTransducer.h
 * @author Andrei Paduraru
 * @brief Sensor class for pressure transducers.
 * 
 * @version 0.1
 * @date 2022-09-24
 *
 * @copyright Copyright (c) 2022
 */
#include <Arduino.h>
#include "ADS1219/ADS1219.h"


#include <librrc/nrcremotesensorbase.h>
#include <rnp_networkmanager.h>

class PTransducer : public NRCRemoteSensorBase<PTransducer>
{

private:
    ADS1219 &_ADS; //reference to an ADS object
    float Pressure;
    static constexpr float VMax = 5;
    static constexpr uint32_t ADCMax = 8388607;
    const float _grad;
    const float _c;
    const uint8_t _ADCchannel;

    float _cnew;
public:
	PTransducer(ADS1219 &ADS, float grad, float c, uint8_t ADCchannel,RnpNetworkManager& netman);
    float calculate();
    float getPressure(){return Pressure;}
    void update();

//protected:
};