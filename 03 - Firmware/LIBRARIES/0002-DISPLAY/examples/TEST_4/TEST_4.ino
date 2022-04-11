/**
 * @file TEST_4.ino
 * @author Javi (Javier@musotoku.com)
 * @brief Este test imprime toda la capacidad posible de la bateria ( 0-100 ).
Ascendente y descendente, de manera ciclica.
 * @version 2
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

    int x, j = 0;
    int32_t t0 = 0;
    int32_t t1 = 0;
    MilliTimer timer;
    while (1)
    {
        for (int16_t i = 0; i <= 100; i++)
        {
            t0 = micros();
            DisplayCap(i, ledmatrix);
            t1 = micros();
            Serial.print("Tiempo de ejecucion PowerBar:");
            Serial.println(t1 - t0);
            delay(100);
        }
        for (int16_t i = 100; i >= 0; i--)
        {
            t0 = micros();
            DisplayCap(i, ledmatrix);
            t1 = micros();
            Serial.print("Tiempo de ejecucion PowerBar:");
            Serial.println(t1 - t0);
            delay(100);
        }
    }
}

void loop()
{
}