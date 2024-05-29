/**
 * @file nrcremotethermocouple.h
 * @author Artem Sheykin (as8520@ic.ac.uk)
 * @brief Rocket component wrapper for thermocouples.
 * @version 0.1
 * @date 2023-08-21
 * 
 * @copyright Copyright (c) 2023
 * 
 */

#pragma once

#include <librrc/remote/nrcremotesensorbase.h>

#include <librnp/rnp_networkmanager.h>
#include <librnp/rnp_packet.h>

#include <Sensors/MAX31856.h>

class NRCRemoteThermocouple : public NRCRemoteSensorBase<NRCRemoteThermocouple>
{
    public:

        NRCRemoteThermocouple(RnpNetworkManager& networkmanager,
                    MAX31856 &MAX) : 
                NRCRemoteSensorBase(networkmanager),
                _MAX(MAX)
            {};
        
        float getValue(){return _MAX.getTemp();};
        void update(){updateSensorValue(_MAX.getTemp());};
    
    protected:

        friend class NRCRemoteSensorBase<NRCRemoteThermocouple>;
        uint8_t _tcIndex;
        MAX31856 &_MAX;

        void loadCalibration();
        
};
