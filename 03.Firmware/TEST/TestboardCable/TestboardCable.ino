/**
 * @file TestboardCable.ino
 * @author your name (you@domain.com)
 * @brief
 * @version 0.1
 * @date 2024-10-01
 *
 * @copyright Copyright (c) 2024
 *
 */

const int C_PIN_A = 17;
const int C_PIN_B = 4;
const int C_PIN_RA = 12;
const int C_PIN_RB = 10;
const int C_PIN_BUTTON = 7;
const int C_PIN_LED_OK = 27;
const int C_PIN_LED_SHT_CRT = 22;
const int C_PIN_LED_CROSSED = 19;
const int C_PIN_LED_OPEN = 21;

int test_ok = false;
int test_ok_b = false;
int test_ok_a = false;
int test_sht_crt = false;
int test_open = false;
int test_crossed = false;

int ra = false;
int rb = false;

void setup()
{
    Serial.begin(9600);
    pinMode(C_PIN_LED_OK, OUTPUT);
    pinMode(C_PIN_LED_SHT_CRT, OUTPUT);
    pinMode(C_PIN_LED_CROSSED, OUTPUT);
    pinMode(C_PIN_LED_OPEN, OUTPUT);
    pinMode(C_PIN_A, OUTPUT);
    pinMode(C_PIN_RA, INPUT);
    pinMode(C_PIN_B, OUTPUT);
    pinMode(C_PIN_RB, INPUT);
}

void loop()
{
    test_ok = false;
    test_ok_a = false;
    test_ok_b = false;
    test_sht_crt = false;
    test_open = false;
    test_crossed = false;
    digitalWrite(C_PIN_LED_CROSSED, test_crossed);
    digitalWrite(C_PIN_LED_OPEN, test_open);
    digitalWrite(C_PIN_LED_OK, test_ok);
    digitalWrite(C_PIN_LED_SHT_CRT, test_sht_crt);

    while (digitalRead(C_PIN_BUTTON))
    {
        digitalWrite(C_PIN_LED_OK, HIGH);
        delay(250);
        digitalWrite(C_PIN_LED_OK, LOW);
        delay(250);
    }
    // Reset Leds

    // Test A
    digitalWrite(C_PIN_A, HIGH);

    delay(10);

    ra = digitalRead(C_PIN_RA);
    rb = digitalRead(C_PIN_RB);
    Serial.println("TEST A:");
    Serial.println(ra);
    Serial.println(rb);

    // Detect Status
    if (ra == HIGH)
    {
        if (rb == HIGH)
        {
            test_open = true;
        }
        else
        {
            test_crossed = true;
        }
    }
    else if (rb == HIGH)
    {
        test_ok_a = true;
    }
    else
    {
        test_sht_crt = true;
    }

    digitalWrite(C_PIN_A, LOW);
    delay(10);

    // Test B
    digitalWrite(C_PIN_B, HIGH);
    delay(10);

    ra = digitalRead(C_PIN_RA);
    rb = digitalRead(C_PIN_RB);

    Serial.println("TEST B:");
    Serial.println(ra);
    Serial.println(rb);

    // Detect Status
    if (rb == HIGH)
    {
        if (ra == HIGH)
        {
            test_open = true;
        }
        else
        {
            test_crossed = true;
        }
    }
    else if (ra == HIGH)
    {
        test_ok_b = true;
    }
    else
    {
        test_sht_crt = true;
    }
    digitalWrite(C_PIN_B, LOW);
    delay(10);

    test_ok = test_ok_a * test_ok_b;
    digitalWrite(C_PIN_LED_CROSSED, test_crossed);
    digitalWrite(C_PIN_LED_OPEN, test_open);
    digitalWrite(C_PIN_LED_OK, test_ok);
    digitalWrite(C_PIN_LED_SHT_CRT, test_sht_crt);

    while (digitalRead(C_PIN_BUTTON))
    {
        /* code */
    }
    while (!digitalRead(C_PIN_BUTTON))
    {
        /* code */
    }
}