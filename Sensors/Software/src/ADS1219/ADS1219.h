#pragma once

/**
 * @file ADS1219.h
 * @author Andrei Paduraru
 * @brief Sensor class for ADS1219 ADC
 * Adapted from the Arduino ADS1219 class:
 * https://github.com/OM222O/ADS1219
 *
 * @version 0.1
 * @date 2022-08-08
 *
 * @copyright Copyright (c) 2022
 *
 */

#include <Wire.h>
#include <global_config.h>

class ADS1219
{
private:
	void start();
	uint8_t readRegister(uint8_t reg);
	void writeRegister(uint8_t data);
	uint32_t readConversionResult();
	uint8_t config;
	bool singleShot;
	uint8_t data_ready;
	uint32_t deviceOffset;
	uint32_t doffset = 0;
	uint32_t prevTime = 0;
	uint32_t iter = 0;

	TwoWire &_wire; // pointer to wire object
	TwoWire *_i2cPort;
	bool _i2cerror; // true if i2c failed to start - shamelessly stolen from Kiran's M8Q code

	// ADS register address
	static constexpr uint8_t CONFIG_REGISTER_ADDRESS = 0x40;
	static constexpr uint8_t STATUS_REGISTER_ADDRESS = 0x24;

	static constexpr uint8_t MUX_MASK = 0b11111000;
	static constexpr uint8_t MUX_DIFF_0_1 = 0b00000000;
	static constexpr uint8_t MUX_DIFF_2_3 = 0b00000001;
	static constexpr uint8_t MUX_DIFF_1_2 = 0b00000010;
	static constexpr uint8_t MUX_SINGLE_0 = 0b00000011;
	static constexpr uint8_t MUX_SINGLE_1 = 0b00000100;
	static constexpr uint8_t MUX_SINGLE_2 = 0b00000101;
	static constexpr uint8_t MUX_SINGLE_3 = 0b00000110;
	static constexpr uint8_t MUX_SHORTED = 0b00000111;

	static constexpr uint8_t GAIN_MASK = 0b11110111;
	static constexpr uint8_t GAIN_ONE = 0b00000000;
	static constexpr uint8_t GAIN_FOUR = 0b0001000;

	static constexpr uint8_t DATA_RATE_MASK = 0b11001111;
	static constexpr uint8_t DATA_RATE_20 = 0b00000000;
	static constexpr uint8_t DATA_RATE_90 = 0b00010000;
	static constexpr uint8_t DATA_RATE_330 = 0b00100000;
	static constexpr uint8_t DATA_RATE_1000 = 0b00110000;

	static constexpr uint8_t MODE_MASK = 0b10111111;
	static constexpr uint8_t MODE_SINGLE_SHOT = 0b00000000;
	static constexpr uint8_t MODE_CONTINUOUS = 0b01000000;

	static constexpr uint8_t VREF_MASK = 0b01111111;
	static constexpr uint8_t VREF_INTERNAL = 0b00000000;
	static constexpr uint8_t VREF_EXTERNAL = 0b10000000;

protected:
	uint8_t address;

public:
	// Constructor
	ADS1219(TwoWire &wire, uint8_t addr);

	// Methods
	void begin(adsGain_t gain = ONE, uint16_t rate = 20, adsMode_t mode = CONTINUOUS, adsRef_t vref = REF_EXTERNAL, uint32_t Offset = 0);
	void resetConfig();
	uint32_t readSingleEnded(uint8_t channel);
	uint32_t readAdjusted(uint8_t channel);
	uint32_t readDifferential_0_1();
	uint32_t readDifferential_2_3();
	uint32_t readDifferential_1_2();
	uint32_t readShorted();
	uint32_t getOffset(const uint32_t readingNumber = 50, const uint16_t timeincrement = 100);
	void setGain(adsGain_t gain);
	void setDataRate(uint16_t rate);
	void setConversionMode(adsMode_t mode);
	void setVoltageReference(adsRef_t vref);
	void powerDown();
};
