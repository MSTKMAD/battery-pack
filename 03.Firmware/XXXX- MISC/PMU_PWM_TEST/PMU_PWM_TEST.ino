/**
 * @file PWM_TEST.ino
 * @author your name (you@domain.com)
 * @brief
 * @version 0.1
 * @date 2024-04-15
 *
 * @copyright Copyright (c) 2024
 *
 */
// Note: Uses pin 13 as the LED pin; may need changing for other boards

#include "MilliTimer.h"
uint16_t C_PIN_OP_SWITCH = 13;
#include <DCDC.h>
// #include <SAMD21turboPWM.h>
#include <Dpad.h>
#include <display.h>

const uint16_t C_DUTY = 0x01;
const uint16_t C_FREQ = 0x02;
const uint16_t C_VOLT = 0x03;

dcdc_controler DCDC(11);
// TurboPWM pwm;
int16_t button_event;
int16_t steps_LUT[] = {500, 100, 50, 33, 25, 20, 16, 12};
int16_t freq_LUT[] = {1, 5, 10, 15, 20, 25, 30, 40};

int16_t duty = 500;
int prescaler = 1;
int steps = steps_LUT[0];
int freq_index = 3;
int volt = 45;

int16_t menu = C_VOLT;
void setup()
{
    pinMode(1, OUTPUT);
    digitalWrite(1, HIGH);
    pinMode(8, INPUT_PULLUP);
    pinMode(13, OUTPUT);
    digitalWrite(13, LOW);
    pwm.setClockDivider(48, false); // Main clock divided by 200 => 240KHz
    pwm.timer(2, 1, steps, true);   // Use timer 2 for pin 13, divide clock by 4, resolution 60000, dual-slope PWM
    pwm.analogWrite(13, duty);      // PWM frequency is now 0.5Hz, dutycycle is 500 / 1000 * 100% = 50%
    pinMode(C_PIN_BUTT_CENTER, INPUT_PULLUP);
    pinMode(C_PIN_BUTT_UP, INPUT_PULLUP);
    pinMode(C_PIN_BUTT_DOWN, INPUT_PULLUP);
    initDisplay();
    DCDC.SetVoltage(volt, C_NON_BOOST_MODE);
    DisplayVolt(volt);
}

void loop()
{
    button_event = ReadDirPad();

    switch (menu)
    {
    case C_DUTY:
        /* code */
        if (button_event == C_CLICK_DOWN)
        {
            duty -= 10;
            duty = constrain(duty, 20, 900);
            DisplayCap(duty / 10);
            pwm.analogWrite(13, duty); // PWM frequency is now 0.5Hz, dutycycle is 500 / 1000 * 100% = 50%
        }
        else if (button_event == C_CLICK_UP)
        {
            duty += 10;
            duty = constrain(duty, 20, 900);
            DisplayCap(duty / 10);
            pwm.analogWrite(13, duty); // PWM frequency is now 0.5Hz, dutycycle is 500 / 1000 * 100% = 50%
        }
        else if (button_event == C_LP_CENTER)
        {
            menu = C_FREQ;
            OLED_display.clearDisplay();
            OLED_display.setTextSize(1);
            OLED_display.setCursor(8, 12);
            OLED_display.print("FREQ");
            OLED_display.drawRect(0, 0, 64, 32, WHITE);
            OLED_display.display();
            delay(1000);
            DisplayCap(freq_LUT[freq_index]);
            pwm.enable(2, false);
            pwm.timer(2, 1, steps_LUT[freq_index], true); // Use timer 2 for pin 13, divide clock by 4, resolution 60000, dual-slope PWM
            pwm.analogWrite(13, duty);
            pwm.enable(2, true);
        }

        break;
    case C_FREQ:
        if (button_event == C_CLICK_DOWN)
        {
            if (freq_index >= 1)
            {
                freq_index--;
            }
            DisplayCap(freq_LUT[freq_index]);
            pwm.enable(2, false);
            pwm.timer(2, 1, steps_LUT[freq_index], true); // Use timer 2 for pin 13, divide clock by 4, resolution 60000, dual-slope PWM
            pwm.analogWrite(13, duty);
            pwm.enable(2, true);
        }
        else if (button_event == C_CLICK_UP)
        {
            if (freq_index <= 5)
            {
                freq_index++;
            }
            DisplayCap(freq_LUT[freq_index]);
            pwm.enable(2, false);
            pwm.timer(2, 1, steps_LUT[freq_index], true); // Use timer 2 for pin 13, divide clock by 4, resolution 60000, dual-slope PWM
            pwm.analogWrite(13, duty);
            pwm.enable(2, true);
        }
        else if (button_event == C_LP_CENTER)
        {
            menu = C_VOLT;
            OLED_display.clearDisplay();
            OLED_display.setTextSize(1);
            OLED_display.setCursor(8, 12);
            OLED_display.print("VOLT");
            OLED_display.drawRect(0, 0, 64, 32, WHITE);
            OLED_display.display();
            delay(1000);
            DisplayVolt(volt);
            DCDC.SetVoltage(volt, C_NON_BOOST_MODE);
        }
        // PWM frequency is now 0.5Hz, dutycycle is 500 / 1000 * 100% = 50%
        break;
    case C_VOLT:
        /* code */
        if (button_event == C_CLICK_DOWN)
        {
            volt -= 5;
            volt = constrain(volt, 45, 120);
            DisplayVolt(volt);

            DCDC.SetVoltage(volt, C_NON_BOOST_MODE);
        }
        else if (button_event == C_CLICK_UP)
        {
            volt += 5;
            volt = constrain(volt, 45, 120);
            DisplayVolt(volt);

            DCDC.SetVoltage(volt, C_NON_BOOST_MODE);
        }
        else if (button_event == C_LP_CENTER)
        {

            menu = C_DUTY;
            OLED_display.clearDisplay();
            OLED_display.setTextSize(1);
            OLED_display.setCursor(8, 12);
            OLED_display.print("DUTY");
            OLED_display.drawRect(0, 0, 64, 32, WHITE);
            OLED_display.display();
            delay(1000);
            duty = constrain(duty, 20, 900);
            DisplayCap(duty / 10);
            pwm.analogWrite(13, duty); // PWM frequency is now 0.5Hz, dutycycle is 500 / 1000 * 100% = 50%
        }

        break;

    default:
        break;
    }
}