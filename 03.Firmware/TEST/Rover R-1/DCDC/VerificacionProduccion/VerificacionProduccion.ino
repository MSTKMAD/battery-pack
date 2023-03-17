/**
 * @file VerificacionProduccion.ino
 * @author Javi (Javier@musotoku.com)
 * @brief
 * @version 2
 * @date 2023-01-25
 *
 * @copyright Copyright (c) 2023
 *
 */
#include "Arduino.h"
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
const uint16_t C_FASE_4 = 8;

const uint16_t C_VOUT_FASE_0 = 500;
const uint16_t C_VOUT_FASE_1 = 3000;
const uint16_t C_VOUT_FASE_2 = 500;
const uint16_t C_VOUT_FASE_3 = 10000;

const uint16_t C_IOUT_TEST_0_1 = 100;

const uint16_t C_VOUT_TEST_2 = 50;
const uint16_t C_VOUT_TEST_3 = 98;

const uint16_t TOLERANCIA = 20; // %
const uint16_t LOWER_TOL = 100 - TOLERANCIA;
const uint16_t UPPER_TOL = 100 + TOLERANCIA;

Adafruit_SSD1306 display(128, 64, C_PIN_MOSI, C_PIN_SCK, C_PIN_MISO, C_PIN_DSP_RST, C_PIN_SS);

MCP4725 MCP(0x60, &Wire);

MilliTimer timer_t1, timer_t3;

bool flag_testing = true;
uint16_t flag_dac_test = 0;
bool flag_1w_tested = false;
bool flag_4w_tested = false;

uint16_t status = 0;
uint16_t fase = C_FASE_0;
uint32_t vout_sample = 0;
uint16_t iout_sample = 0;
uint16_t dac_count = 0;

uint16_t volt_test_2 = 40;

