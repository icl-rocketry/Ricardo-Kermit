/*
**********************
* PINS               *
**********************
 */
#pragma once

namespace PinMap{

    static constexpr int SNSR_MISO = 37;
    static constexpr int SNSR_MOSI = 36;
    static constexpr int SNSR_SCLK = 38;

    static constexpr int SD_SCLK = 33;
    static constexpr int SD_MISO = 34;
    static constexpr int SD_MOSI = 47;

    static constexpr int TC0_Cs = 12;
    static constexpr int TC1_Cs = 11;
    
    static constexpr int ADC0_Cs = 40;

    static constexpr int ADC_CLK = 35; //clkout pin for the ledc for ADC clock.xxxxxxxxxxxxxxxxx

    static constexpr int TxCan = 42;
    static constexpr int RxCan = 41;

    static constexpr int SdDet_1 = 10;
    static constexpr int SdCs_1 = 48;
    static constexpr int SD_EN = 21;

    static constexpr int FS0 = 4;
};


