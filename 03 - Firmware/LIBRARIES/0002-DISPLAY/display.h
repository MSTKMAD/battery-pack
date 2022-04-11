/**
 * @file display.h
 * @author Javi (Javier@musotoku.com)
 * @brief Libreria MUSOTOKU para display 15x7.
 * @version 4
 * @date 2020-8-4
 * 
 * @copyright Copyright (c) 2020
 * 
 */
#include "bitmaps.h"
#include <Wire.h>
// #include <Adafruit_GFX.h>
// #include <Adafruit_IS31FL3731.h>

#include <batt_Adafruit_GFX.h>
#include <batt_Adafruit_IS31FL3731.h>

//#include <MilliTimer.h>


#if POWERBAR_LOCATION == 1
const int16_t C_PWBAR_Y_AXE = 6;        // Posicion de la barra de potencia.
const int16_t C_TEXT_CHAR_OFFSET_Y = 0; // Altura de inicio de escritura de los caracteres.
#elif POWERBAR_LOCATION == 0
const int16_t C_PWBAR_Y_AXE = 0;        // Posicion de la barra de potencia.
const int16_t C_TEXT_CHAR_OFFSET_Y = 1; // Altura de inicio de escritura de los caracteres.
#endif

const bool C_MODE_HIGH_BRIGHT = true; //
const bool C_MODE_LOW_BRIGHT = false; //

const int16_t C_HIGH_BRIGHTNESS = 20; // Nivel de brillo alto de los Leds.
const int16_t C_LOW_BRIGHTNESS = 5;   // Nivel de brillo alto de los Leds.
int16_t C_DISPLAY_ON = 0;             // Encendido del led.
const int16_t C_DISPLAY_OFF = 0;      // Apagado del LED.

const bool C_DISPLAY_ST_BUSSY = false;
const bool C_DISPLAY_ST_NOT_BUSSY = true;

const uint8_t C_NUMBER_BLINK_LOW_BATTERY = 4;   // Numero de veces que parpadea la pantalla de Low Batery.
const uint16_t C_DELAY_BLINK_LOW_BATTERY = 300; // Duracion del parpadeo del Low Battery.

const int32_t C_TIMER_BTW_FRAMES = 75;  // Tiempo de refresco de la pantalla.
const int16_t C_TEXT_BITMAP_WEIGHT = 8; // Tamaño del bitmap.

const bool blink_OFF = false;
const bool blink_ON = true;

const int16_t C_PIXEL_START_STRING = 7; // Pixel en que empieza la rotacion de la cadena.
const int16_t C_PIXEL_STOP_STRING = 0;  // Pixel en que termina la rotacion de la cadena.
/**
 * @brief Apagado de la pantalla.
 * 
 */
void SwitchScreenOff(Adafruit_IS31FL3731_Wing ledmatrix)
{
    ledmatrix.fillRect(0, C_TEXT_CHAR_OFFSET_Y, C_DISPLAY_WIDTH, C_TEXT_CHAR_OFFSET_Y + 6, C_DISPLAY_OFF);
}
/**
 * @brief Esta funcion muestra en el display una cadena de caracteres en un scroll lateral der-izq.
 * 
 * @param cadena 
 * @param ledmatrix 
 * @param reset 
 * @param mode_bright 
 * @return true 
 * @return false 
 */
