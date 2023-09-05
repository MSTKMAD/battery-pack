/**
 * @file sw_testbenchdk.ino
 * @author Javier (Javier@musotoku.com)
 * @brief
 * @version 0.1
 * @date 2023-08-22
 *
 * @copyright Copyright (c) 2023
 *
 */

const int16_t C_PIN_S1 = 2;
const int16_t C_PIN_S2 = 3;
const int16_t C_PIN_SDA = 4;
const int16_t C_PIN_SCL = 5;
const int16_t C_PIN_S3 = 6;
const int16_t C_PIN_R1 = 7;
const int16_t C_PIN_R2 = 8;
const int16_t C_PIN_R3 = 9;
const int16_t C_PIN_R4 = 10;
const int16_t C_PIN_R5 = 11;
const int16_t C_PIN_OPTO1 = 12;
const int16_t C_PIN_OPTO2 = 13;
const int16_t C_PIN_PWM1 = 14;
const int16_t C_PIN_PWM2 = 15;
const int16_t C_PIN_RST = 17;
const int16_t C_PIN_SCK = 18;
const int16_t C_PIN_MOSI = 19;
const int16_t C_PIN_MISO = 20;
const int16_t C_PIN_SS = 21;
const int16_t C_PIN_I_IN_SENSE = A2;
const int16_t C_PIN_I_OUT_SENSE = A1;

#include <MilliTimer.h>
#include "Adafruit_SSD1306_096.h"

Adafruit_SSD1306 display(128, 64, C_PIN_MOSI, C_PIN_SCK, C_PIN_MISO, C_PIN_RST, C_PIN_SS);

int16_t out_resistance = 0;

void setup()
{
    Serial.begin(9600);

    Serial.println("Im alive");
    display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
    display.clearDisplay();
    display.display();
    display.setTextColor(WHITE); // Color por defecto del texto.
    display.setTextSize(3);
    display.setCursor(40, 20);
    display.printf("OK");
    display.display();

    pinMode(C_PIN_R1, OUTPUT);
    pinMode(C_PIN_R2, OUTPUT);
    pinMode(C_PIN_R3, OUTPUT);
    pinMode(C_PIN_R4, OUTPUT);
    pinMode(C_PIN_R5, OUTPUT);
}

void loop()
{
    if (digitalRead(C_PIN_S1) == HIGH)
    {
        out_resistance++;
        delay(1000);
    }
    if (digitalRead(C_PIN_S2) == HIGH)
    {
        out_resistance++;
        delay(1000);
    }
    if (digitalRead(C_PIN_S3) == HIGH)
    {
        out_resistance++;
        delay(1000);
    }
    switch (out_resistance)
    {
    case 0:
        digitalWrite(C_PIN_R1, LOW);
        digitalWrite(C_PIN_R2, LOW);
        digitalWrite(C_PIN_R3, LOW);
        digitalWrite(C_PIN_R4, LOW);
        digitalWrite(C_PIN_R5, LOW);

        break;
    case 1:
        digitalWrite(C_PIN_R1, HIGH);
        digitalWrite(C_PIN_R2, LOW);
        digitalWrite(C_PIN_R3, LOW);
        digitalWrite(C_PIN_R4, LOW);
        digitalWrite(C_PIN_R5, LOW);
        break;
    case 2:
        digitalWrite(C_PIN_R1, LOW);
        digitalWrite(C_PIN_R2, HIGH);
        digitalWrite(C_PIN_R3, LOW);
        digitalWrite(C_PIN_R4, LOW);
        digitalWrite(C_PIN_R5, LOW);
        break;
    case 3:
        digitalWrite(C_PIN_R1, LOW);
        digitalWrite(C_PIN_R2, LOW);
        digitalWrite(C_PIN_R3, HIGH);
        digitalWrite(C_PIN_R4, LOW);
        digitalWrite(C_PIN_R5, LOW);
        break;
    case 4:
        digitalWrite(C_PIN_R1, LOW);
        digitalWrite(C_PIN_R2, LOW);
        digitalWrite(C_PIN_R3, LOW);
        digitalWrite(C_PIN_R4, HIGH);
        digitalWrite(C_PIN_R5, LOW);
        break;
    case 5:
        digitalWrite(C_PIN_R1, LOW);
        digitalWrite(C_PIN_R2, LOW);
        digitalWrite(C_PIN_R3, LOW);
        digitalWrite(C_PIN_R4, LOW);
        digitalWrite(C_PIN_R5, HIGH);
        break;
    case 6:
        out_resistance = 0;
        break;
    default:
        break;
    }
    display.clearDisplay();
    display.setCursor(40, 20);
    display.print(out_resistance);
    display.display();
}
