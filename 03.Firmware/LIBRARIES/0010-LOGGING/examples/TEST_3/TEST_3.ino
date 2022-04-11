/**
 * @file TEST_3.ino
 * @author Javi (Javier@musotoku.com)
 * @brief  Test que mide los tiempos de lectura y escritura de distitnas cantidades de bytes en la memoria EEPROM del Battery Pack.
 * @version 0.1
 * @date 2021-02-17
 * 
 * @copyright Copyright (c) 2021
 * 
 */

#include <diagnostic.h>
void setup()
{
    Serial.begin(9600);
    while (!Serial)
    {
        /* code */
    }
    Init_diagnostic_elements();
    uint16_t init_address = 0x1000;
    uint8_t data_eeprom;
    uint32_t t0 = 0, t1 = 0;
    Serial.print("Tiempo de escritura de 1 byte: ");
    t0 = micros();
    writeEEPROM(init_address, 0);
    t1 = micros();
    Serial.print(t1 - t0);
    Serial.println(" us");
    Serial.print("Tiempo de lectura de 1 byte: ");
    t0 = micros();
    readEEPROM(init_address, &data_eeprom);
    t1 = micros();
    Serial.print(t1 - t0);
    Serial.println(" us");
    //Serial.println(data_eeprom);

    Serial.print("Tiempo de escritura de 10 bytes: ");
    t0 = micros();
    for (int i = 1; i < 11; i++)
    {
        writeEEPROM(init_address + i, i);
    }
    t1 = micros();
    Serial.print(t1 - t0);
    Serial.println(" us");
    Serial.print("Tiempo de lectura de 10 bytes: ");
    t0 = micros();
    for (int i = 1; i < 11; i++)
    {
        readEEPROM(init_address + i, &data_eeprom);
        //Serial.print(data_eeprom);
        //Serial.print(";");
    }
    t1 = micros();
    Serial.print(t1 - t0);
    Serial.println(" us");

    Serial.print("Tiempo de escritura de 20 bytes: ");
    t0 = micros();
    for (int i = 11; i < 30; i++)
    {
        writeEEPROM(init_address + i, i);
    }
    t1 = micros();
    Serial.print(t1 - t0);
    Serial.println(" us");
    Serial.print("Tiempo de lectura de 20 bytes: ");
    t0 = micros();
    for (int i = 11; i < 31; i++)
    {
        readEEPROM(init_address + i, &data_eeprom);
        //Serial.print(data_eeprom);
        //Serial.print(";");
    }
    t1 = micros();
    Serial.print(t1 - t0);
    Serial.println(" us");
}

void loop()
{
}