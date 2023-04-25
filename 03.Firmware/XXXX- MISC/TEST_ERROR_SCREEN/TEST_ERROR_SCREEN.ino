/**
 * @file TEST_ERROR_SCREEN.ino
 * @author Javi (Javier@musotoku.com)
 * @brief 
 * @version 0.1
 * @date 2023-01-31
 * 
 * @copyright Copyright (c) 2023
 * 
 */
#include <MilliTimer.h>
#include <display.h>
#include <power_bar.h>
const uint16_t C_PIN_ENABLE_LDO_VCC_2 = 1;
MilliTimer timer_blink_error;
uint16_t cont;
bool blink_error_state = false;
void setup()
{
    Wire.begin();
    pinMode(C_PIN_ENABLE_LDO_VCC_2, OUTPUT);
    digitalWrite(C_PIN_ENABLE_LDO_VCC_2, HIGH); // Encendido del DCDC
    initDisplay();
    OLED_display.clearDisplay();
}

void loop()
{

    OLED_display.setCursor(27, 7);
    OLED_display.setTextSize(2);
    OLED_display.setTextColor(BLACK);
    //OLED_display.print("!");
    //OLED_display.fillTriangle(31,0,15,26,48,26,WHITE);
    //OLED_display.fillCircle(31, 14, 13, WHITE);
    //OLED_display.fillRect(30, 6, 3, 12, BLACK);
    //OLED_display.fillRect(30, 20, 3, 3, BLACK);
    OLED_display.clearDisplay();
    if (cont < 10)
    {
        OLED_display.drawBitmap(0, 0, error_square, 64, 30, WHITE, BLACK);
    }
    else if (cont < 20)
    {
        OLED_display.fillTriangle(31, 0, 15, 26, 47, 26, WHITE);
        OLED_display.fillRect(30, 7, 3, 12, BLACK);
        OLED_display.fillRect(30, 21, 3, 3, BLACK);
    }
    else if (cont < 30)
    {
        OLED_display.fillCircle(31, 14, 13, WHITE);
        OLED_display.fillRect(30, 6, 3, 12, BLACK);
        OLED_display.fillRect(30, 20, 3, 3, BLACK);
    }
    else
    {
        cont = 0;
    }

    if (timer_blink_error.poll(200) != C_TIMER_NOT_EXPIRED)
    {
        cont++;
        blink_error_state = !blink_error_state;
    }
    if (blink_error_state == true)
    {
        OLED_display.fillRect(0, C_PWBAR_Y_AXE, C_DISPLAY_WIDTH, C_DISPLAY_HEIGHT, BLACK);
    }
    else
    {
        PowerBar(LEDS_IN_POWERBAR);
    }
    OLED_display.display();
}