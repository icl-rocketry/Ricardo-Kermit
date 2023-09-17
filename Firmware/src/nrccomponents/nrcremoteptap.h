#pragma once

#include <librrc/nrcremotesensorbase.h>

#include <librnp/rnp_networkmanager.h>
#include <librnp/rnp_packet.h>

#include <librrc/Helpers/nvsstore.h>
#include <librrc/packets/ptapcalibrationpacket.h>

#include "Sensors/ADS131M06.h"

class NRCRemotePTap : public NRCRemoteSensorBase<NRCRemotePTap>
{
    public:


        NRCRemotePTap(RnpNetworkManager& networkmanager,
                    uint8_t PTAPindex,
                    ADS131M06 &ADS,
                    uint8_t adc_channel,
                    float c = 1,
                    float grad = 1
                    ) : 
            NRCRemoteSensorBase(networkmanager),
            _ptapIndex(PTAPindex),
            _ADS(ADS),
            _adc_channel(adc_channel),
            _c(c),
            _grad(grad)
        {loadCalibration();};

        float getConst(){return _c;};

        float getGrad(){return _grad;};

        void calibrate_impl(packetptr_t packetptr);

        float pressure;

        void update();
        void setup(){loadCalibration();};
        
        float getPressure(){return _Pressure;};

    protected:

        friend class NRCRemoteSensorBase<NRCRemotePTap>;
        uint8_t _ptapIndex;
        ADS131M06 &_ADS;
        static constexpr uint32_t ADSMAX = 8388607;
        uint8_t _adc_channel;
        float _c = 0;
        float _grad = 1;
        float _Pressure;

        void loadCalibration();

        
        

};