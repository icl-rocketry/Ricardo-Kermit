#include "nrcremoteredline.h"
#include <functional>

#include <librnp/rnp_networkmanager.h>                                                                                                                        _g(localgval){};

void NRCRemoteRedline::update(float sensrvalue)
{
    m_sensrvalue = sensrvalue;

    float dt = (esp_timer_get_time() - m_lastSensrTime)/1.0e6;
    if(dt < m_grad_update_time){return;}; 

    float grad = (m_sensrvalue - m_lastSensrReading)/dt ;
    m_lastSensrReading = sensrvalue;
    m_lastSensrTime = esp_timer_get_time();
    movingAvg.update(grad);
    m_grad = movingAvg.getAvg();

    if(abs(m_sensrvalue) > m_valueredlineLim && m_valueredlineLim != 0){
        _value = 0; //if redline is exceeded, abort test
        testAbort();
    }
    else{
        _value = 1; //if value is below redline, proceed
        m_abortcalls = 0;
    }

    if(abs(m_grad) > m_gradredlineLim && m_gradredlineLim != 0){
        _value = 0; //if redline is exceeded, abort test
        testAbort();
    }
    else{
        _value = 1; //if value is below redline, proceed
        m_abortcalls = 0;
    }
}

void NRCRemoteRedline::calibrate_impl(packetptr_t packetptr)
{

    RedlineCalibrationPacket calibrate_comm(*packetptr);

    std::vector<uint8_t> serialisedvect = packetptr->getBody();

    std::string NVSName = "RLMon" + std::to_string(m_monitorIndex);
    NVSStore _NVS(NVSName, NVSStore::calibrationType::LoadCell);

    _NVS.saveBytes(serialisedvect);

    m_valueredlineLim = calibrate_comm.redline;
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

    m_valueredlineLim = calibpacket.redline;

    Preferences pref;
    pref.begin(NVSName.c_str());
    
    for(uint8_t i = 0; i < Valves.size(); ++i){
        std::string vname = "Valve" + std::to_string(i);
        valveActions[i] = pref.getUShort(vname.c_str(),0);
    }
    pref.end();
}

void NRCRemoteRedline::testAbort(){

    if(m_abortcalls < m_maxAbortCalls){

    RicCoreLogging::log<RicCoreLoggingConfig::LOGGERS::SYS>("Abort called by monitor: " + std::to_string(m_service));
    for(uint8_t i = 0; i < Valves.size(); ++i){
        Valves[i].get().abort(valveActions[i],m_service);
    }
        m_abortcalls++;
    }
}