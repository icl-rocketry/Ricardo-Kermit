
#include <Wire.h>
#include "ADS1219.h"
#include <global_config.h>

ADS1219::ADS1219(TwoWire &wire, uint8_t addr) : _wire(wire)
{
  address = addr;
  _i2cPort = &_wire;
  config = 0x00;
  singleShot = false;
}

void ADS1219::begin(adsGain_t gain, ADSDatarates rate, adsMode_t mode, adsRef_t vref, uint32_t Offset)
{
  setGain(gain);
  setDataRate(rate);
  setConversionMode(mode);
  setVoltageReference(vref);
  deviceOffset = Offset;
  start();
}

void ADS1219::start()
{
  _i2cPort->beginTransmission(address);
  _i2cPort->write(0x08);
  _i2cPort->endTransmission();
}

void ADS1219::powerDown()
{
  _i2cPort->beginTransmission(address);
  _i2cPort->write(0x02);
  _i2cPort->endTransmission();
}

uint8_t ADS1219::readRegister(uint8_t reg)
{
  _i2cPort->beginTransmission(address);
  _i2cPort->write(reg);
  _i2cPort->endTransmission();
  _i2cPort->requestFrom((uint8_t)address, (uint8_t)1);
  return _i2cPort->read();
}

void ADS1219::writeRegister(uint8_t data)
{
  _i2cPort->beginTransmission(address);
  _i2cPort->write(CONFIG_REGISTER_ADDRESS);
  _i2cPort->write(data);
  _i2cPort->endTransmission();
}

uint32_t ADS1219::readConversionResult()
{
  _i2cPort->beginTransmission(address);
  _i2cPort->write(0x10);
  _i2cPort->endTransmission();
  _i2cPort->requestFrom((uint8_t)address, (uint8_t)3);
  uint32_t data32 = _i2cPort->read();
  data32 <<= 8;
  data32 |= _i2cPort->read();
  data32 <<= 8;
  data32 |= _i2cPort->read();
  return (data32 << 8) >> 8;
}

void ADS1219::resetConfig()
{
  writeRegister(0x00);
}

uint32_t ADS1219::readSingleEnded(uint8_t channel)
{
  config &= MUX_MASK;
  switch (channel)
  {
  case (0):
    config |= MUX_SINGLE_0;
    break;
  case (1):
    config |= MUX_SINGLE_1;
    break;
  case (2):
    config |= MUX_SINGLE_2;
    break;
  case (3):
    config |= MUX_SINGLE_3;
    break;
  default:
    break;
  }
  writeRegister(config);
  start();
  return readConversionResult();
}

uint32_t ADS1219::readAdjusted(uint8_t channel)
{
  switch (channel)
  {
  case (10):
  {
    return readDifferential_0_1() - deviceOffset;
  }
  case (12):
  {
    return readDifferential_1_2() - deviceOffset;
  }
  case (23):
  {
    return readDifferential_2_3() - deviceOffset;
  }
  default:
    return readSingleEnded(channel) - deviceOffset;
  }
}

/* uint32_t ADS1219::getOffset(const int readingNumber, const int timeincrement){
  prevTime = 0;
  if( iter != readingNumber) {
    currentTime = millis();
    if(currentTime - prevTime >= timeincrement) {
      prevTime = currentTime;
      doffset = doffset + readShorted();
      currentTime = millis();
      iter++;
    }
  }
  else{
  doffset = doffset / readingNumber;
  }
  return doffset;
} */

uint32_t ADS1219::getOffset(const uint32_t readingNumber, const uint16_t timeincrement)
{
  iter = 0;
  doffset = 0;
  readShorted();
  while (iter < readingNumber)
  {
    if (millis() - prevTime >= timeincrement)
    {
      prevTime = millis();
      doffset += readShorted();
      iter++;
    }
  }
  doffset = doffset / readingNumber;
  return doffset;
}

uint32_t ADS1219::readDifferential_0_1()
{
  config &= MUX_MASK;
  config |= MUX_DIFF_0_1;
  writeRegister(config);
  start();
  return readConversionResult();
}

uint32_t ADS1219::readDifferential_2_3()
{
  config &= MUX_MASK;
  config |= MUX_DIFF_2_3;
  writeRegister(config);
  start();
  return readConversionResult();
}

uint32_t ADS1219::readDifferential_1_2()
{
  config &= MUX_MASK;
  config |= MUX_DIFF_1_2;
  writeRegister(config);
  start();
  return readConversionResult();
}

uint32_t ADS1219::readShorted()
{
  config &= MUX_MASK;
  config |= MUX_SHORTED;
  writeRegister(config);
  start();
  return readConversionResult();
}

void ADS1219::setGain(adsGain_t gain)
{
  config &= GAIN_MASK;
  config |= gain;
  writeRegister(config);
}

void ADS1219::setDataRate(ADSDatarates rate)
{
  config &= DATA_RATE_MASK;
  switch (rate)
  {
  case (ADSDatarates::DataRate_20):
    config |= DATA_RATE_20;
    break;
  case (ADSDatarates::DataRate_90):
    config |= DATA_RATE_90;
    break;
  case (ADSDatarates::DataRate_330):
    config |= DATA_RATE_330;
    break;
  case (ADSDatarates::DataRate_1000):
    config |= DATA_RATE_1000;
    break;
  default:
    break;
  }
  writeRegister(config);
}

void ADS1219::setConversionMode(adsMode_t mode)
{
  config &= MODE_MASK;
  config |= mode;
  writeRegister(config);
  if (mode == CONTINUOUS)
  {
    singleShot = false;
  }
  else
  {
    singleShot = true;
  }
}

void ADS1219::setVoltageReference(adsRef_t vref)
{
  config &= VREF_MASK;
  config |= vref;
  writeRegister(config);
}