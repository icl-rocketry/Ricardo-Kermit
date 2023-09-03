/* This is a library for the ADS131M06 6-channel ADC
   
   Product information:
   https://www.ti.com/product/ADS131M06/part-details/ADS131M06IPBSR

   Datasheet: 
   https://www.ti.com/lit/ds/symlink/ads131m06.pdf?ts=1688985721879&ref_url=https%253A%252F%252Fduckduckgo.com%252F

   This library was made for Imperial College London Rocketry
   Created by Aleksandr Kent
   
   Based on ADS131M04-Lib by Daniele Valentino Bella & Iris Clercq-Roques:
   https://github.com/icl-rocketry/ADS131M04-Lib
*/

/*DEV:
  - Add in a command/option to sync devices (or check they are synced). The command SYNC/RESET maybe should be made in the overall code
  - for transferword, can we stick with transmitting 8 bits at a time or should this be changed to 16 bits for commands? (commands are each 16
  bits)
  - function to output array data
  - 
*/

#include <Arduino.h>
#include <SPI.h>
#include "ADS131M06.h"
#include <libriccore/riccorelogging.h>

ADS131M06::ADS131M06(SPIClass &_spi, uint8_t _csPin, uint8_t _clkoutPin, uint8_t _clockCh):
spi(_spi),
_spisettings(SCLK_SPD, MSBFIRST, SPI_MODE1),
csPin(_csPin),
clkoutPin(_clkoutPin),
clockCh(_clockCh),
clockEnabled(true),
initialised(false),
resultsVect(6),
responseVect(8),
outputVect(6)
{}

ADS131M06::ADS131M06(SPIClass &_spi, uint8_t _csPin):
spi(_spi),
_spisettings(SCLK_SPD, MSBFIRST, SPI_MODE1),
csPin(_csPin),
clockEnabled(false),
initialised(false),
resultsVect(6),
responseVect(8),
outputVect(6)
{}
//-----------------------------------------------------------------------------------------------------------------------------------------------
void ADS131M06::setup() {
  pinMode(csPin, OUTPUT);//set the pinmode of csPin to output data
  digitalWrite(csPin, HIGH);//set the csPin to output high (active low)

  /* Set CLKOUT on the ESP32. used for generation of the ADC clock signal using ledc.
    Define the individual clock channel to generate the signal and the clock frequency to
    generate it at.
    AttachPin takes the GPIO pin which will be attached to the channel specified.
    
  */
 //RicCoreLogging::log<RicCoreLoggingConfig::LOGGERS::SYS>("clk: ", clkConfig());//xxxxxxxx
 if (clockEnabled == true){
  ledcSetup(clockCh, CLKIN_SPD, 2); //duty cycle resolution = 2bits? check datasheet mohammad
  ledcAttachPin(clkoutPin, clockCh);
  ledcWrite(clockCh, 2);
 }
  setOSR(OSROPT::OSR8192);
  
  initialised=true;
}
//------------------------------------------------------------------------------------------------------------------------------------------------
void ADS131M06::update(){
  /*serial print the every channel data*/
  const int8_t channelArrLen = 6;
  int8_t channelArrPtr[channelArrLen] = {0,1,2,3,4,5};
  rawChannels(channelArrPtr, channelArrLen, outputVect);
  // for(int8_t i = 0;i<channelArrLen; i++){
  //   RicCoreLogging::log<RicCoreLoggingConfig::LOGGERS::SYS>(i, " ", outputArrPtr[i]);
  // }

}

void ADS131M06::rawChannels(int8_t * channelArrPtr, int8_t channelArrLen, std::vector<int32_t>& outputVect) {
  /* Writes data from the channels specified in channelArr, to outputArr,
     in the correct order.

     channelArr should have values from 0-5 (specifying the 6 ADC channels), and channelArrLen should be the
     length of that array, starting from 1. (length = 4 if 0-3 channels are used)
     function is void as pointers are used to modify the arrays which are passed into the function.
  */
  if(channelArrLen <= 6){//make sure length is within number of channels available
      // Get data
      spiCommFrame(resultsVect); 

      // Save the decoded data for each of the channels
      for (int8_t i = 0; i<channelArrLen; i++) {
         outputVect[i] = twoCompDeco(resultsVect[i+1]);
      }
   }
  else{
      RicCoreLogging::log<RicCoreLoggingConfig::LOGGERS::SYS>("ERROR: Array length specified exceeds maximum number of channels available");
   }
}
/*Overall, this function takes in an array which contains values specifying the channels where the data will be copied from into the outputArr.
All channel data is copied initially into the rawDataArr using spiCommFrame and then the data which corresponds to the channels specified in
channelDataArr is copied from the rawDataArr into the outputArr*/

