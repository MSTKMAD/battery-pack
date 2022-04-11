/**
 * @file Test_Production.ino
 * @author Javi (Javier@musotoku.com)
 * @brief 
 * @version 1
 * @date 2021-06-30
 * 
 * @copyright Copyright (c) 2021
 * 
 */

/**
 *      CONSTANTS
 */

//      PINS

const uint16_t C_PIN_SW1 = 2;
const uint16_t C_PIN_SW2 = 3;
const uint16_t C_PIN_ENABLE_LDO = 5;
const uint16_t C_PIN_SDA_2 = 7;
const uint16_t C_PIN_SCL_2 = 8;
const uint16_t C_PIN_ISENSE_RAW = 10;
const uint16_t C_PIN_IOUT_SENSE = A0;
const uint16_t C_PIN_VCC_2 = A1;
const uint16_t C_PIN_VCC = A2;
const uint16_t C_PIN_LED_1 = 4;
const uint16_t C_PIN_LED_2 = 6;
const uint16_t C_PIN_LED_3 = 9;

const uint16_t version_PCB = 5;
/**
 *      GLOBAL VARIABLES
 */

char data = 0;
int32_t sample_vcc = 0;
int32_t sample_vcc_2 = 0;
int32_t sample_iout_sense = 0;
int32_t cont = 0;

bool sda_pin = true;
bool scl_pin = false;
/**
 * FLAGS
 */
bool test_start = false;

void setup()
{
    Serial.begin(9600);
    pinMode(C_PIN_LED_1, OUTPUT);
    pinMode(C_PIN_LED_2, OUTPUT);
    pinMode(C_PIN_LED_3, OUTPUT);
    pinMode(C_PIN_ENABLE_LDO, OUTPUT);
    pinMode(C_PIN_ISENSE_RAW, OUTPUT);
    pinMode(C_PIN_SW1, OUTPUT);
    pinMode(C_PIN_SW2, OUTPUT);
    pinMode(C_PIN_VCC, INPUT);
    pinMode(C_PIN_VCC_2, INPUT);
    pinMode(C_PIN_SDA_2, INPUT);
    pinMode(C_PIN_SCL_2, INPUT);
    digitalWrite(C_PIN_LED_1, LOW);
    digitalWrite(C_PIN_LED_2, LOW);
    digitalWrite(C_PIN_LED_3, LOW);
    // while (test_start == false)
    // {
    //     if (Serial.available())
    //     {
    //         data = Serial.readString();
    //         data.trim();
    //         Serial.println(data);
    //         if (data == "Start test")
    //         {
    //             test_start = true;
    //         }
    //     }
    //     data = "";
    // }
    digitalWrite(C_PIN_ENABLE_LDO, HIGH);
    digitalWrite(C_PIN_ISENSE_RAW, LOW);
    digitalWrite(C_PIN_SW1, HIGH);
    digitalWrite(C_PIN_SW2, LOW);
    digitalWrite(C_PIN_LED_1, LOW);
    digitalWrite(C_PIN_LED_2, LOW);
    digitalWrite(C_PIN_LED_3, LOW);
}

void loop()
{
    sample_vcc_2 = 0;
    sample_vcc = 0;
    for (int i = 0; i < 8; i++)
    {
        sample_vcc += analogRead(C_PIN_VCC);

        sample_vcc_2 += analogRead(C_PIN_VCC_2);
        delay(10);
    }
    sample_vcc_2 = sample_vcc_2 / 8;
    sample_vcc = sample_vcc / 8;
    delay(100);
    Serial.print("Vcc:");
    Serial.println(sample_vcc);
    Serial.print("Vcc_2:");
    Serial.println(sample_vcc_2);
    Serial.print("SDA:");
    Serial.println(digitalRead(C_PIN_SDA_2));
    Serial.print("SCL:");
    Serial.println(digitalRead(C_PIN_SCL_2));

    if (sda_pin != digitalRead(C_PIN_SDA_2))
    {
        if (cont == 0)
        {
            cont++;
        }
    }
    if (scl_pin != digitalRead(C_PIN_SCL_2))
    {
        if (cont == 1)
        {
            cont++;
        }
    }
    if (cont == 2)
    {
        digitalWrite(C_PIN_LED_3, HIGH);
        cont = 0;
    }

    if ((sample_vcc >= (3 * 1024 / 5)) && (sample_vcc <= (3.5 * 1024 / 5)))
    {
        digitalWrite(C_PIN_LED_1, HIGH);
    }
    else
    {
        digitalWrite(C_PIN_LED_1, LOW);
    }

    if ((sample_vcc_2 >= (3 * 1024 / 5)) && (sample_vcc_2 <= (3.5 * 1024 / 5)))
    {
        digitalWrite(C_PIN_LED_2, HIGH);
    }
    else
    {
        digitalWrite(C_PIN_LED_2, LOW);
    }

    if (Serial.available())
    {
        data = Serial.read();
        Serial.println(data);

        switch (data)
        {
            // Estado base, Vbus = 4v. Iraw = OFF, Enable = ON.
        case '0':
            digitalWrite(C_PIN_ENABLE_LDO, HIGH);
            digitalWrite(C_PIN_ISENSE_RAW, LOW);
            digitalWrite(C_PIN_SW1, HIGH);
            digitalWrite(C_PIN_SW2, LOW);
            break;

            // BattLow, Vbus = 3v. Iraw = OFF, Enable = ON.
        case '2':
            digitalWrite(C_PIN_ENABLE_LDO, HIGH);
            digitalWrite(C_PIN_ISENSE_RAW, LOW);
            digitalWrite(C_PIN_SW1, LOW);
            digitalWrite(C_PIN_SW2, LOW);
            break;

            // Charge, Vbus = 5v. Iraw = OFF, Enable = ON.
        case '1':
            digitalWrite(C_PIN_ENABLE_LDO, HIGH);
            digitalWrite(C_PIN_ISENSE_RAW, LOW);
            digitalWrite(C_PIN_SW1, LOW);
            digitalWrite(C_PIN_SW2, HIGH);
            break;

            // Sensado Corriente, Vbus = 4v. Iraw = ON, Enable = ON.
        case '3':
            digitalWrite(C_PIN_ENABLE_LDO, HIGH);
            digitalWrite(C_PIN_ISENSE_RAW, HIGH);
            digitalWrite(C_PIN_SW1, HIGH);
            digitalWrite(C_PIN_SW2, LOW);
            break;

            // Apagado, Vbus = 0v. Iraw = OFF, Enable = OFF.
        case '4':
            digitalWrite(C_PIN_ENABLE_LDO, LOW);
            digitalWrite(C_PIN_ISENSE_RAW, LOW);
            digitalWrite(C_PIN_SW1, HIGH);
            digitalWrite(C_PIN_SW2, LOW);
            break;

        default:
            break;
        }
    }
}