/**
 * @file Test_Shield_Validacion_input.ino
 * @author Javi (Javier@musotoku.com)
 * @brief 
 * @version 0.1
 * @date 2021-12-01
 * 
 * @copyright Copyright (c) 2021
 * 
 */
#include <EEPROM.h>

const int16_t C_SWITCH = 2;
const int16_t C_CHG = 6;
const int16_t C_LOW_OUTPUT = 8;
const int16_t C_VOLTAGE_BAT_DISCHARGE = A0;
const int16_t C_ISENSE = A1;
const int16_t C_VSYS = A2;

int16_t version_input_pcb_dut = 4;
int32_t voltage_bat_discharge = 0, isense = 0, vsys = 0;
bool chg_state = LOW;
char data;

void setup()
{
    pinMode(C_SWITCH, OUTPUT);
    pinMode(C_CHG, INPUT);
    pinMode(C_LOW_OUTPUT, OUTPUT);
    pinMode(C_VOLTAGE_BAT_DISCHARGE, INPUT);
    pinMode(C_ISENSE, INPUT);
    pinMode(C_VSYS, INPUT);
    Serial.begin(9600);

    digitalWrite(C_SWITCH, HIGH);
    digitalWrite(C_LOW_OUTPUT, LOW);

    // pinMode(C_CHG, OUTPUT);
    // while (1)
    // {
    //     digitalWrite(C_CHG, HIGH);

    //     delay(1000);
    //     digitalWrite(C_CHG, LOW);
    //     delay(1000);
    //     /* code */
    // }
}

void loop()
{
    digitalWrite(C_SWITCH, HIGH);
    digitalWrite(C_LOW_OUTPUT, LOW);

    Serial.println("Test de Validacion del PCB del INPUT del proyecto Battery Pack Rover 2k.");
    Serial.print("Version de la placa:");
    Serial.println(version_input_pcb_dut);
    Serial.println("1) Test 1: Default Conexion");
    Serial.println("2) Test 2: R/W EEPROM");
    Serial.println("3) Test 3: Charge");
    Serial.println("4) Test 4: Low Batt");
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
            digitalWrite(C_SWITCH, HIGH);
            digitalWrite(C_LOW_OUTPUT, LOW);

            break;

        case '2':
            Serial.println("Test 2: Running...");

            Serial.println("Test 2 Finished.");
            break;

        case '3':
            Serial.println("Test 3 Running ...");
            digitalWrite(C_SWITCH, HIGH);
            digitalWrite(C_LOW_OUTPUT, LOW);
            break;

        case '4':
            Serial.println("Test 4 Running ...");
            digitalWrite(C_SWITCH, HIGH);
            digitalWrite(C_LOW_OUTPUT, HIGH);
            break;

        case '5':
            Serial.println("Test 5 Running ...");
            digitalWrite(C_SWITCH, HIGH);
            digitalWrite(C_LOW_OUTPUT, LOW);
            break;

        default:
            break;
        }
        while (data != '\n')
        {
            data = Serial.read();
        }
        data = ' ';
        if (data != '2')
        {
            while (data != '\n')
            {
                data = Serial.read();
                isense = adc2v(analogRead(C_ISENSE));
                voltage_bat_discharge = adc2v(analogRead(C_VOLTAGE_BAT_DISCHARGE));
                vsys = adc2v(analogRead(C_VSYS));
                chg_state = digitalRead(C_CHG);

                Serial.print("Vsys: ");
                Serial.println(vsys);
                Serial.print("VBD: ");
                Serial.println(voltage_bat_discharge);
                Serial.print("Isense: ");
                Serial.println(isense);
                Serial.print("Charge Status:");
                if (chg_state == HIGH)
                {
                    Serial.println("ON");
                }
                else
                {
                    Serial.println("OFF");
                }

                Serial.println();
                delay(1500);
            }
            Serial.println("Test finished.");
        }
    }
}
int32_t adc2v(int32_t adc_measure)
{
    int32_t voltage;
    voltage = adc_measure * 4500 / 1024;
    return voltage;
}