//--------------------------------------------------------------------------------------------------------------------------------------------
// int32_t ADS131M06::rawChannelSingle(int8_t channel) {
//   /* Returns raw value from a single channel
//      channel input from 0-5 (specifies the single channel, one of the 6 channels, zero indexed)
//      This function is wrapper function of the raw channels function since this method is called inside.
//   */
  
//   int32_t outputArr[1]; // define output array with 1 memory location (just a pointer)
//   int8_t channelArr[1] = {channel}; /*define a channel array which contains only the number which specifies the designated input channel.
//   An output channel is also created of length 1 in order to provide a location for this data to be copied to, then this value is outputted*/

//   rawChannels(&channelArr[0], 1, &outputArr[0]);/*Using rawChannels() function: input memory location of first and only element of channelArr[]
//   , set length to 1 and give memory location of first and only element of outputArr[] where the data from the channel specified by channelArr[0]
//   (or *channelArr) is copied to.*/

//   return outputArr[0]; // returns the dereferenced pointer, therefore just the value of the data.
// }
//-----------------------------------------------------------------------------------------------------------------------------------------------
bool ADS131M06::globalChop(bool enabled, uint8_t log2delay) {//XXXXXXXXXXXXXXXXXXXXXXXX
  /* Function to configure global chop mode for the ADS131M04.

     INPUTS:
     enabled - Whether to enable global-chop mode.
     log2delay   - Base 2 log of the desired delay in modulator clocks periods
     before measurment begins
     Possible values are between and including 1 and 16, to give delays
     between 2 and 65536 (2^16) clock periods respectively
     For more information, refer to the datasheet.

     Returns true if settings were written succesfully.
  */

  uint8_t delayRegData = log2delay - 1;

  // Get current settings for current detect mode from the CFG register
  uint16_t currentDetSett = (readReg(CFG) << 8) >>8; // CFG = config
  /*CFG is an address of the chop mode config settings, therefore readReg(CFG) returns the value stored at register CFG
  This result is then shifted 8 bits to the left, then shifted 8 bits to the right to zero the zero the most significant byte and
  sets currentDetSett to the value stored at the address CFG.
  
  shifting actually zeros out the most significant byte (8 bits). This means that 1111111100000000 -> 0000000000000000 for 2bytes. the most
  significant byte was the first 8 bits (left side) which are all 1's. Shifting to the left by a byte discards the first 8 bits, and shifting
  back to the right by a byte replaces the left byte by zeros whilst keeping the values on the right byte (second byte in 16bits) the same
  value.
  The value stored at the CFG address should be within the second byte, therefore this makes sure this is true before setting the currentDetSett
  variable to this value.*/
  // TRY LOOP HERE INSTEAD?

  uint16_t newRegData = (delayRegData << 12) + (enabled << 8) + currentDetSett;
  /*Define the new register data as the sum of a shifted delay data, shifted enable bool var (1 = true, 0=false. these are then shifted)
  and currentdetsett. More likely on datasheet as this configuration is arbitrary and defines the delay time and whether to set the globalchop
  as enabled or not*/

  return writeReg(CFG, newRegData);// returns true is successfull in writing the data to the CFG register.
}
//-----------------------------------------------------------------------------------------------------------------------------------------------
// bool ADS131M06::writeReg(uint8_t reg, uint16_t data, uint8_t number = 0x00) {// XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
//   /* Writes the content of data to the register reg (contains memory location)
//      Returns true if successful