bool DisplayText(String cadena, Adafruit_IS31FL3731_Wing ledmatrix, bool reset = false, bool mode_bright = C_MODE_HIGH_BRIGHT)
{
    static bool display_status = C_DISPLAY_ST_NOT_BUSSY;
    static int16_t length_bit = 0; // Tamaño del bitmap de la cadena.
    char char_of_string;
    static int16_t pos_x_display = 0;
    static int16_t x_axis_index = 0;
    static String cadena_local;
    static MilliTimer frame_delay_timer; // Timer de refresco de la pantalla.
    if (mode_bright == C_MODE_HIGH_BRIGHT)
    {
        C_DISPLAY_ON = C_HIGH_BRIGHTNESS;
    }
    else if (mode_bright == C_MODE_LOW_BRIGHT)
    {
        C_DISPLAY_ON = C_LOW_BRIGHTNESS;
    }
    if ((display_status == C_DISPLAY_ST_NOT_BUSSY) || (reset == true))
    {
        length_bit = 0;
        cadena_local = cadena;
        for (uint16_t i = 0; i < cadena_local.length(); i++)
        {
            char_of_string = toupper(cadena_local.charAt(i));
            if (char_of_string >= 32 && char_of_string <= 126)
            {
                length_bit += width_bitmap[char_of_string - 32];
            }
        }
        display_status = C_DISPLAY_ST_BUSSY;
        if (length_bit <= 15)
        {
            x_axis_index = (15 - length_bit + 1) / 2;
        }
        else
        {
            x_axis_index = C_PIXEL_START_STRING;
            frame_delay_timer.set(C_TIMER_BTW_FRAMES);
        }
    }
    else if (display_status == C_DISPLAY_ST_BUSSY)
    {
        if (length_bit <= 15)
        {
            ledmatrix.fillRect(0, C_TEXT_CHAR_OFFSET_Y, C_DISPLAY_WIDTH, C_TEXT_CHAR_OFFSET_Y + 6, C_DISPLAY_OFF);
            pos_x_display = x_axis_index;
            for (uint16_t i = 0; i < cadena_local.length(); i++)
            {
                char_of_string = toupper(cadena_local.charAt(i));
                if (char_of_string >= 32 && char_of_string <= 126)
                {
                    Char2Bitmap(char_of_string, bitmap_char, sizeof(bitmap_char));
                    ledmatrix.drawBitmap(pos_x_display, C_TEXT_CHAR_OFFSET_Y, bitmap_char, C_TEXT_BITMAP_WEIGHT, C_TEXT_CHAR_HEIGHT, C_DISPLAY_ON);
                    pos_x_display += width_bitmap[char_of_string - 32];
                }
                else
                {
                    Char2Bitmap(' ', bitmap_char, sizeof(bitmap_char));
                    ledmatrix.drawBitmap(pos_x_display, C_TEXT_CHAR_OFFSET_Y, bitmap_char, C_TEXT_BITMAP_WEIGHT, C_TEXT_CHAR_HEIGHT, C_DISPLAY_ON);
                    pos_x_display += width_bitmap[' ' - 32];
                }
            }
            display_status = C_DISPLAY_ST_NOT_BUSSY;
        }
        else
        {
            if (frame_delay_timer.poll())
            {
                ledmatrix.fillRect(0, C_TEXT_CHAR_OFFSET_Y, C_DISPLAY_WIDTH, C_TEXT_CHAR_OFFSET_Y + 6, C_DISPLAY_OFF);
                pos_x_display = x_axis_index;
                for (uint16_t i = 0; i < cadena_local.length(); i++)
                {
                    char_of_string = toupper(cadena_local.charAt(i));
                    if (char_of_string >= 32 && char_of_string <= 126)
                    {
                        Char2Bitmap(char_of_string, bitmap_char, sizeof(bitmap_char));
                        ledmatrix.drawBitmap(pos_x_display, C_TEXT_CHAR_OFFSET_Y, bitmap_char, C_TEXT_BITMAP_WEIGHT, C_TEXT_CHAR_HEIGHT, C_DISPLAY_ON);
                        pos_x_display += width_bitmap[char_of_string - 32];
                    }
                    else
                    {
                        Char2Bitmap(' ', bitmap_char, sizeof(bitmap_char));
                        ledmatrix.drawBitmap(pos_x_display, C_TEXT_CHAR_OFFSET_Y, bitmap_char, C_TEXT_BITMAP_WEIGHT, C_TEXT_CHAR_HEIGHT, C_DISPLAY_ON);
                        pos_x_display += width_bitmap[' ' - 32];
                    }
                }
                if (x_axis_index == -length_bit + C_PIXEL_STOP_STRING)
                {
                    display_status = C_DISPLAY_ST_NOT_BUSSY;
                }
                else
                {
                    x_axis_index--;
                    frame_delay_timer.set(C_TIMER_BTW_FRAMES);
                }
            }
        }
    }
    return display_status;
}

/**
 * @brief Esta funcion imprime la pantalla de voltaje, que muestra le voltaje al que se encuentra la salida de la bateria, con un decimal.
 * 
 * @param number 
 * @param ledmatrix 
 * @param mode_bright 
 */
