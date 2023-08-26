#include "MAX31856.h"

MAX31856::MAX31856(SPIClass &spi, uint8_t cs, TCType TCType, OCSet OCSet, AVSet AVSet, ConvModes Mode, ENCJComp EN, FaultModes Fault, FilterFreqs Freq):
_spi(spi), _spisettings(5e6, MSBFIRST, SPI_MODE1), _cs(cs), _Type(TCType), _OpenCircDet(OCSet), _Averaging(AVSet), _ConvMode(Mode), _ColdJComp(EN), _FMode(Fault), _Freq(Freq) {};

void MAX31856::setup()
{
    clearFault();
    setConversionMode(ConvModes::normOff);
    setTCType(_Type);
    setOCDetection(_OpenCircDet);
    setAveraging(_Averaging);
    enableCJComp(_ColdJComp);
    setFaultMode(_FMode);
    setFilter(_Freq);
    setConversionMode(_ConvMode);
}

void MAX31856::writeRegister(writeRegisters target, uint8_t data){
    
    digitalWrite(_cs, LOW);
    _spi.beginTransaction(_spisettings);
    _spi.transfer(static_cast<uint8_t>(target));
    _spi.transfer(data);
    _spi.endTransaction();
    digitalWrite(_cs, HIGH);
}

uint32_t MAX31856::readRegister(readRegisters target, uint8_t Nbytes){
    uint32_t regData = 0;
    
    digitalWrite(_cs, LOW);
    _spi.beginTransaction(_spisettings);
    _spi.transfer(static_cast<uint8_t>(target));
    regData = _spi.transfer(0x00);
    for( uint8_t i = 0 ; i < Nbytes - 1 ; i++){ 
        regData <<= 8;
        regData |= _spi.transfer(0x00);
    }
    _spi.endTransaction();
    digitalWrite(_cs, HIGH);
    return regData; 
}

void MAX31856::update(){
    int32_t TempReg = readRegister(readRegisters::LinTempB2,3);

    Temp = (TempReg << 8) * fpScale;
    if(TempReg & SignMask19Bit){
        TempReg ^= SignMask19Bit;
        Temp = -1.0 * (float) TempReg * fpScale;
    }
    else{
        Temp = (float) TempReg * fpScale;
    }
    Temp += 273.15;
    if(Temp < 0){
        Temp = NAN;
    }
}

void MAX31856::clearFault(){
    C0Reg &= FaultClearMask;
    C0Reg |= 0b00000010;
    writeRegister(writeRegisters::Config0, C0Reg);
}

void MAX31856::setTCType(TCType TCType){
    C1Reg &= TypeMask;
    C1Reg |= TCType;
    writeRegister(writeRegisters::Config1, C1Reg);
}

void MAX31856::setOCDetection(OCSet OCSet){
    C0Reg &= OCFaultMask;
    C0Reg |= OCSet;
    writeRegister(writeRegisters::Config0, C0Reg);
}

void MAX31856::setAveraging(AVSet AVset){
    C1Reg &= AVMask;
    C1Reg |= AVset;
    writeRegister(writeRegisters::Config1, C1Reg);
}

void MAX31856::setConversionMode(ConvModes Mode){
    C0Reg &= ModeMask;
    C0Reg |= Mode;
    writeRegister(writeRegisters::Config0, C0Reg);
}

void MAX31856::enableCJComp(ENCJComp EN){
    C0Reg &= CJCompENMask;
    C0Reg |= EN;
    writeRegister(writeRegisters::Config0, C0Reg);
}

void MAX31856::setFaultMode(FaultModes Mode){
    C0Reg &= FaultModeMask;
    C0Reg |= Mode;
    writeRegister(writeRegisters::Config0, C0Reg);
}

void MAX31856::setFilter(FilterFreqs Freq){
    setConversionMode(ConvModes::normOff);
    C0Reg &= FilterMask;
    C0Reg |= Freq;
    writeRegister(writeRegisters::Config0, C0Reg);
    setConversionMode(ConvModes::conti);
}
