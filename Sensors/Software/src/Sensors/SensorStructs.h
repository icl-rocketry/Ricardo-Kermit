#pragma once
/*
definition of structs used within sensor classes
*/
#include <stdint.h>

namespace SensorStructs
{

    struct ADC{
        float ch0; 
        float ch1;
        float ch2;
        float ch3;
        float ch4;
        float ch5;
        float ch6;
        float ch7;
        float ch8;
        float ch9;
    };

    struct raw_measurements_t
    {
        ADC ADCallch;

        uint64_t system_time;
    };

}

