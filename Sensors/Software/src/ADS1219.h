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

class ADS1219  {
  	private:

		void start();
		uint8_t readRegister(uint8_t reg);
		void writeRegister(uint8_t data);
		long readConversionResult();
		uint8_t config;
		bool singleShot;
		int data_ready;

		TwoWire& _wire; //pointer to wire object
		TwoWire *_i2cPort;
		bool _i2cerror; // true if i2c failed to start - shamelessly stolen from Kiran's M8Q code
		
		//ADS register address
		static constexpr uint8_t CONFIG_REGISTER_ADDRESS = 0x40;
		static constexpr uint8_t STATUS_REGISTER_ADDRESS = 0x24;

		static constexpr uint8_t MUX_MASK = 0x1F;
		static constexpr uint8_t MUX_DIFF_0_1 = 0x00;
		static constexpr uint8_t MUX_DIFF_2_3 = 0x20;
		static constexpr uint8_t MUX_DIFF_1_2 = 0x40;
		static constexpr uint8_t MUX_SINGLE_0 = 0x60;
		static constexpr uint8_t MUX_SINGLE_1 = 0x80;
		static constexpr uint8_t MUX_SINGLE_2 = 0xA0;
		static constexpr uint8_t MUX_SINGLE_3 = 0xC0;
		static constexpr uint8_t MUX_SHORTED = 0xE0;

		static constexpr uint8_t GAIN_MASK  = 0xEF;
		static constexpr uint8_t GAIN_ONE = 0x00;
		static constexpr uint8_t GAIN_FOUR = 0x10;

		static constexpr uint8_t DATA_RATE_MASK = 0xF3;
		static constexpr uint8_t DATA_RATE_20 = 0x00;
		static constexpr uint8_t DATA_RATE_90 = 0x04;
		static constexpr uint8_t DATA_RATE_330 = 0x08;
		static constexpr uint8_t DATA_RATE_1000	= 0x0c;

		static constexpr uint8_t MODE_MASK = 0xFD;
		static constexpr uint8_t MODE_SINGLE_SHOT = 0x00;
		static constexpr uint8_t MODE_CONTINUOUS = 0x02;

		static constexpr uint8_t VREF_MASK = 0xFE;
		static constexpr uint8_t VREF_INTERNAL = 0x00;
		static constexpr uint8_t VREF_EXTERNAL = 0x01;

		typedef enum{
		ONE	= GAIN_ONE,
		FOUR	= GAIN_FOUR
		}adsGain_t;

		typedef enum{
		SINGLE_SHOT	= MODE_SINGLE_SHOT,
		CONTINUOUS	= MODE_CONTINUOUS
		}adsMode_t;

		typedef enum{
		REF_INTERNAL	= VREF_INTERNAL,
		REF_EXTERNAL	= VREF_EXTERNAL
		}adsRef_t;
  
  protected:
		uint8_t address;
  public:
		// Constructor 
		ADS1219(TwoWire& wire,uint8_t addr);

		// Methods
		void begin(adsGain_t gain = ONE, int rate = 20, adsMode_t mode = SINGLE_SHOT, adsRef_t vref = REF_EXTERNAL);
		void resetConfig();
		long readSingleEnded(int channel);
		long readDifferential_0_1();
		long readDifferential_2_3();
		long readDifferential_1_2();
		long readShorted();
		void setGain(adsGain_t gain);
		void setDataRate(int rate);
		void setConversionMode(adsMode_t mode);
		void setVoltageReference(adsRef_t vref);
		void powerDown();
};
