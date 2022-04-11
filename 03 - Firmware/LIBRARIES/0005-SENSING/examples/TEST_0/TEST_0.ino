/**
 * @file TEST_0.ino
 * @author Javi (Javier@musotoku.com)
 * @brief 
 * @version 1
 * @date 2020-12-10
 * 
 * @copyright Copyright (c) 2020
 * 
 */

#include <HealthMonitor.h>
#include <MilliTimer.h>

#define DAC_PIN A0

HealthMonitor sensor_1(3000, C_RISE_COUNT, C_FALL_COUNT, C_COUNTER_LIMIT);

uint16_t sample = 0;
uint16_t count = 0;
uint16_t dac_output;

MilliTimer trigger_timer;

void setup()
{
    pinMode(A2, OUTPUT);
    Serial.begin(9600);
    while (!Serial)
        ;
    analogWriteResolution(10); // Set analog out resolution to max, 10-bits

    sensor_1.setCounter(0);
}

void loop()
{
    sample = sensor_1.getSample(A1);
    Serial.print("Sample:");
    Serial.print(sample);
    if (sensor_1.check(sample) == true)
    {
        count = sensor_1.getCounter();
        Serial.print("    Conter:");
        Serial.print(count);
        dac_output = (int)(count * 1023.0 / 100.0);
        analogWrite(DAC_PIN, dac_output); // 0= 0V, 1023=3.3V, 512=1.65V, etc.
        Serial.println("TRIGGERED");
        trigger_timer.set(10);
        sensor_1.setCounter(0);
        digitalWrite(A2, HIGH);
    }
    else
    {
        count = sensor_1.getCounter();
        Serial.print("    Conter:");
        Serial.println(count);
        dac_output = (int)(count * 1023.0 / 100.0);
        analogWrite(DAC_PIN, dac_output); // 0= 0V, 1023=3.3V, 512=1.65V, etc.
    }

    if (trigger_timer.poll() != 0)
    {
        digitalWrite(A2, LOW);
    }

    delay(10);
}