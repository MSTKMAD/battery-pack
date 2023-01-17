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
const uint8_t init_char = 0x41;
const uint8_t end_char = 0x5A;

char array_letras[] = {0x41, 0x42, 0x43, 0x44, 0x45, 0x46, 0x47, 0x48, 0x49, 0x4A, 0x4B, 0x4C, 0x4D, 0x4E, 0x4F, 0x50, 0x51, 0x52, 0x53, 0x54, 0x55, 0x56, 0x57, 0x58, 0x59, 0x5A, 0xFA, 0x11};

uint16_t size_array_letras = sizeof(array_letras) / sizeof(array_letras[0]);
char name[25];
uint16_t num_char = 0;
char char_index = 0;
uint16_t button_event = C_NONE_EVENT;
uint16_t index_phase = 0;
int16_t cursor_x_name = 0;
int16_t cursor_x_underscore = 0;
int16_t num_swft = 0;
uint16_t cursor_y = 0;
bool underscore_st = true;
bool end_flag = false;

MilliTimer blink_underscore;

void setup()
{
    Serial5.begin(57600);
    Serial5.println("START!\n\r");

    pinMode(C_PIN_BUTT_CENTER, INPUT_PULLUP);
    pinMode(C_PIN_BUTT_UP, INPUT_PULLUP);
    pinMode(C_PIN_BUTT_DOWN, INPUT_PULLUP);
    pinMode(C_PIN_ENABLE_LDO_VCC_2, OUTPUT);

    Wire.begin();
    digitalWrite(C_PIN_ENABLE_LDO_VCC_2, HIGH); // Encendido del DCDC
    initDisplay();

    OLED_display.clearDisplay();
    OLED_display.setTextSize(2);
    OLED_display.setTextColor(WHITE);
    OLED_display.setCursor(0, 0);
    OLED_display.drawChar(0, 0, 0x1B, WHITE, BLACK, 2);
    OLED_display.drawChar(28, 0, init_char, WHITE, BLACK, 2);
    OLED_display.drawChar(52, 0, 0x1A, WHITE, BLACK, 2);
    OLED_display.display();
    cursor_x_name = 0;
    cursor_y = 16;
    while (end_flag == false)
    {
        while (button_event == C_NONE_EVENT)
        {
            button_event = ReadDirPad();
            if (blink_underscore.poll(500) != C_TIMER_NOT_EXPIRED)
            {
                if (underscore_st)
                {
                    OLED_display.drawChar(cursor_x_underscore, cursor_y, 0x16, WHITE, BLACK, 2);
                    underscore_st = !underscore_st;
                    OLED_display.display();
                }
                else
                {
                    underscore_st = !underscore_st;
                    OLED_display.drawChar(cursor_x_underscore, cursor_y, 0x0, WHITE, BLACK, 2);
                    OLED_display.display();
                }
            }
        }
        if ((button_event == C_CLICK_UP) || (button_event == C_LP_UP))
        {
            if (char_index == size_array_letras - 1)
            {
                char_index = 0;
            }
            else
            {
                char_index++;
            }
            OLED_display.fillRect(16, 0, 36, 14, BLACK);
            if (array_letras[char_index] == 0x11)
            {
                OLED_display.setCursor(16, 0);
                OLED_display.print("DEL");
            }
            else if (array_letras[char_index] == 0xFA)
            {
                OLED_display.setCursor(16, 0);
                OLED_display.print("END");
            }
            else
            {

                OLED_display.drawChar(28, 0, array_letras[char_index], WHITE, BLACK, 2);
            }
        }
        else if ((button_event == C_CLICK_DOWN) || (button_event == C_LP_DOWN))
        {
            if (char_index == 0)
            {
                char_index = size_array_letras - 1;
            }
            else
            {
                char_index--;
            }
            OLED_display.fillRect(16, 0, 36, 14, BLACK);
            if (array_letras[char_index] == 0x11)
            {
                OLED_display.setCursor(16, 0);
                OLED_display.print("DEL");
            }
            else if (array_letras[char_index] == 0xFA)
            {
                OLED_display.setCursor(16, 0);
                OLED_display.print("END");
            }
            else
            {
                OLED_display.drawChar(28, 0, array_letras[char_index], WHITE, BLACK, 2);
            }
        }
        else if ((button_event == C_CLICK_CENTER) || (button_event == C_LP_CENTER))
        {
            if (array_letras[char_index] == 0xFA)
            {
                end_flag = true;
            }
            else
            {
                if (array_letras[char_index] == 0x11)
                {
                    if (num_char >= 1)
                    {
                        name[num_char - 1] = 0x00;
                        num_char -= 1;
                    }
                }
                else
                {
                    name[num_char] = array_letras[char_index];
                    num_char++;
                }
                OLED_display.fillRect(16, 16, 64, 16, BLACK);
                if (num_char < 5)
                {
                    cursor_x_name = 0;
                    for (int i = 0; i < num_char; i++)
                    {
                        OLED_display.drawChar(cursor_x_name, cursor_y, name[i], WHITE, BLACK, 2);
                        cursor_x_name += 12;
                    }
                }
                else
                {
                    cursor_x_name = 52 - num_char * 12;
                    for (int i = 0; i < num_char; i++)
                    {
                        OLED_display.drawChar(cursor_x_name, cursor_y, name[i], WHITE, BLACK, 2);
                        cursor_x_name += 12;
                    }
                }
                cursor_x_underscore = cursor_x_name;
            }
        }
        OLED_display.display();
        button_event = ReadDirPad();
    }

    OLED_display.clearDisplay();
    OLED_display.setCursor(0, 0);
    OLED_display.print("Hi!");

    num_swft = num_char * 12 - 64;
    if (num_swft < 0)
    {
        cursor_x_name = 0;
        for (int i = 0; i < num_char; i++)
        {
            OLED_display.drawChar(cursor_x_name, cursor_y, name[i], WHITE, BLACK, 2);
            cursor_x_name += 12;
        }
        OLED_display.display();
    }
    else
    {
        int num_spaces = 3;
        for (int j = num_char - 1; j >= 0; j--)
        {
            name[j + num_spaces] = name[j];
        }
        for (int i = 1; i <= num_spaces; i++)
        {
            /* code */
            name[(num_spaces + num_char * 2) - i] = 0x00;
            name[i - 1] = 0x00;
        }
        num_char = num_char + num_spaces * 2;
        num_swft = num_char * 12 - 64;

        for (int k = 0; k < 4; k++)
        {
            /* code */

            for (int i = 0; i <= (num_swft / 4); i++)
            {
                cursor_x_name = i * -4;
                for (int j = 0; j < num_char; j++)
                {
                    OLED_display.drawChar(cursor_x_name, cursor_y, name[j], WHITE, BLACK, 2);
                    cursor_x_name += 12;
                }
                OLED_display.display();
                delay(100);
            }
        }
    }
}

void loop()
{
}