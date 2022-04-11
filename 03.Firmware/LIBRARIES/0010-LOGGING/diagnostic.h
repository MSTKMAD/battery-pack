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

#include <batt_FlashStorage.h>
#include <batt_SlowSoftI2CMaster.h>
#include "EepromBitBang.h"
#include "Eeprom_LUT.h"
/*extern "C"
{
#include <SEGGER_RTT.h>
}
*/
/**
 *                  CONSTANTS
 */
const uint16_t MIN_VOLTAGE = 5000;
const uint16_t MAX_VOLTAGE = 16000;
const uint16_t MIN_WATS = 500;
const uint16_t MAX_WATS = 5000;

/**
 *                  VARIABLES
 */
typedef struct
{
    int16_t data[EEPROM_EMULATION_SIZE];
    boolean valid;
} EEPROM_EMULATION;

typedef struct
{
    uint16_t value = 0;
    char category[2] = {'X', 'X'}; // category[0] = 'S' (Serial Port); category[1] = 'B' Baterie Eeprom, 'C' Chasis Eeprom
    String name = "NoName";
} Element;

Element elements[C_NUM_ELEMENTS];
EEPROM_EMULATION _eeprom_RAM;

bool _dirty = false;
int eeprom_index = 0;

uint16_t voltage_values[111]; // 5000V - 16000 step of 100mV = 111 positions
uint16_t power_values[46];    // 500mW - 5000mW step of 100mW = 46 postions.

uint8_t init_flag = 0;

FlashStorage(eeprom_flash, EEPROM_EMULATION);

/**
 *      FUNCTIONS
 */
/**
 * @brief Reset de las posiciones de la EEPROM de la Bateria
 * 
 */
void ResetBateriaEeprom()
{
    for (uint32_t i = POS_EEPROM_CHECKSUM; i < C_BAT_EEPROM_SIZE; i++)
    {
        writeEEPROM(i, 0xFF);
    }
}
/**
 * @brief Inicializacion del valor de los elementos de diagnostico.
 *          Cada elemento posee el valor, el nombre y la categoria:
 *          * Serial Port (X o S). Con X no se muestra con S si.
 *          * Guardado en EEPROM (X,B,C). X no se guarda, B se guarda en la Bateria y C se guarda en el Chasis.
 * 
 */
