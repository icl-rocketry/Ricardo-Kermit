#pragma once

#include <librrc/remote/nrcremotesensorbase.h>

#include <librnp/rnp_networkmanager.h>
#include <librnp/rnp_packet.h>

#include <librrc/Helpers/nvsstore.h>
#include "packets/redlinecalibrationpacket.h"
#include <libriccore/filtering/movingAvg.h>

class NRCRemoteRedline : public NRCRemoteSensorBase<NRCRemoteRedline>
{
    public:


        NRCRemoteRedline(RnpNetworkManager& networkmanager,
                    uint8_t monitorInd,
                    float redlineLimit,
                    bool gradientmonitor,
                    float gradUpdateTime = 5e-3
                    ): 
            NRCRemoteSensorBase(networkmanager),
            m_monitorIndex(monitorInd),
            m_redlineLim(redlineLimit),
            m_gradientmonitor(gradientmonitor),
            m_grad_update_time(gradUpdateTime)
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
        uint64_t m_lastSensrTime;
        float m_lastSensrReading;
        float m_grad = 0;
        MovingAvg movingAvg = MovingAvg(2);
        float m_grad_update_time;
        bool m_gradientmonitor;


        void loadCalibration();

        
        

};