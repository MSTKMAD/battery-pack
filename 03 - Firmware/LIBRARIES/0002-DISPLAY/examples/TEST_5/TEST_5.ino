/**
 * @file TEST_5.ino
 * @author Javi (Javier@musotoku.com)
 * @brief Test de la funcion DisplayBattCharging. Este test muestra la evolucion de la carga de la bateria, desde el 0% al 100%.
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
    int32_t t0 = 0;
    int32_t t1 = 0;
    ledmatrix.begin();
    Serial.println("IS31 Found!");

    for (int16_t i = 0; i <= 100; i++)
    {
        t0 = micros();
        DisplayBattCharging(i, ledmatrix);
        t1 = micros();
        Serial.print("Tiempo de ejecucion DisplayBattCharging:");
        Serial.println(t1 - t0);
        delay(100);
    }
}

void loop()
{
}