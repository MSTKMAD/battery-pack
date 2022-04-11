/**
 * @file TEST_3.ino
 * @author Javi (Javier@musotoku.com)
 * @brief Este test pone a prueba la compatibilidad y convivencia de la barra de potencia y el display de informacion, en este caso mostrando cambios de voltaje.
 * @version 2
 * @date 2021-03-16
 * 
 * @copyright Copyright (c) 2020
 * 
 */

#include <MilliTimer.h>
#include <display.h>
#include <power_bar.h>
Adafruit_IS31FL3731_Wing ledmatrix = Adafruit_IS31FL3731_Wing();

void setup()
{
    // Serial.begin(9600);
    // while (!Serial)
    // {
    //     ;
    // }
    ledmatrix.begin();

    int x, j = 0;
    MilliTimer timer;
    while (1)
    {
        for (int16_t i = 40; i <= 160; i++)
        {
            DisplayVolt(i, ledmatrix);
            if (timer.poll(10))
            {
                j++;
                if (j == 360)
                {
                    j = 0;
                }
                x = abs(sin(radians(j++))) * 16;
                Serial.println(x);
                PowerBar(x, ledmatrix);
                delay(2);
            }
        }
        for (int16_t i = 160; i >= 40; i--)
        {
            DisplayVolt(i, ledmatrix);
            if (timer.poll(10))
            {
                j++;
                if (j == 360)
                {
                    j = 0;
                }
                x = abs(sin(radians(j++))) * 16;
                Serial.println(x);
                PowerBar(x, ledmatrix);
                delay(2);
            }
        }
    }
}

void loop()
{
}