bool Init_diagnostic_elements()
{
    // Calculo del CheckSum
    uint16_t checksum = 0;
    // Read EEPROM memory.
    _eeprom_RAM = eeprom_flash.read();

    elements[C_RAM_HACK_START_SOUND].category[0] = {'X'};
    elements[C_RAM_HACK_START_SOUND].category[1] = {'C'};
    elements[C_RAM_HACK_START_SOUND].name = "HACK_START_SOUND";

    elements[C_RAM_HACK_END_SOUND].category[0] = {'S'};
    elements[C_RAM_HACK_END_SOUND].category[1] = {'C'};
    elements[C_RAM_HACK_END_SOUND].name = "HACK_END_SOUND";

    elements[C_RAM_HACK_CHARGE_SOUND].category[0] = {'S'};
    elements[C_RAM_HACK_CHARGE_SOUND].category[1] = {'C'};
    elements[C_RAM_HACK_CHARGE_SOUND].name = "HACK_CHARGE_SOUND";

    elements[C_RAM_HACK_FULL_CHARGE_SOUND].category[0] = {'X'};
    elements[C_RAM_HACK_FULL_CHARGE_SOUND].category[1] = {'C'};
    elements[C_RAM_HACK_FULL_CHARGE_SOUND].name = "HACK_FULL_CHARGE_SOUND";

    elements[C_RAM_HACK_START_DISPLAY].category[0] = {'X'};
    elements[C_RAM_HACK_START_DISPLAY].category[1] = {'C'};
    elements[C_RAM_HACK_START_DISPLAY].name = "HACK_START_DISPLAY";

    elements[C_RAM_HACK_END_DISPLAY].category[0] = {'X'};
    elements[C_RAM_HACK_END_DISPLAY].category[1] = {'C'};
    elements[C_RAM_HACK_END_DISPLAY].name = "HACK_END_DISPLAY";

    elements[C_RAM_SERIAL_NUMBER].category[0] = {'X'};
    elements[C_RAM_SERIAL_NUMBER].category[1] = {'B'};
    elements[C_RAM_SERIAL_NUMBER].name = "SERIAL NUMBER";
    elements[C_RAM_SERIAL_NUMBER].value = ReadWordEEPROM(POS_EEPROM_SERIAL_NUMBER);

    elements[C_RAM_WORK_TIME].category[0] = {'X'};
    elements[C_RAM_WORK_TIME].category[1] = {'B'};
    elements[C_RAM_WORK_TIME].name = "Work Time";
    elements[C_RAM_WORK_TIME].value = ReadWordEEPROM(POS_EEPROM_WORK_TIME);
    checksum += elements[C_RAM_WORK_TIME].value;

    elements[C_RAM_POWER_ERROR].category[0] = {'X'};
    elements[C_RAM_POWER_ERROR].category[1] = {'B'};
    elements[C_RAM_POWER_ERROR].name = "Power Error";
    elements[C_RAM_POWER_ERROR].value = ReadWordEEPROM(POS_EEPROM_POWER_ERROR);
    checksum += elements[C_RAM_POWER_ERROR].value;

    elements[C_RAM_CONSUMPTION_ERROR].category[0] = {'X'};
    elements[C_RAM_CONSUMPTION_ERROR].category[1] = {'B'};
    elements[C_RAM_CONSUMPTION_ERROR].name = "Consumption Error";
    elements[C_RAM_CONSUMPTION_ERROR].value = ReadWordEEPROM(POS_EEPROM_CONSUMPTION_ERROR);
    checksum += elements[C_RAM_CONSUMPTION_ERROR].value;

    elements[C_RAM_VOLTAGE_ERROR].category[0] = {'X'};
    elements[C_RAM_VOLTAGE_ERROR].category[1] = {'B'};
    elements[C_RAM_VOLTAGE_ERROR].name = "Voltage Error";
    elements[C_RAM_VOLTAGE_ERROR].value = ReadWordEEPROM(POS_EEPROM_VOLTAGE_ERROR);
    checksum += elements[C_RAM_VOLTAGE_ERROR].value;

    elements[C_RAM_INSTANT_VOLTAGE].category[0] = {'S'};
    elements[C_RAM_INSTANT_VOLTAGE].category[1] = {'X'};
    elements[C_RAM_INSTANT_VOLTAGE].name = "Instant Voltage";
    elements[C_RAM_INSTANT_VOLTAGE].value = 0;

    elements[C_RAM_INSTANT_POWER].category[0] = {'S'};
    elements[C_RAM_INSTANT_POWER].category[1] = {'X'};
    elements[C_RAM_INSTANT_POWER].name = "Instant Power";
    elements[C_RAM_INSTANT_POWER].value = 0;

    elements[C_RAM_INSTANT_CURRENT].category[0] = {'S'};
    elements[C_RAM_INSTANT_CURRENT].category[1] = {'X'};
    elements[C_RAM_INSTANT_CURRENT].name = "Instant Power";
    elements[C_RAM_INSTANT_CURRENT].value = 0;

    elements[C_RAM_THEORY_VOLTAGE].category[0] = {'X'};
    elements[C_RAM_THEORY_VOLTAGE].category[1] = {'C'};
    elements[C_RAM_THEORY_VOLTAGE].name = "Voltage programed";

    elements[C_RAM_MODEL].category[0] = {'X'};
    elements[C_RAM_MODEL].category[1] = {'B'};
    elements[C_RAM_MODEL].name = "Model";
    elements[C_RAM_MODEL].value = ReadWordEEPROM(POS_EEPROM_MODEL);

    elements[C_RAM_BAT_CHECKSUM].category[0] = {'X'};
    elements[C_RAM_BAT_CHECKSUM].category[1] = {'X'};
    elements[C_RAM_BAT_CHECKSUM].name = "Checksum";
    elements[C_RAM_BAT_CHECKSUM].value = ReadWordEEPROM(POS_EEPROM_CHECKSUM);

    elements[C_RAM_CAPACITY].category[0] = {'S'};
    elements[C_RAM_CAPACITY].category[1] = {'X'};
    elements[C_RAM_CAPACITY].name = "Capacity";

    /* 
        Feature en pausa 28-03-2022.
    //ReadArrayEEPROM(POS_EEPROM_VOLTS_ARRAY, &voltage_values[0], (sizeof(voltage_values) / sizeof(voltage_values[0])));
    //ReadArrayEEPROM(POS_EEPROM_POWER_ARRAY, &power_values[0], (sizeof(power_values) / sizeof(power_values[0])));

    // for (uint i = 0; i < (sizeof(power_values) / sizeof(power_values[0])); i++)
    // {
    //     checksum += power_values[i];
    // }
    // for (uint i = 0; i < (sizeof(voltage_values) / sizeof(voltage_values[0])); i++)
    // {
    //     checksum += voltage_values[i];
    // }
    */

    eeprom_index = 0;
    for (int i = 0; i < C_NUM_ELEMENTS; i++)
    {
        if ((elements[i].category[1] == 'C'))
        {
            elements[i].value = _eeprom_RAM.data[eeprom_index];
            eeprom_index++;
            eeprom_index = constrain(eeprom_index, 0, EEPROM_EMULATION_SIZE);
        }
    }
    // Initialization to 0 if value 0xFFFF;
    for (int i = 0; i < C_NUM_ELEMENTS; i++)
    {
        if ((elements[i].category[1] == 'B'))
        {
            if (elements[i].value == 0xFFFF)
            {
                elements[i].value = 0;
            }
        }
    }

    //Serial5.println(checksum + 1);
    //Serial5.println(elements[C_RAM_BAT_CHECKSUM].value);
    if (elements[C_RAM_BAT_CHECKSUM].value == 0)
    {
        return false;

        Serial5.println("EEPROM FAIL");
    }
    else
    {
        if ((checksum + 1) != elements[C_RAM_BAT_CHECKSUM].value)
        {
            Serial5.println("FALLO DE CHECKSUM!!!");
            ResetBateriaEeprom();
            writeEEPROM(POS_EEPROM_FLAG_CORRUPTION, 0x00);
            return true;
        }
        else
        {
            Serial5.println("CHECKSUM CORRECTO :)");
            return true;
        }
    }

    byte data8 = 0;
    for (int i = 0; i <= POS_EEPROM_CHECKSUM + 1; i++)
    {
        readEEPROM(i, &data8);
        Serial5.print(data8);
        Serial5.print(",");
    }
    Serial5.println();

    // for (int i = 0; i < C_NUM_ELEMENTS; i++)
    // {
    //     Serial5.print(elements[i].value);
    //     Serial5.print(",");
    // }
    // Serial5.println();

    pinMode(C_PIN_BUTT_CENTER, INPUT_PULLUP);
}
/**
 * @brief Actualizar el valor en Ram de una variable de adiagnostico
 * 
 * @param data Valor del dato
 * @param address Direccion en la memoria RAM de la variable
 */
