//process commands stored in command buffer and execute

#pragma once



#include <vector>
#include <memory>
#include <functional>

#include "commands.h"
#include "rnp_packet.h"
#include "rnp_networkmanager.h"
#include "commandpacket.h"


class stateMachine;//forward declaration 


class CommandHandler {
    public:
        CommandHandler(stateMachine* sm);
        
        std::function<void(std::unique_ptr<RnpPacketSerialized>)> getCallback();

        static constexpr uint8_t serviceID = static_cast<uint8_t>(DEFAULT_SERVICES::COMMAND); // serivce ID for network manager

        enum class PACKET_TYPES:uint8_t{
            SIMPLE = 0,
            MAGCAL = 10,
            MESSAGE_RESPONSE = 100,
            TELEMETRY_RESPONSE = 101,
            RAW_ADCs = 102
        };

    private:
        stateMachine* _sm; //pointer to state machine

        void handleCommand(std::unique_ptr<RnpPacketSerialized> packetptr);
        
        void TelemetryCommand(const RnpPacketSerialized& packet);
        void rawADCCommand(const RnpPacketSerialized& packet);
        void FreeRamCommand(const RnpPacketSerialized& packet);

};	