//      number: the number of extra consecutive registers to be read, after the register location specified in reg
//      Cannot write to RESERVED address as this requires a 7 bit address. command can only handle addresses which require up to 6 bits.
//   */
//   //command format: 011a aaaa annn nnnn
//   uint8_t commandPref = 0x06;//0000 0110
//   if (number > 127){
//     /*check the number of bits required for specifying the number of consecutive registers to read is less than 7.
//       Set to maximum if exceeded.
//     */
//     number = 0x7E; //set to max value available
//     RicCoreLogging::log<RicCoreLoggingConfig::LOGGERS::SYS>("ERROR: number of consecutive registers specified exceeds the limit of 7 bits. Number set to maximum of 127");
//   }
//   // Make command word using syntax found in data sheet
//   uint16_t commandWord = (commandPref<<12) + (reg<<7) + (number);

//   digitalWrite(csPin, LOW);
//   spi.beginTransaction(SPISettings(SCLK_SPD, MSBFIRST, SPI_MODE1));
//   /*this actually starts a trasmission process and configures the clock speed, MSB or LSB
//    config for
//    transfer of bits out of the respective registers, and the SPI mode which configures the CPOL
//    and CPHA and whether the falling edge and data capture are on falling or rising edges of
//    the clk signal
//   */
  
//   // sends command word and data to be written in the register location along the SPI lines. return data is discarded (not assigned to anything)
//   spiTransferWord(commandWord);
//   /*transfer word sends 24 bits of data along the spi line, 1 byte at a time (technically 2bytes, check the function). the input data is 16bits
//   ,however the most siginificant byte of this is sent first, then the next byte after. Finally the last byte is due to sending 0x00 along spi.
//   The results are collected in a 32 bit result, which returned, with the first byte as zeros, second byte as the result of the most significant
//   byte of the input data and so forth, with the last byte being the result of 0x00. Therefore the result if in order of Most significant bit first.*/
//   spiTransferWord(data);

//   // Send 6 empty words
//   for (uint8_t i=0; i<6; i++) {
//     spiTransferWord(); //sends 2bytes of zeros by default
//   }

//   spi.endTransaction();
//   digitalWrite(csPin, HIGH); //active low due to CPHA as mentioned before, therefore deactivate here.

//   // Get response in order to determine whether the writing was successfull:
//   uint32_t responseArr[8];
//   spiCommFrame(&responseArr[0]); // Copy SPI data from all channels into the responseArr[]

//   // Determining whether the register was successfully written to or not:
//   if ( ( (0x04<<12) + (reg<<7) ) == responseArr[0]) {
//     return true;
//   } else {
//     return false;
//   }
//   /*If the first element of the responseArr has a value equal to 4 (100 in binary) shifted 12 bits to the left making it 16384 plus
//   the register address value shifted 7 bits left (x128), then the */
// }

bool ADS131M06::writeReg(uint8_t reg, uint16_t data) {// XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
  /* Writes the content of data to the register reg (contains memory location)
     Returns true if successful

     number: the number of extra consecutive registers to be read, after the register location specified in reg
     Cannot write to RESERVED address as this requires a 7 bit address. command can only handle addresses which require up to 6 bits.
  */
  //command format: 011a aaaa annn nnnn
  uint8_t commandPref = 0x06;//0000 0110
  // Make command word using syntax found in data sheet
  uint16_t commandWord = (commandPref<<12) + (reg<<7);

  digitalWrite(csPin, LOW);
  spi.beginTransaction(_spisettings);
  
  // sends command word and data to be written in the register location along the SPI lines. return data is discarded (not assigned to anything)
  spiTransferWord(commandWord);
  /*Use transferWord() function instead of spiCommFrame() since the data needs to be sent directly
  after the command, instead of the 6 empty words being sent after the command in spiCommFrame()*/
  spiTransferWord(data);

  // Send 6 empty words
  for (uint8_t i=0; i<6; i++) {
    spiTransferWord(); //sends 2bytes of zeros by default. completes the 8 word frame
  }

  spi.endTransaction();
  digitalWrite(csPin, HIGH); //active low due to CPHA as mentioned before, therefore deactivate here.

  // Get response in order to determine whether the writing was successfull:
  spiCommFrame(responseVect); // Copy SPI data from all channels into the responseArr[]

  // Determining whether the register was successfully written to or not:
  //response format: 010a aaaa ammm mmmm
  if ( ( (0x04<<12) + (reg<<7) ) == (responseVect[0] >> 16)) { //check against the 16 bit response
    return true;
  } else {
    return false;
  }
}

