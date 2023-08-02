/**
 * @file Dpad.ino
 * @author Javi (Javier@musotoku.com)
 * @brief Este archivo contiene las funciones realcionadas con el periferico del Directional Pad.
 * @version 6
 * @date 2021-3-16
 *
 * @copyright Copyright (c) 2020
 *
 */

const int16_t C_LP_CENTER = 0x01;
// const int16_t C_LP_RIGHT = 0x02;
// const int16_t C_LP_LEFT = 0x03;
const int16_t C_LP_UP = 0x04;
const int16_t C_LP_DOWN = 0x05;
const int16_t C_CLICK_CENTER = 0x06;
// const int16_t C_CLICK_RIGHT = 0x07;
// const int16_t C_CLICK_LEFT = 0x08;
const int16_t C_CLICK_UP = 0x09;
const int16_t C_CLICK_DOWN = 0x0A;
const int16_t C_NONE_EVENT = 0x0B;

const int16_t C_PIN_BUTT_UP = 3;
const int16_t C_PIN_BUTT_DOWN = 4;
// const int16_t C_PIN_BUTT_RIGHT = 6;
// const int16_t C_PIN_BUTT_LEFT = 7;
const int16_t C_PIN_BUTT_CENTER = 7;

const int16_t C_TIMER_DEBOUNCE = 50;
const int32_t C_TIMER_LONGPRESS = 1000 + C_TIMER_DEBOUNCE;
const int32_t C_TIMER_LONGPRESS_LOOP = 500;

const bool button_pressed = LOW;
const bool button_not_pressed = HIGH;
// #include <MilliTimer.h>
/**
 * @brief Esta funcion lee y detecta las pulsaciones cortas(CLICKs) y largas (LP) de 5 botones. Incluye protecciones antirebotes.
 *
 * @return int16_t
 */
