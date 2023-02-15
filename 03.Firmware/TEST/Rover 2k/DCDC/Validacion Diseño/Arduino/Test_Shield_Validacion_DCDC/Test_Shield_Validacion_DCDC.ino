/**
 * @file Test_Shield_Validacion_DCDC.ino
 * @author Javi (Javier@musotoku.com)
 * @brief Archivo para la validacion del PCB del DCDC poniendo a prueba todas las funcionalidades a desarrollar ademas puntos de trabajo limites que deberia soportar a  futuro.
 * @version 1
 * @date 2021-11-25
 * 
 * @copyright Copyright (c) 2021
 * 
 */
#include <DCDC.h>

const int16_t C_PIN_ENABLE_R1 = 4;
const int16_t C_PIN_ENABLE_R2 = 7;
const int16_t C_PIN_EN_DCDC = 2;
const int16_t C_PIN_OP_SWITCH = 3;
const int16_t C_PIN_ISENSE_RAW = A1;
const int16_t C_PIN_VOUT = A0;

int32_t i_sense_raw;
int32_t v_out;
int32_t version_dcdc_pcb_dut;
char data;

dcdc_controler DCDC(C_PIN_EN_DCDC);

void setup()
{
    pinMode(C_PIN_ENABLE_R1, OUTPUT);
    pinMode(C_PIN_ENABLE_R2, OUTPUT);
    pinMode(C_PIN_EN_DCDC, OUTPUT);
    pinMode(C_PIN_OP_SWITCH, OUTPUT);
    pinMode(C_PIN_ISENSE_RAW, INPUT);
    pinMode(C_PIN_VOUT, INPUT);

    digitalWrite(C_PIN_ENABLE_R1, LOW);
    digitalWrite(C_PIN_ENABLE_R2, LOW);
    digitalWrite(C_PIN_EN_DCDC, LOW);
    digitalWrite(C_PIN_OP_SWITCH, LOW);

    Serial.begin(9600);
    Wire.begin();
}