//----------------------------------------------------------------------------------------------------------------------------------------------
bool ADS131M06::setGain(uint8_t log2Gain0, uint8_t log2gainCommand, uint8_t log2Gain2, uint8_t log2Gain3) {//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
  /* Function to set the gain of the four channels of the ADC
     
     Inputs are the log base 2 of the desired gain to be applied to each
     channel.

     Returns true if gain was succesfully set.

     Function written by Iris Clercq-Roques
  */

 //defining the configuration of the gain command. More information of the configuration on the data sheet:
  uint16_t gainCommand=log2Gain3<<4;
  /*shift the channel 3 gain by 4 bits to the left and assign to gain command*/
  gainCommand+=log2Gain2;
  gainCommand<<=8;
  gainCommand+=(log2gainCommand<<4);
  gainCommand+=log2Gain0;
  return writeReg(GAIN1, gainCommand); // GAIN1 is the register containing the command value to specify the gain. write new command here.
}
//-----------------------------------------------------------------------------------------------------------------------------------------------
// uint8_t ADS131M06::readReg(uint8_t reg, uint8_t number = 0x00) {//xxxxxxxxxxxxx-RESERVED address and the output type?
//   /* Reads the content of single register found at address reg
//      Returns register value
//      Note: formatting only allows for 6 bits of actual address data, therefore 63 total addresses. RESERVED address cannot be used as a result.
//   */
//   if (number > 127){
//     /*check the number of bits required for spe+cifying the number of consecutive registers to read is less than 7.
//       Set to maximum if exceeded.
//     */
//     number = 0x7E; //set to max value available
//     RicCoreLogging::log<RicCoreLoggingConfig::LOGGERS::SYS>("ERROR: number of consecutive registers specified exceeds the limit of 7 bits. Number set to maximum of 127");
//   }

//   //reg: aaaa aaaa 8 bits, however, no register address requires more than 6 bits other than the RESERVED address (requires 7 bits)
//   uint8_t commandPref = 0x0A;//10 = 0000 1010
//   //1010 0000 0000 0000
//   // Make command word using syntax found in data sheet: 101a aaaa annn nnnn
//   uint16_t commandWord = (commandPref << 12) + (reg << 7) + number;

//   //DYNAMIC MEMORY:XXXXXXXXXXXXXXXXXXXXXXXXXX-check how to compile with a specified length? does it need to?
//   // Define size depending on whether a single or multiple registers are being read
//   bool multiple = false;
//   if (number == 0x00){
//     uint32_t responseArr[8];
//   }else{
//     std::unique_ptr<uint32_t> responseArr(new uint32_t); //length = number + 2 -> 2 for the ack word at the start and the CRC at the end
//     multiple = true;
//   }
//   uint16_t AckCheck = (0x0D << 12) + (reg << 7) + number;

//   // Use first frame to send command
//   spiCommFrame(&responseArr[0], commandWord);

//   // Read response: response occurs on the output frame following the command, rather than at the same time the command is sent.
//   spiCommFrame(&responseArr[0]);
  
//   if((multiple == true)&&((responseArr[0]>>16) != AckCheck)){
//     RicCoreLogging::log<RicCoreLoggingConfig::LOGGERS::SYS>("ERROR: False Acknowledgement");
//     return 0x00; //false
//   }else {
//     return &responseArr[0];
//   }
//   /*Outputs the memory location of the first element of the array instead. This is 8 bits and the length of the array is the number of
//   consecutive registers to be read after the first specified, which is specified in the command at the start of the function.*/
//   //return responseArr[0] >> 16;
// }

