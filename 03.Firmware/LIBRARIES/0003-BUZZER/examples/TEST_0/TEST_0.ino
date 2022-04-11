/**
 * @file TEST_0.ino
 * @author Javi (Javier@musotoku.com)
 * @brief 
 * @version 1
 * @date 2021-02-09
 * 
 * @copyright Copyright (c) 2021
 * 
 */
#include <MilliTimer.h>
#include <Arduino.h>
#include <Buzzer.h>

uint32_t t0 = 0, t1 = 0, i = 0;
;
void setup()
{
    Serial.begin(9600);
    // while (!Serial)
    // {
    //     /* code */
    // }
    InitBuzzer(C_MODE_DEFAULT);
}

void loop()
{
    // t0 = micros();
    // playSound(C_SOUND_START);
    // t1 = micros();
    // Serial.println(t1 - t0);
    // delay(2000);
    // t0 = micros();
    // playSound(C_SOUND_END);
    // t1 = micros();
    // Serial.println(t1 - t0);
    // delay(2000);
    // t0 = micros();
    // playSound(C_SOUND_UP);
    // t1 = micros();
    // Serial.println(t1 - t0);
    // delay(2000);
    // t0 = micros();
    // playSound(C_SOUND_DOWN);
    // t1 = micros();
    // Serial.println(t1 - t0);
    // delay(2000);

    t0 = micros();
    if (playSound(i) == C_BUZZER_NOT_BUSSY)
    {
        delay(2000);
        i++;
        if (i == 11)
        {
            i = 0;
        }
    }

    t1 = micros();
    //Serial.println(t1 - t0);
}