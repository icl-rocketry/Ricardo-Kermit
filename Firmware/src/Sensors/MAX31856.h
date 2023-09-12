#pragma once

#include <Arduino.h>
#include <SPI.h>

class MAX31856{
    
    public:
        enum TCType : uint8_t {
            TB = 0b0000,
            TE = 0b0001,
            TJ = 0b0010,
            TK = 0b0011,
            TN = 0b0100,
            TR = 0b0101,
            TS = 0b0110,
            TT = 0b0111
        };

        enum OCSet : uint8_t {
            disabled = 0b00,
            RSL5K = 0b01,
            RSH5KTCL2 = 0b10,
            RSH5KTCH2 = 0b11
        };
        
        enum AVSet : uint8_t {
            onesamp = 0b000,
            twosamp = 0b001,
            foursamp = 0b010,
            eightsamp = 0b011,
            sixteensamp = 0b100
        };

        enum ConvModes : uint8_t {
            normOff = 0,
            conti = 0b10000000
        };

        enum ENCJComp : uint8_t {
            enable = 0,
            disable = 0b00001000
        };

        enum FaultModes : uint8_t {
            comparator = 0,
            interrupt = 0b00000100
        };

        enum FilterFreqs : uint8_t {
            sixtyHz = 0b00000001,
            fiftyHz = 0
        };
        
        MAX31856(SPIClass &spi, uint8_t cs, TCType TCType = TCType::TK, OCSet OCSet = OCSet::RSH5KTCL2, AVSet AVSet = AVSet::onesamp, ConvModes Mode = ConvModes::conti, ENCJComp EN = ENCJComp::enable, FaultModes Fault = FaultModes::comparator, FilterFreqs Freq = FilterFreqs::sixtyHz);
        void setup();
        void reset();
        void setTCType(TCType TCType);
        void setOCDetection(OCSet OCSet);
        void setAveraging(AVSet AVset);
        void setConversionMode(ConvModes Mode);
        void enableCJComp(ENCJComp EN);
        void setFaultMode(FaultModes Mode);
        void setFilter(FilterFreqs Freq);
        float getTemp() {return Temp;}
        void update();
        
    private:

        enum class readRegisters : uint8_t {
            Config0 = 0x00,
            Config1 = 0x01,
            FaultMask = 0x02,
            CJHFault = 0x03,
            CJLFault = 0x04,
            LinTempFltHMSB = 0x05,
            LinTempFlttHLSB = 0x06,
            LinTempFltLMSB = 0x07,
            LinTempFltLLSB = 0x08,
            CJTOffset = 0x09,
            CJTH = 0x0A,
            CJTL = 0x0B,
            LinTempB2 = 0x0C,
            LinTempB1 = 0x0D,
            LinTempB0 = 0x0E,
            Fault = 0x0F 
        };

        enum class writeRegisters : uint8_t {
            Config0 = 0x80,
            Config1 = 0x81,
            FaultMask = 0x82,
            CJHFault = 0x83,
            CJLFault = 0x84,
            LinTempFltHMSB = 0x85,
            LinTempFlttHLSB = 0x86,
            LinTempFltLMSB = 0x87,
            LinTempFltLLSB = 0x88,
            CJTOffset = 0x89,
            CJTH = 0x8A,
            CJTL = 0x8B,
        };
        
        void setCJCompOffset();
        uint32_t readRegister(readRegisters target, uint8_t Nbytes);
        void writeRegister(writeRegisters target, uint8_t data);
        void clearFault();
        float Temp = 0;
        SPIClass& _spi;
        SPISettings _spisettings;

        uint8_t _cs;

        //Settings
        TCType _Type;
        OCSet _OpenCircDet;
        AVSet _Averaging;
        ConvModes _ConvMode;
        ENCJComp _ColdJComp;
        FaultModes _FMode;
        FilterFreqs _Freq;

        //Shadow registers, all set to factory defaults
        uint8_t C0Reg = 0x00;
        uint8_t C1Reg = 0x03;
    
        //Masks
        static constexpr float fpScale = 1.0/4096.0;
        static constexpr uint32_t SignMask24Bit = 0b100000000000000000000000;
        static constexpr uint32_t MaskAll24Bits = 0b111111111111111111111111;
        //C0
        static constexpr uint8_t ModeMask = 0b01111111;
        static constexpr uint8_t OCFaultMask = 0b11001111;
        static constexpr uint8_t CJCompENMask = 0b11110111;
        static constexpr uint8_t FaultModeMask = 0b11111011;
        static constexpr uint8_t FaultClearMask = 0b11111101;
        static constexpr uint8_t FilterMask = 0b11111110;

        //C1
        static constexpr uint8_t TypeMask = 0b11110000;
        static constexpr uint8_t AVMask = 0b10001111;

};