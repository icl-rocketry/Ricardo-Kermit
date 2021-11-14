#Engine electronics - Sensor board

#Sensor board built off Ricardo and DAQ, adapted to USB C (2.0)

#up to 12 ch outputs at 250Hz max on continuous sampling mode (from I2C ADC's)
#Currently supports 10 thermocouple SPI IC's, but in theory could support up to 14 assuming no issues with SPI bus impedence (otherwise 11)
#Main files are Mainboard.pro/Mainboard.sch/analogueI2C.sch/power.sch/thermocouple.sch, ignore the other files 