void DisplayVolt(int16_t number, Adafruit_IS31FL3731_Wing ledmatrix, bool mode_bright = C_MODE_HIGH_BRIGHT)
{
   
    String str_number = String(number);
    if (str_number.length() == 2)
    {
        Char2Bitmap(' ', volt_cent_number, sizeof(volt_cent_number));
        Char2Bitmap(str_number.charAt(0), volt_dec_number, sizeof(volt_dec_number));
        Char2Bitmap(str_number.charAt(1), volt_unit_number, sizeof(volt_unit_number));
    }
    else if (str_number.length() == 3)
    {
        Char2Bitmap(str_number.charAt(0), volt_cent_number, sizeof(volt_cent_number));
        Char2Bitmap(str_number.charAt(1), volt_dec_number, sizeof(volt_dec_number));
        Char2Bitmap(str_number.charAt(2), volt_unit_number, sizeof(volt_unit_number));
    }
    if (mode_bright == C_MODE_HIGH_BRIGHT)
    {
        C_DISPLAY_ON = C_HIGH_BRIGHTNESS;
    }
    else if (mode_bright == C_MODE_LOW_BRIGHT)
    {
        C_DISPLAY_ON = C_LOW_BRIGHTNESS;
    }

    ledmatrix.fillRect(0, C_TEXT_CHAR_OFFSET_Y, C_DISPLAY_WIDTH, C_TEXT_CHAR_OFFSET_Y + 6, C_DISPLAY_OFF);
    ledmatrix.drawBitmap(1, C_TEXT_CHAR_OFFSET_Y, volt_cent_number, C_TEXT_BITMAP_WEIGHT, C_TEXT_CHAR_HEIGHT, C_DISPLAY_ON);
    ledmatrix.drawBitmap(4, C_TEXT_CHAR_OFFSET_Y, volt_dec_number, C_TEXT_BITMAP_WEIGHT, C_TEXT_CHAR_HEIGHT, C_DISPLAY_ON);
    ledmatrix.drawPixel(9, C_TEXT_CHAR_OFFSET_Y + 5, C_DISPLAY_ON);
    ledmatrix.drawBitmap(11, C_TEXT_CHAR_OFFSET_Y, volt_unit_number, C_TEXT_BITMAP_WEIGHT, C_TEXT_CHAR_HEIGHT, C_DISPLAY_ON);
}

/**
 * @brief Esta fncion muestra la pantalla de porcentaje de capacidad de bateria. si la capacidad es 100% muestra la pantalla FULL.
 * 
 * @param capacity 
 * @param ledmatrix 
 * @param mode_bright 
 */
void DisplayCap(int16_t capacity, Adafruit_IS31FL3731_Wing ledmatrix, bool mode_bright = C_MODE_HIGH_BRIGHT)
{

    capacity = constrain(capacity, 0, 100);
    if (mode_bright == C_MODE_HIGH_BRIGHT)
    {
        C_DISPLAY_ON = C_HIGH_BRIGHTNESS;
    }
    else if (mode_bright == C_MODE_LOW_BRIGHT)
    {
        C_DISPLAY_ON = C_LOW_BRIGHTNESS;
    }

    String str_capacity = String(capacity);
    if (capacity == 100)
    {
        ledmatrix.fillRect(0, C_TEXT_CHAR_OFFSET_Y, C_DISPLAY_WIDTH, C_TEXT_CHAR_OFFSET_Y + 6, C_DISPLAY_OFF);
        ledmatrix.drawBitmap(1, C_TEXT_CHAR_OFFSET_Y, Fchar, C_TEXT_BITMAP_WEIGHT, C_TEXT_CHAR_HEIGHT, C_DISPLAY_ON);
        ledmatrix.drawBitmap(5, C_TEXT_CHAR_OFFSET_Y, Uchar, C_TEXT_BITMAP_WEIGHT, C_TEXT_CHAR_HEIGHT, C_DISPLAY_ON);
        ledmatrix.drawBitmap(9, C_TEXT_CHAR_OFFSET_Y, Lchar, C_TEXT_BITMAP_WEIGHT, C_TEXT_CHAR_HEIGHT, C_DISPLAY_ON);
        ledmatrix.drawBitmap(12, C_TEXT_CHAR_OFFSET_Y, Lchar, C_TEXT_BITMAP_WEIGHT, C_TEXT_CHAR_HEIGHT, C_DISPLAY_ON);
    }
    else
    {
        if (str_capacity.length() == 1)
        {
            Char2Bitmap(' ', cap_dec_number, sizeof(cap_dec_number));
            Char2Bitmap(str_capacity.charAt(0), cap_unit_number, sizeof(cap_unit_number));
        }
        else if (str_capacity.length() == 2)
        {
            Char2Bitmap(str_capacity.charAt(0), cap_dec_number, sizeof(cap_dec_number));
            Char2Bitmap(str_capacity.charAt(1), cap_unit_number, sizeof(cap_unit_number));
        }

        ledmatrix.fillRect(0, C_TEXT_CHAR_OFFSET_Y, C_DISPLAY_WIDTH, C_TEXT_CHAR_OFFSET_Y + 6, C_DISPLAY_OFF);
        ledmatrix.drawBitmap(11, C_TEXT_CHAR_OFFSET_Y, PERCENTchar, C_TEXT_BITMAP_WEIGHT, C_TEXT_CHAR_HEIGHT, C_DISPLAY_ON);
        ledmatrix.drawBitmap(1, C_TEXT_CHAR_OFFSET_Y, cap_dec_number, C_TEXT_BITMAP_WEIGHT, C_TEXT_CHAR_HEIGHT, C_DISPLAY_ON);
        ledmatrix.drawBitmap(6, C_TEXT_CHAR_OFFSET_Y, cap_unit_number, C_TEXT_BITMAP_WEIGHT, C_TEXT_CHAR_HEIGHT, C_DISPLAY_ON);
    }
}

