/**
 * @file DCDC.h
 * @author Javi (Javier@musotoku.com)
 * @brief Libreria que describe la clase del DCDC.
 * @version 2
 * @date 2020-12-18
 * 
 * @copyright Copyright (c) 2020
 * 
 */
#include <Wire.h>

const uint16_t LenDCDCvalues = 121;
const bool C_BOOST_MODE = true;
const bool C_NON_BOOST_MODE = false;
PROGMEM const byte BOOST_ARRAY[LenDCDCvalues] = {36, 164, 100, 228, 20, 148, 84, 52, 180, 116, 244, 12, 140, 76, 204, 44, 172, 108, 236, 28, 92, 220, 60, 188, 124, 252, 2, 130, 194, 34, 162, 98, 226, 18, 146, 82, 50, 178, 114, 242, 10, 138, 74, 202, 42, 170, 106, 234, 26, 90, 218, 58, 186, 122, 250, 6, 70, 198, 38, 166, 102, 230, 22, 150, 214, 54, 182, 118, 246, 14, 142, 78, 206, 46, 174, 110, 238, 30, 94, 222, 62, 190, 126, 254, 193, 33, 161, 97, 17, 145, 81, 209, 49, 177, 113, 241, 9, 137, 73, 201, 41, 105, 233, 25, 153, 89, 217, 57, 121, 249, 5, 133, 69, 197, 37, 101, 229, 21, 149, 85, 213};

PROGMEM const byte NON_BOOST_ARRAY[LenDCDCvalues] = {248, 4, 132, 68, 36, 164, 100, 228, 20, 148, 84, 52, 180, 116, 244, 12, 140, 76, 204, 44, 172, 108, 236, 28, 92, 220, 60, 188, 124, 252, 2, 130, 194, 34, 162, 98, 226, 18, 146, 82, 50, 178, 114, 242, 10, 138, 74, 202, 42, 170, 106, 234, 26, 90, 218, 58, 186, 122, 250, 6, 70, 198, 38, 166, 102, 230, 22, 150, 214, 54, 182, 118, 246, 14, 142, 78, 206, 46, 174, 110, 238, 30, 94, 222, 62, 190, 126, 254, 193, 33, 161, 97, 17, 145, 81, 209, 49, 177, 113, 241, 9, 137, 73, 201, 41, 105, 233, 25, 153, 89, 217, 57, 121, 249, 5, 133, 69, 197, 37, 101, 229};

class dcdc_controler
{
private:
    /**
     * @brief Tbhis function send  a I2C commad to TPIC controler.
     * 
     * @param address 
     * @param data 
     */
    void Write_TPIC2810(byte data)
    {
        Wire.beginTransmission(byte(96)); // transmit command to device TPIC2810
        Wire.write(byte(68));             // Command to transfer next value to output register
        Wire.write(byte(data));
        Wire.endTransmission(); // stop transmitting
    }

public:
    uint16_t encoderPos;
    byte TPICvalue;
    int16_t pin_enable;

    /**
     * @brief Construct a new dcdc controler object
     * 
     * @param pin Pin conectado al enable del DCDC.
     */
    dcdc_controler(int16_t pin)
    {

        pin_enable = pin;
        pinMode(pin_enable, OUTPUT);
        digitalWrite(pin_enable, LOW);
    }

    /**
     * @brief Set the Voltage object
     * 
     * @param volt 
     * @param mode 
     */
    void SetVoltage(int volt, bool mode)
    {
        volt = constrain(volt, 40, 160);
        encoderPos = volt - 40;
        if (mode == C_BOOST_MODE)
        {
            TPICvalue = pgm_read_byte_near(BOOST_ARRAY + encoderPos);
        }
        else if (mode == C_NON_BOOST_MODE)
        {
            TPICvalue = pgm_read_byte_near(NON_BOOST_ARRAY + encoderPos);
        }

        Write_TPIC2810(TPICvalue);
    }
    /**
     * @brief Habilita la activacion del DCDC.
     * 
     * @param enable 
     */
    void EnableDCDC(bool enable = LOW)
    {
        digitalWrite(pin_enable, enable);
    }
};