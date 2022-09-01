#include "Arduino.h"
#include "ADS1219/ADS1219.h"
#include "sensor_pins.h"
#include "Wire.h"

TwoWire I2C(0);
ADS1219 ADS1219(I2C,0b1000000);

uint32_t I2cfreq = 400000;

void setup() {
  // put your setup code here, to run once:
I2C.begin(_SDA,_SCL,I2cfreq);
ADS1219.begin();
Serial.begin(115200);

}

void loop() {
  // put your main code here, to run repeatedly:
Serial.println(ADS1219.readSingleEnded(1));
delay(100);
Serial.println(ADS1219.readSingleEnded(2));
delay(100);
Serial.println(ADS1219.readSingleEnded(3));
delay(100);

}