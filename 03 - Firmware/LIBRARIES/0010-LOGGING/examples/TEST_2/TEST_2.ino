/**
 * @file TEST_2.ino
 * @author Javi (Javier@musotoku.com)
 * @brief Test del guardado y lectura de la eeprom chasis. Para ello se utiliza y prueba la funcion de estadisticas.
 * @version 1
 * @date 2021-03-22
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
    uint16_t voltage = 0;
    uint16_t power = 0;
    Init_diagnostic_elements();
    for (int i = 0; i < 30; i++)
    {
        voltage = random(5000, 16000);
        power = random(500, 5000);
        Serial.print(voltage);
        Serial.print(";");
        Serial.println(power);
        Stats(voltage, power);
    }
    PrintStats();
    Serial.println();
    int t0 = millis();
    UpdateEepromBatery();
    int t1 = millis();
    Serial.println(t1 - t0);
}

void loop()
{
}