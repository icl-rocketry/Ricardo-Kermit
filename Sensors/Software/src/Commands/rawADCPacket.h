#pragma once

#include "rnp_packet.h"
#include "rnp_serializer.h"

#include <vector>

class rawADCPacket : public RnpPacket{
    private:
    //serializer framework
        static constexpr auto getSerializer()
        {
            auto ret = RnpSerializer(
                &rawADCPacket::ch0,
                &rawADCPacket::ch1,
                &rawADCPacket::ch2,
                &rawADCPacket::ch3,
                &rawADCPacket::ch4,
                &rawADCPacket::ch5,
                &rawADCPacket::ch6,
                &rawADCPacket::ch7,
                &rawADCPacket::ch8,
                &rawADCPacket::ch9,
                &rawADCPacket::system_status,
                &rawADCPacket::system_time

            
            );
            return ret;
        }
        
    public:
        ~rawADCPacket();

        rawADCPacket();
        /**
         * @brief Deserialize Telemetry Packet
         * 
         * @param data 
         */
        rawADCPacket(const RnpPacketSerialized& packet);

        /**
         * @brief Serialize Telemetry Packet
         * 
         * @param buf 
         */
        void serialize(std::vector<uint8_t>& buf) override;

        
        //packet header
        //PacketHeader header{static_cast<uint8_t>(packet::TELEMETRY), packet_size()};

        //ADC channels
        uint32_t ch0, ch1, ch2, ch3, ch4, ch5, ch6, ch7, ch8, ch9;

        //system details
        uint32_t system_status;
        uint64_t system_time;


        static constexpr size_t size(){
            return getSerializer().member_size();
        }

};


