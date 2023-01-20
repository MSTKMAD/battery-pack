/**
 * @file diagnostic.h
 * @author Javi (Javier@musotoku.com)
 * @brief 
 * @version 1
 * @date 2021-01-14
 * 
 * @copyright Copyright (c) 2021
 * 
 */

#ifndef EEPROM_EMULATION_SIZE
#define EEPROM_EMULATION_SIZE 127
#endif
#ifndef INTEGRATED_VERSION
#define INTEGRATED_VERSION 0
#endif
#ifndef MAX_VOLTAGE
#define MAX_VOLTAGE 120
#endif
#ifndef MIN_VOLTAGE
#define MIN_VOLTAGE 50
#endif

#include <batt_FlashStorage.h>
#include "Eeprom_LUT.h"
/*extern "C"
{
#include <SEGGER_RTT.h>
}
*/
/**
 *                  CONSTANTS
 */
const uint16_t MIN_WATS = 500;
const uint16_t MAX_WATS = 5000;

/**
 *                  VARIABLES
 */

FlashStorage(flash_eeprom, EEPROM);
EEPROM local_eeprom;
uint32_t checksum;

/**
 * @brief Inicializacion del valor de los elementos de diagnostico.
 *          Cada elemento posee el valor, el nombre y la categoria:
 *          * Serial Port (X o S). Con X no se muestra con S si.
 *          * Guardado en EEPROM (X,B,C). X no se guarda, B se guarda en la Bateria y C se guarda en el Chasis.
 * 
 */
bool Init_local_eeprom()
{
    local_eeprom = flash_eeprom.read();
    if ((local_eeprom.flag_init == false) && (local_eeprom.flag_corruption == false))
    {
        Serial5.println("EEPROM sin Incializar.");
        Serial5.print("Inicializando...");
        //local_eeprom.serial_number = 0;
        local_eeprom.integrated_version = INTEGRATED_VERSION;
        local_eeprom.work_time = 0;
        local_eeprom.power_errors = 0;
        local_eeprom.consumption_errors = 0;
        local_eeprom.voltage_errors = 0;
        local_eeprom.shortcircuit_errors = 0;
        local_eeprom.save_voltage = 0;
        local_eeprom.flag_init = true;
        local_eeprom.flag_corruption = false;
        for (int16_t i = 0; i < 71; i++)
        {
            local_eeprom.array_power_use[i] = 0;
        }
        for (uint16_t i = 0; i < 21; i++)
        {
            local_eeprom.array_percent_use[i] = 0;
        }
        local_eeprom.checksum = 0;
        local_eeprom.checksum += local_eeprom.serial_number;
        local_eeprom.checksum += local_eeprom.integrated_version;
        local_eeprom.checksum += local_eeprom.work_time;
        local_eeprom.checksum += local_eeprom.power_errors;
        local_eeprom.checksum += local_eeprom.consumption_errors;
        local_eeprom.checksum += local_eeprom.voltage_errors;
        local_eeprom.checksum += local_eeprom.shortcircuit_errors;
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
        Serial5.println("INICIALIZADA");
        return 1;
    }
    else
    {
        if (local_eeprom.flag_corruption == true)
        {
            return 0;
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
            Serial5.println(local_eeprom.shortcircuit_errors);
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
            checksum += local_eeprom.shortcircuit_errors;
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
            Serial5.println("Volcado de EEPROM Finalizado.");
            if (checksum == local_eeprom.checksum)
            {
                return 1;
            }
            else
            {
                return 0;
            }
        }
    }
}
/**
 * @brief Actualizar el valor en Ram de una variable de adiagnostico
 * 
 * @param data Valor del dato
 * @param address Direccion en la memoria RAM de la variable
 */
void LogDiagnosticData(int16_t data, int16_t address)
{
    int16_t wats_pos = 0;
    int16_t perc_pos = 0;
    switch (address)
    {

    case C_THEORY_VOLTAGE:
        local_eeprom.save_voltage = data;
        break;

    case C_POWER_USE:
        wats_pos = (data - MIN_WATS) / 100;
        local_eeprom.array_power_use[wats_pos]++;
        break;

    case C_PERCENT_USE:
        perc_pos = data / 5;
        local_eeprom.array_percent_use[perc_pos]++;
        break;

    default:
        break;
    }
}
/**
 * @brief Lectura del valor de un elemento de diagnostico.
 * 
 * @param address
 * 
 */
uint16_t ReadDiagnosticData(int16_t address)
{
    switch (address)
    {
    case C_THEORY_VOLTAGE:
        return local_eeprom.save_voltage;
        break;

    default:
        return 0;
        break;
    }
}
/**
 * @brief Incrementar el valor de una variable de diagnostico en un valor
 * 
 * @param inc Valor de incremento
 * @param address Direccion en la memoria RAM de la variable.
 */
void IncrementDiagnosticData(int16_t address)
{
    switch (address)
    {
    case C_POWER_ERROR:
        local_eeprom.power_errors += 1;
        break;
    case C_CONSUMPTION_ERROR:
        local_eeprom.consumption_errors += 1;
        break;
    case C_VOLTAGE_ERROR:
        local_eeprom.voltage_errors += 1;
        break;
    case C_SHORT_CIRCUIT_ERROR:
        local_eeprom.shortcircuit_errors += 1;
        break;
    case C_WORK_TIME:
        static int cont_hour = 0;
        cont_hour++;
        if (cont_hour == 10)
        {
            local_eeprom.work_time += 1;
            cont_hour = 0;
        }
        break;

    default:
        break;
    }
}

/**
 * @brief Guardado en la EEPROM del chasis de las variables de diagnostico con categoria de Memoria 'C'.
 * 
 */
void SaveEeprom()
{
    local_eeprom.checksum = 0;
    local_eeprom.checksum += local_eeprom.serial_number;
    local_eeprom.checksum += local_eeprom.integrated_version;
    local_eeprom.checksum += local_eeprom.work_time;
    local_eeprom.checksum += local_eeprom.power_errors;
    local_eeprom.checksum += local_eeprom.consumption_errors;
    local_eeprom.checksum += local_eeprom.voltage_errors;
    local_eeprom.checksum += local_eeprom.shortcircuit_errors;
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
}
/**
 * @brief Logeo de los valores del postmortem
 * 
 * @param value 
 * @param address 
 */
void PostMortemLog(int16_t power_value, int16_t percnt_value, int16_t voltage_value, int16_t errors_value)
{
    Serial5.println("PM-Logging");
    for (int i = 0; i < PM_POSITIONS - 1; i++)
    {
        local_eeprom.pm_power[i] = local_eeprom.pm_power[i + 1];
        local_eeprom.pm_percent[i] = local_eeprom.pm_percent[i + 1];
        local_eeprom.pm_voltage[i] = local_eeprom.pm_voltage[i + 1];
        local_eeprom.pm_errors[i] = local_eeprom.pm_errors[i + 1];
    }
    local_eeprom.pm_power[PM_POSITIONS - 1] = power_value;
    local_eeprom.pm_percent[PM_POSITIONS - 1] = percnt_value;
    local_eeprom.pm_voltage[PM_POSITIONS - 1] = voltage_value;
    local_eeprom.pm_errors[PM_POSITIONS - 1] = errors_value;
}