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
#include "Adafruit_SSD1306_096.h"
#include "MilliTimer.h"
#include <Wire.h>

const uint16_t C_PIN_SW1 = 2;             // 3.2v
const uint16_t C_PIN_SW2 = 3;             // 4.0v
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
    Serial.begin(9600);
    pinMode(C_PIN_SW1, OUTPUT);
    pinMode(C_PIN_SW2, OUTPUT);

    pinMode(C_PIN_TEST_1, INPUT);
    pinMode(C_PIN_TEST_2, INPUT);
    pinMode(C_PIN_TEST_3, INPUT);

    pinMode(C_PIN_ENABLE_LDO_ADJ, OUTPUT);

    pinMode(C_PIN_EN_DCDC, INPUT);
    pinMode(C_PIN_OP_SWITCH, INPUT);

    pinMode(C_PIN_VCC_CPU, INPUT);
    pinMode(C_PIN_VCC_2_CPU, INPUT);
    pinMode(C_PIN_FB_DAC, INPUT);

    // 4 Voltios de Salida por V_USB
    digitalWrite(C_PIN_SW1, LOW);
    digitalWrite(C_PIN_SW1, HIGH);
    digitalWrite(C_PIN_ENABLE_LDO_ADJ, HIGH);

    display.begin();
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