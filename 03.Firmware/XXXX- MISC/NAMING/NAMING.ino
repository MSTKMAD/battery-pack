/**
 * @file NAMING.ino
 * @author Javi (Javier@musotoku.com)
 * @brief 
 * @version 0.1
 * @date 2022-12-19
 * 
 * @copyright Copyright (c) 2022
 * 
 */

#include <MilliTimer.h>
#include "display.h"
#include <Dpad.h>
const uint16_t C_PIN_ENABLE_LDO_VCC_2 = 1;
const uint8_t C_PHASE_NAME = 0;
const uint8_t C_PHASE_CONFIRM = 1;

char name[25];
char char_index = 0;
uint16_t button_event = C_NONE_EVENT;
uint16_t index_phase = 0;
uint16_t cursor_x = 0;
uint16_t cursor_y = 0;

void setup()
{
    Wire.begin();
    pinMode(C_PIN_BUTT_CENTER, INPUT_PULLUP);
    pinMode(C_PIN_BUTT_UP, INPUT_PULLUP);
    pinMode(C_PIN_BUTT_DOWN, INPUT_PULLUP);
    //pinMode(C_PIN_FLAG_CHARG, INPUT_PULLUP);
    //pinMode(C_PIN_TEST_MODE, INPUT_PULLUP);
    pinMode(C_PIN_ENABLE_LDO_VCC_2, OUTPUT);
    digitalWrite(C_PIN_ENABLE_LDO_VCC_2, HIGH); // Encendido del DCDC
    initDisplay();
    OLED_display.clearDisplay();
    OLED_display.setTextSize(2);
    OLED_display.setCursor(0, 0);
    OLED_display.print("NAME");
    OLED_display.setCursor(0, 16);
    OLED_display.display();
    cursor_x = 0;
    cursor_y = 16;
}

void loop()
{
    // Mostrar NAME: Fila de arriba.
    while (button_event == C_NONE_EVENT)
    {
        button_event = ReadDirPad();
    }
    if ((button_event == C_CLICK_UP) || (button_event == C_LP_UP))
    {
        char_index++;
        constrain(char_index, 65, 90);
        OLED_display.drawChar(cursor_x, 16, char_index, WHITE, BLACK, 2);
    }
    else if ((button_event == C_CLICK_DOWN) || (button_event == C_LP_DOWN))
    {
        char_index--;
        constrain(char_index, 65, 90);
        OLED_display.drawChar(cursor_x, 16, char_index, WHITE, BLACK, 2);
    }
    else if ((button_event == C_CLICK_CENTER) || (button_event == C_LP_CENTER))
    {
        cursor_x += 12;
        OLED_display.setCursor(cursor_x, 16);
    }
    OLED_display.display();
}