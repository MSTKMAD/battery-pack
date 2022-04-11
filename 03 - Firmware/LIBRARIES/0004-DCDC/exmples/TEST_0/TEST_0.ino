/**
 * @file TEST_0.ino
 * @author Javi (Javier@musotoku.com)
 * @brief 
 * @version 2
 * @date 2020-12-15
 * 
 * @copyright Copyright (c) 2020
 * 
 */
#include <DCDC.h>

// Pines de sensado del Makinator.
const int16_t C_PIN_I_IN = 15;
const int16_t C_PIN_V_OUT = 16;
const uint16_t C_PIN_EN_DCDC = 2;

dcdc_controler DCDC(C_PIN_EN_DCDC);

int16_t v_sense = 0;
int16_t i_sense = 0;
float v_diff = 0;

void setup()
{
    Wire.begin();
    Serial.begin(9600);

    String toString;
    Serial.println("Theory Volt \t Real Volt \t CURRENT \t Increment");

    for (int i = 40; i <= 160; i++)
    {
        DCDC.SetVoltage(i, C_NON_BOOST_MODE);
        v_sense = 0;
        i_sense = 0;
        delay(1000);
        for (int j = 0; j < 8; j++)
        {
            v_sense += analogRead(C_PIN_V_OUT);
            i_sense += analogRead(C_PIN_I_IN);
            delay(100);
        }
        v_sense = (int)((v_sense / 8) * 3300 * 4.84 / 4096);
        i_sense = (int)((i_sense / 8) * 3300 / 4096);
        Serial.print((float)(i) / 10);
        Serial.print("\t\t");
        Serial.print((float)(v_sense) / 1000);
        Serial.print("\t\t");
        Serial.print((float)(i_sense) / 1000);
        v_diff = ((float)(i) / 10) - ((float)(v_sense) / 1000);
        Serial.print("\t\t");
        Serial.println(v_diff);
    }

    delay(100);
}

void loop()
{
}