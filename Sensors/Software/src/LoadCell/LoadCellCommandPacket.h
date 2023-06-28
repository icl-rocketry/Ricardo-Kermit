#pragma once

#include "rnp_packet.h"
#include "rnp_serializer.h"

#include <default_packets/simplecommandpacket.h>

#include <Eigen/Core>


class LoadCellCommandPacket: public RnpPacket{
    private:
        static constexpr auto getSerializer()
        {
            auto ret = RnpSerializer(
                &LoadCellCommandPacket::command,
                &LoadCellCommandPacket::gradient,
                &LoadCellCommandPacket::zeroReading,
                &LoadCellCommandPacket::gravConst
            );
            return ret;
        }
    public:
        ~LoadCellCommandPacket();
        LoadCellCommandPacket(uint8_t command);

        /**
         * @brief Deserialize Command Packet from serialized data
         * 
         * @param packet 
         */
        LoadCellCommandPacket(const RnpPacketSerialized& packet);

        /**
         * @brief Serialize into provided buffer
         * 
         * @param buf 
         */
        void serialize(std::vector<uint8_t>& buf) override;

        //data members
        command_t command;

        float gradient;
        uint32_t zeroReading;
        float gravConst;
        
        static constexpr size_t size(){
            return getSerializer().member_size();
        }
};


