/**
 * @file OLED_display.h
 * @author Javi (Javier@musotoku.com)
 * @brief Libreria MUSOTOKU para OLED_display 15x7.
 * @version 4
 * @date 2020-8-4
 *
 * @copyright Copyright (c) 2020
 *
 */
#include "bitmaps.h"
#include <Wire.h>

#ifndef MAX_VOLTAGE
#define MAX_VOLTAGE 120
#endif
#ifndef MIN_VOLTAGE
#define MIN_VOLTAGE 50
#endif
// #include <Adafruit_GFX.h>
// #include <Adafruit_IS31FL3731.h>

#include <Adafruit_GFX.h>
// #include <batt_Adafruit_IS31FL3731.h>
#include <Adafruit_SSD1306_049.h>
// #include <MilliTimer.h>
Adafruit_SSD1306 OLED_display(0);
#define X_SIZE 64
#define Y_SiZE 32
#define MAX_LEDS X_SIZE / 4

const int16_t C_PWBAR_Y_AXE = 30;       // Posicion de la barra de potencia.
const int16_t C_TEXT_CHAR_OFFSET_Y = 0; // Altura de inicio de escritura de los caracteres.

const int16_t C_HIGH_BRIGHTNESS = 20; // Nivel de brillo alto de los Leds.
const int16_t C_LOW_BRIGHTNESS = 5;   // Nivel de brillo alto de los Leds.
int16_t C_DISPLAY_ON = WHITE;         // Encendido del led.
const int16_t C_DISPLAY_OFF = BLACK;  // Apagado del LED.

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
 * @brief Construct a new init Display object
 *
 */
void initDisplay()
{
    OLED_display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
    OLED_display.clearDisplay();
    OLED_display.display();
    OLED_display.setTextColor(WHITE); // Color por defecto del texto.
}
/**
 * @brief
 *
 */
void DisplayLogo()
{
    OLED_display.clearDisplay();
    OLED_display.drawBitmap(0, 0, MusotokuLogo1, 8, 32, WHITE, BLACK);
    OLED_display.drawBitmap(8, 0, MusotokuLogo2, 8, 32, WHITE, BLACK);
    OLED_display.drawBitmap(16, 0, MusotokuLogo3, 8, 32, WHITE, BLACK);
    OLED_display.drawBitmap(24, 0, MusotokuLogo4, 8, 32, WHITE, BLACK);
    OLED_display.drawBitmap(32, 0, MusotokuLogo5, 8, 32, WHITE, BLACK);
    OLED_display.drawBitmap(40, 0, MusotokuLogo6, 8, 32, WHITE, BLACK);
    OLED_display.drawBitmap(48, 0, MusotokuLogo7, 8, 32, WHITE, BLACK);
    OLED_display.drawBitmap(56, 0, MusotokuLogo8, 8, 32, WHITE, BLACK);
    OLED_display.display();
}
/**
 * @brief Apagado de la pantalla.
 *
 */
void SwitchScreenOff()
{
    OLED_display.clearDisplay();
    OLED_display.display();
}
/**
 * @brief Encender toda la pantalla con una cantidad de brillo.
 *
 */
void ScreenON()
{
    OLED_display.fillRect(0, 0, C_DISPLAY_WIDTH, C_DISPLAY_HEIGHT, WHITE);
    OLED_display.display();
}
/**
 * @brief
 *
 */
void DisplayEndScreen()
{

    OLED_display.clearDisplay();
    OLED_display.setTextSize(2);
    OLED_display.setCursor(0, 0);
    OLED_display.print("GOOD");
    OLED_display.setCursor(0, 16);
    OLED_display.print("JOB");
    OLED_display.display();
}

/**
 * @brief Esta funcion imprime la pantalla de voltaje, que muestra le voltaje al que se encuentra la salida de la bateria, con un decimal.
 *
 * @param number
 * @param OLED_display
 */
void DisplayVolt(int16_t number)
{
    int intPart;
    int decPart;
    int fracPart;
    int cursor;

    // Config the text
    OLED_display.setTextSize(4);
    OLED_display.setTextColor(WHITE);

    OLED_display.fillRect(0, 0, X_SIZE, 30, BLACK); // clears the screen and buffer
    intPart = number / 10;
    decPart = intPart / 10;
    fracPart = number - 10 * intPart;

    if (number >= 100)
    {
        OLED_display.setCursor(-4, 0);
        OLED_display.print(intPart);
    }
    else
    {
        OLED_display.setCursor(15, 0);
        OLED_display.print(intPart);
    }
    OLED_display.fillRect(38, 25, 3, 3, WHITE);
    cursor = 44; //
    OLED_display.setCursor(cursor, 0);
    OLED_display.print(fracPart);
    OLED_display.display();
}

