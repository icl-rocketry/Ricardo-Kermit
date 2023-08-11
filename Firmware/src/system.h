#pragma once

#include <libriccore/riccoresystem.h>

#include "Config/systemflags_config.h"
#include "Config/commands_config.h"
#include "Config/pinmap_config.h"
#include "Sensors/MAX31856.h"

#include "Commands/commands.h"

class System : public RicCoreSystem<System,SYSTEM_FLAG,Commands::ID>
{
    public:

        System();
        
        void systemSetup();

        void systemUpdate();

        SPIClass SDSPI;
        SPIClass SNSRSPI;
        
        MAX31856 TC0;
        MAX31856 TC1;
        MAX31856 TC2;
        MAX31856 TC3;
        

    // private:


};