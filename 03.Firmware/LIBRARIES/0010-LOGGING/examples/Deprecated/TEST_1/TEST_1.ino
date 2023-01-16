/**
 * @file TEST_1.ino
 * @author Javi (Javier@musotoku.com)
 * @brief Testeo de la funcionalidad de guardado en EEMPROM tanto Chasis como Batt Pack. Prueba de las funciones isValid(), Incremeent DiagnosticData.
 * @version 1
 * @date 2021-03-22
 * 
 * @copyright Copyright (c) 2021
 * 
 */
const int16_t C_PIN_SCL_2 = 13;
const int16_t C_PIN_SDA_2 = 10;

const int16_t C_PIN_BUTT_UP = 3;
const int16_t C_PIN_BUTT_DOWN = 4;
const int16_t C_PIN_BUTT_CENTER = 13;

#include <diagnostic.h>
void setup()
{
    Serial5.begin(9600);
    Init_diagnostic_elements();
    if (isValid() == true)
    {
        Serial5.println("Valid!");
        for (int i = 0; i < C_NUM_ELEMENTS; i++)
        {
            Serial5.print(ReadDiagnosticData(i));
            Serial5.print(";");
        }
        Serial5.println();
    }
    else
    {
        Serial5.println("Not Valid");
        for (int i = 0; i < C_NUM_ELEMENTS; i++)
        {
            LogDiagnosticData(i, i);
        }
        for (int i = 0; i < 5; i++)
        {
            IncrementDiagnosticData(i * 10, 3 + i);
        }
        for (int i = 0; i < C_NUM_ELEMENTS; i++)
        {
            Serial5.print(ReadDiagnosticData(i));
            Serial5.print(";");
        }
        Serial5.println();
        SaveEepromChasis();
        UpdateEepromBatery();
    }
    Serial5.println("------------------------------------------------------");
}

void loop()
{
}