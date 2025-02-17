#include "processedsensorpacket.h"

#include <vector>
#include <librnp/rnp_packet_types.h>


ProcessedSensorPacket::~ProcessedSensorPacket()
{};

ProcessedSensorPacket::ProcessedSensorPacket():
RnpPacket(0,
          static_cast<uint8_t>(RNP_TYPES::KERMIT_PROCESSED),
          size())
{};

ProcessedSensorPacket::ProcessedSensorPacket(const RnpPacketSerialized& packet):
RnpPacket(packet,size())
{
    getSerializer().deserialize(*this,packet.getBody());
};

void ProcessedSensorPacket::serialize(std::vector<uint8_t>& buf){
    RnpPacket::serialize(buf);
	size_t bufsize = buf.size();
	buf.resize(bufsize + size());
	std::memcpy(buf.data() + bufsize,getSerializer().serialize(*this).data(),size());
};