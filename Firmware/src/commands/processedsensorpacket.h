#pragma once

#include <librnp/rnp_packet.h>
#include <librnp/rnp_serializer.h>

#include <vector>

class ProcessedSensorPacket : public RnpPacket{
    private:
    //serializer framework
        static constexpr auto getSerializer()
        {
            auto ret = RnpSerializer(
                &ProcessedSensorPacket::ch0sens,
                &ProcessedSensorPacket::ch1sens,
                &ProcessedSensorPacket::ch2sens,
                &ProcessedSensorPacket::ch3sens,
                &ProcessedSensorPacket::ch4sens,
                &ProcessedSensorPacket::ch5sens,
                &ProcessedSensorPacket::ch6sens,
                &ProcessedSensorPacket::ch7sens,
                &ProcessedSensorPacket::ch8sens,
                &ProcessedSensorPacket::ch9sens,
                &ProcessedSensorPacket::ch10sens,
                &ProcessedSensorPacket::ch11sens,
                &ProcessedSensorPacket::temp0,
                &ProcessedSensorPacket::temp1,
                &ProcessedSensorPacket::temp2,
                &ProcessedSensorPacket::temp3,
                &ProcessedSensorPacket::system_status,
                &ProcessedSensorPacket::system_time

            
            );
            return ret;
        }
        
    public:
        ~ProcessedSensorPacket();

        ProcessedSensorPacket();
        /**
         * @brief Deserialize Telemetry Packet
         * 
         * @param data 
         */
        ProcessedSensorPacket(const RnpPacketSerialized& packet);

        /**
         * @brief Serialize Telemetry Packet
         * 
         * @param buf 
         */
        void serialize(std::vector<uint8_t>& buf) override;

        
        //packet header
        //PacketHeader header{static_cast<uint8_t>(packet::TELEMETRY), packet_size()};

        //ADC channels
        float ch0sens;
        float ch1sens;
        float ch2sens;
        float ch3sens;
        float ch4sens;
        float ch5sens;
        float ch6sens;
        float ch7sens;
        float ch8sens;
        float ch9sens;
        float ch10sens;
        float ch11sens;

        float temp0;
        float temp1;
        float temp2;
        float temp3;


        //system details
        uint32_t system_status;
        uint64_t system_time;


        static constexpr size_t size(){
            return getSerializer().member_size();
        }

};


