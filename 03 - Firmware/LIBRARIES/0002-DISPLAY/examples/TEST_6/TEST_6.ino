/**
 * @file TEST_6.ino
 * @author Javi (Javier@musotoku.com)
 * @brief  Testeo de la funcion DisplayTest mostrando todos los caracteres imprimibles posibles.
 * @version 2
 * @date 2020-12-03
 * 
 * @copyright Copyright (c) 2020
 * 
 */

#include <display.h>

Adafruit_IS31FL3731_Wing ledmatrix = Adafruit_IS31FL3731_Wing();
bool display_status = C_DISPLAY_ST_NOT_BUSSY;
bool reset_init_text = true;
bool mode_bright_display = C_MODE_HIGH_BRIGHT;
void setup()
{
    Serial.begin(9600);
    ledmatrix.begin();

    Serial.println("IS31 Found!");

    char c;
    String toString = "";
    for (int i = 32; i <= 126; i++)
    {
        c = i;
        toString += String(c) + " ";
    }
    Serial.println(toString);
    display_status = DisplayText(toString, ledmatrix, reset_init_text, mode_bright_display);
    reset_init_text = false;
    while (display_status != C_DISPLAY_ST_NOT_BUSSY)
    {
        display_status = DisplayText(toString, ledmatrix, reset_init_text, mode_bright_display);
    }
}

void loop()
{
}