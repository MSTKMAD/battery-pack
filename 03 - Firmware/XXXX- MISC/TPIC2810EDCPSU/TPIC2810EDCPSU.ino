/* Exercises the TPIC2810 in the EDCPSU v1 HW. No display, no rotary encoder, neither switches
 * It uses a long array in program memory (PROGMEM) to store the array of values
 * that drives the LDO through the 3 to 10 V span.
 * There is a continuous ramping of values from 3 to 10V that takes about 1s to complete.
 * Current consumption including Arduino board is from 100 to 115mA.
 * 
 */

//-------------INCLUDE-------------------
//#include <Ports.h>  // Implementa el wrap MilliTimer 
#include <Wire.h>    // Required for I2C communication
#include "PCF8575.h" // Required for PCF8575
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
const uint16_t   LenLDOvalues = 172;
const byte       ADDR_I2C_LDO = 0;
// // ------- FUNCTIONS -------
void Write_TPIC2810();
// // ------- VARIABLES --------
//byte     LDOvalues[LenLDOvalues]= {171,17,114,2,236,12,84};

PROGMEM  byte     LDOvalues[LenLDOvalues]= {171, 43, 203, 75, 139, 11, 243, 115, 179, 51, 211, 83, 147, 19, 227, 99, 163, 35, 195, 67, 131, 3, 253, 125, 189, 61, 221, 93, 157, 29, 237, 109, 173, 45, 205, 77, 141, 13, 245, 117, 181, 53, 213, 85, 149, 21, 229, 101, 165, 37, 197, 69, 133, 5, 249, 121, 185, 57, 217, 89, 153, 25, 233, 105, 169, 41, 201, 73, 137, 9, 241, 113, 177, 49, 209, 81, 145, 17, 225, 97, 161, 33, 193, 65, 129, 1, 254, 126, 190, 62, 222, 94, 158, 30, 238, 110, 174, 46, 206, 78, 142, 14, 246, 118, 182, 54, 214, 86, 150, 22, 230, 102, 166, 38, 198, 70, 134, 6, 250, 122, 186, 58, 218, 90, 154, 26, 234, 106, 170, 42, 202, 74, 138, 10, 242, 114, 178, 50, 210, 82, 146, 18, 226, 98, 162, 34, 194, 66, 130, 2, 252, 124, 188, 60, 220, 92, 156, 28, 236, 108, 172, 44, 204, 76, 140, 12, 244, 116, 180, 52, 212, 84};
byte  TPICvalue;


int encoderPos = 0;
/** PCF8575 instance */
PCF8575 I2Cexpander; 

// // ------- PIN NAMES -------
// ------- PIN NAMES -------
const int BUZZ    = 0;    // 
const int TX      = 1;    // 
const int PUSH    = 2;    // 
const int ROTPUSH = 3;    // 
const int ROTA    = 4;    // 
const int ROTB    = 5;    // 
const int LDO_YES = 6;    // 
const int LDO_NO  = 7;    // 
const int DSP_RST = 8;    // 
const int ONOFF   = 9;    //
const int ISEN    = A0;   //
const int VOSEN   = A1;   //
const int VDCDCSEN= A2;   //
//--------------------------



void setup()
{
  // ------ DEVICE CONFIGURATION ------
  Wire.begin();
  Serial.begin(9600);
  Serial.println("VERSION v1");
  Serial.println("Initilialization Done");
  
  // ------ VAR INITIALIZATION ------

  pinMode(BUZZ,    OUTPUT);
  pinMode(PUSH,    INPUT);
  pinMode(ROTPUSH, INPUT);
  pinMode(ROTA,    INPUT);
  pinMode(ROTB,    INPUT);
  pinMode(LDO_YES, OUTPUT);
  pinMode(LDO_NO,  OUTPUT);
  pinMode(ONOFF,   OUTPUT);
  pinMode(ISEN,    INPUT);
  pinMode(VOSEN,   INPUT);
  pinMode(VDCDCSEN,INPUT);

  digitalWrite(LDO_YES, LOW);
  digitalWrite(ONOFF,   LOW);
  digitalWrite(LDO_NO,  LOW);

  // /* Start I2C bus and PCF8575 instance */
  I2Cexpander.begin(0x20);
  
  /* Setup some PCF8575 pins */
  I2Cexpander.pinMode(0, OUTPUT);
  I2Cexpander.pinMode(1, OUTPUT);
  I2Cexpander.pinMode(2, OUTPUT);
  I2Cexpander.pinMode(3, OUTPUT);
  I2Cexpander.pinMode(4, OUTPUT);
  I2Cexpander.pinMode(5, OUTPUT);
  I2Cexpander.pinMode(6, OUTPUT);
  I2Cexpander.pinMode(7, OUTPUT);
  I2Cexpander.pinMode(8, OUTPUT);
  I2Cexpander.pinMode(9, OUTPUT);
  I2Cexpander.pinMode(10, OUTPUT);
  I2Cexpander.pinMode(11, OUTPUT);
  I2Cexpander.pinMode(12, OUTPUT);
  I2Cexpander.pinMode(13, OUTPUT);
  I2Cexpander.pinMode(14, OUTPUT);
  I2Cexpander.pinMode(15, OUTPUT);
  I2Cexpander.pinMode(16, OUTPUT);
  I2Cexpander.write(0x800);     // VDCDC=12V

  encoderPos = 0;
}



void loop()
{
  TPICvalue = pgm_read_byte_near(LDOvalues + encoderPos);
  Write_TPIC2810(ADDR_I2C_LDO, TPICvalue);
  encoderPos++;

  if(encoderPos>LenLDOvalues)
      {
       encoderPos = 0; 
      }
  Serial.println(encoderPos);

  delay(10);

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