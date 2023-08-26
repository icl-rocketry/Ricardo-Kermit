#pragma once

#include <librrc/nrcremotesensorbase.h>

#include <librnp/rnp_networkmanager.h>
#include <librnp/rnp_packet.h>
#include <librrc/packets/ptapcalibrationpacket.h>

#include "Sensors/ADS131M06.h"

class NRCRemotePTap : public NRCRemoteSensorBase<NRCRemotePTap>
{
    public:


        NRCRemotePTap(RnpNetworkManager& networkmanager,
                    ADS131M06 &ADS,
                    uint8_t adc_channel,
                    float c = 1,
                    float grad = 1,
                    float resistance = 1
                    ) : 
            NRCRemoteSensorBase(networkmanager),
            _ADS(ADS),
            _adc_channel(adc_channel),
            _c(c),
            _grad(grad),
            _resistance(resistance)
        {};

        uint32_t getValue();

    protected:

        friend class NRCRemoteSensorBase<NRCRemotePTap>;
        ADS131M06 &_ADS;
        static constexpr uint32_t ADSMAX = 8388607;
        uint8_t _adc_channel;
        float _c = 0;
        float _grad = 1;
        float _resistance = 1;

        void calibrate(float c, float grad, float resistance);
        void calibrate_impl(packetptr_t packetptr);     
};