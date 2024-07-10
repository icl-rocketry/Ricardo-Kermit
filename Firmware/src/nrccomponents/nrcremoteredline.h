#pragma once

#include <librrc/remote/nrcremotesensorbase.h>

#include <librnp/rnp_networkmanager.h>
#include <librnp/rnp_packet.h>

#include <librrc/Helpers/nvsstore.h>
#include "packets/redlinecalibrationpacket.h"
#include <libriccore/filtering/movingAvg.h>
#include "valvehelper.h"
#include <functional>
#include <libriccore/riccorelogging.h>

class NRCRemoteRedline : public NRCRemoteSensorBase<NRCRemoteRedline>
{
    public:


        NRCRemoteRedline(RnpNetworkManager& networkmanager,
                    uint8_t monitorInd,
                    uint8_t service,
                    float redlineLimit,
                    float gradientLimit,
                    float gradUpdateTime = 5e-3
                    ): 
            NRCRemoteSensorBase(networkmanager),
            m_monitorIndex(monitorInd),
            m_service(service),
            m_valueredlineLim(redlineLimit),
            m_gradredlineLim(gradientLimit),
            m_grad_update_time(gradUpdateTime)
            {};

        void calibrate_impl(packetptr_t packetptr);

        void update(float sensrvalue);
        void setup(){loadCalibration();};

        void addvalve(valvehelper& valve)
        {
            Valves.push_back(valve);
            valveActions.push_back(1);
        };


        float getRedline(){return m_valueredlineLim;};

        void extendedCommandHandler_impl(const NRCPacket::NRC_COMMAND_ID commandID,packetptr_t packetptr){
            
            SimpleCommandPacket command_packet(*packetptr);
            setValveAction(static_cast<uint8_t>(commandID),command_packet.arg);
        }
     
        
    protected:

        friend class NRCRemoteSensorBase<NRCRemoteRedline>;
        uint8_t m_monitorIndex;
        uint8_t m_service;
        float m_valueredlineLim;
        float m_gradredlineLim;

        float m_sensrvalue;

        uint64_t m_lastSensrTime;
        float m_lastSensrReading;
        float m_grad = 0;
        MovingAvg movingAvg = MovingAvg(2);

        float m_grad_update_time;
        uint8_t m_maxAbortCalls;

        uint8_t m_abortcalls = 0;

        std::vector<std::reference_wrapper<valvehelper>> Valves;
        std::vector<uint8_t> valveActions;

        void loadCalibration();
        void testAbort();
        void setValveAction(uint8_t valveIndex, uint8_t action){
            
            if(valveIndex > valveActions.size()-1){
                return;
            }

            valveActions.at(valveIndex) = action;

            std::string NVSName = "RLMon" + std::to_string(m_monitorIndex);
            Preferences pref;
            pref.begin(NVSName.c_str());
            std::string vname = "Valve" + std::to_string(valveIndex);
            RicCoreLogging::log<RicCoreLoggingConfig::LOGGERS::SYS>("Set abort action for valve: " + std::to_string(valveIndex) + " in redline monitor: " + std::to_string(m_service));
            pref.putUShort(vname.c_str(),action);
            pref.end();
        };
};