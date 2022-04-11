/**
 * @file ErrorAlert.ino     
 * @author Javi (Javier@musotoku.com)
 * @brief Sketch para simular la activacion de la alerta del error.
 * @version 1
 * @date 2021-09-21
 * 
 * @copyright Copyright (c) 2021
 * 
 */
#include <MilliTimer.h>
#include <display.h>
#include <Buzzer.h>

// Inicializamos un objeto tipo LCD para la pantalla.
MilliTimer timer_display_error;
Adafruit_IS31FL3731_Wing ledmatrix = Adafruit_IS31FL3731_Wing();
bool display_error_status = C_DISPLAY_ST_NOT_BUSSY;
bool buzzer_error_state = C_BUZZER_NOT_BUSSY;
bool ok = true;
void setup()
{
    ledmatrix.begin();
    InitBuzzer();
    display_error_status = C_DISPLAY_ST_BUSSY;
    buzzer_error_state = C_BUZZER_BUSSY;
    timer_display_error.set(1000);
    playSound(C_SOUND_ERROR);
    SwitchScreenOff(ledmatrix);
    DisplayError(ledmatrix);
    while (ok == true)
    {
        if (display_error_status == C_DISPLAY_ST_BUSSY)
        {
            DisplayError(ledmatrix);

            if (timer_display_error.poll() != C_TIMER_NOT_EXPIRED)
            {
                display_error_status = C_DISPLAY_ST_NOT_BUSSY;
            }
        }
        if (buzzer_error_state == C_BUZZER_BUSSY)
        {
            buzzer_error_state = playSound(C_SOUND_ERROR);
        }

        if ((buzzer_error_state == C_BUZZER_NOT_BUSSY) && (display_error_status == C_DISPLAY_ST_NOT_BUSSY))
        {
            ok = false;
            SwitchScreenOff(ledmatrix);
        }
    }
}

void loop()
{
}