/**
 * @brief Esta fncion muestra la pantalla de porcentaje de capacidad de bateria. si la capacidad es 100% muestra la pantalla FULL.
 *
 * @param capacity
 * @param OLED_display
 */
void DisplayCap(int capacity)
{
    OLED_display.clearDisplay();
    capacity = constrain(capacity, 0, 100);
    // Config the text
    OLED_display.setTextSize(3);
    OLED_display.setTextColor(WHITE);

    OLED_display.fillRect(0, 0, X_SIZE, 28, BLACK); // clears the screen and buffer

    if (capacity < 10)
    {
        OLED_display.setCursor(31, 4);
    }
    else if (capacity < 20)
    {
        OLED_display.setCursor(18, 4);
    }
    else if (capacity < 100)
    {
        OLED_display.setCursor(13, 4);
    }
    else
    {
        OLED_display.setCursor(0, 4);
    }

    OLED_display.print(capacity);
    OLED_display.drawBitmap(50, 0, BattCap1, 8, 32, WHITE, BLACK);
    OLED_display.drawBitmap(58, 0, BattCap2, 8, 32, WHITE, BLACK);

    OLED_display.fillRect(54, 6, 6, 18, WHITE);
    OLED_display.fillRect(54, 6, 6, 18 - capacity * 10 / 55, BLACK);
    OLED_display.display();
}

/**
 * @brief Animacion Bateria Baja.
 *
 */

void DisplayLowBattery()
{
#ifdef SERIAL_DEBUG
    Serial5.printf("Display LowBatt\n");
#endif
    OLED_display.clearDisplay();
    OLED_display.setTextSize(2);
    OLED_display.setCursor(0, 0);
    OLED_display.print("Low");
    OLED_display.setCursor(0, 16);
    OLED_display.print("Batt");
    OLED_display.display();
}
/**
 * @brief Bateria Agotada
 *
 */

void DisplayNoBattery()
{
#ifdef SERIAL_DEBUG
    Serial5.printf("Display Empty Battery\n");
#endif
    for (uint16_t i = 0; i < 3; i++)
    {
#ifdef SERIAL_DEBUG
        Serial5.printf("Display Empty Battery %d\n", i);
#endif
        OLED_display.clearDisplay();
        OLED_display.setTextSize(2);
        OLED_display.setCursor(0, 0);
        OLED_display.print("Empty");
        OLED_display.setCursor(0, 16);
        OLED_display.print("Batt");
        OLED_display.display();
        // delay(750);
        for (int i = 0; i < 75; i++)
        {
            delay(10);
            Watchdog.reset();
        }

        OLED_display.clearDisplay();
        OLED_display.drawBitmap(0, 0, batt_empty, 64, 32, BLACK, WHITE);
        OLED_display.display();
        // delay(750);
        for (int i = 0; i < 75; i++)
        {
            delay(10);
            Watchdog.reset();
        }
    }
}
/**
 * @brief
 *
 */
void DisplayArray(uint16_t array_to_display[], uint16_t size_array)
{
    char buffer[50];
    for (int i = 0; i < size_array; i++)
    {
        OLED_display.clearDisplay();
        OLED_display.setTextSize(2);
        OLED_display.setCursor(9, 0);
        sprintf(buffer, "%04X", i);
        OLED_display.print(buffer);
        OLED_display.setCursor(9, 16);
        sprintf(buffer, "%04X", array_to_display[i]);
        OLED_display.print(buffer);
        OLED_display.display();
        // delay(333);
        for (int j = 0; j < 33; j++)
        {
            Watchdog.reset();
            delay(10);
        }
    }
}
/**
 * @brief
 *
 * @param current
 * @param voltage
 * @param theory_voltage
 */
void DebugDisplay(uint32_t current, uint32_t current_raw, uint32_t voltage, uint32_t theory_voltage, uint32_t power)
{
    OLED_display.clearDisplay();
    OLED_display.setTextSize(1);
    OLED_display.setCursor(0, 0);
    OLED_display.printf("I %d/%d", current, current_raw);
    OLED_display.setCursor(0, 10);
    OLED_display.printf("V %d/%d", voltage, theory_voltage);
    OLED_display.setCursor(0, 20);
    OLED_display.printf("P %d", power);
    OLED_display.display();
}