void LogDiagnosticData(int16_t data, int16_t address)
{
    pinMode(C_PIN_SCL_2, OUTPUT);
    pinMode(C_PIN_SDA_2, OUTPUT);
    address = constrain(address, 0, C_NUM_ELEMENTS);
    if (elements[address].value != data)
    {
        _dirty = true;
        elements[address].value = data;
    }
    pinMode(C_PIN_BUTT_CENTER, INPUT_PULLUP);
}
/**
 * @brief Lectura del valor de un elemento de diagnostico.
 * 
 * @param address
 * 
 */
uint16_t ReadDiagnosticData(int16_t address)
{
    return elements[address].value;
}
/**
 * @brief Incrementar el valor de una variable de diagnostico en un valor
 * 
 * @param inc Valor de incremento
 * @param address Direccion en la memoria RAM de la variable.
 */
void IncrementDiagnosticData(int16_t inc, int16_t address)
{
    pinMode(C_PIN_SCL_2, OUTPUT);
    pinMode(C_PIN_SDA_2, OUTPUT);
    if (elements[address].value >= 0xFFEE)
    {
        elements[address].value = 0xFFEE;
    }
    else
    {
        elements[address].value = elements[address].value + inc;
    }

    _dirty = true;
    pinMode(C_PIN_BUTT_CENTER, INPUT_PULLUP);
}

/**
 * @brief Muestreo por el puerto serie de las variable de diagnostico con categoria de Serial Port 'S'
 * 
 */
void PrintDiagnosticData()
{
    pinMode(C_PIN_SCL_2, OUTPUT);
    pinMode(C_PIN_SDA_2, OUTPUT);
    for (int16_t i = 0; i < C_NUM_ELEMENTS; i++)
    {
        if (elements[i].category[0] == 'S')
        {
            //SEGGER_RTT_printf(0, "%s%d;", RTT_CTRL_TEXT_WHITE, elements[i].value);
            Serial.print(elements[i].value);
            Serial.print(";");
        }
    }
    //SEGGER_RTT_printf(0, "\n");
    Serial.println();
    pinMode(C_PIN_BUTT_CENTER, INPUT_PULLUP);
}

/**
 * @brief Guardado en la EEPROM del chasis de las variables de diagnostico con categoria de Memoria 'C'.
 * 
 */
