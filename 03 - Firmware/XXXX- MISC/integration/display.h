/**
 * @file display.h
 * @author Javi (Javier@musotoku.com)
 * @brief 
 * @version 0.1
 * @date 2020-09-23
 * 
 * @copyright Copyright (c) 2020
 * 
 */
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_IS31FL3731.h>

void dig2bitmap(char dig, uint8_t *bitmap, int16_t array_len)
{
    if (array_len < sizeof(ONEchar7))
    {
        for (int i = 0; i < array_len; i++)
        {
            bitmap[i] = 0xFF;
        }
    }

    if (dig == '1')
    {
        memcpy(bitmap, ONEchar7, array_len);
        //  bitmap = *ONEchar7;
    }
    else if (dig == '2')
    {
        memcpy(bitmap, TWOchar7, array_len);
        //bitmap = TWOchar7;
    }
    else if (dig == '3')
    {
        memcpy(bitmap, THREEchar7, array_len);
        //bitmap = THREEchar7;
    }
    else if (dig == '4')
    {
        memcpy(bitmap, FOURchar7, array_len);
    }
    else if (dig == '5')
    {
        memcpy(bitmap, FIVEchar7, array_len);
    }
    else if (dig == '6')
    {
        memcpy(bitmap, SIXchar7, array_len);
    }
    else if (dig == '7')
    {
        memcpy(bitmap, SEVENchar7, array_len);
    }
    else if (dig == '8')
    {
        memcpy(bitmap, EIGHTchar7, array_len);
    }
    else if (dig == '9')
    {
        memcpy(bitmap, NINEchar7, array_len);
    }
    else if (dig == '0')
    {
        memcpy(bitmap, ZEROchar7, array_len);
    }
    else
    {
        memcpy(bitmap, NULLchar7, array_len);
    }
}
void char2bitmap(char character, uint8_t *bitmap, int16_t array_len)
{
    if (array_len < sizeof(ONEchar7))
    {
        for (int i = 0; i < array_len; i++)
        {
            bitmap[i] = 0xFF;
        }
    }

    if (character == 'A')
    {
        memcpy(bitmap, Achar, array_len);
    }
    else if (character == 'B')
    {
        memcpy(bitmap, Bchar, array_len);
    }
    else if (character == 'C')
    {
        memcpy(bitmap, Cchar, array_len);
    }
    else if (character == 'D')
    {
        memcpy(bitmap, Dchar, array_len);
    }
    else if (character == 'E')
    {
        memcpy(bitmap, Echar, array_len);
    }
    else if (character == 'F')
    {
        memcpy(bitmap, Fchar, array_len);
    }
    else if (character == 'G')
    {
        memcpy(bitmap, Gchar, array_len);
    }
    else if (character == 'H')
    {
        memcpy(bitmap, Hchar, array_len);
    }
    else if (character == 'I')
    {
        memcpy(bitmap, Ichar, array_len);
    }
    else if (character == 'J')
    {
        memcpy(bitmap, Jchar, array_len);
    }
    else if (character == 'K')
    {
        memcpy(bitmap, Kchar, array_len);
    }
    else if (character == 'L')
    {
        memcpy(bitmap, Lchar, array_len);
    }
    else if (character == 'M')
    {
        memcpy(bitmap, Mchar, array_len);
    }
    else if (character == 'N')
    {
        memcpy(bitmap, Nchar, array_len);
    }
    else if (character == 'O')
    {
        memcpy(bitmap, Ochar, array_len);
    }
    else if (character == 'P')
    {
        memcpy(bitmap, Pchar, array_len);
    }
    else if (character == 'Q')
    {
        memcpy(bitmap, Qchar, array_len);
    }
    else if (character == 'R')
    {
        memcpy(bitmap, Rchar, array_len);
    }
    else if (character == 'S')
    {
        memcpy(bitmap, Schar, array_len);
    }
    else if (character == 'T')
    {
        memcpy(bitmap, Tchar, array_len);
    }
    else if (character == 'U')
    {
        memcpy(bitmap, Uchar, array_len);
    }
    else if (character == 'V')
    {
        memcpy(bitmap, Vchar, array_len);
    }
    else if (character == 'W')
    {
        memcpy(bitmap, Wchar, array_len);
    }
    else if (character == 'Y')
    {
        memcpy(bitmap, Ychar, array_len);
    }
    else if (character == 'Z')
    {
        memcpy(bitmap, Zchar, array_len);
    }
    else
    {
        memcpy(bitmap, NULLchar7, array_len);
    }
}
void displayVolts(int16_t number, Adafruit_IS31FL3731_Wing ledmatrix)
{
    number = constrain(number, 40, 160);
    uint8_t dec_number[] = {B00000000,
                            B00000000,
                            B00000000,
                            B00000000,
                            B00000000,
                            B00000000,
                            B00000000};
    uint8_t cent_number[] = {B00000000,
                             B00000000,
                             B00000000,
                             B00000000,
                             B00000000,
                             B00000000,
                             B00000000};
    uint8_t unit_number[] = {B00000000,
                             B00000000,
                             B00000000,
                             B00000000,
                             B00000000,
                             B00000000,
                             B00000000};
    uint8_t *pont_num;

    String str_number = String(number);

    if (str_number.length() == 2)
    {
        memcpy(cent_number, NULLchar7, sizeof(cent_number));
        dig2bitmap(str_number.charAt(0), dec_number, sizeof(dec_number));
        dig2bitmap(str_number.charAt(1), unit_number, sizeof(unit_number));
    }
    else if (str_number.length() == 3)
    {
        dig2bitmap(str_number.charAt(0), cent_number, sizeof(cent_number));
        dig2bitmap(str_number.charAt(1), dec_number, sizeof(dec_number));
        dig2bitmap(str_number.charAt(2), unit_number, sizeof(unit_number));
    }
    uint8_t brightness = 30;

    ledmatrix.setRotation(0);
    ledmatrix.clear();
    ledmatrix.drawBitmap(1, 0, cent_number, 8, 7, brightness);
    ledmatrix.drawBitmap(4, 0, dec_number, 8, 7, brightness);
    ledmatrix.drawPixel(9, 6, brightness);
    ledmatrix.drawBitmap(11, 0, unit_number, 8, 7, brightness);
}

