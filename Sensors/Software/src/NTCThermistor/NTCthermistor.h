#pragma once 

/**
 * @file NTCthermistor.h
 * @author Andrei Paduraru
 * @brief Sensor class for NTC thermistors. 
 * Returns temperature value as a float, calculated
 * using the Steinhart-Hart equation:
 * https://en.wikipedia.org/wiki/Steinhart%E2%80%93Hart_equation
 * 
 * @version 0.1
 * @date 2022-09-22
 *
 * @copyright Copyright (c) 2022
 */
#include <ricardo_pins.h>
#include <global_config.h>
#include "ADS1219/ADS1219.h"
#include <Arduino.h>
#include <librrc/nrcremotesensorbase.h>
#include <rnp_networkmanager.h>
class NTCThermistor : public NRCRemoteSensorBase<NTCThermistor>
{ 

private:
    ADS1219 &_ADS; //reference to an ADS object
    float Temp;
    float Rtherm;
    float SHHA;
    float SHHB;
    float SHHC;
    uint32_t RFixedPD;
    uint32_t Rseries;
    uint8_t channel;
    float Beta;
    float constant;
    //static constexpr uint32_t ADCMax = 11530974; //gnd station
    static constexpr uint32_t ADCMax = 13209007; //feed system
    static constexpr float Vsupply = 5;
    float Vout;
    float T1;
    float R1;
    float calfactor;

public:
	//NTCThermistor(ADS1219 &ADS, const uint32_t Rfixed, const uint32_t Rseriesextra, const float A, const float B, const float C, uint8_t ADCchannel,RnpNetworkManager& netman);
    NTCThermistor(ADS1219 &ADS, const uint32_t Rfixed, const uint32_t Rseriesextra, const float T1, const float R1, const float Beta, const float calibfactor, uint8_t ADCchannel,RnpNetworkManager& netman);
    float calculate();
    float calculateLinear();
    void update();
    float getTempLinear(){return Temp;};

};