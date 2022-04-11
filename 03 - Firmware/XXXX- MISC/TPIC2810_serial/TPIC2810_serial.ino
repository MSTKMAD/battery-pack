/*
 * This example test the TPIC2810
 * The program expects a numeric character (+enter to send) through the serial link
 * This number corresponds to the (0 to 7) transistor of the TPIC2810 to activate.
 * If other than a numeric character is sent, then it is initiated a consecutive 
 * sending of values (activation of TPIC outputs 0 to 7). The values are shown through the serial link.
 * 
 * 
 */

//-------------INCLUDE-------------------
//#include <Ports.h>  // Implementa el wrap MilliTimer 
#include <Wire.h>    // Required for I2C communication


//-------------TYPES---------------------
// typedef struct
// {
//    byte Kindofdata;   //
//    byte Data;         //
// }typePayload;

// ------- DEFINEs & CONSTANTS -------
// const byte  = ;      //
// const byte  = ;      //

// // ------- FUNCTIONS -------
void Write_TPIC2810();
// // ------- VARIABLES --------

int  value;
int    tpicVal;


// // ------- PIN NAMES -------
// const int = ;    // 
// const int = ;    // 
//--------------------------



void setup()
{
  // ------ DEVICE CONFIGURATION ------
  Wire.begin();
  Serial.begin(9600);
  Serial.println("VERSION v1");
  Serial.println("Initilialization Done");
  
  // ------ VAR INITIALIZATION ------
  
}
 


void loop()
{
  if ( Serial.available()) // Check to see if at least one character is available
  {
    char ch = Serial.read();
    if( isDigit(ch) ) // is this an ascii digit between 0 and 9?
    {
       value = (ch - '0');      // ASCII value converted to numeric value
       Serial.print("value: ");
       Serial.println(value);
     
       
       tpicVal  = 1 << value;
       
       Serial.print("tpicVal: ");
       Serial.println(tpicVal, BIN);      
       
       
       Write_TPIC2810(0, tpicVal); 
    }
    else
    {
      // Secuencia automatica
      for(int i= 1; i<256; i=i<<1)
      {
         Write_TPIC2810(0, i);
         Serial.println(i);
         delay(4000);
      }
    }
  }
  
  /*
  

*/
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
