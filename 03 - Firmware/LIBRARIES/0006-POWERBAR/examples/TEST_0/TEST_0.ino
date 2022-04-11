/**
 * @file TEST_0.ino
 * @author Javi (Javier@musotoku.com)
 * @brief Test para probar la actualizacion de la barra de potencia. Oscila los valores entre el maximo y el minimo de potencia que puede mostrar la barra.
 * @version 0.1
 * @date 2021-03-18
 * 
 * @copyright Copyright (c) 2021
 * 
 */
#include "display.h"
#include "power_bar.h"

Adafruit_IS31FL3731_Wing ledmatrix = Adafruit_IS31FL3731_Wing();

void setup()
{
    ledmatrix.begin();
    while (1)
    {
        for (int i = 0; i < MAX_POWER_DISPLAYED; i++)
        {
            UpdatePowerBar(i, ledmatrix, C_MODE_HIGH_BRIGHT);
            delay(1);
        }
    }
}

void loop()
{
}