uint16_t ADS131M06::readReg(uint8_t reg){
  //reg: aaaa aaaa 8 bits, however, no register address requires more than 6 bits other than the RESERVED address (requires 7 bits)
  uint8_t commandPref = 0x0A;//10 = 0000 1010
  //1010 0000 0000 0000
  // Make command word using syntax found in data sheet: 101a aaaa annn nnnn
  uint16_t commandWord = (commandPref << 12) + (reg << 7);

  //Define response array:

  // Use first frame to send command
  spiCommFrame(responseVect, commandWord);

  // Read response: response occurs on the output frame following the command, rather than at the same time the command is sent.
  spiCommFrame(responseVect);//xxxx test if this actuall gets the register data (might just output
  //the channel data, which is not what we want here)
  return (responseVect[0] >> 16); //return the 2bytes of actual data in the first index of the array.

}
//---------------------------------------------------------------------------------------------------------------------------------------------
bool ADS131M06::clkConfig(){//xxxxxxxxxxxxxxxxxxxx
  /*Writes to the clock register and sets crystal oscillator to disable and enable */

  uint16_t data = 0b0011111111001110;
  return(writeReg(CLOCK, data));
}
//-----------------------------------------------------------------------------------------------
bool ADS131M06::reset(){//xxxxxxxxxxxxxxx- deos the command latch then require a second frame to get the acknowledgment?
  /*command to reset the ADC device
  outputs true if successfull
  */
  uint16_t commandWord = 0x11;

  uint16_t commandResponse = (0xFF <<8) + 0x22; //expected response to be checked against

  spiCommFrame(responseVect, commandWord);
  spiCommFrame(responseVect);

  // check if command was acknowledged:
  if((responseVect[0] >> 16) == commandResponse){ //reponseArr is 32bit MSB aligned, therefore shift 16 and get rid of LSB zeroes.
    return true;
  }else{
    return false;
  }
}
bool ADS131M06::standby(void){
  /*Enter standby mode for the ADC
    output true if successfull
  */
  uint16_t commandWord = 0x20;

  spiCommFrame(responseVect, commandWord);
  spiCommFrame(responseVect);

  // check if command was acknowledged:
  if((responseVect[0] >> 16) == commandWord){//response word is the same as the commandword
    return true;
  }else{
    return false;
  }
}
bool ADS131M06::wakeup(void){
  /*Wakeup from the standby mode
    Output true if successfull*/
  uint16_t commandWord = 0x33; // 0000 0000 0011 0011


  spiCommFrame(responseVect, commandWord);
  spiCommFrame(responseVect);

  // check if command was acknowledged:
  if((responseVect[0] >> 16) == commandWord){//response word is the same as the commandword
    return true;
  }else{
    return false;
  }
}
bool ADS131M06::lock(void){
  /**/
  uint16_t commandWord = (0x05 << 8) + (0x05 << 4) + 0x05; // 0000 0101 0101 0101

  spiCommFrame(responseVect, commandWord);
  spiCommFrame(responseVect);

  // check if command was acknowledged:
  if((responseVect[0] >> 16) == commandWord){//response word is the same as the commandword
    return true;
  }else{
    return false;
  }
}
bool ADS131M06::unlock(void){
  /**/
  uint16_t commandWord = (0x06 << 8) + (0x05 << 4) + 0x05; // 0000 0110 0101 0101

  spiCommFrame(responseVect, commandWord);
  spiCommFrame(responseVect);

  // check if command was acknowledged:
  if((responseVect[0] >> 16) == commandWord){//response word is the same as the commandword
    return true;
  }else{
    return false;
  }
}
//---------------------------------------------------------------------------------------------------------------------------------
uint32_t ADS131M06::spiTransferWord(uint16_t inputData) {//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX-check if 8 bits at a time is fine
  /* Transfer a 24 bit word (sends the two bytes of input data and 1 byte of zeros)
     Data returned is MSB aligned (MSB is first bit)
     Sends the first byte (most significant byte) of input data first, and then the next byte after. Lastly 0x00 is sent. All results from SPI
     are concatinated into a 32 bit return result
     Input data is 16 bits
  */ 
  
  uint32_t data = spi.transfer(inputData >> 8);
  /*Input data (16bits command, see spi arduino) is shifted to the left by a byte, getting rid of the least significant byte and shifting 
  the most significant byte
  to its place, effectively making it a 16bit number with 8 bits of data in LSB, with MSB zeroed. This sent along
  the spi line and the response is set to a 32bit var called data. The response is also 2 bytes, however with the most significant byte all zeros
  as we passed in all zeros for the most significant byte.
  Effectively send ONLY the most significant byte of InputData along the spi line first, as a 16 bit number*/
  data <<= 8; 
  /*data returned is shifted left, effectively deleting the most significant byte and moving the second most significant byte in
  its place. All other bytes are shifted up also, effectively leaving the new least significant byte to be zeroed.
  Effectively this makes space for a new byte as the least significant byte to replace the zeros*/
  data |= spi.transfer((inputData<<8) >> 8);
  /*Input data is shifted to zero the most significant byte of data, effectively sending only the least significant byte this time, but as a 
  16 bit number. Each bit of the return signal from the spi
  line is compared with each bit from data. If either a bit from data or the result from the spi line is 1, then 1 is assigned as data for
  that bit, otherwise it is set to zero. this comparison is repeated for each bit
  This means the result of the spitransfer of the least significant byte of input data takes the place of the least significant byte of data
  (as we made space for it previously with the byte shift up)*/ 
  data <<= 8; // shift data by a byte to the right to make room for one more spi result (remember, as the first byte of the 2byte spi input signal
  //is always zero, the output of spi is also zero in the most significant byte, therefore shifting 1 byte to make room is enough)
  data |= spi.transfer(0x00);
  /*Finally send 8 bits of zeros (8 bits likely as type int is this and default is 8 bits if 16 bit datatype was not defined). The return value is
  set to the new */
  return data << 8;
  /*final datat is shifted again. Therefore the order is: MSByteResponse LSbyteResponse ZeroResponse zeros*/
}
//----------------------------------------------------------------------------------------------------------------------------------------------
void ADS131M06::spiCommFrame(std::vector<uint32_t>& outputVect, uint16_t command) {
  // Saves all the data of a communication frame to an array with pointer outPtr.

  digitalWrite(csPin, LOW);/*set the chip select pin to low mode: communicates with the
  controller (other cspins for other spi devices should be set to high)
  Active low since the settining is SPI_MODE1 which sets the CPOL = 0 (check wiki for more)*/

  spi.beginTransaction(_spisettings);
  /*starts an spi transaction. Each time this is called, the settings for the transaction must
  be specified, including the clock speed, whether the MSB or the LSB is moved first and the
  CPHA and CPOL modes via SPI_MODE1*/

  // Send the command in the first word
  outputVect[0] = spiTransferWord(command); //save the 32 bit result from SPI in the first location of outPtr (array)
  /*If no command word specified, the for loop ahead starts with skipping the first location of the array, so this would be empty.
  Each spiTransferWord sends only 24 bits of data*/

  // For the next 6 words, just read the data
  for (uint8_t i=1; i < 7; i++) {//6 times for all ADC channels

    outputVect[i] = spiTransferWord() >> 8;
    /*collects the 32 bit data for each channel. Each time spiTransferWord() is called with no input data, the next channel's data is called?
    The cycling through different channels may be due to spi.beginTransaction() being called beforehand. 
    Each 32 bit result is shifted to the right by a byte, deleting the last byte result from spiTransferWord() which is the result of 0x00.
    This means each word is now 24 bits, with the MSByteResponse, LSByteResponse ZeroResponse and thats it.*/
  }

  // Save CRC bits
  /*CRC: Cyclic Redundancy check. If spiTransferWord() is called for more times than the number of channels, then the CRC value 
  is transmitted (as these are the last bytes of the transaction code). An automatic CRC is done by calculating an 8 bit CRC on the entire
  32 bits of each command, then compares this with the tranmitted CRC. An error occurs if they do not match.*/
  
  outputVect[8] = spiTransferWord();

  /*Note: as the first index of the array is for the optional command, the next 4 indexes are for all the channel data and the last array index
  is for the CRC bits. Therefore the minimum length of the array must be 1 + 4 + 1 = 6.*/
  spi.endTransaction();

  //set the csPin back to HIGH as active low config.
  digitalWrite(csPin, HIGH);
}

bool ADS131M06::setOSR(OSROPT OSR){
    CLOCKREG &= OSRMASK;
    CLOCKREG |= static_cast<uint8_t>(OSR);
    return writeReg(CLOCK,CLOCKREG);
}
//-----------------------------------------------------------------------------------------------------------------------------------------------
int32_t ADS131M06::twoCompDeco(uint32_t data) {//XXXXXXXXXXXXXXXXXXXXXXXXXXXXX
  // Take the two's complement of the data

  data <<= 8;/*shifts input data by 8 bits to the right, effectively multiplying the value by
  2^8 = pow(2,8)
  gets rid of most significant byte of data with the multiplication.*/
  int32_t dataInt = (int)data;/*This casts data as a signed int32 bits*/

  return dataInt/pow(2,8);/*return the signed data, which is divided by the factor we previously
  multiplied it by, effectively giveing back the original value, but casted as signed instead.*/
}