#pragma once

#include "rnp_packet.h"
#include "rnp_serializer.h"

#include <vector>

class TelemetryPacket : public RnpPacket{
    private:
    //serializer framework
        static constexpr auto getSerializer()
        {
            auto ret = RnpSerializer(
                &TelemetryPacket::ch0sens,
                &TelemetryPacket::ch1sens,
                &TelemetryPacket::ch2sens,
                &TelemetryPacket::ch3sens,
                &TelemetryPacket::ch4sens,
                &TelemetryPacket::ch5sens,
                &TelemetryPacket::ch6sens,
                &TelemetryPacket::ch7sens,
                &TelemetryPacket::ch8sens,
                &TelemetryPacket::ch9sens,
                &TelemetryPacket::system_status,
                &TelemetryPacket::system_time,

            
            );
            return ret;
        }
        
    public:
        ~TelemetryPacket();

        TelemetryPacket();
        /**
         * @brief Deserialize Telemetry Packet
         * 
         * @param data 
         */
        TelemetryPacket(const RnpPacketSerialized& packet);

        /**
         * @brief Serialize Telemetry Packet
         * 
         * @param buf 
         */
        void serialize(std::vector<uint8_t>& buf) override;

        
        //packet header
        //PacketHeader header{static_cast<uint8_t>(packet::TELEMETRY), packet_size()};

        //ADC channels
        float ch0sens, ch1sens, ch2sens, ch3sens, ch4sens, ch5sens, ch6sens, ch7sens, ch8sens, ch9sens;

        //system details
        uint32_t system_status;
        uint64_t system_time;


        static constexpr size_t size(){
            return getSerializer().member_size();
        }

};