int16_t ReadDirPad(bool reset = false)
{
    int16_t event = C_NONE_EVENT;

    // Flags debouncing.
    static bool up_debouncing = false;
    static bool down_debouncing = false;
    static bool center_debouncing = false;

    // Flag preEvent.
    static bool up_preEvent = false;
    static bool down_preEvent = false;
    static bool center_preEvent = false;

    // Flags del modo rapido.
    static bool up_long_press_flag = false;
    static bool down_long_press_flag = false;
    static bool center_long_press_flag = false;

    // Timers filtro antirebote.
    static MilliTimer timer_up_debounce;
    static MilliTimer timer_down_debounce;
    static MilliTimer timer_center_debounce;

    // Timers Long press detect
    static MilliTimer timer_up_longpress;
    static MilliTimer timer_down_longpress;
    static MilliTimer timer_center_longpress;

    // Estado actual del boton.
    static bool up_butt_state = button_not_pressed;
    static bool down_butt_state = button_not_pressed;
    static bool center_butt_state = button_not_pressed;

    // Estado anterior del boton.
    static bool up_butt_last_state = button_not_pressed;
    static bool down_butt_last_state = button_not_pressed;
    static bool center_butt_last_state = button_not_pressed;
    if (reset)
    {
        up_butt_last_state = button_not_pressed;
        down_butt_last_state = button_not_pressed;
        center_butt_last_state = button_not_pressed;
        up_butt_state = button_not_pressed;
        down_butt_state = button_not_pressed;
        center_butt_state = button_not_pressed;
        up_long_press_flag = false;
        down_long_press_flag = false;
        center_long_press_flag = false;
        up_preEvent = false;
        down_preEvent = false;
        center_preEvent = false;
        up_debouncing = false;
        down_debouncing = false;
        center_debouncing = false;
        return C_NONE_EVENT;
    }
    else
    {
        /* code */

        /**
         *  UP BUTTON
         */
        if (up_debouncing == true)
        {
            if (timer_up_debounce.poll())
            {
                up_debouncing = false;
            }
        }
        else if (up_debouncing == false)
        {
            up_butt_state = digitalRead(C_PIN_BUTT_UP);
            if (up_butt_state == button_pressed)
            {
                if (up_butt_last_state == button_not_pressed)
                {
                    timer_up_debounce.set(C_TIMER_DEBOUNCE);
                    up_debouncing = true;
                    timer_up_longpress.set(C_TIMER_LONGPRESS);
                }
                else
                {
                    if (timer_up_longpress.poll())
                    {
                        up_long_press_flag = true;
                        event = C_LP_UP;
#ifdef SERIAL_DEBUG
                        Serial5.println("C_LP_UP");
#endif
                        timer_up_longpress.set(C_TIMER_LONGPRESS_LOOP);
                    }
                }
            }
            else
            {
                if (up_butt_last_state == button_pressed)
                {
                    timer_up_debounce.set(C_TIMER_DEBOUNCE);
                    up_debouncing = true;
                    up_preEvent = true;
                }
                else if (up_preEvent == true)
                {
                    up_preEvent = false;
                    if (up_long_press_flag == false)
                    {
                        event = C_CLICK_UP;
#ifdef SERIAL_DEBUG
                        Serial5.println("C_CLICK_UP");
#endif
                    }
                    else
                    {
                        up_long_press_flag = false;
                    }
                }
            }
            up_butt_last_state = up_butt_state;
        }

        /**
         *  DOWN BUTTON
         */

        if (down_debouncing == true)
        {
            if (timer_down_debounce.poll())
            {
                down_debouncing = false;
            }
        }
        else if (down_debouncing == false)
        {
            down_butt_state = digitalRead(C_PIN_BUTT_DOWN);
            if (down_butt_state == button_pressed)
            {
                if (down_butt_last_state == button_not_pressed)
                {
                    timer_down_debounce.set(C_TIMER_DEBOUNCE);
                    down_debouncing = true;
                    timer_down_longpress.set(C_TIMER_LONGPRESS);
                }
                else
                {
                    if (timer_down_longpress.poll())
                    {
                        down_long_press_flag = true;
                        event = C_LP_DOWN;
#ifdef SERIAL_DEBUG
                        Serial5.println("C_LP_DOWN");
#endif
                        timer_down_longpress.set(C_TIMER_LONGPRESS_LOOP);
                    }
                }
            }
            else
            {
                if (down_butt_last_state == button_pressed)
                {
                    timer_down_debounce.set(C_TIMER_DEBOUNCE);
                    down_debouncing = true;
                    down_preEvent = true;
                }
                else if (down_preEvent == true)
                {
                    down_preEvent = false;
                    if (down_long_press_flag == false)
                    {
                        event = C_CLICK_DOWN;
#ifdef SERIAL_DEBUG
                        Serial5.println("C_CLICK_DOWN");
#endif
                    }
                    else
                    {
                        down_long_press_flag = false;
                    }
                }
            }
            down_butt_last_state = down_butt_state;
        }

        /**
         *  CENTER BUTTON
         */
        if (center_debouncing == true)
        {
            if (timer_center_debounce.poll())
            {
                center_debouncing = false;
            }
        }
        else if (center_debouncing == false)
        {
            center_butt_state = digitalRead(C_PIN_BUTT_CENTER);
            if (center_butt_state == button_pressed)
            {
                if (center_butt_last_state == button_not_pressed)
                {
                    timer_center_debounce.set(C_TIMER_DEBOUNCE);
                    center_debouncing = true;
                    timer_center_longpress.set(C_TIMER_LONGPRESS);
                }
                else
                {
                    if (timer_center_longpress.poll())
                    {
                        center_long_press_flag = true;
                        event = C_LP_CENTER;
#ifdef SERIAL_DEBUG
                        Serial5.println("C_LP_CENTER");
#endif
                        timer_center_longpress.set(C_TIMER_LONGPRESS_LOOP);
                    }
                }
            }
            else
            {
                if (center_butt_last_state == button_pressed)
                {
                    timer_center_debounce.set(C_TIMER_DEBOUNCE);
                    center_debouncing = true;
                    center_preEvent = true;
                }
                else if (center_preEvent == true)
                {
                    center_preEvent = false;
                    if (center_long_press_flag == false)
                    {
                        event = C_CLICK_CENTER;
#ifdef SERIAL_DEBUG
                        Serial5.println("C_CLICK_CENTER");
#endif
                    }
                    else
                    {
                        center_long_press_flag = false;
                    }
                }
            }
            center_butt_last_state = center_butt_state;
        }

        return event;
    }
}
