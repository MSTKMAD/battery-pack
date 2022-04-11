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
// const uint8_t C_INIT_STATE = 0xBD;
// const uint8_t C_MODEL = 0xBD;
// const uint8_t C_SERIAL_NUMBER = 0xBD;

/**
 * @brief RAM LUT EEPROM
 * 
 */
const uint16_t C_RAM_HACK_START_SOUND = 0;
const uint16_t C_RAM_HACK_END_SOUND = 1;
const uint16_t C_RAM_HACK_CHARGE_SOUND = 2;
const uint16_t C_RAM_HACK_FULL_CHARGE_SOUND = 3;
const uint16_t C_RAM_HACK_START_DISPLAY = 4;
const uint16_t C_RAM_SERIAL_NUMBER = 5;
const uint16_t C_RAM_WORK_TIME = 6;
const uint16_t C_RAM_POWER_ERROR = 7;
const uint16_t C_RAM_CONSUMPTION_ERROR = 8;
const uint16_t C_RAM_VOLTAGE_ERROR = 9;
const uint16_t C_RAM_INSTANT_VOLTAGE = 10;
const uint16_t C_RAM_INSTANT_CURRENT = 11;
const uint16_t C_RAM_INSTANT_POWER = 12;
const uint16_t C_RAM_THEORY_VOLTAGE = 13;
const uint16_t C_RAM_MODEL = 14;
const uint16_t C_RAM_CAPACITY = 15;
const uint16_t C_RAM_HACK_END_DISPLAY = 16;
const uint16_t C_RAM_BAT_CHECKSUM = 16;
const uint16_t C_NUM_ELEMENTS = 17; //          NOTE: MODIFICAR CADA VEZ QUE SE AÑADA UN ELEMENTO!!!!

/**
 * @brief Baterie LUT EEPROM
 * 
 */
const uint16_t POS_EEPROM_INIT = 0x0000;              // byte -> 1 pos
const uint16_t POS_EEPROM_FLAG_CORRUPTION = 0x0001;   // byte -> 1 pos
const uint16_t POS_EEPROM_MODEL = 0x0002;             // uint16_t   -> 2 pos
const uint16_t POS_EEPROM_CHECKSUM = 0x0004;          // uint16_t   -> 2 pos
const uint16_t POS_EEPROM_SERIAL_NUMBER = 0x0006;     // uint16_t   -> 2 pos
const uint16_t POS_EEPROM_WORK_TIME = 0x0008;         // uint16_t   -> 2 pos
const uint16_t POS_EEPROM_POWER_ERROR = 0x000A;       // uint16_t   -> 2 pos
const uint16_t POS_EEPROM_CONSUMPTION_ERROR = 0x000C; // uint16_t   -> 2 pos
const uint16_t POS_EEPROM_VOLTAGE_ERROR = 0x000E;     // uint16_t   -> 2 pos
const uint16_t C_BAT_EEPROM_SIZE = POS_EEPROM_VOLTAGE_ERROR + 2;

// const uint16_t POS_EEPROM_VOLTS_ARRAY = 0x0005;       // uint16_t[111]  -> 222 pos Feature desestimada 28-03
// const uint16_t POS_EEPROM_POWER_ARRAY = 0x00E3;       // uint16_t[46]   -> 92 pos Feature desestimada 28-03