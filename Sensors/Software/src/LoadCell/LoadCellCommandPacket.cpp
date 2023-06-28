#include "LoadCellCommandPacket.h"
#include "rnp_packet.h"
#include "Commands/commandHandler.h"



LoadCellCommandPacket::~LoadCellCommandPacket()
{};

LoadCellCommandPacket::LoadCellCommandPacket(uint8_t _command):
RnpPacket(0,
          static_cast<uint8_t>(CommandHandler::PACKET_TYPES::LCCAL),
          size()),
command(_command)
{};

LoadCellCommandPacket::LoadCellCommandPacket(const RnpPacketSerialized& packet):
RnpPacket(packet,size())
{
    getSerializer().deserialize(*this,packet.getBody());
};

void LoadCellCommandPacket::serialize(std::vector<uint8_t>& buf){
    RnpPacket::serialize(buf);
	size_t bufsize = buf.size();
	buf.resize(bufsize + size());
	std::memcpy(buf.data() + bufsize,getSerializer().serialize(*this).data(),size());
};