#include "nrcremoteredline.h"
#include <functional>

#include <librnp/rnp_networkmanager.h>      

void NRCRemoteRedline::dumpVars()
{
    Serial.println("m_valueredlineLim " + String(m_valueredlineLim));
    Serial.println("m_gradredlineLim " + String(m_gradredlineLim));
    Serial.println("Which Redline " + String(m_monitorIndex));
    Serial.println("m_sensrvalue " + String(m_sensrvalue));
}

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

    if(abs(m_sensrvalue) > m_valueredlineLim && m_valueredlineLim >= 0.001){ // if value is less than 1mbar then take it as unset
        _value = 0; //if redline is exceeded, abort test
        testAbort();
        return;
    }
    else{
        _value = 1; //if value is below redline, proceed
        m_abortcalls = 0;
        // if (debug_print < 10)
        // {
            // dumpVars();
        //     debug_print++;
        // }
    }

    if(abs(m_grad) > m_gradredlineLim && m_gradredlineLim >= 0.001){
        _value = 0; //if redline is exceeded, abort test
        testAbort();
        return;
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

    m_valueredlineLim = calibrate_comm.reading_redline;
    m_gradredlineLim = calibrate_comm.gradient_redline;
}

void NRCRemoteRedline::loadCalibration()
{
    // Serial.println("Loaded calibration for redline monitor: " + String(m_monitorIndex));

    std::string NVSName = "RLMon" + std::to_string(m_monitorIndex);
    NVSStore _NVS(NVSName, NVSStore::calibrationType::LoadCell);

    std::vector<uint8_t> calibSerialised = _NVS.loadBytes();

    if (calibSerialised.size() == 0)
    {
        
        return;
    }

    RedlineCalibrationPacket calibpacket;
    calibpacket.deserializeBody(calibSerialised);

    m_valueredlineLim = calibpacket.reading_redline;
    m_gradredlineLim = calibpacket.gradient_redline;

    Preferences pref;
    pref.begin(NVSName.c_str());
    
    for(uint8_t i = 0; i < Valves.size(); ++i){
        std::string vname = "Valve" + std::to_string(i);
        valveActions[i] = pref.getUShort(vname.c_str(),0);
    }
    pref.end();

    // Serial.println("Redline: " + String(m_valueredlineLim));
}

void NRCRemoteRedline::testAbort(){

    if(m_abortcalls < m_maxAbortCalls){

        RicCoreLogging::log<RicCoreLoggingConfig::LOGGERS::SYS>("Abort called by monitor: " + std::to_string(m_service));
        for(uint8_t i = 0; i < Valves.size(); ++i){
            Valves[i].get().abort(valveActions[i],m_service);
        }
        m_abortcalls++;
        _state = m_abortcalls;
        // Serial.println("Abort called");
        // dumpVars();
    }
}