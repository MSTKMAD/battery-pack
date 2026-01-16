/**
 * @file VerificacionProduccion.ino
 * @author Javi (Javier@musotoku.com)
 * @brief 
 * @version 2
 * @date 2023-01-24
 * 
 * @copyright Copyright (c) 2023
 * 
 */
#include "Arduino.h"
#include "Adafruit_SSD1306_096.h"
#include "MilliTimer.h"
#include <Wire.h>

const uint16_t C_PIN_SW1 = 3;             // 3.2v
const uint16_t C_PIN_SW2 = 2;             // 4.0v
const uint16_t C_PIN_SDA = 4;             //
const uint16_t C_PIN_SCL = 5;             //
const uint16_t C_PIN_TEST_1 = 13;         // True = OFF , False = ON
const uint16_t C_PIN_TEST_2 = 14;         // True = OFF , False = ON
const uint16_t C_PIN_TEST_3 = 15;         // True = OFF , False = ON
const uint16_t C_PIN_MISO = 16;           // MISO Display
const uint16_t C_PIN_DSP_RST = 12;        // Reset Display
const uint16_t C_PIN_SS = 17;             // SS Display
const uint16_t C_PIN_SCK = 18;            // SCK Display
const uint16_t C_PIN_MOSI = 19;           // MOSI Display
const uint16_t C_PIN_ENABLE_LDO_ADJ = 20; // TRUE ON - False OFF
const uint16_t C_PIN_EN_DCDC = 21;        //
const uint16_t C_PIN_OP_SWITCH = 22;      //
const uint16_t C_PIN_VCC_CPU = A0;        // 1.5v es correcto
const uint16_t C_PIN_VCC_2_CPU = A1;      // 1.5v es correcto
const uint16_t C_PIN_FB_DAC = A2;         // 0 - 1.4v

const uint16_t C_OFFSET_ADC = 75;

const uint16_t C_TEST_0 = 0;
const uint16_t C_TEST_1 = 1;
const uint16_t C_TEST_2 = 2;
const uint16_t C_TEST_3 = 3;

const uint16_t TOLERANCIA = 10; // %
const uint16_t LOWER_TOL = 100 - TOLERANCIA;
const uint16_t UPPER_TOL = 100 + TOLERANCIA;
const uint16_t VCC_ADC_VALUE = 1500;
const uint16_t VCC_2_ADC_VALUE = 1500;
const uint16_t DAC_LOW_VALUE = 174;
const uint16_t DAC_HIGH_VALUE = 1551;

Adafruit_SSD1306 display(128, 64, C_PIN_MOSI, C_PIN_SCK, C_PIN_MISO, C_PIN_DSP_RST, C_PIN_SS);

bool flag_programacion_ok = false;
bool flag_testing = true;
bool flag_vcc_tested = false;
bool flag_vcc_2_tested = false;
bool flag_en_dcdc_tested = false;
bool flag_op_switch_tested = false;
bool flag_dac_tested = false;
bool flag_vout_tested = false;
bool flag_iout_tested = false;

bool prev_st_op_switch = false;
bool act_st_op_switch = false;
bool prev_st_en_dcdc = false;
bool act_st_en_dcdc = false;
String data;

uint16_t status = C_TEST_1;
uint16_t vcc_sense = 0;
uint16_t vcc_2_sense = 0;
uint16_t fb_dac_sense = 0;

uint16_t cont_op_swtch = 0;
uint16_t cont_en_dcdc = 0;
uint16_t cont_dac = 0;

