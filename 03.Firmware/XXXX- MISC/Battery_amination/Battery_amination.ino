/**
 * @file Battery_amination.ino
 * @author Javi (Javier@musotoku.com)
 * @brief 
 * @version 1
 * @date 2021-09-02
 * 
 * @copyright Copyright (c) 2021
 * 
 */
#include <MilliTimer.h>
#include <display.h>

Adafruit_IS31FL3731_Wing ledmatrix = Adafruit_IS31FL3731_Wing();

int j;
bool ok = true;
int x;
int cont = 0;
void setup()
{
    ledmatrix.begin();
    SwitchScreenOff(ledmatrix);
    delay(3000);
    DisplayCap(95, ledmatrix);
    delay(1000);
    SwitchScreenOff(ledmatrix);
    delay(500);
    DisplayVolt(147, ledmatrix);
    while (ok)
    {
        int x = abs(sin(radians(++j))) * 15;
        PowerBar(x, ledmatrix);
        delay(10);
        if (x == 3)
        {
            cont++;
            if (cont == 4)
            {
                ok = false;
            }
        }
    }
}

void loop()
{
}