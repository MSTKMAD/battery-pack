/**
 * @file DCDC.h
 * @author Javi (Javier@musotoku.com)
 * @brief Libreria que describe la clase del DCDC.
 * @version 2
 * @date 2020-12-18
 *
 * @copyright Copyright (c) 2020
 *
 */
#include <Wire.h>
#include <SAMD21turboPWM.h>

#ifndef MAX_VOLTAGE
#define MAX_VOLTAGE 120
#endif
#ifndef MIN_VOLTAGE
#define MIN_VOLTAGE 50
#endif
const uint16_t LenDCDCvalues = 121;
const bool C_BOOST_MODE = true;
const bool C_NON_BOOST_MODE = false;

const uint16_t C_VDAC_MIN = 1435;                          // mv del DAC para dar a la salida el MIN_VOLTAGE
const uint16_t C_VDAC_MAX = 140;                           // mv del DAC para dar a la salida el MAX_VOLTAGE
const uint16_t C_DAC_RESOLUTON = 1024;                     // Fondo de escala del DAC
const uint16_t C_DAC_MIN_COUNT = C_VDAC_MIN * 1024 / 3000; // cuentas dac para dar el Minimo voltaje permitido (5v)
const uint16_t C_DAC_MAX_COUNT = C_VDAC_MAX * 1024 / 3000; // cuentas dac para dar el Minimo voltaje permitido (5v)

const uint16_t C_ARRAY_DUTY[] = {62, 60, 58, 56, 54, 52, 50, 48, 46, 44, 42, 40, 38, 36, 34, 32, 30, 28, 26, 24, 22, 20, 18, 16, 14};
const uint16_t C_PIN_DAC = A0;
TurboPWM pwm;
class dcdc_controler
{
private:
public:
    int16_t pin_enable;
    int16_t dac_count;
    int16_t duty;

    /**
     * @brief Construct a new dcdc controler object
     *
     * @param pin Pin conectado al enable del DCDC.
     */
    dcdc_controler(int16_t pin)
    {
        pin_enable = pin;
        pinMode(pin_enable, OUTPUT);
        digitalWrite(pin_enable, LOW);
        pwm.setClockDivider(48, false); // Main clock divided by 200 => 240KHz
        pwm.timer(2, 1, 50, false);     // Use timer 2 for pin 13, divide clock by 4, resolution 60000, dual-slope PWM
    }

    /**
     * @brief Set the Voltage object
     *
     * @param volt
     * @param mode
     */
    void SetVoltage(int volt, bool mode)
    {
        if (volt >= 50)
        {
            /* code */

            if (mode == C_BOOST_MODE)
            {
                volt += 3;
            }
            volt = constrain(volt, 50, 120);
            dac_count = C_DAC_MIN_COUNT - (((volt - 50) * (C_DAC_MIN_COUNT - C_DAC_MAX_COUNT)) / (120 - 50));
            analogWrite(C_PIN_DAC, dac_count);
        }
        else if (volt < 50)
        {
            dac_count = C_DAC_MIN_COUNT - (((60 - 50) * (C_DAC_MIN_COUNT - C_DAC_MAX_COUNT)) / (120 - 50));
            analogWrite(C_PIN_DAC, dac_count);
            duty = C_ARRAY_DUTY[volt - MIN_VOLTAGE];
            pwm.analogWrite(C_PIN_OP_SWITCH, duty * 10); // PWM frequency is now 0.5Hz, dutycycle is 500 / 1000 * 100% = 50%
        }
    }
    /**
     * @brief Habilita la activacion del DCDC.
     *
     * @param enable
     */
    void EnableDCDC(bool enable = LOW)
    {
        digitalWrite(pin_enable, enable);
    }
};