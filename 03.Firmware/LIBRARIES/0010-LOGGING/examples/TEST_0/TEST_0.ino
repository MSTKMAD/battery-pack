/**
 * @file TEST_0.ino
 * @author Javi (Javier@musotoku.com)
 * @brief Test de las funciones de PrintHeader, PrintDiagnosticData y LogDiagnosticData.
 * @version 2
 * @date 2021-03-22
 * 
 * @copyright Copyright (c) 2021
 * 
 */
#include <diagnostic.h>
extern "C"
{
#include <SEGGER_RTT.h>
}

void setup()
{
    Serial.begin(9600);
    while (!Serial)
    {
        /* code */
    }
    Init_diagnostic_elements();

    for (int i = 0; i < 50; i++)
    {
        LogDiagnosticData(i * 3, i);
    }
    PrintStaticData();
    PrintDiagnosticData();
    delay(1000);
}

void loop()
{
}