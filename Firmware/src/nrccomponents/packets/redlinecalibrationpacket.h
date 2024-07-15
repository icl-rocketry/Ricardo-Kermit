#pragma once

#include <librnp/rnp_packet.h>
#include <librnp/rnp_serializer.h>

#include <vector>

class RedlineCalibrationPacket : public RnpPacket{
    private:
    //serializer framework
        static constexpr auto getSerializer()
        {
            auto ret = RnpSerializer(
                &RedlineCalibrationPacket::command,
                &RedlineCalibrationPacket::reading_redline,
                &RedlineCalibrationPacket::gradient_redline
            );

            return ret;
        }
        
    public:
        ~RedlineCalibrationPacket();

        RedlineCalibrationPacket();
        /**
         * @brief Deserialize Packet
         * 
         * @param data 
         */
        RedlineCalibrationPacket(const RnpPacketSerialized& packet);
        
        void deserializeBody(std::vector<uint8_t>& buf);
        /**
         * @brief Serialize Packet
         * 
         * @param buf 
         */
        void serialize(std::vector<uint8_t>& buf) override;

        uint8_t command;
        float reading_redline;
        float gradient_redline;

        static constexpr size_t size(){
            return getSerializer().member_size();
        }

};