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
// #define MAX_VOLTAGE 120
#endif
#ifndef MIN_VOLTAGE
// #define MIN_VOLTAGE 50
#endif
const uint16_t LenDCDCvalues = 121;
const bool C_BOOST_MODE = true;
const bool C_NON_BOOST_MODE = false;

const uint16_t C_VDAC_MIN = 1536;                          // mv del DAC para dar a la salida el MIN_VOLTAGE
const uint16_t C_VDAC_MAX = 140;                           // mv del DAC para dar a la salida el MAX_VOLTAGE
const uint16_t C_DAC_RESOLUTON = 1024;                     // Fondo de escala del DAC
const uint16_t C_DAC_MIN_COUNT = C_VDAC_MIN * 1024 / 3000; // cuentas dac para dar el Minimo voltaje permitido (5v)
const uint16_t C_DAC_MAX_COUNT = C_VDAC_MAX * 1024 / 3000; // cuentas dac para dar el Minimo voltaje permitido (5v)

const uint16_t C_ARRAY_DUTY[] = {38, 38, 34, 34, 30, 30, 26, 26, 22, 22};
const uint16_t C_ARRAY_VOLT_PWM[] = {45, 45, 45, 45, 45, 45, 45, 45, 45, 45};

const uint16_t C_PIN_DAC = A0;
TurboPWM pwm;
class dcdc_controler
{
private:
public:
    int16_t pin_enable;
    int16_t dac_count;
    int16_t duty;
    int16_t volt_pwm;

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
        pwm.timer(2, 1, 25, false);     // Use timer 2 for pin 13, divide clock by 4, resolution 60000, dual-slope PWM
    }

    /**
     * @brief Set the Voltage object
     *
     * @param volt
     * @param mode
     */
    void SetVoltage(int volt, bool mode)
    {
        if (volt >= 45)
        {
            if (mode == C_BOOST_MODE)
            {
                volt += 3;
            }
            volt = constrain(volt, 45, 120);
            dac_count = C_DAC_MIN_COUNT - (((volt - 45) * (C_DAC_MIN_COUNT - C_DAC_MAX_COUNT)) / (120 - 45));
            analogWrite(C_PIN_DAC, dac_count);
        }
        else if (volt < 45)
        {
            volt_pwm = C_ARRAY_VOLT_PWM[volt - MIN_VOLTAGE];
            dac_count = C_DAC_MIN_COUNT - (((volt_pwm - 45) * (C_DAC_MIN_COUNT - C_DAC_MAX_COUNT)) / (120 - 45));
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