void loop()
{
    i_sense_raw = 0;
    v_out = 0;
    for (int i = 0; i < 8; i++)
    {
        i_sense_raw += analogRead(C_PIN_ISENSE_RAW);
        v_out += analogRead(C_PIN_VOUT);
    }
    i_sense_raw /= 8;
    v_out /= 8;

    Serial.println("Test de Validacion del PCB de la CPU del proyecto Battery Pack Rover 2k.");
    Serial.print("Version de la placa:");
    Serial.println(version_dcdc_pcb_dut);
    Serial.println("1) Test 1: Enable DCDC y Output");
    Serial.println("2) Test 2: TPIC , Rango de salida y Iraw 0W");
    Serial.println("3) Test 3: Iraw 1W");
    Serial.println("4) Test 4: Iraw 4W");
    Serial.println("5) Test 5: Cortocircuito");

    while (!Serial.available())
    {
        /* code */
    }

    if (Serial.available())
    {
        data = Serial.read();
        switch (data)
        {
        case '1':
            Serial.println("Test 1 Running ...");

            Serial.println("EN_DCDC = 0\tOP_SWITCH = 0");
            digitalWrite(C_PIN_EN_DCDC, LOW);
            digitalWrite(C_PIN_OP_SWITCH, LOW);
            delay(1500);
            v_out = analogRead(C_PIN_VOUT);
            Serial.print("Vout:");
            Serial.println(adc2v(v_out));

            Serial.println("EN_DCDC = 0\tOP_SWITCH = 1");
            digitalWrite(C_PIN_EN_DCDC, LOW);
            digitalWrite(C_PIN_OP_SWITCH, HIGH);
            delay(1500);
            v_out = analogRead(C_PIN_VOUT);
            Serial.print("Vout:");
            Serial.println(adc2v(v_out));

            Serial.println("EN_DCDC = 1\tOP_SWITCH = 0");
            digitalWrite(C_PIN_EN_DCDC, HIGH);
            digitalWrite(C_PIN_OP_SWITCH, LOW);
            delay(1500);
            v_out = analogRead(C_PIN_VOUT);
            Serial.print("Vout:");
            Serial.println(adc2v(v_out));

            Serial.println("EN_DCDC = 1\tOP_SWITCH = 1");
            digitalWrite(C_PIN_EN_DCDC, HIGH);
            digitalWrite(C_PIN_OP_SWITCH, HIGH);
            delay(1500);
            v_out = analogRead(C_PIN_VOUT);
            Serial.print("Vout:");
            Serial.println(adc2v(v_out));

            Serial.println("Test finished.");

            break;

        case '2':
            Serial.println("Test 2: Running...");
            digitalWrite(C_PIN_EN_DCDC, LOW);
            digitalWrite(C_PIN_OP_SWITCH, HIGH);
            for (int i = 40; i < 161; i++)
            {
                Serial.println("Theory voltage / Vout:");
                Serial.print(i * 100);
                Serial.print("/");
                DCDC.SetVoltage(i, C_NON_BOOST_MODE);
                delay(500);
                v_out = analogRead(C_PIN_VOUT);
                Serial.println(adc2v(v_out));
                i_sense_raw = analogRead(C_PIN_ISENSE_RAW);
                Serial.print("IsenseRaw:");
                Serial.println(i_sense_raw);
            }
            Serial.println("Test 2 Finished.");
            break;

        case '3':
            Serial.println("Test 3 Running ...");
            digitalWrite(C_PIN_EN_DCDC, LOW);
            digitalWrite(C_PIN_OP_SWITCH, HIGH);
            DCDC.SetVoltage(70, C_NON_BOOST_MODE);
            digitalWrite(C_PIN_ENABLE_R1, HIGH);
            while (data != '\n')
            {
                data = Serial.read();
            }
            data = "";
            while (data != '\n')
            {
                data = Serial.read();
                i_sense_raw = analogRead(C_PIN_ISENSE_RAW);
                Serial.print("IsenseRaw:");
                Serial.println(i_sense_raw * 5000 / 1024 * 100 / 32);
                delay(100);
                Serial.println(i_sense_raw);
                delay(100);
                v_out = analogRead(C_PIN_VOUT);
                Serial.print("Vout:");
                Serial.println(adc2v(v_out));
                delay(1000);
            }
            digitalWrite(C_PIN_ENABLE_R1, LOW);
            Serial.println("Test 3 Finished");
            break;

        case '4':
            Serial.println("Test 4 Running ...");
            digitalWrite(C_PIN_EN_DCDC, LOW);
            digitalWrite(C_PIN_OP_SWITCH, HIGH);
            DCDC.SetVoltage(70, C_NON_BOOST_MODE);
            digitalWrite(C_PIN_ENABLE_R2, HIGH);
            while (data != '\n')
            {
                data = Serial.read();
            }
            data = "";
            while (data != '\n')
            {
                data = Serial.read();
                i_sense_raw = analogRead(C_PIN_ISENSE_RAW);
                Serial.print("IsenseRaw:");
                Serial.println(i_sense_raw * 5000 / 1024 * 100 / 32);
                delay(100);
                Serial.println(i_sense_raw);
                delay(100);
                v_out = analogRead(C_PIN_VOUT);
                Serial.print("Vout:");
                Serial.println(adc2v(v_out));
                delay(500);
            }
            digitalWrite(C_PIN_ENABLE_R2, LOW);
            Serial.println("Test 4 Finished");
            break;

        case '5':
            Serial.println("Test 5 Running ...");
            digitalWrite(C_PIN_EN_DCDC, LOW);
            digitalWrite(C_PIN_OP_SWITCH, HIGH);
            DCDC.SetVoltage(70, C_NON_BOOST_MODE);
            while (data != '\n')
            {
                data = Serial.read();
            }
            data = "";
            while (data != '\n')
            {
                data = Serial.read();
                DCDC.SetVoltage(70, C_NON_BOOST_MODE);
                i_sense_raw = analogRead(C_PIN_ISENSE_RAW);
                Serial.print("IsenseRaw:");
                Serial.println(i_sense_raw * 5000 / 1024 * 100 / 32);
                delay(100);
                Serial.println(i_sense_raw);
                delay(100);
                v_out = analogRead(C_PIN_VOUT);
                Serial.print("Vout:");
                Serial.println(adc2v(v_out));
                delay(500);
            }
            Serial.println("Test 5 Finished");
            break;

        default:
            break;
        }
        while (Serial.available())
        {
            Serial.read();
        }
    }
}
int32_t adc2v(int32_t adc_measure)
{
    int32_t voltage;
    voltage = adc_measure * 5000 / 1024 * 189 / 39;
    return voltage;
}