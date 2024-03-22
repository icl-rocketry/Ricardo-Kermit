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

void MAX31856::writeRegister(writeRegisters target, uint8_t data)
{
    _spi.beginTransaction(_spisettings);
    digitalWrite(_cs, LOW);
    _spi.transfer(static_cast<uint8_t>(target));
    _spi.transfer(data);
    _spi.endTransaction();
    digitalWrite(_cs, HIGH);
}

uint32_t MAX31856::readRegister(readRegisters target, uint8_t Nbytes)
{
    uint32_t regData = 0;
    _spi.beginTransaction(_spisettings);
    digitalWrite(_cs, LOW);
    _spi.transfer(static_cast<uint8_t>(target));
    regData = _spi.transfer(0x00);
    for (uint8_t i = 0; i < Nbytes - 1; i++)
    {
        regData <<= 8;
        regData |= _spi.transfer(0x00);
    }
    _spi.endTransaction();
    digitalWrite(_cs, HIGH);
    return regData;
}

void MAX31856::setParameter(writeRegisters Addr, uint8_t ShadowReg, uint8_t Mask, uint8_t Parameter)
{
    ShadowReg &= Mask;
    ShadowReg |= Parameter;
    writeRegister(Addr, ShadowReg);
}

void MAX31856::update()
{
    if(millis() - m_CJtimerOFF > 60000 && _ColdJComp == ENCJComp::disable){
        enableCJComp(ENCJComp::enable);
        m_CJtimerON = millis();
    }

    if(millis() - m_CJtimerON > 200 && _ColdJComp == ENCJComp::disable){
        enableCJComp(_ColdJComp);
        m_CJtimerOFF = millis();
    }
    
    if (millis() - m_prevUpdate > m_updateDelta)
    {
        int32_t TempReg = readRegister(readRegisters::LinTempB2, 3);

        if (TempReg & SignMask24Bit)
        {
            TempReg |= 0xFF000000;
        }
        Temp = (float)TempReg * fpScale;

        if (Temp < -250)
        {
            Temp = NAN;
        }
        m_prevUpdate = millis();
    }
}

void MAX31856::clearFault()
{
    setParameter(writeRegisters::Config0, C0Reg, FaultClearMask, 0b00000010);
}

void MAX31856::setTCType(TCType TCType)
{
    setParameter(writeRegisters::Config1, C1Reg, TypeMask, TCType);
}

void MAX31856::setOCDetection(OCSet OCSet)
{
    setParameter(writeRegisters::Config0, C0Reg, OCFaultMask, OCSet);
}

void MAX31856::setAveraging(AVSet AVset)
{
    setParameter(writeRegisters::Config1, C1Reg, AVMask, AVset);
}

void MAX31856::setConversionMode(ConvModes Mode)
{
    setParameter(writeRegisters::Config0, C0Reg, ModeMask, Mode);
}

void MAX31856::enableCJComp(ENCJComp EN)
{
    setParameter(writeRegisters::Config0, C0Reg, CJCompENMask, EN);
}

void MAX31856::setFaultMode(FaultModes Mode)
{
    setParameter(writeRegisters::Config0, C0Reg, FaultModeMask, Mode);
}

void MAX31856::setFilter(FilterFreqs Freq)
{
    setConversionMode(ConvModes::normOff);
    setParameter(writeRegisters::Config0, C0Reg, FilterMask, Freq);
    setConversionMode(_ConvMode);
}

void MAX31856::setCJOffset(float CJOffset)
{
    uint8_t CJOff = CJOffset;
    setParameter(writeRegisters::CJTOffset, CJOffReg, 0, CJOff);
}