void setup()
{
    Serial2.setRX(9);
    Serial2.setTX(8);
    Serial2.begin(57600);
    Serial.begin(9600);
    pinMode(C_PIN_SW1, OUTPUT);
    pinMode(C_PIN_SW2, OUTPUT);

    pinMode(C_PIN_TEST_1, INPUT);
    pinMode(C_PIN_TEST_2, INPUT);
    pinMode(C_PIN_TEST_3, INPUT);

    pinMode(C_PIN_ENABLE_LDO_ADJ, INPUT);

    pinMode(C_PIN_EN_DCDC, INPUT);
    pinMode(C_PIN_OP_SWITCH, INPUT);

    pinMode(C_PIN_VCC_CPU, INPUT);
    pinMode(C_PIN_VCC_2_CPU, INPUT);
    pinMode(C_PIN_FB_DAC, INPUT);

    // 4 Voltios de Salida por V_USB
    digitalWrite(C_PIN_SW1, LOW);
    digitalWrite(C_PIN_SW2, HIGH);
    analogReadResolution(12);
    display.begin();
    display.clearDisplay();
    display.setTextSize(2);
    display.setTextColor(WHITE);
    /* Fase de Testeo de Placa de Testeo */
    uint16_t vol_sel = 0;
    /*while (1)
    {
        Serial.println("Bip-Bip"); //HeartBit enviado al DUT.
        display.clearDisplay();
        display.setTextSize(1);
        display.setCursor(0, 0);
        display.print("TEST CARD");
        display.setTextColor(WHITE);
        // 4 Voltios de Salida por V_USB
        if (digitalRead(C_PIN_TEST_1) == LOW)
        {
            // 3v
            digitalWrite(C_PIN_SW1, LOW);
            digitalWrite(C_PIN_SW2, LOW);
            vol_sel = 30;
        }
        else if (digitalRead(C_PIN_TEST_2) == LOW)
        {
            // 3.2v
            digitalWrite(C_PIN_SW1, HIGH);
            digitalWrite(C_PIN_SW2, LOW);
            vol_sel = 32;
        }
        else if (digitalRead(C_PIN_TEST_3) == LOW)
        {
            // 4v
            digitalWrite(C_PIN_SW1, LOW);
            digitalWrite(C_PIN_SW2, HIGH);
            vol_sel = 40;
        }
        switch (vol_sel)
        {
        case 30:
            display.setCursor(64, 0);
            display.print("OUT -> 3v");
            break;
        case 32:
            display.setCursor(64, 0);
            display.print("OUT -> 3.2v");
            break;
        case 40:
            display.setCursor(64, 0);
            display.print("OUT -> 4v");
            break;

        default:
            display.setCursor(64, 0);
            display.print("OUT ->");
            break;
        }
        display.setCursor(0, 10);
        display.printf("VCC");
        display.setCursor(64, 10);
        display.printf("= %d mV", analogRead(C_PIN_VCC_CPU) * 3300 / 4096);

        display.setCursor(0, 20);
        display.print("VCC_2");
        display.setCursor(64, 20);
        display.printf("= %d mV", analogRead(C_PIN_VCC_2_CPU) * 3300 / 4096);

        display.setCursor(0, 30);
        display.printf("FB_DAC");
        display.setCursor(64, 30);
        display.printf("= %d mV", analogRead(C_PIN_FB_DAC) * 3300 / 4096);

        display.setCursor(0, 40);
        display.printf("EN_DCDC");
        display.setCursor(64, 40);
        display.printf("= %s", digitalRead(C_PIN_EN_DCDC) ? "true" : "false");

        display.setCursor(0, 50);
        display.printf("OP_SWITCH");
        display.setCursor(64, 50);
        display.printf("= %s", digitalRead(C_PIN_OP_SWITCH) ? "true" : "false");
        display.display();
        delay(100);
    }

    /*                                  */

    status = C_TEST_1;
    // 4v
    digitalWrite(C_PIN_SW1, LOW);
    digitalWrite(C_PIN_SW2, HIGH);
    while (flag_testing == true)
    {
        if (digitalRead(C_PIN_TEST_1) == false)
        {
            status = C_TEST_1;
        }
        else if (digitalRead(C_PIN_TEST_2) == false)
        {
            cont_en_dcdc = 0;
            cont_op_swtch = 0;
            status = C_TEST_2;
        }
        else if (digitalRead(C_PIN_TEST_3) == false)
        {
            cont_dac = 0;
            status = C_TEST_3;
        }

        if (status == C_TEST_1)
        {
            for (int i = 0; i < 8; i++)
            {
                vcc_sense += analogRead(C_PIN_VCC_CPU);
                vcc_2_sense += analogRead(C_PIN_VCC_2_CPU);
            }
            vcc_sense = vcc_sense / 8;
            vcc_2_sense = vcc_2_sense / 8;

            vcc_sense = (vcc_sense - C_OFFSET_ADC) * 3300 / 4095;
            vcc_2_sense = (vcc_2_sense - C_OFFSET_ADC) * 3300 / 4095;

            Serial.println(vcc_2_sense);
            if ((vcc_2_sense >= (VCC_ADC_VALUE * LOWER_TOL / 100)) && (vcc_2_sense <= (VCC_ADC_VALUE * UPPER_TOL / 100)))
            {
                if (digitalRead(C_PIN_TEST_1) == false)
                {
                    flag_programacion_ok = true;
                }
                if (Serial2.available())
                {
                    data = Serial2.readStringUntil('\n');
                    display.clearDisplay();
                    display.setCursor(0, 0);
                    display.print("N. Serie:");
                    display.setCursor(0, 25);
                    display.print(data);
                    Serial.println(data);
                }
            }
            else
            {
                display.clearDisplay();
                display.setTextSize(2);
                display.setCursor(20, 20);
                display.print("Waiting");
                display.setCursor(30, 40);
                display.print("Card");
            }
            display.display();
        }
        else if (status == C_TEST_2) // EN_DCDC y OP_SWITCH
        {
            display.clearDisplay();
            display.setTextSize(2);
            display.setTextColor(WHITE);
            display.setCursor(0, 0);
            display.print("Test 1");
            display.setCursor(0, 25);
            display.print("EN: ");
            display.setCursor(0, 50);
            display.print("OP: ");

            prev_st_en_dcdc = act_st_en_dcdc;
            act_st_en_dcdc = digitalRead(C_PIN_EN_DCDC);
            prev_st_op_switch = act_st_op_switch;
            act_st_op_switch = digitalRead(C_PIN_OP_SWITCH);

            if ((prev_st_op_switch == false) && (act_st_op_switch == true))
            {
                cont_op_swtch++;
            }
            if ((prev_st_en_dcdc == false) && (act_st_en_dcdc == true))
            {
                cont_en_dcdc++;
            }

            if (cont_op_swtch >= 5)
            {
                display.setCursor(70, 50);
                display.print("OK");
            }
            if (cont_en_dcdc >= 5)
            {
                display.setCursor(70, 25);
                display.print("OK");
            }
            display.display();
        }
        else if (status == C_TEST_3)
        {
            display.clearDisplay();
            display.setTextSize(2);
            display.setTextColor(WHITE);
            display.setCursor(0, 0);
            display.print("Test 2");
            display.setCursor(0, 32);
            display.print("DAC: ");

            if (cont_dac < 15)
            {
                fb_dac_sense = analogRead(C_PIN_FB_DAC);
                Serial.printf("DAC: %d\n", fb_dac_sense);
                if (fb_dac_sense >= (DAC_LOW_VALUE * LOWER_TOL / 100))
                {
                    if (fb_dac_sense <= (DAC_LOW_VALUE * UPPER_TOL / 100))
                    {
                        cont_dac++;
                    }
                }
            }
            else if (cont_dac < 30)
            {
                fb_dac_sense = analogRead(C_PIN_FB_DAC);
                Serial.printf("DAC: %d\n", fb_dac_sense);
                if (fb_dac_sense >= (DAC_HIGH_VALUE * LOWER_TOL / 100))
                {
                    if (fb_dac_sense <= (DAC_HIGH_VALUE * UPPER_TOL / 100))
                    {
                        cont_dac++;
                    }
                }
            }

            if (cont_dac >= 30)
            {
                display.setCursor(70, 32);
                display.print("OK");
            }
            display.display();
        }
    }
}

void loop()
{
}