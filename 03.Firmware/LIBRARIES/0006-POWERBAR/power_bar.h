/**
 * @file power_bar.h
 * @author Javi (Javier@musotoku.com)
 * @brief Esta libreria contiene las funciones y cosntantes encesarias para controlar la ultima fila de leds de matriz de leds de 15x7, con el funcion de reproducir el funcionamiento de una barra de potencia. Esta barra iluminara mas leds o menos en funcion de la potencia que se este suminsitrando por parte de la bateria a la maquina.
 * @version 1
 * @date 2021-02-24
 *
 * @copyright Copyright (c) 2021
 *
 */

// #include <Adafruit_IS31FL3731.h>
// #include <OLED_display.h>
// #include <MilliTimer.h>
const int16_t MAX_POWER_DISPLAYED = 2500;
const int16_t LEDS_IN_POWERBAR = 15;
const uint16_t x_LED_WORK = 1;

const int16_t power_by_led = MAX_POWER_DISPLAYED / LEDS_IN_POWERBAR;

MilliTimer refresh_timer;

/**
 * @brief Esta funcion se encarga de encender y apagar los leds necesario en la barra de potencia.
 *
 * @param leds
 * @param OLED_display
 */
void PowerBar(int16_t leds)
{
    int xsize_rect_powerbar = C_DISPLAY_WIDTH;
    int ysize_rect_powerbar = 18;

    int cursor_x = 0;

    leds = constrain(leds, 0, LEDS_IN_POWERBAR);

    if (leds == 0)
    {
        OLED_display.fillRect(4, C_PWBAR_Y_AXE, C_DISPLAY_WIDTH, C_DISPLAY_HEIGHT, BLACK);
        OLED_display.display();
    }
    else
    {
        for (int16_t i = 0; i <= leds; i++)
        {
            OLED_display.fillRect(cursor_x, C_PWBAR_Y_AXE, 2, 7, WHITE);
            cursor_x += 4;
        }
        OLED_display.fillRect(cursor_x, C_PWBAR_Y_AXE, C_DISPLAY_WIDTH, ysize_rect_powerbar, BLACK);
    }
    OLED_display.display();
}
/**
 * @brief Actualiza la cantidad de leds encendidos en la barra de potencia segun el parametro de entrada "power_sample"
 *
 * @param power_sample
 * @param OLED_display
 */
void UpdatePowerBar(int16_t power_sample)
{
    static int16_t high_sample = 0;

    if (high_sample < power_sample)
    {
        high_sample = power_sample;
    }

    // if (refresh_timer.poll(25) != C_TIMER_NOT_EXPIRED)
    //{
    uint16_t num_leds = (high_sample + (power_by_led / 2)) / power_by_led;
    PowerBar(num_leds);
    high_sample = 0;
    //}
}
/**
 * @brief  Funcion que permite encender y apagar el boton que representa el estado de la salida del voltage.
 *
 * @param state_led
 * @param OLED_display
 */
void LedWork(bool state_led)
{
    if (state_led == true)
    {
        OLED_display.fillRect(0, C_PWBAR_Y_AXE, 2, 7, WHITE);
    }
    else if (state_led == false)
    {
        OLED_display.fillRect(0, C_PWBAR_Y_AXE, 2, 7, BLACK);
    }
    OLED_display.display();
}