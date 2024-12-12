# Kermit
RAE board designed for data acquisition. All versions currently include power regulation for different types of analog sensors, as well as ADC and TC inputs.

# Supported Versions

## v2
v2 Kermit includes:
  - [Ricardo Template](https://github.com/icl-rocketry/Ricardo-BoardTemplate) components.
  - 5V and 10V power outputs from an LDO.
  - 12 ADC channels with a theoretical maximum of 32kSPS from 2x ADS131M06 chips. All 12 channels can be configured for differential and single-ended inputs, and 3.3V supply voltage sensors. 10 of the channels can be configured for 5v voltage-based sensors, and 2 of the channels can be configured for 4-20mA sensors (10V supply).
  - 4 Thermocouple chips (MAX31856) supporting every type of thermocouple.
  - SD Card for in-flight, and high rate, data acquisition.

## v3
v3 Kermit (nicknamed Kermit mini) includes:
  - [Ricardo Template](https://github.com/icl-rocketry/Ricardo-BoardTemplate) components.
  - 5V and 12V power outputs from an LDO.
  - 4 ADC channels with a theoretical maximum of 64kSPS from 1x ADS131M04 chip. All 4 channels can be configured for differential and single-ended inputs, 4-20mA input, voltage sensor input.
  - 2 Thermocouple chips (MAX31856) supporting every type of thermocouple.
  - SD Card for in-flight, and high rate, data acquisition.
