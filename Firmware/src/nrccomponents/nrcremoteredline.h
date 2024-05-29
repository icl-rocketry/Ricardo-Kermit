#pragma once

#include <librrc/remote/nrcremotesensorbase.h>

#include <librnp/rnp_networkmanager.h>
#include <librnp/rnp_packet.h>

#include <librrc/Helpers/nvsstore.h>
#include "packets/redlinecalibrationpacket.h"

class NRCRemoteRedline : public NRCRemoteSensorBase<NRCRemoteRedline>
{
    public:


        NRCRemoteRedline(RnpNetworkManager& networkmanager,
                    uint8_t monitorInd,
                    float redlineLimit,
                    bool gradientmonitor
                    ): 
            NRCRemoteSensorBase(networkmanager),
            m_monitorIndex(monitorInd),
            m_redlineLim(redlineLimit),
            m_gradientmonitor(gradientmonitor)
            {};

        void calibrate_impl(packetptr_t packetptr);

        void update(float sensrvalue);
        void setup(){loadCalibration();};

        float getRedline(){return m_redlineLim;};
        
    protected:

        friend class NRCRemoteSensorBase<NRCRemoteRedline>;
        uint8_t m_monitorIndex;
        float m_redlineLim;
        float m_sensrvalue;
        bool m_gradientmonitor;


        void loadCalibration();

        
        

};