/**
 * @brief Esta funcion muestra el marco de una bateria, se encarga de ir rellenando e manera solida la capacidad recibida, y deja parpadeando la que seria el proximo 10% que se encuentra cargando.
 * 
 * @param capacity 
 * @param ledmatrix 
 * @param mode_bright 
 */
void DisplayBattCharging(int16_t capacity, Adafruit_IS31FL3731_Wing ledmatrix, bool mode_bright = C_MODE_HIGH_BRIGHT)
{
    capacity = constrain(capacity, 10, 100);
    int16_t fill_capacity_column;
    static MilliTimer timer_blinking_column;
    int16_t blinking_column = 0;
    static bool blink_state = blink_OFF;
    if (mode_bright == C_MODE_HIGH_BRIGHT)
    {
        C_DISPLAY_ON = C_HIGH_BRIGHTNESS;
    }
    else if (mode_bright == C_MODE_LOW_BRIGHT)
    {
        C_DISPLAY_ON = C_LOW_BRIGHTNESS;
    }

    ledmatrix.drawBitmap(0, C_TEXT_CHAR_OFFSET_Y, Frame_Batt_1, C_TEXT_BITMAP_WEIGHT, C_TEXT_CHAR_HEIGHT, C_DISPLAY_ON);
    ledmatrix.drawBitmap(8, C_TEXT_CHAR_OFFSET_Y, Frame_Batt_2, C_TEXT_BITMAP_WEIGHT, C_TEXT_CHAR_HEIGHT, C_DISPLAY_ON);

    fill_capacity_column = capacity / 10;

    ledmatrix.fillRect(1, C_TEXT_CHAR_OFFSET_Y + 1, fill_capacity_column, C_TEXT_CHAR_OFFSET_Y + 5, C_DISPLAY_ON);

    blinking_column = fill_capacity_column + 1;

    if (timer_blinking_column.poll(500))
    {
        if (blink_state == blink_ON)
        {
            ledmatrix.writeFastVLine(blinking_column, C_TEXT_CHAR_OFFSET_Y + 1, DISPLAY_SIZE - 2, C_DISPLAY_ON);
            blink_state = blink_OFF;
        }
        else if (blink_state == blink_OFF)
        {
            ledmatrix.writeFastVLine(blinking_column, C_TEXT_CHAR_OFFSET_Y + 1, DISPLAY_SIZE - 2, C_DISPLAY_OFF);
            blink_state = blink_ON;
        }
    }
}

/**
 * @brief Animacion de Entrada del USB. A los 2 segundos disminuye su brillo.
 * 
 * @param ledmatrix 
 */
