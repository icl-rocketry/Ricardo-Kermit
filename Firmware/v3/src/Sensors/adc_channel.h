/**
 * @file adc_channel.h
 * @author Andrei Paduraru
 * @brief Dynamic sensor channel class. This wraps an ADC channel, and handles its settings (such as gain), and what type of
 * sensor the ADC is reading (p.t., loadcell, etc.)
 * TODO
 * - All of it
 * @version 0.1
 * @date 2025-02-07
 *
 * @copyright Copyright (c) 2025
 *
 */
#pragma once
#include <stdint.h>
#include <memory.h>
#include "librrc/Remote/nrcremoteptap.h"
// #include "librrc/Remote/nrcremotethermistor.h"
#include "librrc/Remote/nrcremoteloadcell.h"
#include "librrc/Helpers/nvsstore.h"

#include <librnp/rnp_networkmanager.h>
#include "ADS131M04.h"

#include <variant>
#include <string>

class adc_channel: public NRCRemoteSensorBase<adc_channel>{

using ptap_t = NRCRemoteSensorBase<NRCRemotePTap>;
// using thermistor_t = NRCRemoteSensorBase<NRCRemoteT>; //? There used to be one of these, got lost to time I guess?
using loadcell_t = NRCRemoteSensorBase<NRCRemoteLoadcell>;
    public:

        adc_channel(RnpNetworkManager& networkmanager, ADS131M04& ADC, uint8_t sens_ind, uint8_t adc_ind):
        NRCRemoteSensorBase(networkmanager),
        m_NVSName("ADC" + std::to_string(sens_ind))
        m_ADC(ADC),
        m_sensor_ind(sens_ind),
        m_channel_ind_ADC(adc_ind)
        {};

        /** @brief Retrieve processed value from the underlying sensor bound to this channel.
        * @return Processed sensor value. */
        float getValueProcessed(){return m_sensor.getProcessed()//!this is currently wrong, getValue returns int32 not float};

        /** @brief Return stored raw adc value from the adc peripheral.
        * @return Raw adc value. */
        int32_t getValueRaw(){return m_raw_counts;};

        bool setup(){
            // loadCalibration();//TODO implement
            m_sensor.setup();};
        
        /// @brief Poll ADC peripheral for counts updates, and update underlying sensor value
        void update(){
            m_raw_counts = m_ADC.getOutput(m_channel_ind_ADC);//!TODO Implement ADC...
            m_sensor.update(m_raw_counts);
        };


    private:
        friend NRCRemoteSensorBase<adc_channel>;
        
        std::string m_NVSName; // Name to use for interacting with non volatile storage.

        ADS131M04& m_ADC; //TODO make this ADC device agnostic (HAL?)
        
        uint8_t m_sensor_ind; // Board-level channel index.

        uint8_t m_channel_ind_ADC; // Peripheral-level channel index.

        int32_t m_raw_counts; //Raw adc value from the peripheral.

        std::variant<ptap_t, loadcell_t> m_sensor; //Sensor type connected to the channel

        
        /** @brief Push configuration to ADC peripheral.
        * @param settingsStruct  Struct containing peripheral settings.
        * @return 0 - Error in setting up channel
        * 
        *  1 - Channel setup successful
        */
        bool setupChannel(int32_t settingsStruct); //TODO implement

};