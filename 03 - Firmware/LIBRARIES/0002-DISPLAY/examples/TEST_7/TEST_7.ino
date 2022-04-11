/**
 * @file TEST_7.ino
 * @author Javi (Javier@musotoku.com)
 * @brief  Testeo del boton que marca el estado de la salida de voltage que forma parte de la pantalla. 
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
    bool led_state = false;
    for (int i = 32; i <= 126; i++)
    {
        LedWork(true, ledmatrix, mode_bright_display);
        delay(1000);
        LedWork(false, ledmatrix, mode_bright_display);
        delay(1000);
    }
}

void loop()
{
}