void SaveEepromChasis()
{
    pinMode(C_PIN_SCL_2, OUTPUT);
    pinMode(C_PIN_SDA_2, OUTPUT);
    if (_dirty == true)
    {
        eeprom_index = 0;
        for (int16_t i = 0; i < C_NUM_ELEMENTS; i++)
        {
            if ((elements[i].category[1] == 'C'))
            {
                _eeprom_RAM.data[eeprom_index] = elements[i].value;
                eeprom_index++;
                eeprom_index = constrain(eeprom_index, 0, EEPROM_EMULATION_SIZE);
            }
        }
        _eeprom_RAM.valid = true;
        eeprom_flash.write(_eeprom_RAM);
        _dirty = false;
    }
    pinMode(C_PIN_BUTT_CENTER, INPUT_PULLUP);
}

/**
 * @brief Devuelve true si la memoeria RAM contiene datos y false si la memeoria RAM esta vacia.
 * 
 * @return true 
 * @return false 
 */
bool isValid()
{
    pinMode(C_PIN_SCL_2, OUTPUT);
    pinMode(C_PIN_SDA_2, OUTPUT);
    return _eeprom_RAM.valid;
    pinMode(C_PIN_BUTT_CENTER, INPUT_PULLUP);
}

/**
 * @brief Incremento de las estadisticas del valor de voltahe y potencia usado.
 * 
 * @param volts 
 * @param wats 
 */
/*
    Feature en pausa 28-03-2022.
void Stats(uint16_t volts, uint16_t wats)
{
    pinMode(C_PIN_SCL_2, OUTPUT);
    pinMode(C_PIN_SDA_2, OUTPUT);
    int16_t voltage_pos = 0, wats_pos = 0;

    voltage_pos = (volts - MIN_VOLTAGE) / 100;
    voltage_values[voltage_pos]++;

    wats_pos = (wats - MIN_WATS) / 100;
    power_values[wats_pos]++;
    pinMode(C_PIN_BUTT_CENTER, INPUT_PULLUP);
}
*/

/**
 * @brief Actualizacion de la memoria EEPROM de la Bateria.
 * 
 */