void setup()
{
    // Wire.setClock(100000);
    MCP.begin(C_PIN_SDA, C_PIN_SCL);
    Serial.begin(9600);

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
    analogReadResolution(12);
    display.begin();
    /* Fase de Testeo de Placa de Testeo */
    uint16_t r_sel = 0;
    uint16_t volt = 50;
    digitalWrite(C_PIN_EN_DCDC, HIGH);
    digitalWrite(C_PIN_OP_SWITCH, HIGH);
    status = C_TEST_0;
    fase = C_FASE_0;
    timer_t1.set(2000);
    display.clearDisplay();

    /* while (1)
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
            // R1
            digitalWrite(C_PIN_R1, HIGH);
            digitalWrite(C_PIN_R2, LOW);
            digitalWrite(C_PIN_R3, LOW);
            digitalWrite(C_PIN_EN_DCDC, LOW);
            digitalWrite(C_PIN_OP_SWITCH, LOW);
            r_sel = 1;
        }
        else if (digitalRead(C_PIN_TEST_2) == LOW)
        {
            // R2
            digitalWrite(C_PIN_R1, LOW);
            digitalWrite(C_PIN_R2, HIGH);
            digitalWrite(C_PIN_R3, LOW);
            digitalWrite(C_PIN_EN_DCDC, LOW);
            digitalWrite(C_PIN_OP_SWITCH, HIGH);
            r_sel = 2;
        }
        else if (digitalRead(C_PIN_TEST_3) == LOW)
        {
            // R3
            digitalWrite(C_PIN_R1, LOW);
            digitalWrite(C_PIN_R2, LOW);
            digitalWrite(C_PIN_R3, HIGH);
            digitalWrite(C_PIN_EN_DCDC, HIGH);
            digitalWrite(C_PIN_OP_SWITCH, LOW);
            r_sel = 3;
        }
        switch (r_sel)
        {
        case 1:
            display.setCursor(0, 40);
            display.print("OP_SWITCH");
            display.setCursor(64, 40);
            display.print("-> OFF");
            display.setCursor(0, 50);
            display.print("EN_DCDC");
            display.setCursor(64, 50);
            display.print("-> OFF");
            display.setCursor(100, 0);
            display.printf("1");
            break;
        case 2:
            display.setCursor(0, 40);
            display.print("OP_SWITCH");
            display.setCursor(64, 40);
            display.print("-> ON");
            display.setCursor(0, 50);
            display.print("EN_DCDC");
            display.setCursor(64, 50);
            display.print("-> OFF");
            display.setCursor(100, 0);
            display.printf("2");
            break;
        case 3:
            display.setCursor(0, 40);
            display.print("OP_SWITCH");
            display.setCursor(64, 40);
            display.print("-> OFF");
            display.setCursor(0, 50);
            display.print("EN_DCDC");
            display.setCursor(64, 50);
            display.print("-> ON");
            display.setCursor(100, 0);
            display.printf("3");
            break;

        default:
            display.setCursor(0, 40);
            display.print("OP_SWITCH");
            display.setCursor(64, 40);
            display.print("-> ");
            display.setCursor(0, 50);
            display.print("EN_DCDC");
            display.setCursor(64, 50);
            display.print("-> ");
            break;
        }
    display.setCursor(0, 10);
    display.printf("IO");
    display.setCursor(64, 10);
    display.printf("= %d mV", analogRead(C_PIN_IO_SENSE) * 3300 / 4096);

    display.setCursor(0, 20);
    display.print("VO");
    display.setCursor(64, 20);
    display.printf("= %d mV", analogRead(C_PIN_VOUT_SENSE) * 3300 / 4096);
    volt += 5;
    if (volt == 120)
    {
        volt = 50;
    }
    if (!MCP.isConnected())
    {
        display.setCursor(60, 0);
        display.printf("ERROR", volt);
    }
    else
    {
        display.setCursor(60, 0);
        display.printf("Good", volt);
    }

    dac_count = 1781 - (((volt - MIN_VOLTAGE) * (1781 - 174)) / (MAX_VOLTAGE - MIN_VOLTAGE));
    MCP.setValue(dac_count);
    display.setCursor(0, 30);
    display.print("FB_DAC");
    display.setCursor(64, 30);
    display.printf("= %d mV", volt);
    display.display();
    delay(250);
    }*/

    display.begin();
    display.clearDisplay();
    status = C_TEST_0;

    while (flag_testing == true)
    {
        if (digitalRead(C_PIN_TEST_1) == LOW)
        {
            digitalWrite(C_PIN_R1, LOW);
            digitalWrite(C_PIN_R2, LOW);
            digitalWrite(C_PIN_R3, LOW);
            // EN_DCDC = OFF - OP_SWITCH = OFF
            digitalWrite(C_PIN_EN_DCDC, HIGH);
            digitalWrite(C_PIN_OP_SWITCH, HIGH);
            status = C_TEST_0;
            fase = C_FASE_0;
            timer_t1.set(2000);
            display.clearDisplay();
        }
        else if (digitalRead(C_PIN_TEST_2) == LOW)
        {
            digitalWrite(C_PIN_R1, LOW);
            digitalWrite(C_PIN_R2, LOW);
            digitalWrite(C_PIN_R3, LOW);
            display.clearDisplay();
            display.setTextSize(2);
            display.setTextColor(WHITE);
            // EN_DCDC = ON - OP_SWITCH = ON
            digitalWrite(C_PIN_EN_DCDC, LOW);
            digitalWrite(C_PIN_OP_SWITCH, LOW);
            volt_test_2 = 40;
            flag_dac_test = 0;
            status = C_TEST_1;
        }
        else if (digitalRead(C_PIN_TEST_3) == LOW)
        {
            flag_1w_tested = true;
            flag_4w_tested = false;
            dac_count = 1781 - (((50 - MIN_VOLTAGE) * (1781 - 174)) / (MAX_VOLTAGE - MIN_VOLTAGE));
            MCP.setValue(dac_count);
            display.clearDisplay();
            display.setTextSize(2);
            display.setTextColor(WHITE);
            display.fillRect(64, 0, 1, 64, WHITE);
            display.display();
            digitalWrite(C_PIN_R1, LOW);
            digitalWrite(C_PIN_R2, LOW);
            digitalWrite(C_PIN_R3, HIGH);
            digitalWrite(C_PIN_EN_DCDC, LOW);
            digitalWrite(C_PIN_OP_SWITCH, LOW);
            status = C_TEST_2;
        }

        if (status == C_TEST_0)
        {
            if (fase != C_FASE_4)
            {
                display.setTextSize(2);
                display.setTextColor(WHITE);
                display.fillRect(64, 0, 1, 64, WHITE);
                display.fillRect(0, 32, 128, 1, WHITE);

                dac_count = 1781 - (((100 - MIN_VOLTAGE) * (1781 - 174)) / (MAX_VOLTAGE - MIN_VOLTAGE));
                MCP.setValue(dac_count);

                vout_sample = 0;
                iout_sample = 0;
                for (int i = 0; i < 32; i++)
                {
                    vout_sample += analogRead(C_PIN_VOUT_SENSE);
                    iout_sample += analogRead(C_PIN_IO_SENSE);
                    delay(75);
                }
                vout_sample = vout_sample / 32;
                iout_sample = iout_sample / 32;

                vout_sample = vout_sample * 208 / 39 * 3300 / 4096;
                iout_sample = iout_sample * 3300 / 4096 * 10 / 15;

                if (fase == C_FASE_0)
                {
                    if (vout_sample < C_VOUT_FASE_0)
                    {
                        Serial.println("Fase 0.");
                        Serial.println(vout_sample);
                        fase = C_FASE_1;
                        display.setCursor(10, 10);
                        display.print("YES");
                        timer_t1.set(2000);
                        // EN_DCDC = OFF - OP_SWITCH = ON
                        digitalWrite(C_PIN_EN_DCDC, HIGH);
                        digitalWrite(C_PIN_OP_SWITCH, LOW);
                    }
                    else if ((timer_t1.poll() != C_TIMER_NOT_EXPIRED) || (iout_sample >= C_IOUT_TEST_0_1))
                    {
                        Serial.printf("Fase 0.\n %d/%d\n", vout_sample, iout_sample);
                        timer_t1.set(2000);
                        fase = C_FASE_1;
                        display.setCursor(10, 10);
                        if (iout_sample >= C_IOUT_TEST_0_1)
                        {
                            display.print("!!!");
                        }
                        else
                        {
                            display.print("NO");
                        }

                        // EN_DCDC = OFF - OP_SWITCH = ON
                        digitalWrite(C_PIN_EN_DCDC, HIGH);
                        digitalWrite(C_PIN_OP_SWITCH, LOW);
                    }
                }
                else if (fase == C_FASE_1)
                {

                    if ((vout_sample <= (4000 * UPPER_TOL / 100)) && (vout_sample >= (C_VOUT_FASE_1 * LOWER_TOL / 100)))
                    {
                        Serial.println("Fase 1.");
                        Serial.println(vout_sample);
                        fase = C_FASE_2;
                        display.setCursor(74, 10);
                        display.print("YES");
                        timer_t1.set(2000);
                        // EN_DCDC = ON - OP_SWITCH = OFF
                        digitalWrite(C_PIN_EN_DCDC, LOW);
                        digitalWrite(C_PIN_OP_SWITCH, HIGH);
                    }
                    else if ((timer_t1.poll() != C_TIMER_NOT_EXPIRED) || (iout_sample >= C_IOUT_TEST_0_1))
                    {
                        Serial.printf("Fase 1.\n %d/%d\n", vout_sample, iout_sample);
                        timer_t1.set(2000);
                        fase = C_FASE_2;
                        display.setCursor(74, 10);
                        if (iout_sample >= C_IOUT_TEST_0_1)
                        {
                            display.print("!!!");
                        }
                        else
                        {
                            display.print("NO");
                        }
                        // EN_DCDC = ON - OP_SWITCH = OFF
                        digitalWrite(C_PIN_EN_DCDC, LOW);
                        digitalWrite(C_PIN_OP_SWITCH, HIGH);
                    }
                }
                else if (fase == C_FASE_2)
                {

                    if (vout_sample < C_VOUT_FASE_2)
                    {
                        fase = C_FASE_3;
                        Serial.println("Fase 2.");
                        Serial.println(vout_sample);
                        display.setCursor(10, 42);
                        display.print("YES");
                        timer_t1.set(2000);
                        // EN_DCDC = ON - OP_SWITCH = ON
                        digitalWrite(C_PIN_EN_DCDC, LOW);
                        digitalWrite(C_PIN_OP_SWITCH, LOW);
                    }
                    else if ((timer_t1.poll() != C_TIMER_NOT_EXPIRED) || (iout_sample >= C_IOUT_TEST_0_1))
                    {
                        Serial.printf("Fase 2.\n %d/%d\n", vout_sample, iout_sample);
                        timer_t1.set(2000);
                        fase = C_FASE_3;
                        display.setCursor(10, 42);
                        if (iout_sample >= C_IOUT_TEST_0_1)
                        {
                            display.print("!!!");
                        }
                        else
                        {
                            display.print("NO");
                        }
                        // EN_DCDC = ON - OP_SWITCH = ON
                        digitalWrite(C_PIN_EN_DCDC, LOW);
                        digitalWrite(C_PIN_OP_SWITCH, LOW);
                    }
                }
                else if (fase == C_FASE_3)
                {
                    if ((vout_sample >= (C_VOUT_FASE_3 * LOWER_TOL / 100)) && (vout_sample <= (C_VOUT_FASE_3 * UPPER_TOL / 100)))
                    {
                        fase = C_FASE_4;
                        Serial.println("Fase 3.");
                        Serial.println(vout_sample);
                        display.setCursor(74, 42);
                        display.print("YES");
                        timer_t1.set(2000);
                    }
                    else if ((timer_t1.poll() != C_TIMER_NOT_EXPIRED) || (iout_sample >= C_IOUT_TEST_0_1))
                    {
                        Serial.printf("Fase 3.\n %d/%d\n", vout_sample, iout_sample);
                        timer_t1.set(2000);
                        fase = C_FASE_4;
                        display.setCursor(74, 42);
                        if (iout_sample >= C_IOUT_TEST_0_1)
                        {
                            display.print("!!!");
                        }
                        else
                        {
                            display.print("NO");
                        }
                    }
                }
                display.display();
            }
        }
        else if (status == C_TEST_1)
        {
            if (flag_dac_test == 0)
            {
                if (volt_test_2 < MAX_VOLTAGE)
                {
                    volt_test_2 += 10;
                    dac_count = 1781 - (((volt_test_2 - MIN_VOLTAGE) * (1781 - 174)) / (MAX_VOLTAGE - MIN_VOLTAGE));
                    MCP.setValue(dac_count);
                    display.clearDisplay();
                    display.setCursor(30, 30);
                    display.print(volt_test_2 / 10);
                    display.display();
                    delay(1000);
                    vout_sample = 0;
                    iout_sample = 0;
                    for (int i = 0; i < 32; i++)
                    {
                        vout_sample += analogRead(C_PIN_VOUT_SENSE);
                        iout_sample += analogRead(C_PIN_IO_SENSE);
                        delay(75);
                    }
                    vout_sample = vout_sample / 32;
                    iout_sample = iout_sample / 32;

                    vout_sample = vout_sample * 208 / 39 * 3300 / 4096;
                    iout_sample = iout_sample * 3300 / 4096 * 10 / 15;

                    if ((vout_sample >= (volt_test_2 * LOWER_TOL)) && (vout_sample <= (volt_test_2 * UPPER_TOL)) && (iout_sample <= C_IOUT_TEST_0_1))
                    {
                        display.setCursor(70, 30);
                        display.print("GOOD");
                    }
                    else
                    {
                        flag_dac_test = 2;
                        display.setCursor(70, 30);
                        display.print("BAD");
                    }
                    Serial.printf("%d.\n %d/%d\n", volt_test_2, vout_sample, iout_sample);
                    display.display();
                }
                else if (flag_dac_test == 0)
                {
                    flag_dac_test = 2;
                }

                display.display();
            }
            else if (flag_dac_test == 2)
            {
            }
        }
        else if (status == C_TEST_2)
        {
            if (flag_1w_tested == true)
            {
                vout_sample = 0;
                iout_sample = 0;
                for (int i = 0; i < 32; i++)
                {
                    vout_sample += analogRead(C_PIN_VOUT_SENSE);
                    iout_sample += analogRead(C_PIN_IO_SENSE);
                    delay(75);
                }
                vout_sample = vout_sample / 32;
                iout_sample = iout_sample / 32;

                vout_sample = vout_sample * 208 / 39 * 3300 / 4096;
                iout_sample = iout_sample * 3300 / 4096 * 10 / 15;
                Serial.println(vout_sample);
                Serial.println(iout_sample);
                display.setCursor(10, 30);
                display.print(vout_sample);
                display.setCursor(10, 50);
                display.print(iout_sample);

                if ((iout_sample >= (200 * LOWER_TOL / 100)) && (iout_sample <= (200 * UPPER_TOL / 100)))
                {
                    if ((vout_sample >= (5000 * LOWER_TOL / 100)) && (vout_sample <= (5000 * UPPER_TOL / 100)))
                    {
                        display.setCursor(10, 0);
                        display.print("GOOD");
                        flag_1w_tested = false;
                        flag_4w_tested = true;
                        dac_count = 1781 - (((100 - MIN_VOLTAGE) * (1781 - 174)) / (MAX_VOLTAGE - MIN_VOLTAGE));
                        MCP.setValue(dac_count);
                    }
                    else
                    {
                        display.setCursor(10, 0);
                        display.print("GOOD");
                        flag_1w_tested = false;
                        flag_4w_tested = true;
                        dac_count = 1781 - (((100 - MIN_VOLTAGE) * (1781 - 174)) / (MAX_VOLTAGE - MIN_VOLTAGE));
                        MCP.setValue(dac_count);
                    }
                }
                else
                {
                    display.setCursor(10, 0);
                    display.print("BAD");
                    flag_1w_tested = false;
                    flag_4w_tested = true;
                    dac_count = 1781 - (((100 - MIN_VOLTAGE) * (1781 - 174)) / (MAX_VOLTAGE - MIN_VOLTAGE));
                    MCP.setValue(dac_count);
                }
            }
            else if (flag_4w_tested == true)
            {
                vout_sample = 0;
                iout_sample = 0;
                for (int i = 0; i < 32; i++)
                {
                    vout_sample += analogRead(C_PIN_VOUT_SENSE);
                    iout_sample += analogRead(C_PIN_IO_SENSE);
                    delay(75);
                }
                vout_sample = vout_sample / 32;
                iout_sample = iout_sample / 32;

                vout_sample = vout_sample * 208 / 39 * 3300 / 4096;
                iout_sample = iout_sample * 3300 / 4096 * 10 / 15;
                Serial.println(vout_sample);
                Serial.println(iout_sample);
                display.setCursor(80, 30);
                display.print(vout_sample);
                display.setCursor(80, 50);
                display.print(iout_sample);

                if ((iout_sample >= (400 * LOWER_TOL / 100)) && (iout_sample <= (400 * UPPER_TOL / 100)))
                {
                    if ((vout_sample >= (10000 * 95 / 100)) && (vout_sample <= (10000 * 105 / 100)))
                    {
                        display.setCursor(80, 0);
                        display.print("GOOD");
                        flag_1w_tested = false;
                        flag_4w_tested = false;
                        digitalWrite(C_PIN_R1, LOW);
                        digitalWrite(C_PIN_R2, LOW);
                        digitalWrite(C_PIN_R3, LOW);
                        digitalWrite(C_PIN_EN_DCDC, LOW);
                        digitalWrite(C_PIN_OP_SWITCH, HIGH);
                    }
                    else
                    {
                        display.setCursor(80, 0);
                        display.print("BAD");
                        flag_1w_tested = false;
                        flag_4w_tested = false;
                        digitalWrite(C_PIN_R1, LOW);
                        digitalWrite(C_PIN_R2, LOW);
                        digitalWrite(C_PIN_R3, LOW);
                        digitalWrite(C_PIN_EN_DCDC, LOW);
                        digitalWrite(C_PIN_OP_SWITCH, HIGH);
                    }
                }
                else
                {
                    display.setCursor(80, 0);
                    display.print("BAD");
                    flag_1w_tested = false;
                    flag_4w_tested = false;
                    digitalWrite(C_PIN_R1, LOW);
                    digitalWrite(C_PIN_R2, LOW);
                    digitalWrite(C_PIN_R3, LOW);
                    digitalWrite(C_PIN_EN_DCDC, LOW);
                    digitalWrite(C_PIN_OP_SWITCH, HIGH);
                }
            }
            display.display();
        }
    }
}

void loop()
{
}