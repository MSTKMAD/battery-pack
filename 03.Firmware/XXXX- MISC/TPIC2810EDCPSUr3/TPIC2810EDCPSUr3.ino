/* Exercises the TPIC2810 in the EDCPSU release 3 HW (r3 for tattoo!!). No display, no rotary encoder, neither switches
 * It uses a long array in program memory (PROGMEM) to store the array of values
 * that drives the DCDC through the 4,5 to 14 V span (0.2 volt stride).
 * 
 */

//-------------INCLUDE-------------------
//#include <Ports.h>  // Implementa el wrap MilliTimer 
#include <Wire.h>    // Required for I2C communication
#include <avr/pgmspace.h>


//-------------TYPES---------------------
// typedef struct
// {
//    byte Kindofdata;   //
//    byte Data;         //
// }typePayload;

// ------- DEFINEs & CONSTANTS -------
// const byte  = ;      //
// const byte  = ;      //
const uint16_t   LenDCDCvalues = 49;
const byte       ADDR_I2C_DCDC = 0;

// ------- FUNCTIONS -------
void Write_TPIC2810();

// ------- VARIABLES --------

PROGMEM  const byte     DCDCvalues[LenDCDCvalues]= {127,95,111,79,119,87,103,71,251,
59,27,43,11,51,19,35,3,189,157,173,141,181,149,165,133,249,217,233,201,241,209,225,
193,254,222,238,206,246,214,230,198,6,186,154,170,74,178,146,226}; 


byte  TPICvalue;


int encoderPos = 0;


// // ------- PIN NAMES -------
// ------- PIN NAMES -------

const int DCDC_EN = A1;   //
const int OP_EN1   = 7;    // Arduino 7 (pin 11 in ATMEGA368)
const int OP_EN2   = 9;    // Arduino 9 (pin 13 in ATMEGA368)
//--------------------------



void setup()
{
  // ------ DEVICE CONFIGURATION ------
  Wire.begin();
  Serial.begin(9600);
  Serial.println("VERSION v1");
  Serial.println("Initilialization Done");
  
  // ------ VAR INITIALIZATION ------
/*

  pinMode(PUSH,    INPUT);
  pinMode(ROTPUSH, INPUT);
  pinMode(ROTA,    INPUT);
  pinMode(ROTB,    INPUT);
  pinMode(LDO_YES, OUTPUT);
  pinMode(LDO_NO,  OUTPUT);
  pinMode(ONOFF,   OUTPUT);
  pinMode(ISEN,    INPUT);
  pinMode(VOSEN,   INPUT);
  */
  pinMode(DCDC_EN, OUTPUT);
  pinMode(OP_EN1,    OUTPUT);
  pinMode(OP_EN2,    OUTPUT);
  
  digitalWrite(DCDC_EN, HIGH);
  analogWrite(OP_EN2, 255);

  encoderPos = 0;
}



void loop()
{
  TPICvalue = pgm_read_byte_near(DCDCvalues + encoderPos);
  Write_TPIC2810(ADDR_I2C_DCDC, TPICvalue);
  encoderPos++;
  


int ind = 0;
while(1)
{
  TPICvalue = pgm_read_byte_near(DCDCvalues + ind);
  Write_TPIC2810(ADDR_I2C_DCDC, TPICvalue);
  delay(111);
  ind = (ind+1)%(LenDCDCvalues);
  Serial.print(ind);
  Serial.print('\t');
  Serial.println(TPICvalue);
}


}





//***************************************************
// ================  FUNCTIONS =====================*
//***************************************************



// ================  Write_TPIC2810 =====================*

void Write_TPIC2810(byte address, byte data)
{
	Wire.beginTransmission(byte(96)); // transmit command to device TPIC2810
  Wire.write(byte(68));             // Command to transfer next value to output register
  Wire.write(byte(data));
  Wire.endTransmission();     // stop transmitting
}
