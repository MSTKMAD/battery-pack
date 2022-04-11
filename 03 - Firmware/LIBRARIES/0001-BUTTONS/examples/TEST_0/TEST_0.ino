/**
 * @file TEST_0.ino
 * @author Javi (Javier@musotoku.com)
 * @brief Este test comprobarta si se detectan lo eventos de del DPAD.
 * @version 4
 * @date 2021-3-16
 * 
 * @copyright Copyright (c) 2020
 * 
 */
#include <MilliTimer.h>
#include <Dpad.h>

void setup()
{
    Serial.begin(9600);
    while (!Serial)
    {
        ;
    }

    int32_t computing_time = 0;
    int16_t dpad_event;
    int time_0 = 0;
    int time_1 = 0;
    //pinMode(3, OUTPUT);
    pinMode(C_PIN_BUTT_CENTER, INPUT_PULLUP);
    pinMode(C_PIN_BUTT_UP, INPUT_PULLUP);
    pinMode(C_PIN_BUTT_DOWN, INPUT_PULLUP);
    //pinMode(C_PIN_BUTT_RIGHT, INPUT_PULLUP);
    //pinMode(C_PIN_BUTT_LEFT, INPUT_PULLUP);

   // digitalWrite(3, LOW);
    Serial.println("Test 0.Presione los botones del Dpad. En casa activacion se mostrara el evento detectado y el tiempo de computo de la funcion.");

    while (1)
    {
        time_0 = micros();
        dpad_event = ReadDirPad();
        time_1 = micros();
        computing_time = time_1 - time_0;
        //digitalWrite(3, LOW);
        if (dpad_event == C_CLICK_CENTER)
        {
            Serial.print("CENTER CLICK Detected.");
            Serial.print("Computing Time:");
            Serial.print(computing_time);
            Serial.println("us.");
        }
        // else if (dpad_event == C_CLICK_RIGHT)
        // {
        //     Serial.print("RIGHT CLICK Detected.");
        //     Serial.print("Computing Time:");
        //     Serial.print(computing_time);
        //     Serial.println("us.");
        // }
        // else if (dpad_event == C_CLICK_LEFT)
        // {
        //     Serial.print("LEFT CLICK Detected.");
        //     Serial.print("Computing Time:");
        //     Serial.print(computing_time);
        //     Serial.println("us.");
        // }
        else if (dpad_event == C_CLICK_UP)
        {
            //digitalWrite(3, HIGH);
            Serial.print("UP CLICK Detected.");
            Serial.print("Computing Time:");
            Serial.print(computing_time);
            Serial.println("us.");
        }
        else if (dpad_event == C_CLICK_DOWN)
        {
            Serial.print("DOWN CLICK Detected.");
            Serial.print("Computing Time:");
            Serial.print(computing_time);
            Serial.println("us.");
        }
        else if (dpad_event == C_LP_CENTER)
        {
            Serial.print("CENTER LONG PRESS Detected.");
            Serial.print("Computing Time:");
            Serial.print(computing_time);
            Serial.println("us.");
        }
        // else if (dpad_event == C_LP_RIGHT)
        // {
        //     Serial.print("RIGHT LONG PRESS Detected.");
        //     Serial.print("Computing Time:");
        //     Serial.print(computing_time);
        //     Serial.println("us.");
        // }
        // else if (dpad_event == C_LP_LEFT)
        // {
        //     Serial.print("LEFT LONG PRESS Detected.");
        //     Serial.print("Computing Time:");
        //     Serial.print(computing_time);
        //     Serial.println("us.");
        // }
        else if (dpad_event == C_LP_UP)
        {
            //digitalWrite(3, HIGH);
            Serial.print("UP LONG PRESS Detected.");
            Serial.print("Computing Time:");
            Serial.print(computing_time);
            Serial.println("us.");
        }
        else if (dpad_event == C_LP_DOWN)
        {
            Serial.print("DOWN LONG PRESS Detected.");
            Serial.print("Computing Time:");
            Serial.print(computing_time);
            Serial.println("us.");
        }
        else if (dpad_event == C_NONE_EVENT)
        {
            //Serial.println("NONE");
        }
        else
        {
            Serial.println("Error with the Dpad event Detection.");
        }
    }
}

void loop()
{
}