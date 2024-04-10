/**
 * @file NAMING.h
 * @author Javi (Javier@musotoku.com)
 * @brief
 * @version 1
 * @date 2023-01-27
 *
 * @copyright Copyright (c) 2022
 *
 */
// #include <Dpad.h>
const uint8_t C_PHASE_NAME = 0;
const uint8_t C_PHASE_CONFIRM = 1;
const uint8_t init_char = 0x41;
const uint8_t end_char = 0x5A;
const uint16_t MAX_CHAR_NAME = 15;

char array_letras[] = {0x41, 0x42, 0x43, 0x44, 0x45, 0x46, 0x47, 0x48, 0x49, 0x4A, 0x4B, 0x4C, 0x4D, 0x4E, 0x4F, 0x50, 0x51, 0x52, 0x53, 0x54, 0x55, 0x56, 0x57, 0x58, 0x59, 0x5A, 0xFA, 0x11};

uint16_t size_array_letras = sizeof(array_letras) / sizeof(array_letras[0]);
char name[50];
uint16_t num_char = 0;
char char_index = 0;
uint16_t index_phase = 0;
int16_t cursor_x_name = 0;
int16_t cursor_x_underscore = 0;
int16_t num_swft = 0;
uint16_t cursor_y = 0;
bool underscore_st = true;
bool end_flag = false;
bool active_question = false;
uint16_t button_event_naming = 0;
MilliTimer blink_underscore;

