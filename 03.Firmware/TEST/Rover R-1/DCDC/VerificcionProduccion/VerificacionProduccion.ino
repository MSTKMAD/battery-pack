/**
 * @file VerificacionProduccion.ino
 * @author Javi (Javier@musotoku.com)
 * @brief 
 * @version 0.1
 * @date 2023-01-25
 * 
 * @copyright Copyright (c) 2023
 * 
 */
#include "Adafruit_SSD1306_096.h"
#include "MilliTimer.h"
#include <Wire.h>
#include "MCP4725.h"
#include "DCDC.h"

const uint16_t C_PIN_SDA = 4;         // I2C para el DAC MCP4725
const uint16_t C_PIN_SCL = 5;         // I2C para el DAC MCP4725
const uint16_t C_PIN_EN_DCDC = 9;     // True = OFF , False = ON
const uint16_t C_PIN_OP_SWITCH = 10;  // True = ON, False = OFF
const uint16_t C_PIN_FB_DAC_PWM = 11; // NO SE USA MIENTRAS SE USE EL MCP4725.
const uint16_t C_PIN_TEST_3 = 13;     // LOW = ON , HIGH = OFF
const uint16_t C_PIN_TEST_2 = 14;     // LOW = ON , HIGH = OFF
const uint16_t C_PIN_TEST_1 = 15;     // LOW = ON , HIGH = OFF
const uint16_t C_PIN_DSP_RST = 12;    // Reset del Display
const uint16_t C_PIN_MISO = 16;       // MISO del Display
const uint16_t C_PIN_SS = 17;         // SS del Display
const uint16_t C_PIN_SCK = 18;        // SCK del Display
const uint16_t C_PIN_MOSI = 19;       // MOSI del Display
const uint16_t C_PIN_R3 = 20;         // Resistencia X. HIGH = ON, LOW = OFF
const uint16_t C_PIN_R2 = 21;         // Resistencia X. HIGH = ON, LOW = OFF
const uint16_t C_PIN_R1 = 22;         // Resistencia X. HIGH = ON, LOW = OFF
const uint16_t C_PIN_VOUT_SENSE = A1; // Sensado del DCDC de la tension de salida.
const uint16_t C_PIN_IO_SENSE = A2;   // Sensado del DCDC de la corriente de salida.

const uint16_t C_TEST_0 = 0;
const uint16_t C_TEST_1 = 1;
const uint16_t C_TEST_2 = 2;
const uint16_t C_TEST_3 = 3;

const uint16_t C_FASE_0 = 4;
const uint16_t C_FASE_1 = 5;
const uint16_t C_FASE_2 = 6;
const uint16_t C_FASE_3 = 7;

const uint16_t C_VOUT_FASE_0 = 4000; // V BUS
const uint16_t C_VOUT_FASE_1 = 6000;
const uint16_t C_VOUT_FASE_2 = 500;

const uint16_t C_VOUT_TEST_2 = 50;
const uint16_t C_VOUT_TEST_3 = 50;

const uint16_t TOLERANCIA = 10; // %
const uint16_t LOWER_TOL = 100 - TOLERANCIA;
const uint16_t UPPER_TOL = 100 + TOLERANCIA;

Adafruit_SSD1306 display(128, 64, C_PIN_MOSI, C_PIN_SCK, C_PIN_MISO, C_PIN_DSP_RST, C_PIN_SS);

MCP4725 MCP(0x62); // 0x62 or 0x63

bool flag_testing = false;
bool flag_dac_test = false;
bool flag_1w_tested = false;
bool flag_8w_tested = false;

uint16_t status = 0;
uint16_t fase = 0;
uint16_t vout_sample = 0;
uint16_t iout_sample = 0;
uint16_t dac_count = 0;

