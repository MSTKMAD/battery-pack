/**
 * @file TEST_1.ino
 * @author Javi (Javier@musotoku.com)
 * @brief Test de la funcion DisplayVolts.
 * @version 3
 * @date 2021-03-16
 * 
 * @copyright Copyright (c) 2020
 * 
 */

#include <display.h>
Adafruit_IS31FL3731_Wing ledmatrix = Adafruit_IS31FL3731_Wing();

void setup()
{
    Serial.begin(9600);
    while (!Serial)
    {
        ;
    }
    ledmatrix.begin();

    Serial.println("IS31 Found!");
    int32_t t0 = 0;
    int32_t t1 = 0;
    for (int16_t i = 40; i <= 160; i++)
    {
        t0 = micros();
        DisplayVolt(i, ledmatrix);
        t1 = micros();
        Serial.print("Tiempo de ejecucion DisplayVolt:");
        Serial.println(t1 - t0);
        delay(100);
    }
    for (int16_t i = 160; i >= 40; i--)
    {
        t0 = micros();
        DisplayVolt(i, ledmatrix);
        t1 = micros();
        Serial.print("Tiempo de ejecucion DisplayVolt:");
        Serial.println(t1 - t0);
        delay(100);
    }
}

void loop()
{
}
