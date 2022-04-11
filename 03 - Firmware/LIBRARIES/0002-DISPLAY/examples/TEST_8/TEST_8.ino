/**
 * @file TEST_8.ino
 * @author Javi (Javier@musotoku.com)
 * @brief Test de la funcion DisplayUsbIn y DisplayUsbOut.
 * @version 1
 * @date 2021-03-26
 * 
 * @copyright Copyright (c) 2021
 * 
 */
#include "display.h"
//#include "MilliTimer.h"

Adafruit_IS31FL3731_Wing ledmatrix = Adafruit_IS31FL3731_Wing();
MilliTimer timer;

void setup()
{
    ledmatrix.begin();
    timer.set(10000);
    while (timer.poll() == C_TIMER_NOT_EXPIRED)
    {
        DisplayUsbIn(ledmatrix);
    }
    while (true)
    {
        DisplayUsbOut(ledmatrix);
    }
}

void loop()
{
}