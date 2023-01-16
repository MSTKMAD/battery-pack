/**
 * @file TEST_1.ino
 * @author Javi (Javier@musotoku.com)
 * @brief Skecht que prueba la funcionalidad del PostMortem. Llena el Aray para despues mostrarlo.
 * @version 0.1
 * @date 2022-12-14
 * 
 * @copyright Copyright (c) 2022
 * 
 */
#include "diagnostic.h"
void setup()
{
    Serial5.begin(57600);
    local_eeprom = flash_eeprom.read();
    for (int i = 0; i < PM_POSITIONS; i++)
    {
        PostMortemLog(i, i * 2, i * 3, i * 4);
    }
    for (int i = 0; i < PM_POSITIONS; i++)
    {
        Serial5.print("Power:");
        Serial5.print(local_eeprom.pm_power[i]);
        Serial5.print(",");
        Serial5.print("Percent:");
        Serial5.print(local_eeprom.pm_percent[i]);
        Serial5.print(",");
        Serial5.print("Voltage:");
        Serial5.print(local_eeprom.pm_voltage[i]);
        Serial5.print(",");
        Serial5.print("Errors:");
        Serial5.println(local_eeprom.pm_errors[i]);
    }
}

void loop()
{
}