void displayTextBlock(String cadena, Adafruit_IS31FL3731_Wing ledmatrix)
{
    uint8_t bitmap_char[] = {B00000000,
                             B00000000,
                             B00000000,
                             B00000000,
                             B00000000,
                             B00000000,
                             B00000000};
    int16_t length_bit = 0;
    uint8_t brightness = 30;
    char char_of_string;
    for (int i = 0; i < cadena.length(); i++)
    {
        char_of_string = toupper(cadena.charAt(i));
        if (char_of_string - 65 >= 0 && char_of_string - 65 <= 25)
        {
            length_bit += width_letter[char_of_string - 65];
        }
        else if (char_of_string == 32)
        {
            length_bit += width_letter[26];
        }
      //  Serial.println(char_of_string);
       // Serial.println(length_bit);
    }

    if (length_bit > 16)
    {
            int16_t x = 0;
            int16_t j = 0;
            for (int8_t j = 0; j >= -(length_bit - 16); j--)
            {
                ledmatrix.clear();
                x = j;
                for (int i = 0; i < cadena.length(); i++)
                {
                    char_of_string = toupper(cadena.charAt(i));
                    if (char_of_string - 65 >= 0 && char_of_string - 65 <= 25)
                    {

                        char2bitmap(char_of_string, bitmap_char, sizeof(bitmap_char));
                        ledmatrix.drawBitmap(x, 0, bitmap_char, 8, 7, brightness);
                        x += width_letter[char_of_string-65];
                    }
                    else if (char_of_string == 32)
                    {
                        char2bitmap(char_of_string, bitmap_char, sizeof(bitmap_char));
                        ledmatrix.drawBitmap(x, 0, bitmap_char, 8, 7, brightness);
                        x += width_letter[26];
                    }
                }
                delay(100);
            }
    }
    else
    {
        int16_t x = 0;
        ledmatrix.clear();
        for (int i = 0; i < cadena.length(); i++)
        {

            char_of_string = toupper(cadena.charAt(i));
            if (char_of_string - 65 >= 0 && char_of_string - 65 <= 25)
            {
                char2bitmap(char_of_string, bitmap_char, sizeof(bitmap_char));
                ledmatrix.drawBitmap(x, 0, bitmap_char, 8, 7, brightness);
                x += width_letter[char_of_string - 65];
            }
            else if (char_of_string == 32)
            {
                char2bitmap(char_of_string, bitmap_char, sizeof(bitmap_char));
                ledmatrix.drawBitmap(x, 0, bitmap_char, 8, 7, brightness);
                x += width_letter[26];
            }
           // Serial.println(char_of_string);
        }
    }
}