#include "nrcremoteptap.h"
#include <Preferences.h>

// uint32_t NRCRemotePTap::getValue()
// {
//     return (((float) _ADS.getOutput(_adc_channel) - (float)_c)/_grad);
// }

void NRCRemotePTap::update(){
    updateSensorValue(pressure);
}

void NRCRemotePTap::calibrate_impl(packetptr_t packetptr){
    
    PTapCalibrationPacket calibrate_comm(*packetptr);

    std::vector<uint8_t> serialisedvect = packetptr->getBody();

    std::string NVSName = "PTAP" + std::to_string(_ptapIndex);
    NVSStore _NVS(NVSName, NVSStore::calibrationType::PTap);
    
    _NVS.saveBytes(serialisedvect);

    _c = calibrate_comm.c;
    _grad = calibrate_comm.grad;
}

void NRCRemotePTap::loadCalibration(){
    
    std::string NVSName = "PTAP" + std::to_string(_ptapIndex);
    NVSStore _NVS(NVSName, NVSStore::calibrationType::PTap);

    std::vector<uint8_t> calibSerialised = _NVS.loadBytes();
    
    if(calibSerialised.size() == 0){
        return;
    }
    PTapCalibrationPacket calibpacket;
    calibpacket.deserializeBody(calibSerialised);

    _c = calibpacket.c;
    _grad = calibpacket.grad;

}