const static unsigned char __attribute__((progmem)) MusotokuLogo[] = {
    0x03, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x00,
    0x06, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x80,
    0x0c, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xc0,
    0x18, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x60,
    0x30, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x30,
    0x60, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x18,
    0xc0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0c,
    0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x04,
    0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x04,
    0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x04,
    0x80, 0x00, 0x41, 0x10, 0x83, 0xc1, 0xe0, 0x00, 0x04,
    0x80, 0x00, 0x63, 0x10, 0x84, 0x22, 0x10, 0x00, 0x04,
    0x80, 0x00, 0x77, 0x10, 0x88, 0x12, 0x10, 0x00, 0x04,
    0x80, 0x00, 0x5d, 0x10, 0x84, 0x02, 0x10, 0x00, 0x04,
    0x80, 0x00, 0x49, 0x10, 0x83, 0xc2, 0x10, 0x00, 0x04,
    0x80, 0x00, 0x41, 0x10, 0x80, 0x22, 0x10, 0x00, 0x04,
    0x80, 0x00, 0x41, 0x10, 0x80, 0x12, 0x10, 0x00, 0x04,
    0x80, 0x00, 0x41, 0x10, 0x88, 0x12, 0x10, 0x00, 0x04,
    0x80, 0x00, 0x41, 0x10, 0x84, 0x22, 0x10, 0x00, 0x04,
    0x80, 0x00, 0x41, 0x0f, 0x03, 0xc1, 0xa0, 0x00, 0x04,
    0xe0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x1c,
    0x20, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x10,
    0x20, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x10,
    0x20, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x10,
    0x20, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x10,
    0x20, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x10,
    0x20, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x10,
    0xe0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x1c,
    0x80, 0x00, 0x7f, 0x0f, 0x04, 0x22, 0x10, 0x00, 0x04,
    0x80, 0x00, 0x08, 0x10, 0x84, 0x42, 0x10, 0x00, 0x04,
    0x80, 0x00, 0x08, 0x10, 0x84, 0x82, 0x10, 0x00, 0x04,
    0x80, 0x00, 0x08, 0x10, 0x85, 0x02, 0x10, 0x00, 0x04,
    0x80, 0x00, 0x08, 0x10, 0x86, 0x02, 0x10, 0x00, 0x04,
    0x80, 0x00, 0x08, 0x10, 0x86, 0x02, 0x10, 0x00, 0x04,
    0x80, 0x00, 0x08, 0x10, 0x85, 0x02, 0x10, 0x00, 0x04,
    0x80, 0x00, 0x08, 0x10, 0x84, 0x82, 0x10, 0x00, 0x04,
    0x80, 0x00, 0x08, 0x10, 0x84, 0x42, 0x10, 0x00, 0x04,
    0x80, 0x00, 0x08, 0x0f, 0x04, 0x21, 0xe0, 0x00, 0x04,
    0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x04,
    0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x04,
    0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x04,
    0xc0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0c,
    0x60, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x18,
    0x30, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x30,
    0x18, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x60,
    0x0c, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xc0,
    0x06, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x80,
    0x03, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x00

};

