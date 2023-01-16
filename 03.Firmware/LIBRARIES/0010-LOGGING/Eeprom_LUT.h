/**
 * @file Eeprom_LUT.h
 * @author Javi (Javier@musotoku.com)
 * @brief Look Up Table de direcciones de memeoria de la eeprom de la bateria.
 * @version 1
 * @date 2021-03-22
 * 
 * @copyright Copyright (c) 2021
 * 
 */
const uint8_t HOURS_PM = 10;
const uint16_t LOG_PER_HOUR = 10;
const uint16_t PM_POSITIONS = HOURS_PM * LOG_PER_HOUR;
typedef struct EEPROM
{
    uint8_t serial_number;
    uint8_t integrated_version;
    uint16_t work_time;
    uint16_t power_errors;
    uint16_t consumption_errors;
    uint16_t voltage_errors;
    uint16_t save_voltage;
    bool flag_init;
    bool flag_corruption;
    uint16_t array_power_use[46];   // 500w to 5000w in step of 100w -> 46 step
    uint16_t array_percent_use[21]; // 0% to 100% in step of 5% -> 21 step
    uint16_t pm_power[PM_POSITIONS];
    uint16_t pm_percent[PM_POSITIONS];
    uint16_t pm_voltage[PM_POSITIONS];
    uint16_t pm_errors[PM_POSITIONS];
    uint32_t checksum;
};

const uint8_t C_POWER_ERROR = 0;
const uint8_t C_CONSUMPTION_ERROR = 1;
const uint8_t C_VOLTAGE_ERROR = 2;
const uint8_t C_WORK_TIME = 3;

const uint8_t C_FLAG_CORRUPTION = 4;
const uint8_t C_FLAG_INIT = 5;

const uint8_t C_SERIAL_NUMBER = 6;
const uint8_t C_INTEGRATED_VERSION = 7;

const uint8_t C_THEORY_VOLTAGE = 8;

const uint8_t C_POWER_USE = 9;
const uint8_t C_PERCENT_USE = 10;

const uint8_t C_PM_POWER = 11;
const uint8_t C_PM_PERCENT = 12;
const uint8_t C_PM_VOLTAGE = 13;
const uint8_t C_PM_ERRORS = 14;

const uint8_t C_ERROR_PWR = 1001;
const uint8_t C_ERROR_VOLT = 1002;
const uint8_t C_ERROR_CONS = 1003;