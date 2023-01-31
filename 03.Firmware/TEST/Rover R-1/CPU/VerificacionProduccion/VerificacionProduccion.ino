/**
 * @file VerificacionProduccion.ino
 * @author Javi (Javier@musotoku.com)
 * @brief 
 * @version 0.1
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

const uint16_t C_TEST_0 = 0;
const uint16_t C_TEST_1 = 1;
const uint16_t C_TEST_2 = 2;
const uint16_t C_TEST_3 = 3;

const uint16_t TOLERANCIA = 10; // %
const uint16_t LOWER_TOL = 100 - TOLERANCIA;
const uint16_t UPPER_TOL = 100 + TOLERANCIA;
const uint16_t VCC_ADC_VALUE = 1500 * 4096 / 3300;
const uint16_t VCC_2_ADC_VALUE = 1500 * 4096 / 3300;

Adafruit_SSD1306 display(128, 64, C_PIN_MOSI, C_PIN_SCK, C_PIN_MISO, C_PIN_DSP_RST, C_PIN_SS);

bool flag_programacion_ok = false;
bool flag_testing = false;
bool flag_vcc_tested = false;
bool flag_vcc_2_tested = false;
bool flag_en_dcdc_tested = false;
bool flag_op_switch_tested = false;
bool flag_dac_tested = false;
bool flag_vout_tested = false;
bool flag_iout_tested = false;
char data = 0;

uint16_t status = C_TEST_0;
uint16_t vcc_sense = 0;
uint16_t vcc_2_sense = 0;
uint16_t fb_dac_sense = 0;

void setup()
{
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
    /* Fase de Testeo de Placa de Testeo */
    uint16_t vol_sel = 0;
    while (1)
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

    display.clearDisplay();
    display.setTextSize(2);
    display.setTextColor(WHITE);
    display.setCursor(0, 0);
    display.print("Fase");
    display.setCursor(0, 25);
    display.print("Programacion");
    display.display();

    // Programacion

    while (flag_programacion_ok == false)
    {
        Serial.print("Bip-Bip"); //HeartBit enviado al DUT.
        if (Serial.available())
        {
            data = Serial.read();
            if (data == 'A')
            {
                flag_programacion_ok = true;
            }
        }
        delay(100);
    }

    status = C_TEST_0;

    while (flag_testing == true)
    {
        if (status == C_TEST_0)
        {
            display.clearDisplay();
            display.setTextSize(2);
            display.setTextColor(WHITE);
            display.setCursor(0, 0);
            display.print("Test 0");
            display.setCursor(0, 25);
            display.print("VCC: ");
            display.setCursor(0, 50);
            display.print("VCC 2: ");

            for (int i = 0; i < 8; i++)
            {
                vcc_sense += analogRead(C_PIN_VCC_CPU);
                vcc_2_sense += analogRead(C_PIN_VCC_2_CPU);
            }
            vcc_sense = vcc_sense / 8;
            vcc_2_sense = vcc_2_sense / 8;

            if (vcc_sense >= (VCC_ADC_VALUE * LOWER_TOL / 100))
            {
                if (vcc_sense <= (VCC_ADC_VALUE * UPPER_TOL / 100))
                {
                    flag_vcc_tested = true;
                }
                else
                {
                    flag_vcc_tested = false;
                }
            }
            else
            {
                flag_vcc_tested = false;
            }
            if (vcc_2_sense >= (VCC_2_ADC_VALUE * LOWER_TOL / 100))
            {
                if (vcc_2_sense <= (VCC_2_ADC_VALUE * UPPER_TOL / 100))
                {
                    flag_vcc_tested = true;
                }
                else
                {
                    flag_vcc_tested = false;
                }
            }
            else
            {
                flag_vcc_tested = false;
            }
            if (flag_vcc_tested == true)
            {
                display.setCursor(70, 25);
                display.print("OK");
            }
            else
            {
                display.setCursor(70, 25);
                display.print("ERROR");
            }

            if (flag_vcc_2_tested == true)
            {

                display.setCursor(70, 50);
                display.print("OK");
            }
            else
            {
                display.setCursor(70, 50);
                display.print("ERROR");
            }
            display.display();

            if ((flag_vcc_tested == true) && (flag_vcc_2_tested == true))
            {
                if (digitalRead(C_PIN_TEST_1) == false)
                {
                    status = C_TEST_1;
                }
            }
        }
        else if (status == C_TEST_1) // EN_DCDC y OP_SWITCH
        {
            display.clearDisplay();
            display.setTextSize(2);
            display.setTextColor(WHITE);
            display.setCursor(0, 0);
            display.print("Test 1");
            display.setCursor(0, 25);
            display.print("EN_DCDC: ");
            display.setCursor(0, 50);
            display.print("OP_SWTICH: ");

            display.display();
            if ((flag_en_dcdc_tested == true) && (flag_op_switch_tested == true))
            {
                if (digitalRead(C_PIN_TEST_2) == false)
                {
                    status = C_TEST_2;
                }
            }
        }
        else if (status == C_TEST_2)
        {
            display.clearDisplay();
            display.setTextSize(2);
            display.setTextColor(WHITE);
            display.setCursor(0, 0);
            display.print("Test 2");
            display.setCursor(0, 25);
            display.print("DAC: ");

            display.display();
            if (flag_dac_tested == true)
            {
                if (digitalRead(C_PIN_TEST_3) == false)
                {
                    status = C_TEST_3;
                }
            }
        }
        else if (status == C_TEST_3)
        {
            display.clearDisplay();
            display.setTextSize(2);
            display.setTextColor(WHITE);
            display.setCursor(0, 0);
            display.print("Test 3");
            display.setCursor(0, 25);
            display.print("Iout: ");
            display.setCursor(0, 50);
            display.print("Vout: ");

            display.display();
            if (flag_dac_tested == true)
            {
                if (digitalRead(C_PIN_TEST_3) == false)
                {
                    flag_testing = false;
                }
            }
        }
    }
}

void loop()
{
}