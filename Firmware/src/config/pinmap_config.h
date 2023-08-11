/*
**********************
* PINS               *
**********************
 */
#pragma once

namespace PinMap{

    static constexpr int SNSR_MISO = 12;
    static constexpr int SNSR_MOSI = 13;
    static constexpr int SNSR_SCLK = 14;

    static constexpr int SD_SCLK = 18;
    static constexpr int SD_MISO = 19;
    static constexpr int SD_MOSI = 23;

    static constexpr int TC0_Cs = 25;
    static constexpr int TC1_Cs = 25;
    static constexpr int TC2_Cs = 25;
    static constexpr int TC3_Cs = 25;

    static constexpr int SdCs_1 = 25;

    static constexpr int TxCan = 32;
    static constexpr int RxCan = 33;

    static constexpr int SdDet_1 = 34;
    static constexpr int SdDet_2 = 35;
    static constexpr int BattVolt = 36;
    static constexpr int LoraInt = 39;
};