bool UpdateEepromBatery()
{
    uint16_t scrap = 0;
    // uint16_t scrap1_array[111];
    // uint16_t scrap2_array[46];
    uint16_t checksum = 0;

    scrap = ReadWordEEPROM(POS_EEPROM_WORK_TIME);
    Serial5.print(scrap);
    Serial5.print("/");
    Serial5.println(elements[C_RAM_WORK_TIME].value);
    checksum += elements[C_RAM_WORK_TIME].value;
    if (elements[C_RAM_WORK_TIME].value != scrap)
    {
        WriteWordEEPROM(POS_EEPROM_WORK_TIME, elements[C_RAM_WORK_TIME].value);
        if (ReadWordEEPROM(POS_EEPROM_WORK_TIME) != elements[C_RAM_WORK_TIME].value)
        {
            return false;
        }
    }

    scrap = ReadWordEEPROM(POS_EEPROM_POWER_ERROR);
    Serial5.print(scrap);
    Serial5.print("/");
    Serial5.println(elements[C_RAM_POWER_ERROR].value);
    checksum += elements[C_RAM_POWER_ERROR].value;
    if (elements[C_RAM_POWER_ERROR].value != scrap)
    {
        WriteWordEEPROM(POS_EEPROM_POWER_ERROR, elements[C_RAM_POWER_ERROR].value);
        if (ReadWordEEPROM(POS_EEPROM_POWER_ERROR) != elements[C_RAM_POWER_ERROR].value)
        {
            return false;
        }
    }

    scrap = ReadWordEEPROM(POS_EEPROM_CONSUMPTION_ERROR);
    Serial5.print(scrap);
    Serial5.print("/");
    Serial5.println(elements[C_RAM_CONSUMPTION_ERROR].value);
    checksum += elements[C_RAM_CONSUMPTION_ERROR].value;
    if (elements[C_RAM_CONSUMPTION_ERROR].value != scrap)
    {
        WriteWordEEPROM(POS_EEPROM_CONSUMPTION_ERROR, elements[C_RAM_CONSUMPTION_ERROR].value);
        if (ReadWordEEPROM(POS_EEPROM_CONSUMPTION_ERROR) != elements[C_RAM_CONSUMPTION_ERROR].value)
        {
            return false;
        }
    }

    scrap = ReadWordEEPROM(POS_EEPROM_VOLTAGE_ERROR);
    Serial5.print(scrap);
    Serial5.print("/");
    Serial5.println(elements[C_RAM_VOLTAGE_ERROR].value);
    checksum += elements[C_RAM_VOLTAGE_ERROR].value;
    if (elements[C_RAM_VOLTAGE_ERROR].value != scrap)
    {
        WriteWordEEPROM(POS_EEPROM_VOLTAGE_ERROR, elements[C_RAM_VOLTAGE_ERROR].value);
        if (ReadWordEEPROM(POS_EEPROM_VOLTAGE_ERROR) != elements[C_RAM_VOLTAGE_ERROR].value)
        {
            return false;
        }
    }
    /*
        Feature en pausa 28-03-2022.
        
    // ReadArrayEEPROM(POS_EEPROM_VOLTS_ARRAY, &scrap1_array[0], (sizeof(scrap1_array) / sizeof(scrap1_array[0])));
    // for (uint i = 0; i < (sizeof(voltage_values) / sizeof(voltage_values[0])); i++)
    // {
    //     checksum += voltage_values[i];
    //     Serial5.print(scrap1_array[i]);
    //     Serial5.print("/");
    //     Serial5.println(voltage_values[i]);
    //     if (scrap1_array[i] != voltage_values[i])
    //     {
    //         WriteWordEEPROM(POS_EEPROM_VOLTS_ARRAY + i * 2, voltage_values[i]);
    //         if (ReadWordEEPROM(POS_EEPROM_VOLTS_ARRAY + i * 2) != voltage_values[i])
    //         {
    //             return false;
    //         }
    //     }
    // }

    // ReadArrayEEPROM(POS_EEPROM_POWER_ARRAY, &scrap2_array[0], (sizeof(scrap2_array) / sizeof(scrap2_array[0])));
    // for (uint i = 0; i < (sizeof(power_values) / sizeof(power_values[0])); i++)
    // {
    //     checksum += power_values[i];
    //     Serial5.print(scrap2_array[i]);
    //     Serial5.print("/");
    //     Serial5.println(power_values[i]);
    //     if (scrap2_array[i] != power_values[i])
    //     {
    //         WriteWordEEPROM(POS_EEPROM_POWER_ARRAY + i * 2, power_values[i]);
    //         if (ReadWordEEPROM(POS_EEPROM_POWER_ARRAY + i * 2) != power_values[i])
    //         {
    //             return false;
    //         }
    //     }
    // }
    */

    Serial5.println(checksum + 1);
    WriteWordEEPROM(POS_EEPROM_CHECKSUM, checksum + 1);
    return true;
    // uint8_t data8;
    // for (int i = 0; i <= POS_EEPROM_CHECKSUM + 1; i++)
    // {
    //     readEEPROM(i, &data8);
    //     Serial5.print(data8);
    //     Serial5.print(",");
    // }
    // Serial5.println();
}
/**
 * @brief Muestreo por el puerto serie de los valores acumulados de las estadisticas de potencia y voltage.
 * 
 */
/*
    Feature en pausa 28-03-2022.
void PrintStats()
{
    pinMode(C_PIN_SCL_2, OUTPUT);
    pinMode(C_PIN_SDA_2, OUTPUT);
    for (uint i = 0; i < sizeof(voltage_values) / sizeof(voltage_values[0]); i++)
    {
        Serial.print(voltage_values[i]);
        Serial.print(";");
    }
    for (uint i = 0; i < sizeof(power_values) / sizeof(power_values[0]); i++)
    {
        Serial.print(power_values[i]);
        Serial.print(";");
    }
    pinMode(C_PIN_BUTT_CENTER, INPUT_PULLUP);
}*/

/**
 * @brief Impresion por el puerto serie de los datos estaticos.
 * 
 */
void PrintStaticData()
{
    pinMode(C_PIN_SCL_2, OUTPUT);
    pinMode(C_PIN_SDA_2, OUTPUT);
    Serial.print("\t");
    Serial.print(elements[C_RAM_MODEL].value);
    Serial.print(";");
    Serial.print(elements[C_RAM_SERIAL_NUMBER].value);
    Serial.print(";");
    Serial.print(elements[C_RAM_CONSUMPTION_ERROR].value);
    Serial.print(";");
    Serial.print(elements[C_RAM_POWER_ERROR].value);
    Serial.print(";");
    Serial.print(elements[C_RAM_VOLTAGE_ERROR].value);
    Serial.print(";");
    Serial.print(elements[C_RAM_WORK_TIME].value);
    Serial.print(";");
    // PrintStats(); Feature en pausa 28-03-2022.
    Serial.print("\n");
    pinMode(C_PIN_BUTT_CENTER, INPUT_PULLUP);
    //SEGGER_RTT_printf(0, "\n");
}