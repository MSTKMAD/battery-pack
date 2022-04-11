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
/**
 *      GLOBAL VARIABLES
 */

char data = 0;
int16_t sample_vcc = 0;
int16_t sample_vcc_2 = 0;
int16_t sample_iout_sense = 0;

bool sda_pin;
bool scl_pin;
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
    for (int i = 0; i < 8; i++)
    {
        sample_iout_sense += analogRead(C_PIN_IOUT_SENSE);
        sample_vcc += analogRead(C_PIN_VCC);
        sample_vcc_2 += analogRead(C_PIN_VCC_2);
    }
    sample_vcc_2 = sample_vcc_2 / 8;
    sample_vcc = sample_vcc / 8;
    sample_iout_sense = sample_iout_sense / 8;
    sda_pin = digitalRead(C_PIN_SDA_2);
    scl_pin = digitalRead(C_PIN_SCL_2);

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
            digitalWrite(C_PIN_LED_1, LOW);
            digitalWrite(C_PIN_LED_2, LOW);
            digitalWrite(C_PIN_LED_3, LOW);
            break;

            // BattLow, Vbus = 3v. Iraw = OFF, Enable = ON.
        case '1':
            digitalWrite(C_PIN_ENABLE_LDO, HIGH);
            digitalWrite(C_PIN_ISENSE_RAW, LOW);
            digitalWrite(C_PIN_SW1, LOW);
            digitalWrite(C_PIN_SW2, LOW);
            digitalWrite(C_PIN_LED_1, HIGH);
            digitalWrite(C_PIN_LED_2, LOW);
            digitalWrite(C_PIN_LED_3, LOW);
            break;

            // Charge, Vbus = 5v. Iraw = OFF, Enable = ON.
        case '2':
            digitalWrite(C_PIN_ENABLE_LDO, HIGH);
            digitalWrite(C_PIN_ISENSE_RAW, LOW);
            digitalWrite(C_PIN_SW1, LOW);
            digitalWrite(C_PIN_SW2, HIGH);
            digitalWrite(C_PIN_LED_1, LOW);
            digitalWrite(C_PIN_LED_2, HIGH);
            digitalWrite(C_PIN_LED_3, LOW);
            break;

            // Sensado Corriente, Vbus = 4v. Iraw = ON, Enable = ON.
        case '3':
            digitalWrite(C_PIN_ENABLE_LDO, HIGH);
            digitalWrite(C_PIN_ISENSE_RAW, HIGH);
            digitalWrite(C_PIN_SW1, HIGH);
            digitalWrite(C_PIN_SW2, LOW);
            digitalWrite(C_PIN_LED_1, LOW);
            digitalWrite(C_PIN_LED_2, LOW);
            digitalWrite(C_PIN_LED_3, HIGH);
            break;

            // Apagado, Vbus = 0v. Iraw = OFF, Enable = OFF.
        case '4':
            digitalWrite(C_PIN_ENABLE_LDO, LOW);
            digitalWrite(C_PIN_ISENSE_RAW, LOW);
            digitalWrite(C_PIN_SW1, HIGH);
            digitalWrite(C_PIN_SW2, LOW);
            digitalWrite(C_PIN_LED_1, HIGH);
            digitalWrite(C_PIN_LED_2, HIGH);
            digitalWrite(C_PIN_LED_3, HIGH);
            break;

        default:
            break;
        }
    }

    delay(1000);
    Serial.print("\t");
    Serial.print(sample_vcc);
    Serial.print(";");
    Serial.print(sample_vcc_2);
    Serial.print(";");
    Serial.print(sample_iout_sense);
    Serial.print(";");
    Serial.print(scl_pin);
    Serial.print(";");
    Serial.print(sda_pin);
    Serial.print(";");
    Serial.print(sample_iout_sense);
    Serial.print("\n");
}