void setup()
{
    Wire.begin();
    Serial.begin(9600);
    MCP.begin(C_PIN_SDA, C_PIN_SCL);

    pinMode(C_PIN_EN_DCDC, OUTPUT);
    pinMode(C_PIN_OP_SWITCH, OUTPUT);

    pinMode(C_PIN_TEST_1, INPUT);
    pinMode(C_PIN_TEST_2, INPUT);
    pinMode(C_PIN_TEST_3, INPUT);

    pinMode(C_PIN_R3, OUTPUT);
    pinMode(C_PIN_R2, OUTPUT);
    pinMode(C_PIN_R1, OUTPUT);

    pinMode(C_PIN_VOUT_SENSE, INPUT);
    pinMode(C_PIN_IO_SENSE, INPUT);

    digitalWrite(C_PIN_EN_DCDC, LOW);
    digitalWrite(C_PIN_OP_SWITCH, HIGH);

    digitalWrite(C_PIN_R1, LOW);
    digitalWrite(C_PIN_R2, LOW);
    digitalWrite(C_PIN_R3, LOW);

    display.begin();
    display.clearDisplay();
    display.setTextSize(2);
    display.setTextColor(WHITE);
    display.setCursor(0, 0);
    display.print("Fase");
    display.setCursor(0, 25);
    display.print("Programacion");
    display.display();

    status = C_TEST_0;

    while (flag_testing == true)
    {
        if (status == C_TEST_0)
        {
            display.clearDisplay();
            display.setCursor(0, 0);
            display.print("Test 0");
            display.setCursor(0, 25);
            display.print("EN DCDC");
            display.setCursor(0, 50);
            display.print("OP SWITCH");

            vout_sample = 0;
            for (int i = 0; i < 8; i++)
            {
                vout_sample += analogRead(C_PIN_VOUT_SENSE);
                delay(50);
            }

            vout_sample = vout_sample / 8 * 208 / 39 * 3000 / 4096;

            if (fase == C_FASE_0)
            {
                display.setCursor(70, 25);
                display.print("OFF");
                display.setCursor(70, 50);
                display.print("ON");
                // EN_DCDC = OFF - OP_SWITCH = ON
                digitalWrite(C_PIN_EN_DCDC, true);
                digitalWrite(C_PIN_OP_SWITCH, true);

                if (vout_sample <= C_VOUT_FASE_0)
                {
                    fase = C_FASE_1;
                }
            }
            else if (fase == C_FASE_1)
            {
                display.setCursor(70, 25);
                display.print("ON");
                display.setCursor(70, 50);
                display.print("ON");
                // EN_DCDC = ON - OP_SWITCH = ON
                digitalWrite(C_PIN_EN_DCDC, true);
                digitalWrite(C_PIN_OP_SWITCH, true);

                if (vout_sample <= C_VOUT_FASE_1)
                {
                    fase = C_FASE_2;
                }
            }
            else if (fase == C_FASE_2)
            {
                display.setCursor(70, 25);
                display.print("ON");
                display.setCursor(70, 50);
                display.print("OFF");
                // EN_DCDC = ON - OP_SWITCH = OFF
                digitalWrite(C_PIN_EN_DCDC, true);
                digitalWrite(C_PIN_OP_SWITCH, true);

                if (vout_sample <= C_VOUT_FASE_2)
                {
                    display.clearDisplay();
                    display.setCursor(0, 0);
                    display.print("Test 0");
                    display.setCursor(0, 25);
                    display.print("DONE");
                    display.setCursor(0, 50);
                    display.print("PRESS TEST 1");
                    if (digitalRead(C_PIN_TEST_1) == false)
                    {
                        status = C_TEST_1;
                    }
                }
            }
            display.display();
        }
        else if (status == C_TEST_1)
        {
            display.setCursor(0, 0);
            display.print("Test 1");
            display.setCursor(0, 25);
            display.print("DAC");
            display.setCursor(0, 50);
            display.print("VOUT");

            for (int i = MIN_VOLTAGE; i < MAX_VOLTAGE; i)
            {
                dac_count = C_DAC_MIN_COUNT - (((i - MIN_VOLTAGE) * (C_DAC_MIN_COUNT - C_DAC_MAX_COUNT)) / (MAX_VOLTAGE - MIN_VOLTAGE));
                MCP.setValue(dac_count);
                display.setCursor(70, 25);
                display.print(dac_count);
                display.setCursor(70, 50);
                display.print(i);

                vout_sample = 0;
                for (int j = 0; j < 8; j++)
                {
                    vout_sample += analogRead(C_PIN_VOUT_SENSE);
                    delay(50);
                }
                vout_sample = vout_sample / 8 * 208 / 39 * 3000 / 4096;

                if ((vout_sample <= (i * LOWER_TOL)) || (vout_sample >= (i * UPPER_TOL)))
                {
                    flag_dac_test = false;
                }
            }
            if (flag_dac_test == true)
            {
                display.clearDisplay();
                display.setCursor(0, 0);
                display.print("Test 1");
                display.setCursor(0, 25);
                display.print("CORRECT");
                display.setCursor(0, 50);
                display.print("PRESS TEST 2");
                if (digitalRead(C_PIN_TEST_2) == false)
                {
                    status = C_TEST_2;
                }
            }
            else
            {
                display.clearDisplay();
                display.setCursor(0, 0);
                display.print("Test 1");
                display.setCursor(0, 25);
                display.print("ERROR");
                display.setCursor(0, 50);
                display.print("RESET");
            }

            display.display();
        }
        else if (status == C_TEST_2)
        {
            display.clearDisplay();
            display.setCursor(0, 0);
            display.print("Test 2");
            display.setCursor(0, 25);
            display.print("POWER 1W");

            digitalWrite(C_PIN_R1, HIGH);

            dac_count = C_DAC_MIN_COUNT - (((C_VOUT_TEST_2 - MIN_VOLTAGE) * (C_DAC_MIN_COUNT - C_DAC_MAX_COUNT)) / (MAX_VOLTAGE - MIN_VOLTAGE));
            MCP.setValue(dac_count);

            vout_sample = 0;
            for (int i = 0; i < 8; i++)
            {
                vout_sample += analogRead(C_PIN_VOUT_SENSE);
                delay(50);
            }

            vout_sample = vout_sample / 8 * 208 / 39 * 3000 / 4096;

            iout_sample = 0;
            for (int i = 0; i < 8; i++)
            {
                iout_sample += analogRead(C_PIN_IO_SENSE);
                delay(50);
            }

            iout_sample = iout_sample / 8 * 208 / 39 * 3000 / 4096;

            if (((iout_sample * vout_sample / 1000) >= (1000 * LOWER_TOL / 100)) && ((iout_sample * vout_sample / 1000) <= (1000 * UPPER_TOL / 100)))
            {
                flag_1w_tested = true;
            }

            if (flag_1w_tested == true)
            {
                display.clearDisplay();
                display.setCursor(0, 0);
                display.print("Test 2");
                display.setCursor(0, 25);
                display.print("CORRECT");
                display.setCursor(0, 50);
                display.print("PRESS TEST 3");
                if (digitalRead(C_PIN_TEST_3) == false)
                {
                    status = C_TEST_3;
                }
            }

            display.display();
        }
        else if (status == C_TEST_3)
        {
            display.clearDisplay();
            display.setCursor(0, 0);
            display.print("Test 3");
            display.setCursor(0, 25);
            display.print("Power 8W");

            digitalWrite(C_PIN_R2, HIGH);
            dac_count = C_DAC_MIN_COUNT - (((C_VOUT_TEST_3 - MIN_VOLTAGE) * (C_DAC_MIN_COUNT - C_DAC_MAX_COUNT)) / (MAX_VOLTAGE - MIN_VOLTAGE));
            MCP.setValue(dac_count);

            vout_sample = 0;
            for (int i = 0; i < 8; i++)
            {
                vout_sample += analogRead(C_PIN_VOUT_SENSE);
                delay(50);
            }

            vout_sample = vout_sample / 8 * 208 / 39 * 3000 / 4096;

            iout_sample = 0;
            for (int i = 0; i < 8; i++)
            {
                iout_sample += analogRead(C_PIN_IO_SENSE);
                delay(50);
            }

            iout_sample = iout_sample / 8 * 208 / 39 * 3000 / 4096;

            if (((iout_sample * vout_sample / 1000) >= (8000 * LOWER_TOL / 100)) && ((iout_sample * vout_sample / 1000) <= (8000 * UPPER_TOL / 100)))
            {
                flag_8w_tested = true;
            }

            if (flag_8w_tested == true)
            {
                display.clearDisplay();
                display.setCursor(0, 0);
                display.print("Test 3");
                display.setCursor(0, 25);
                display.print("CORRECT");
                display.setCursor(0, 50);
                display.print("TEST ENDED");
                flag_testing = false;
            }
            display.display();
        }
    }
}

void loop()
{
}