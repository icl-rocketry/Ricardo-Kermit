#pragma once

#include <librrc/Remote/nrcremoteactuatorbase.h>

#include <librnp/rnp_networkmanager.h>
#include <librnp/rnp_packet.h>
#include <librrc/Packets/servocalibrationpacket.h>

#include <math.h>
#include <Preferences.h>

typedef uint16_t counts_t;

class valvehelper : public NRCRemoteActuatorBase<valvehelper>
{

public:

    valvehelper(RnpNetworkManager &networkmanager,
                    uint8_t destination,
                    uint8_t destination_service): 
    NRCRemoteActuatorBase(networkmanager),
    m_networkmanager(networkmanager),
    m_destination(destination),
    m_destination_service(destination_service)
        {};

    void abort(uint8_t valveaction, uint8_t sourceservice){
        SimpleCommandPacket actuatevalve(2, valveaction);
        actuatevalve.header.source_service = static_cast<uint8_t>(sourceservice);
        actuatevalve.header.destination_service = m_destination_service;
        actuatevalve.header.source = m_networkmanager.getAddress();
        actuatevalve.header.destination = m_destination;
        actuatevalve.header.uid = 0;
        m_networkmanager.sendPacket(actuatevalve);
    }
protected:
    
    RnpNetworkManager &m_networkmanager;
    friend class NRCRemoteActuatorBase;
    friend class NRCRemoteBase;

    uint8_t m_destination;
    uint8_t m_destination_service;

};
