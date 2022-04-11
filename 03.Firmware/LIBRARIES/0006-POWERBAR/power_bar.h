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

//#include <Adafruit_IS31FL3731.h>
//#include <display.h>
//#include <MilliTimer.h>
const int16_t MAX_POWER_DISPLAYED = 2500;
const int16_t LEDS_IN_POWERBAR = 15;
const uint16_t x_LED_WORK = 1;

const int16_t power_by_led = MAX_POWER_DISPLAYED / LEDS_IN_POWERBAR;

MilliTimer refresh_timer;

/**
 * @brief Esta funcion se encarga de encender y apagar los leds necesario en la barra de potencia.
 * 
 * @param leds 
 * @param ledmatrix 
 * @param mode_bright 
 */
void PowerBar(int16_t leds, Adafruit_IS31FL3731_Wing ledmatrix, bool mode_bright = C_MODE_HIGH_BRIGHT)
{
    leds = constrain(leds, 0, LEDS_IN_POWERBAR);
    if (mode_bright == C_MODE_HIGH_BRIGHT)
    {
        C_DISPLAY_ON = C_HIGH_BRIGHTNESS;
    }
    else if (mode_bright == C_MODE_LOW_BRIGHT)
    {
        C_DISPLAY_ON = C_LOW_BRIGHTNESS;
    }
    if (leds == 0)
    {
        ledmatrix.drawFastHLine(0, C_PWBAR_Y_AXE, LEDS_IN_POWERBAR, C_DISPLAY_OFF);
    }
    else
    {

        for (int16_t i = 0; i <= leds; i++)
        {
            ledmatrix.drawPixel(i, C_PWBAR_Y_AXE, C_DISPLAY_ON);
        }
        for (int16_t i = leds; i < LEDS_IN_POWERBAR; i++)
        {
            ledmatrix.drawPixel(i, C_PWBAR_Y_AXE, C_DISPLAY_OFF);
        }
    }
}
/**
 * @brief Actualiza la cantidad de leds encendidos en la barra de potencia segun el parametro de entrada "power_sample"
 * 
 * @param power_sample 
 * @param ledmatrix 
 * @param mode_bright_display 
 */
void UpdatePowerBar(int16_t power_sample, Adafruit_IS31FL3731_Wing ledmatrix, bool mode_bright_display)
{
    static int16_t high_sample = 0;

    if (high_sample < power_sample)
    {
        high_sample = power_sample;
    }

    if (refresh_timer.poll(25) != C_TIMER_NOT_EXPIRED)
    {
        uint16_t num_leds = (high_sample + (power_by_led / 2)) / power_by_led;
        PowerBar(num_leds, ledmatrix, mode_bright_display);
        high_sample = 0;
    }
}
/**
 * @brief  Funcion que permite encender y apagar el boton que representa el estado de la salida del voltage.
 * 
 * @param state_led 
 * @param ledmatrix 
 * @param mode_bright 
 */
void LedWork(bool state_led, Adafruit_IS31FL3731_Wing ledmatrix, bool mode_bright = C_MODE_HIGH_BRIGHT)
{
    if (mode_bright == C_MODE_HIGH_BRIGHT)
    {
        C_DISPLAY_ON = C_HIGH_BRIGHTNESS;
    }
    else if (mode_bright == C_MODE_LOW_BRIGHT)
    {
        C_DISPLAY_ON = C_LOW_BRIGHTNESS;
    }

    if (state_led == true)
    {
        for (int i = 0; i < x_LED_WORK; i++)
        {
            ledmatrix.drawPixel(i, C_PWBAR_Y_AXE, C_DISPLAY_ON);
        }
    }
    else if (state_led == false)
    {
        for (int i = 0; i < x_LED_WORK; i++)
        {
            ledmatrix.drawPixel(i, C_PWBAR_Y_AXE, C_DISPLAY_OFF);
        }
    }
}