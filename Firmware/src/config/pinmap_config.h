/*
**********************
* PINS               *
**********************
 */
#pragma once

namespace PinMap{

    static constexpr int SNSR_MISO = 47;
    static constexpr int SNSR_MOSI = 48;
    static constexpr int SNSR_SCLK = 33;

    static constexpr int SD_SCLK = 12;
    static constexpr int SD_MISO = 11;
    static constexpr int SD_MOSI = 10;

    //note: counting from the leftmost thermocouple to rightmost (layout point of view)
    static constexpr int TC0_Cs = 37;
    static constexpr int TC1_Cs = 36;
    static constexpr int TC2_Cs = 35;
    static constexpr int TC3_Cs = 34;
    
    static constexpr int ADC0_Cs = 13;
    static constexpr int ADC1_Cs = 21;

    static constexpr int TxCan = 17;
    static constexpr int RxCan = 14;

    static constexpr int SdDet_1 = 5;
    //static constexpr int SdDet_2 = 35;
    static constexpr int SdCs_1 = 6;
};


