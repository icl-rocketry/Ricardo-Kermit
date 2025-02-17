#include "rawADCPacket.h"

#include <librnp/rnp_networkmanager.h>
#include <librnp/rnp_packet.h>
#include <librnp/rnp_packet_types.h>

#include <vector>

RawADCPacket::~RawADCPacket() {};

RawADCPacket::RawADCPacket() : RnpPacket(0,
                               static_cast<uint8_t>(RNP_TYPES::KERMIT_PROCESSED),
                               size()) {};

RawADCPacket::RawADCPacket(const RnpPacketSerialized &packet) : RnpPacket(packet, size())
{
    getSerializer().deserialize(*this, packet.getBody());
};

void RawADCPacket::serialize(std::vector<uint8_t> &buf)
{
    RnpPacket::serialize(buf);
    size_t bufsize = buf.size();
    buf.resize(bufsize + size());
    std::memcpy(buf.data() + bufsize, getSerializer().serialize(*this).data(), size());
};