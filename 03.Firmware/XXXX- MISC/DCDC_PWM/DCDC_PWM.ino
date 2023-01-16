/**
 * @file DCDC_PWM.ino
 * @author Javi (Javier@musotoku.com)
 * @brief 
 * @version 0.1
 * @date 2022-12-19
 * 
 * @copyright Copyright (c) 2022
 * 
 */
#define PWM_FREQ 250000 // 1MHz
#define PWM_PERIOD 1000000 / PWM_FREQ

const int16_t dutycycle = 25;
uint32_t period = PWM_PERIOD * dutycycle / 100;
void setup()
{
    pinMode(5, OUTPUT);
}

void loop()
{
    digitalWrite(5, HIGH);
    delayMicroseconds(1);
    digitalWrite(5, LOW);
    delayMicroseconds(3);
}