void DisplayUsbIn(Adafruit_IS31FL3731_Wing ledmatrix)
{
    bool flag_initialize = true;
    int16_t index = 15;

    MilliTimer timer_set_bright_low;
    MilliTimer timer_frames_spacer;
    while (flag_initialize == true)
    {
        if (timer_frames_spacer.poll(75) != C_TIMER_NOT_EXPIRED)
        {
            C_DISPLAY_ON = C_HIGH_BRIGHTNESS;
            index--;
            ledmatrix.fillRect(0, C_TEXT_CHAR_OFFSET_Y, C_DISPLAY_WIDTH, C_DISPLAY_HEIGHT + 1, C_DISPLAY_OFF);
            ledmatrix.drawBitmap(8 - index, C_TEXT_CHAR_OFFSET_Y, Frame_USB_2, C_TEXT_BITMAP_WEIGHT, C_TEXT_CHAR_HEIGHT, C_DISPLAY_ON);
            ledmatrix.drawBitmap(0 - index, C_TEXT_CHAR_OFFSET_Y, Frame_USB_1, C_TEXT_BITMAP_WEIGHT, C_TEXT_CHAR_HEIGHT, C_DISPLAY_ON);
            if (index == 0)
            {
                ledmatrix.drawBitmap(8, C_TEXT_CHAR_OFFSET_Y, Frame_USB_2, C_TEXT_BITMAP_WEIGHT, C_TEXT_CHAR_HEIGHT, C_DISPLAY_ON);
                ledmatrix.drawBitmap(0, C_TEXT_CHAR_OFFSET_Y, Frame_USB_1, C_TEXT_BITMAP_WEIGHT, C_TEXT_CHAR_HEIGHT, C_DISPLAY_ON);
                delay(2000);
                flag_initialize = false;
                C_DISPLAY_ON = C_LOW_BRIGHTNESS;
                ledmatrix.drawBitmap(8, C_TEXT_CHAR_OFFSET_Y, Frame_USB_2, C_TEXT_BITMAP_WEIGHT, C_TEXT_CHAR_HEIGHT, C_DISPLAY_ON);
                ledmatrix.drawBitmap(0, C_TEXT_CHAR_OFFSET_Y, Frame_USB_1, C_TEXT_BITMAP_WEIGHT, C_TEXT_CHAR_HEIGHT, C_DISPLAY_ON);
                index = 11;
            }
        }
    }
}
/**
  * @brief Anuimacion de salida del USB.
  * 
  * @param ledmatrix 
  */
void DisplayUsbOut(Adafruit_IS31FL3731_Wing ledmatrix)
{
    bool flag_initialize = true;
    int16_t index = 0;

    MilliTimer timer_frames_spacer;

    while (flag_initialize == true)
    {
        if (timer_frames_spacer.poll(50) != C_TIMER_NOT_EXPIRED)
        {
            C_DISPLAY_ON = C_HIGH_BRIGHTNESS;
            index++;
            ledmatrix.fillRect(0, C_TEXT_CHAR_OFFSET_Y, C_DISPLAY_WIDTH, C_TEXT_CHAR_OFFSET_Y + 6, C_DISPLAY_OFF);
            ledmatrix.drawBitmap(8 - index, C_TEXT_CHAR_OFFSET_Y, Frame_USB_2, C_TEXT_BITMAP_WEIGHT, C_TEXT_CHAR_HEIGHT, C_DISPLAY_ON);
            ledmatrix.drawBitmap(0 - index, C_TEXT_CHAR_OFFSET_Y, Frame_USB_1, C_TEXT_BITMAP_WEIGHT, C_TEXT_CHAR_HEIGHT, C_DISPLAY_ON);
            if (index == 15)
            {
                flag_initialize = false;
                index = 11;
            }
        }
    }
}

/**
 * @brief Animacion del modo Diagnostico.
 * 
 */
void DisplayDiagnosticMode(Adafruit_IS31FL3731_Wing ledmatrix)
{
    MilliTimer timer_fr_diagnostic;
    int i = 0;
    ledmatrix.fillRect(0, C_TEXT_CHAR_OFFSET_Y, C_DISPLAY_WIDTH, C_TEXT_CHAR_OFFSET_Y + 6, C_DISPLAY_OFF);
    while (i <= 30)
    {
        if (timer_fr_diagnostic.poll(200))
        {
            ledmatrix.drawBitmap(8, C_TEXT_CHAR_OFFSET_Y, FACE_2, C_TEXT_BITMAP_WEIGHT, C_TEXT_CHAR_HEIGHT, i);
            ledmatrix.drawBitmap(0, C_TEXT_CHAR_OFFSET_Y, FACE_1, C_TEXT_BITMAP_WEIGHT, C_TEXT_CHAR_HEIGHT, i);
            i += 6;
        }
    }
}
/**
 * @brief Animacion Bateria Baja.
 * 
 */

