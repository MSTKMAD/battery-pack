/**
 * @file TEST_2.ino
 * @author Javi (Javier@musotoku.com)
 * @brief Este test simula el funcionamiento de la barra de potencia creando un movimeinto senosoidal de manera ciclica.
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

    int x;
    int32_t t0 = 0;
    int32_t t1 = 0;
    while (1)
    {
        for (int16_t i = 0; i <= 360; i++)
        {
            x = abs(sin(radians(i))) * 14;
            t0 = micros();
            PowerBar(x, ledmatrix);
            t1 = micros();
            Serial.print("Tiempo de ejecucion PowerBar:");
            Serial.println(t1 - t0);
            delay(2);
        }
    }
}

void loop()
{
}