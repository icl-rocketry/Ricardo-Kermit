#include "nrcremoteloadcell.h"

#include <librrc/nrcremotesensorbase.h>
#include <librnp/rnp_networkmanager.h>

NRCRemoteLoadcell::NRCRemoteLoadcell(RnpNetworkManager &netman,  uint8_t loadCellIndex, ADS131M06& ADS, uint8_t ADCchannel, uint32_t zeroReading, float grad, float localgval) : NRCRemoteSensorBase(netman),
                                                                                                                           _LCindex(loadCellIndex),
                                                                                                                           _ADS(ADS),
                                                                                                                           _const(zeroReading),
                                                                                                                           _gradient(grad),
                                                                                                                           _adc_channel(ADCchannel),
                                                                                                                           _g(localgval){};

void NRCRemoteLoadcell::update()
{
    _Weight = (float)(_ADS.getOutput(_adc_channel) - (float)_const) / (float)_gradient;
    updateSensorValue(_Weight);
}

void NRCRemoteLoadcell::calibrate_impl(packetptr_t packetptr)
{

    LoadCellCalibrationPacket calibrate_comm(*packetptr);

    std::vector<uint8_t> serialisedvect = packetptr->getBody();

    std::string NVSName = "LCell" + std::to_string(_LCindex);
    NVSStore _NVS(NVSName, NVSStore::calibrationType::LoadCell);

    _NVS.saveBytes(serialisedvect);

    _const = calibrate_comm.c;
    _gradient = calibrate_comm.grad;
}

void NRCRemoteLoadcell::loadCalibration()
{

    std::string NVSName = "LCell" + std::to_string(_LCindex);
    NVSStore _NVS(NVSName, NVSStore::calibrationType::LoadCell);

    std::vector<uint8_t> calibSerialised = _NVS.loadBytes();

    if (calibSerialised.size() == 0)
    {
        return;
    }

    LoadCellCalibrationPacket calibpacket;
    calibpacket.deserializeBody(calibSerialised);

    _const = calibpacket.c;
    _gradient = calibpacket.grad;
}