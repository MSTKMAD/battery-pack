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
const uint16_t MIN_WATS = 0;
const uint16_t MAX_WATS = 2000;

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
bool Init_local_eeprom(bool nitro_state_default)
{
    Watchdog.reset();
    local_eeprom = flash_eeprom.read();
    Watchdog.reset();
    if ((local_eeprom.flag_init == false) && (local_eeprom.flag_corruption == false))
    {
#ifdef SERIAL_DEBUG
        Serial5.println("EEPROM sin Incializar.");
        Serial5.print("Inicializando...");
#endif
        // local_eeprom.serial_number = 2;
        local_eeprom.integrated_version = INTEGRATED_VERSION;
        local_eeprom.work_time = 0;
        local_eeprom.power_errors = 0;
        local_eeprom.consumption_errors = 0;
        local_eeprom.voltage_errors = 0;
        local_eeprom.shortcircuit_errors = 0;
        local_eeprom.save_voltage = 0;
        local_eeprom.test_mode = true;
        local_eeprom.flag_init = true;
        local_eeprom.flag_corruption = false;
        local_eeprom.flag_naming_enable = false;
        local_eeprom.nitro_status = nitro_state_default;
        local_eeprom.num_char_in_name = 0;
        local_eeprom.num_wdt_errors = 0;
        for (int16_t i = 0; i < 25; i++)
        {
            local_eeprom.name[i] = 0;
            Watchdog.reset();
        }
        for (int16_t i = 0; i < 46; i++)
        {
            local_eeprom.array_power_use[i] = 0;
            Watchdog.reset();
        }
        for (uint16_t i = 0; i < 21; i++)
        {
            local_eeprom.array_percent_use[i] = 0;
            Watchdog.reset();
        }

        // Checksum calculate
        local_eeprom.checksum = 0;
        local_eeprom.checksum += local_eeprom.serial_number;
        local_eeprom.checksum += local_eeprom.integrated_version;
        local_eeprom.checksum += local_eeprom.work_time;
        local_eeprom.checksum += local_eeprom.power_errors;
        local_eeprom.checksum += local_eeprom.consumption_errors;
        local_eeprom.checksum += local_eeprom.voltage_errors;
        local_eeprom.checksum += local_eeprom.shortcircuit_errors;
        local_eeprom.checksum += local_eeprom.save_voltage;
        local_eeprom.checksum += local_eeprom.test_mode;
        local_eeprom.checksum += local_eeprom.flag_init;
        local_eeprom.checksum += local_eeprom.flag_corruption;
        local_eeprom.checksum += local_eeprom.flag_naming_enable;
        local_eeprom.checksum += local_eeprom.nitro_status;
        local_eeprom.checksum += local_eeprom.num_char_in_name;
        local_eeprom.checksum += local_eeprom.num_wdt_errors;
        for (int16_t i = 0; i < 25; i++)
        {
            local_eeprom.checksum += local_eeprom.name[i];
            Watchdog.reset();
        }
        for (size_t i = 0; i < 46; i++)
        {
            local_eeprom.checksum += local_eeprom.array_power_use[i];
            Watchdog.reset();
        }
        for (size_t i = 0; i < 21; i++)
        {
            local_eeprom.checksum += local_eeprom.array_percent_use[i];
            Watchdog.reset();
        }
        flash_eeprom.write(local_eeprom);
        Watchdog.reset();
#ifdef SERIAL_DEBUG
        Serial5.println("INICIALIZADA");
#endif
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
#ifdef SERIAL_DEBUG
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
            Serial5.println(local_eeprom.test_mode);
            Serial5.println(local_eeprom.flag_init);
            Serial5.println(local_eeprom.flag_corruption);
            Serial5.println(local_eeprom.flag_naming_enable);
            Serial5.println(local_eeprom.nitro_status);
            Serial5.println(local_eeprom.num_char_in_name);
            Serial5.println(local_eeprom.num_wdt_errors);
            for (int16_t i = 0; i < 25; i++)
            {
                Serial5.print(local_eeprom.name[i]);
                Serial5.print(",");
            }
            Serial5.println();
            for (int16_t i = 0; i < 46; i++)
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
#endif
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
            checksum += local_eeprom.test_mode;
            checksum += local_eeprom.flag_init;
            checksum += local_eeprom.flag_corruption;
            for (int16_t i = 0; i < 25; i++)
            {
                checksum += local_eeprom.name[i];
                Watchdog.reset();
            }
            checksum += local_eeprom.num_char_in_name;
            for (size_t i = 0; i < 46; i++)
            {
                checksum += local_eeprom.array_power_use[i];
                Watchdog.reset();
            }
            for (size_t i = 0; i < 21; i++)
            {
                checksum += local_eeprom.array_percent_use[i];
                Watchdog.reset();
            }
            checksum += local_eeprom.num_wdt_errors;
#ifdef SERIAL_DEBUG
            Serial5.println("Volcado de EEPROM Finalizado.");
            Serial5.printf("Checksum: %d\nEEPROM Checkcum:%d\n", checksum, local_eeprom.checksum);
#endif
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
        data = constrain(data, MIN_WATS, MAX_WATS);
        wats_pos = (data) / 50;
        local_eeprom.array_power_use[wats_pos]++;
        break;

    case C_PERCENT_USE:
        data = constrain(data, 3300, 4100);
        perc_pos = (data - 3300) / 20;
        local_eeprom.array_percent_use[perc_pos]++;
        break;

    case C_FLAG_ENABLE_NAME:
        local_eeprom.flag_naming_enable = data;

    case C_NITRO_STATUS:
        local_eeprom.nitro_status = data;

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

    case C_FLAG_ENABLE_NAME:
        return local_eeprom.flag_naming_enable;

    case C_TEST_MODE:
        return local_eeprom.test_mode;

    case C_NITRO_STATUS:
        return local_eeprom.nitro_status;

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
        local_eeprom.work_time += 1;
        break;
    case C_NUM_WDT_ERRORS:
        local_eeprom.num_wdt_errors += 1;
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
    local_eeprom.checksum += local_eeprom.test_mode;
    local_eeprom.checksum += local_eeprom.flag_init;
    local_eeprom.checksum += local_eeprom.flag_corruption;
    local_eeprom.checksum += local_eeprom.flag_naming_enable;
    local_eeprom.checksum += local_eeprom.nitro_status;
    local_eeprom.checksum += local_eeprom.num_char_in_name;
    local_eeprom.checksum += local_eeprom.num_wdt_errors;
    for (int16_t i = 0; i < 25; i++)
    {
        local_eeprom.checksum += local_eeprom.name[i];
        Watchdog.reset();
    }
    for (size_t i = 0; i < 46; i++)
    {
        local_eeprom.checksum += local_eeprom.array_power_use[i];
        Watchdog.reset();
    }
    for (size_t i = 0; i < 21; i++)
    {
        local_eeprom.checksum += local_eeprom.array_percent_use[i];
        Watchdog.reset();
    }
    Watchdog.reset();
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
#ifdef SERIAL_DEBUG
    Serial5.println("PM-Logging");
#endif
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
/**
 * @brief
 *
 */
void DiagnosticMode()
{
    uint16_t diagnostic_chain[NUM_NON_ARRAY_POS + NUM_POS_NAME + PM_POSITIONS * 4 + POWER_USE_POSITIONS + PERCENT_USE_POSITIONS];
    local_eeprom = flash_eeprom.read();
    diagnostic_chain[0] = local_eeprom.serial_number;
    diagnostic_chain[1] = local_eeprom.integrated_version;
    diagnostic_chain[2] = local_eeprom.work_time;
    diagnostic_chain[3] = local_eeprom.power_errors;
    diagnostic_chain[4] = local_eeprom.consumption_errors;
    diagnostic_chain[5] = local_eeprom.voltage_errors;
    diagnostic_chain[6] = local_eeprom.shortcircuit_errors;
    diagnostic_chain[7] = local_eeprom.save_voltage;
    diagnostic_chain[8] = local_eeprom.test_mode;
    diagnostic_chain[9] = local_eeprom.flag_init;
    diagnostic_chain[10] = local_eeprom.flag_corruption;
    diagnostic_chain[11] = local_eeprom.flag_naming_enable;
    diagnostic_chain[12] = local_eeprom.nitro_status;
    diagnostic_chain[13] = local_eeprom.num_char_in_name;
    diagnostic_chain[14] = local_eeprom.num_wdt_errors;

    for (int i = 0; i < 25; i++)
    {
        diagnostic_chain[NUM_NON_ARRAY_POS + i] = local_eeprom.name[i];
    }

    for (int i = 0; i < PM_POSITIONS; i++)
    {
        diagnostic_chain[NUM_NON_ARRAY_POS + NUM_POS_NAME + i] = local_eeprom.pm_power[i];
    }
    for (int i = 0; i < PM_POSITIONS; i++)
    {
        diagnostic_chain[NUM_NON_ARRAY_POS + NUM_POS_NAME + PM_POSITIONS + i] = local_eeprom.pm_percent[i];
    }
    for (int i = 0; i < PM_POSITIONS; i++)
    {
        diagnostic_chain[NUM_NON_ARRAY_POS + NUM_POS_NAME + PM_POSITIONS * 2 + i] = local_eeprom.pm_voltage[i];
    }
    for (int i = 0; i < PM_POSITIONS; i++)
    {
        diagnostic_chain[NUM_NON_ARRAY_POS + NUM_POS_NAME + PM_POSITIONS * 3 + i] = local_eeprom.pm_errors[i];
    }
    for (uint16_t i = 0; i < POWER_USE_POSITIONS; i++)
    {
        diagnostic_chain[NUM_NON_ARRAY_POS + NUM_POS_NAME + PM_POSITIONS * 4 + i] = local_eeprom.array_power_use[i];
    }
    for (uint16_t i = 0; i < PERCENT_USE_POSITIONS; i++)
    {
        diagnostic_chain[NUM_NON_ARRAY_POS + NUM_POS_NAME + PM_POSITIONS * 4 + POWER_USE_POSITIONS + i] = local_eeprom.array_percent_use[i];
    }

    char buffer[50];
#ifdef SERIAL_DEBUG
    for (int i = 0; i < sizeof(diagnostic_chain) / sizeof(diagnostic_chain[0]); i++)
    {
        sprintf(buffer, "0x%04X ", diagnostic_chain[i]);
        Serial5.print(buffer);
        Watchdog.reset();
    }
    Serial5.println();
#endif

    DisplayArray(diagnostic_chain, sizeof(diagnostic_chain) / sizeof(diagnostic_chain[0]));
}

/**
 * @brief
 *
 * @param array_to_display
 */
void SaveNameEEPROM(char array_to_display[], uint16_t num_char)
{
    for (uint16_t i = 0; i < 25; i++)
    {
        local_eeprom.name[i] = array_to_display[i];
    }
    local_eeprom.num_char_in_name = num_char;
    SaveEeprom();
}

/**
 * @brief
 *
 * @param array_to_name
 * .++
 * .
 * 
 * @return uint16_t
 */
uint16_t ReadNameEEPROM(char array_to_name[])
{
    for (uint16_t i = 0; i < 25; i++)
    {
        array_to_name[i] = local_eeprom.name[i];
    }
    return local_eeprom.num_char_in_name;
}