bool DisplayLowBattery(Adafruit_IS31FL3731_Wing ledmatrix, bool mode_bright = C_MODE_HIGH_BRIGHT)
{
    static MilliTimer timer_blink;
    static int i = 0;
    static bool display_status = C_DISPLAY_ST_NOT_BUSSY;
    static bool blink_state = blink_ON;
    if (mode_bright == C_MODE_HIGH_BRIGHT)
    {
        C_DISPLAY_ON = C_HIGH_BRIGHTNESS;
    }
    else if (mode_bright == C_MODE_LOW_BRIGHT)
    {
        C_DISPLAY_ON = C_LOW_BRIGHTNESS;
    }
    if (display_status == C_DISPLAY_ST_NOT_BUSSY)
    {
        SwitchScreenOff(ledmatrix);
        ledmatrix.drawBitmap(0, C_TEXT_CHAR_OFFSET_Y, Frame_Batt_Low_1, C_TEXT_BITMAP_WEIGHT, C_TEXT_CHAR_HEIGHT, C_DISPLAY_ON);
        ledmatrix.drawBitmap(8, C_TEXT_CHAR_OFFSET_Y, Frame_Batt_Low_2, C_TEXT_BITMAP_WEIGHT, C_TEXT_CHAR_HEIGHT, C_DISPLAY_ON);
        timer_blink.set(C_DELAY_BLINK_LOW_BATTERY);
        display_status = C_DISPLAY_ST_BUSSY;
        i = 0;
    }
    else if (display_status == C_DISPLAY_ST_BUSSY)
    {
        if (timer_blink.poll() != C_TIMER_NOT_EXPIRED)
        {
            if (blink_state == blink_OFF)
            {
                i++;
                if (i == C_NUMBER_BLINK_LOW_BATTERY)
                {
                    display_status = C_DISPLAY_ST_NOT_BUSSY;
                }
                else
                {
                    blink_state = blink_ON;
                    ledmatrix.drawBitmap(0, C_TEXT_CHAR_OFFSET_Y, Frame_Batt_Low_1, C_TEXT_BITMAP_WEIGHT, C_TEXT_CHAR_HEIGHT, C_DISPLAY_ON);
                    ledmatrix.drawBitmap(8, C_TEXT_CHAR_OFFSET_Y, Frame_Batt_Low_2, C_TEXT_BITMAP_WEIGHT, C_TEXT_CHAR_HEIGHT, C_DISPLAY_ON);
                    timer_blink.set(C_DELAY_BLINK_LOW_BATTERY);
                }
            }
            else if (blink_state == blink_ON)
            {
                blink_state = blink_OFF;
                SwitchScreenOff(ledmatrix);
                timer_blink.set(C_DELAY_BLINK_LOW_BATTERY);
            }
        }
    }
    return display_status;
}
/**
 * @brief Animacion de Error
 * 
 */
void DisplayError(Adafruit_IS31FL3731_Wing ledmatrix)
{
    C_DISPLAY_ON = C_HIGH_BRIGHTNESS;
    ledmatrix.drawBitmap(0, C_TEXT_CHAR_OFFSET_Y, Frame_Error_1, C_TEXT_BITMAP_WEIGHT, C_TEXT_CHAR_HEIGHT, C_DISPLAY_ON);
    ledmatrix.drawBitmap(8, C_TEXT_CHAR_OFFSET_Y, Frame_Error_2, C_TEXT_BITMAP_WEIGHT, C_TEXT_CHAR_HEIGHT, C_DISPLAY_ON);
}

/**
 * @brief Encender toda la pantalla con una cantidad de brillo.
 * 
 */
void ScreenON(Adafruit_IS31FL3731_Wing ledmatrix, int16_t bright)
{
    ledmatrix.fillRect(0, C_TEXT_CHAR_OFFSET_Y, C_DISPLAY_WIDTH, C_TEXT_CHAR_OFFSET_Y + 6, bright);
}