/**
 * @file TEST_0.ino
 * @author Javi (Javier@musotoku.com)
 * @brief 
 * @version 0.1
 * @date 2022-12-13
 * 
 * @copyright Copyright (c) 2022
 * 
 */
#include "batt_FlashStorage.h"
#include "Eeprom_LUT.h"

FlashStorage(flash_eeprom, EEPROM);

EEPROM local_eeprom;

void setup()
{
    Serial5.begin(9600);
    local_eeprom = flash_eeprom.read();
    Serial5.println("Leyendo EEPROM...");
    if (local_eeprom.flag_init == false)
    {
        Serial5.println("EEPROM sin Incializar.");
        Serial5.print("Inicializando...");
        local_eeprom.serial_number = 0001;
        local_eeprom.integrated_version = 0002;
        local_eeprom.work_time = 3;
        local_eeprom.power_errors = 4;
        local_eeprom.consumption_errors = 5;
        local_eeprom.voltage_errors = 6;
        local_eeprom.save_voltage = 7;
        local_eeprom.flag_init = true;
        local_eeprom.flag_corruption = true;
        for (int16_t i = 0; i < 71; i++)
        {
            local_eeprom.array_power_use[i] = 8 + i;
        }
        for (uint16_t i = 0; i < 21; i++)
        {
            local_eeprom.array_percent_use[i] = 79 + i;
        }
        local_eeprom.checksum = 0;
        local_eeprom.checksum += local_eeprom.serial_number;
        local_eeprom.checksum += local_eeprom.integrated_version;
        local_eeprom.checksum += local_eeprom.work_time;
        local_eeprom.checksum += local_eeprom.power_errors;
        local_eeprom.checksum += local_eeprom.consumption_errors;
        local_eeprom.checksum += local_eeprom.voltage_errors;
        local_eeprom.checksum += local_eeprom.save_voltage;
        local_eeprom.checksum += local_eeprom.flag_init;
        local_eeprom.checksum += local_eeprom.flag_corruption;
        for (size_t i = 0; i < 71; i++)
        {
            local_eeprom.checksum += local_eeprom.array_power_use[i];
        }
        for (size_t i = 0; i < 21; i++)
        {
            local_eeprom.checksum += local_eeprom.array_percent_use[i];
        }
        flash_eeprom.write(local_eeprom);

        Serial5.print("INICIALIZADA");
    }
    else
    {
        Serial5.println("EEPROM Inicializada.");
        Serial5.println("Volcando EEPROM.");
        Serial5.println(local_eeprom.serial_number);
        Serial5.println(local_eeprom.integrated_version);
        Serial5.println(local_eeprom.work_time);
        Serial5.println(local_eeprom.power_errors);
        Serial5.println(local_eeprom.consumption_errors);
        Serial5.println(local_eeprom.voltage_errors);
        Serial5.println(local_eeprom.save_voltage);
        Serial5.println(local_eeprom.flag_init);
        Serial5.println(local_eeprom.flag_corruption);
        for (int16_t i = 0; i < 71; i++)
        {
            Serial5.print(local_eeprom.array_power_use[i]);
            Serial5.print(",");
        }
        Serial5.println();
        for (uint16_t i = 0; i < 21; i++)
        {
            Serial5.print(local_eeprom.array_percent_use[i]);
            Serial5.print(",");
        }
        Serial5.println();
        uint32_t checksum;
        checksum = 0;
        checksum += local_eeprom.serial_number;
        checksum += local_eeprom.integrated_version;
        checksum += local_eeprom.work_time;
        checksum += local_eeprom.power_errors;
        checksum += local_eeprom.consumption_errors;
        checksum += local_eeprom.voltage_errors;
        checksum += local_eeprom.save_voltage;
        checksum += local_eeprom.flag_init;
        checksum += local_eeprom.flag_corruption;
        for (size_t i = 0; i < 71; i++)
        {
            checksum += local_eeprom.array_power_use[i];
        }
        for (size_t i = 0; i < 21; i++)
        {
            checksum += local_eeprom.array_percent_use[i];
        }
        Serial5.print(checksum);
        Serial5.print("/");
        Serial5.println(local_eeprom.checksum);
        Serial5.println("Volcado de EEPROM Finalizado.");
    }
}

void loop()
{
}
