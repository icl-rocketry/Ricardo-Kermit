#include "MAX31856.h"

MAX31856::MAX31856(SPIClass &spi, uint8_t cs):
_spi(spi), _spisettings(5000000, MSBFIRST, SPI_MODE1), _cs(cs){};

void MAX31856::setup()
{
    setTCType(TCType::TK);
    setOCDetection(OCSet::disabled);
    setAveraging(AVSet::onesamp);
    setConversionMode(ConvModes::conti);
    enableCJComp(ENCJComp::enable);
    setFaultMode(FaultModes::comparator);
    setFilter(FilterFreqs::sixtyHz);
}

void MAX31856::writeRegister(writeRegisters target, uint8_t data){
    _spi.beginTransaction(_spisettings);
    digitalWrite(_cs, LOW);
    _spi.transfer(static_cast<uint8_t>(target));
    _spi.transfer(data);
    digitalWrite(_cs, HIGH);
    _spi.endTransaction();
}

uint32_t MAX31856::readRegister(readRegisters target, uint8_t Nbytes){
    uint32_t regData = 0;
    _spi.beginTransaction(_spisettings);
    digitalWrite(_cs, LOW);
    _spi.transfer(static_cast<uint8_t>(target));
    regData = _spi.transfer(0);
    for( uint8_t i = 0 ; i < Nbytes ; i++){ 
        regData <<= 8;
        regData |= _spi.transfer(0);
    }
    digitalWrite(_cs, HIGH);
    _spi.endTransaction();
    return regData; 
}

void MAX31856::update(){
    uint32_t TempReg = readRegister(readRegisters::LinTempB2,3);
    TempReg >>= 5; //get rid of the 5 useless bits
    if(TempReg & SignMask19Bit){
        TempReg ^= SignMask19Bit;
        Temp = -1.0 * (float) TempReg * fpScale;
    }
    else{
        Temp = (float) TempReg * fpScale;
    }
    Temp += 273.15;
}

void MAX31856::clearFault(){
    C0Reg &= FaultClearMask;
    C0Reg |= 1;
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
    C0Reg &= FilterMask;
    C0Reg |= Freq;
    writeRegister(writeRegisters::Config0, C0Reg);
}
