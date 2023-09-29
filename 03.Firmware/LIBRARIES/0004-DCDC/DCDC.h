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

#ifndef MAX_VOLTAGE
#define MAX_VOLTAGE 120
#endif
#ifndef MIN_VOLTAGE
#define MIN_VOLTAGE 25
#endif
const uint16_t LenDCDCvalues = 121;
const bool C_BOOST_MODE = true;
const bool C_NON_BOOST_MODE = false;

const uint16_t C_VDAC_MIN = 1435;                          // mv del DAC para dar a la salida el MIN_VOLTAGE
const uint16_t C_VDAC_MAX = 140;                           // mv del DAC para dar a la salida el MAX_VOLTAGE
const uint16_t C_DAC_RESOLUTON = 1024;                     // Fondo de escala del DAC
const uint16_t C_DAC_MIN_COUNT = C_VDAC_MIN * 1024 / 3000; // cuentas dac para dar el Minimo voltaje permitido (5v)
const uint16_t C_DAC_MAX_COUNT = C_VDAC_MAX * 1024 / 3000; // cuentas dac para dar el Minimo voltaje permitido (5v)

const uint16_t C_PIN_DAC = A0;
class dcdc_controler
{
private:
public:
    int16_t pin_enable;
    int16_t dac_count;

    /**
     * @brief Construct a new dcdc controler object
     *
     * @param pin Pin conectado al enable del DCDC.
     */
    dcdc_controler(int16_t pin)
    {
        pin_enable = pin;
        pwm.setClockDivider(48, false); // Main clock divided by 200 => 240KHz
        pwm.timer(2, 1, 50, false);     // Use timer 2 for pin 13, divide clock by 4, resolution 60000, dual-slope PWM
        pinMode(pin_enable, OUTPUT);
        digitalWrite(pin_enable, LOW);
        pinMode(C_PIN_OP_SWITCH, OUTPUT);
        pinMode(C_PIN_ENABLE_LDO_VCC_2, OUTPUT);
        //------------------------ INITIALITATION PERIFERICOS ----------------------------

        digitalWrite(C_PIN_OP_SWITCH, HIGH);        // Interruptor Salida
        digitalWrite(C_PIN_ENABLE_LDO_VCC_2, HIGH); // Encendido del DCDC
    }

    /**
     * @brief Set the Voltage object
     *
     * @param volt
     * @param mode
     */
    void SetVoltage(int volt, bool mode)
    {
        if (mode == C_BOOST_MODE)
        {
            if (mode == C_BOOST_MODE)
            {
                volt += 3;
            }
            volt = constrain(volt, 50, 120);
            dac_count = C_DAC_MIN_COUNT - (((volt - 50) * (C_DAC_MIN_COUNT - C_DAC_MAX_COUNT)) / (120 - 50));
            analogWrite(C_PIN_DAC, dac_count);
        }
        volt = constrain(volt, MIN_VOLTAGE, MAX_VOLTAGE);
        dac_count = C_DAC_MIN_COUNT - (((volt - MIN_VOLTAGE) * (C_DAC_MIN_COUNT - C_DAC_MAX_COUNT)) / (MAX_VOLTAGE - MIN_VOLTAGE));
        analogWrite(C_PIN_DAC, dac_count);
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