void Config_Naming()
{
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
        Watchdog.reset();
        while (button_event_naming == C_NONE_EVENT)
        {
            Watchdog.reset();
            button_event_naming = ReadDirPad();

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
        if ((button_event_naming == C_CLICK_UP) || (button_event_naming == C_LP_UP))
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
        else if ((button_event_naming == C_CLICK_DOWN) || (button_event_naming == C_LP_DOWN))
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
        else if ((button_event_naming == C_CLICK_CENTER) || (button_event_naming == C_LP_CENTER))
        {
            if (array_letras[char_index] == 0xFA)
            {
                active_question = true;
            }
            else
            {
                if (array_letras[char_index] == 0x11)
                {
                    if (num_char >= 1)
                    {
                        name[num_char - 1] = 0x00;
                        num_char -= 1;
                        num_char = constrain(num_char, 0, MAX_CHAR_NAME);
                        Serial5.println(num_char);
                    }
                }
                else
                {
                    name[num_char] = array_letras[char_index];
                    num_char++;
                    num_char = constrain(num_char, 0, MAX_CHAR_NAME);
                    Serial5.println(num_char);
                }
                OLED_display.fillRect(0, 16, 64, 16, BLACK);
                if (num_char < 5)
                {
                    cursor_x_name = 0;
                    for (int i = 0; i < num_char; i++)
                    {
                        OLED_display.drawChar(cursor_x_name, cursor_y, name[i], WHITE, BLACK, 2);
                        cursor_x_name += 12;
                        Watchdog.reset();
                    }
                }
                else
                {
                    cursor_x_name = 52 - num_char * 12;
                    for (int i = 0; i < num_char; i++)
                    {
                        OLED_display.drawChar(cursor_x_name, cursor_y, name[i], WHITE, BLACK, 2);
                        cursor_x_name += 12;
                        Watchdog.reset();
                    }
                }
                cursor_x_underscore = cursor_x_name;
            }
        }
        if (active_question == true)
        {
            uint8_t confirm = 0;
            OLED_display.clearDisplay();
            OLED_display.drawChar(0, 0, 0x59, WHITE, BLACK, 2);
            // OLED_display.drawChar(26, 0,0x2F , WHITE, BLACK, 2);
            OLED_display.drawChar(52, 0, 0x4E, WHITE, BLACK, 2);
            while (active_question == true)
            {
                Watchdog.reset();
                button_event_naming = ReadDirPad();

                if ((button_event_naming == C_CLICK_UP) || (button_event_naming == C_LP_UP))
                {
                    OLED_display.drawChar(0, 16, 0x00, WHITE, BLACK, 2);
                    OLED_display.drawChar(52, 16, 0x18, WHITE, BLACK, 2);
                    confirm = 1;
                }
                else if ((button_event_naming == C_CLICK_DOWN) || (button_event_naming == C_LP_DOWN))
                {
                    OLED_display.drawChar(0, 16, 0x18, WHITE, BLACK, 2);
                    OLED_display.drawChar(52, 16, 0x00, WHITE, BLACK, 2);
                    confirm = 2;
                }
                else if ((button_event_naming == C_CLICK_CENTER) || (button_event_naming == C_LP_CENTER))
                {
                    if (confirm == 2)
                    {
                        end_flag = true;
                        active_question = false;
                    }
                    else if (confirm == 1)
                    {
                        active_question = false;
                        OLED_display.clearDisplay();
                        OLED_display.setTextSize(2);
                        OLED_display.setTextColor(WHITE);
                        OLED_display.setCursor(0, 0);
                        OLED_display.drawChar(0, 0, 0x1B, WHITE, BLACK, 2);
                        OLED_display.drawChar(28, 0, init_char, WHITE, BLACK, 2);
                        OLED_display.drawChar(52, 0, 0x1A, WHITE, BLACK, 2);
                        OLED_display.display();
                        Watchdog.reset();
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
                        char_index = 0;
                    }
                }
                OLED_display.display();
                Watchdog.reset();
            }
        }

        OLED_display.display();
        Watchdog.reset();
        button_event_naming = ReadDirPad();
    }

    OLED_display.clearDisplay();
    OLED_display.setCursor(0, 0);
    OLED_display.print("Hi!");
    Watchdog.reset();
    num_swft = num_char * 12 - 64;
    if (num_swft < 0)
    {
        cursor_x_name = 0;
        for (int i = 0; i < num_char; i++)
        {
            OLED_display.drawChar(cursor_x_name, cursor_y, name[i], WHITE, BLACK, 2);
            cursor_x_name += 12;
            Watchdog.reset();
        }
        OLED_display.display();
    }
    else
    {
        int num_spaces = 5;
        for (int j = num_char - 1; j >= 0; j--)
        {
            name[j + num_spaces] = name[j];
            Watchdog.reset();
        }
        for (int i = 1; i <= num_spaces; i++)
        {
            name[(num_spaces + num_char * 2) - i] = 0x00;
            name[i - 1] = 0x00;
            Watchdog.reset();
        }
        num_char = num_char + num_spaces * 2;
        num_swft = num_char * 12 - 64;

        for (int k = 0; k < 3; k++)
        {
            for (int i = 0; i <= (num_swft / 4) + 1; i++)
            {
                cursor_x_name = i * -4;
                for (int j = 0; j < num_char; j++)
                {
                    OLED_display.drawChar(cursor_x_name, cursor_y, name[j], WHITE, BLACK, 2);
                    cursor_x_name += 12;
                }
                OLED_display.display();
                // delay(100);
                for (int i = 0; i < 5; i++)
                {
                    delay(10);
                    Watchdog.reset();
                }
            }
        }
    }
    SaveNameEEPROM(name, num_char);
    // delay(2000);
    for (int i = 0; i < 200; i++)
    {
        delay(10);
        Watchdog.reset();
    }
}
void ShowName()
{
#ifdef SERIAL_DEBUG
    Serial5.println("ShowName");
#endif
    char name_to_display[NUM_POS_NAME];
    uint16_t num_char_to_show = ReadNameEEPROM(name_to_display);
#ifdef SERIAL_DEBUG
    Serial5.println(num_char_to_show);
#endif
    OLED_display.clearDisplay();
    OLED_display.setTextSize(2);
    OLED_display.setCursor(0, 0);
    OLED_display.print("Hi!");

    num_swft = num_char_to_show * 12 - 64;
    if (num_swft < 0)
    {
        cursor_x_name = 0;
        for (int i = 0; i < num_char_to_show; i++)
        {
            OLED_display.drawChar(cursor_x_name, 16, name_to_display[i], WHITE, BLACK, 2);
            cursor_x_name += 12;
        }
        OLED_display.display();
    }
    else
    {
        num_swft = num_char_to_show * 12 - 64;
        for (int k = 0; k < 3; k++)
        {
            for (int i = 0; i <= (num_swft / 4) + 1; i++)
            {
                cursor_x_name = i * -4;
                for (int j = 0; j < num_char_to_show; j++)
                {
                    OLED_display.drawChar(cursor_x_name, 16, name_to_display[j], WHITE, BLACK, 2);
                    cursor_x_name += 12;
                }
                OLED_display.display();
                for (int i = 0; i < 5; i++)
                {
                    delay(10);
                    Watchdog.reset();
                }
            }
        }
    }
}