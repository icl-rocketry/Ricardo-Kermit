#pragma once

#include <vector>
#include <string>
#include "flags.h"
#include <array>



//temporary for debugging
#include <rnp_networkmanager.h>

enum class LOG_TYPE:uint8_t{
    TELEMETRY = 0,
    SYSTEM = 1,
    NETWORK = 2,
    ALL
};

class LogController{

    public:
        LogController(RnpNetworkManager& netman);
        void setup();
        //network logging
        //system logging
        void log(const std::string &message);
        void log(uint32_t status,uint32_t flag,std::string message);


    private:
        RnpNetworkManager& _netman;

    
};


