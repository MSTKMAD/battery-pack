/**
 * @file TEST_0.ino
 * @author Javi (Javier@musotoku.com)
 * @brief  Testeo de la funcion DisplayTest
 * @version 2
 * @date 2020-12-03
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
    int32_t t0 = 0;
    int32_t t1 = 0;
    bool ok = false;

    t0 = micros();
    bool reset = true;
    while (DisplayText("Hola, que tal?", ledmatrix, reset, C_MODE_HIGH_BRIGHT) == C_DISPLAY_ST_BUSSY)
    {
        reset = false;
        delay(10);
    }
    t1 = micros();
    Serial.print("Tiempo de ejecucion DisplayText:");
    Serial.println(t1 - t0);
    delay(500);
    while (!ok)
    {
        t0 = micros();
        if (DisplayText("Bien y tu?", ledmatrix) == false)
        {
            ok = false;
            t1 = micros();
            Serial.print("Tiempo de ejecucion Bussy DisplayText:");
            Serial.println(t1 - t0);
        }
        else
        {
            ok = true;
            t1 = micros();
            Serial.print("Tiempo de ejecucion Not bussy DisplayText:");
            Serial.println(t1 - t0);
        }
    }
    delay(500);
    ok = false;
    while (!ok)
    {
        t0 = micros();
        if (DisplayText("Muy bien, Hasta luego", ledmatrix) == false)
        {
            ok = false;
            t1 = micros();
            Serial.print("Tiempo de ejecucion Bussy DisplayText:");
            Serial.println(t1 - t0);
        }
        else
        {
            ok = true;
            t1 = micros();
            Serial.print("Tiempo de ejecucion Not bussy DisplayText:");
            Serial.println(t1 - t0);
        }
    }
    delay(500);
    ok = false;
    while (!ok)
    {
        t0 = micros();
        if (DisplayText("Adios! ^^   PD: Hello! (Again) ", ledmatrix) == false)
        {
            ok = false;
            t1 = micros();
            Serial.print("Tiempo de ejecucion Bussy DisplayText:");
            Serial.println(t1 - t0);
        }
        else
        {
            ok = true;
            t1 = micros();
            Serial.print("Tiempo de ejecucion Not bussy DisplayText:");
            Serial.println(t1 - t0);
        }
    }
}

void loop()
{
}