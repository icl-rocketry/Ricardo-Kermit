#include "nrcremoteredline.h"

#include <librnp/rnp_networkmanager.h>                                                                                                                        _g(localgval){};

void NRCRemoteRedline::update(float sensrvalue)
{
    m_sensrvalue = sensrvalue;
    if(m_sensrvalue > m_redlineLim){
        _value = 0; //if redline is exceeded, abort test
    }
    else{
        _value = 1; //if value is below redline, proceed
    }

}

void NRCRemoteRedline::calibrate_impl(packetptr_t packetptr)
{

    RedlineCalibrationPacket calibrate_comm(*packetptr);

    std::vector<uint8_t> serialisedvect = packetptr->getBody();

    std::string NVSName = "RLMon" + std::to_string(m_monitorIndex);
    NVSStore _NVS(NVSName, NVSStore::calibrationType::LoadCell);

    _NVS.saveBytes(serialisedvect);

    m_redlineLim = calibrate_comm.redline;
}

void NRCRemoteRedline::loadCalibration()
{

    std::string NVSName = "RLMon" + std::to_string(m_monitorIndex);
    NVSStore _NVS(NVSName, NVSStore::calibrationType::LoadCell);

    std::vector<uint8_t> calibSerialised = _NVS.loadBytes();

    if (calibSerialised.size() == 0)
    {
        return;
    }

    RedlineCalibrationPacket calibpacket;
    calibpacket.deserializeBody(calibSerialised);

    m_redlineLim = calibpacket.redline;
}