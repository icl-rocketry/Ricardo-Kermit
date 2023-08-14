/* This is a library for the ADS131M06 6-channel ADC
   
   Product information:
   https://www.ti.com/product/ADS131M06/part-details/ADS131M06IPBSR

   Datasheet: 
   https://www.ti.com/lit/ds/symlink/ads131m06.pdf?ts=1688985721879&ref_url=https%253A%252F%252Fduckduckgo.com%252F

   This library was made for Imperial College London Rocketry
   Created by Aleksandr Kent
   
   Based on ADS131M04-Lib by Daniele Valentino Bella & Iris Clercq-Roques:
   https://github.com/icl-rocketry/ADS131M04-Lib
*/

#ifndef ADS131M06_H
#define ADS131M06_H

#include <Arduino.h>
#include <SPI.h>

class ADS131M06 {
  public:
    ADS131M06(int8_t _csPin, int8_t _clkoutPin, SPIClass* _spi, int8_t _clockCh = 1);
    void begin(void);
    void rawChannels(int8_t * channelArrPtr, int8_t channelArrLen, int32_t * outputArrPtr);
    int32_t rawChannelSingle(int8_t channel);
    uint16_t readReg(uint8_t reg, uint8_t number = 0x00);//modify for multiple consecutive register useage
    bool writeReg(uint8_t reg, uint16_t data, uint8_t number = 0x00);//modify for multiple consecutive
    bool reset(void);//xxxxxx
    bool standby();//xxxxxxxx
    bool wakeup();//xxxxxx
    bool lock();//xxxxxxxxx
    bool unlock();//xxxxxxxx
    uint16_t status();//xxxxxxxx NULL COMMAND (required? same as what occurs in spicommframe?)
    bool setGain(uint8_t log2Gain0 = 0, uint8_t log2Gain1 = 0, uint8_t log2Gain2 = 0, uint8_t log2Gain3 = 0);
    bool globalChop(bool enabled = false, uint8_t log2delay = 4);

  private:
    int8_t csPin, clkoutPin, clockCh;
    SPIClass* spi;
    bool initialised;
    
    void spiCommFrame(uint32_t * outputArray, uint16_t command = 0x0000);
    uint32_t spiTransferWord(uint16_t inputData = 0x0000);
    int32_t twoCompDeco(uint32_t data);

    //Register definitions:-------------------------------------------------------------------------
    static constexpr uint8_t ID = 0x00;
    static constexpr uint8_t STATUS = 0x01;
    static constexpr uint8_t MODE = 0x02;
    static constexpr uint8_t CLOCK = 0x03;
    static constexpr uint8_t GAIN1 = 0x04;
    static constexpr uint8_t GAIN2 = 0x05;
    static constexpr uint8_t CFG = 0x06;
    static constexpr uint8_t THRSHLD_MSB = 0x07;
    static constexpr uint8_t THRSHLD_LSB = 0x08;

    static constexpr uint8_t CLKIN_SPD = 8192000; // Clock speed for the CLKIN pin on the DAC
    static constexpr uint8_t SCLK_SPD = 25000000; // SPI frequency of DAC
    
    //channels: config, ... most sig bit, ... least sig bit, 
    static constexpr uint8_t CH0_CFG = 0x09;
    static constexpr uint8_t CH0_OCAL_MSB = 0x0A;
    static constexpr uint8_t CH0_OCAL_LSB = 0x0B;
    static constexpr uint8_t CH0_GCAL_MSB = 0x0C;
    static constexpr uint8_t CH0_GCAL_LSB = 0x0D;
    
    static constexpr uint8_t CH1_CFG = 0x0E;
    static constexpr uint8_t CH1_OCAL_MSB = 0x0F;
    static constexpr uint8_t CH1_OCAL_LSB = 0x10;
    static constexpr uint8_t CH1_GCAL_MSB = 0x11;
    static constexpr uint8_t CH1_GCAL_LSB = 0x12;

    static constexpr uint8_t CH2_CFG = 0x13;
    static constexpr uint8_t CH2_OCAL_MSB = 0x14;
    static constexpr uint8_t CH2_OCAL_LSB = 0x15;
    static constexpr uint8_t CH2_GCAL_MSB = 0x16;
    static constexpr uint8_t CH2_GCAL_LSB = 0x17;

    static constexpr uint8_t CH3_CFG = 0x18;
    static constexpr uint8_t CH3_OCAL_MSB = 0x19;
    static constexpr uint8_t CH3_OCAL_LSB = 0x1A;
    static constexpr uint8_t CH3_GCAL_MSB = 0x1B;
    static constexpr uint8_t CH3_GCAL_LSB = 0x1C;

    static constexpr uint8_t CH4_CFG = 0x1D;
    static constexpr uint8_t CH4_OCAL_MSB = 0x1E;
    static constexpr uint8_t CH4_OCAL_LSB = 0x1F;
    static constexpr uint8_t CH4_GCAL_MSB = 0x20;
    static constexpr uint8_t CH4_GCAL_LSB = 0x21;

    static constexpr uint8_t CH5_CFG = 0x22;
    static constexpr uint8_t CH5_OCAL_MSB = 0x23;
    static constexpr uint8_t CH5_OCAL_LSB = 0x24;
    static constexpr uint8_t CH5_GCAL_MSB = 0x25;
    static constexpr uint8_t CH5_GCAL_LSB = 0x26;

    static constexpr uint8_t REGMAP_CRC = 0x3E;
    static constexpr uint8_t RESERVED = 0x3F; //This register is exempt from read reg for ADC
};

#endif