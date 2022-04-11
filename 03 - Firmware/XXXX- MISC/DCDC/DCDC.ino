/**
 * @file DCDC.ino
 * @author Javi (Javier@musotoku.com)
 * @brief 
 * @version 0.1
 * @date 2020-09-21
 * 
 * @copyright Copyright (c) 2020
 * 
 */

//-------------INCLUDE-------------------
#include <Wire.h> // Required for I2C communication
#include <Adafruit_GFX.h>
#include <Adafruit_IS31FL3731.h>
#include <SparkFunBQ27441.h>
#include <SPI.h>
#include <SD.h>
// ------- DEFINEs & CONSTANTS -------
const uint16_t LenDCDCvalues = 120;
const byte ADDR_I2C_DCDC = 0;
const uint16_t C_PIN_EN_DCDC = 4;
const uint16_t C_PIN_SHIPPING_MOD = 3;
const int16_t C_PIN_I_OUT = 15;
const int16_t C_PIN_V_OUT = 16;
const unsigned int BATTERY_CAPACITY = 2000; // e.g. 850mAh battery
const uint16_t DESIGNE_ENERGY = BATTERY_CAPACITY * 3.7f;
const uint16_t TERMITANTE_VOLTAGE = 3;
const uint16_t TAPER_CURRENT = 150;
const uint16_t TAPER_RATE = 10 * BATTERY_CAPACITY / TAPER_CURRENT;
//------- FUNCTIONS -------
void Write_TPIC2810();

//------- VARIABLES --------

PROGMEM const byte DCDCvalues[LenDCDCvalues] = {248, 4, 132, 68, 36, 164, 100, 228, 20, 148, 84, 52, 180, 116, 244, 12, 140, 76, 204, 44, 172, 108, 236, 28, 92, 220, 60, 188, 124, 252, 2, 130, 194, 34, 162, 98, 226, 18, 146, 82, 50, 178, 114, 242, 10, 138, 74, 202, 42, 170, 106, 234, 26, 90, 218, 58, 186, 122, 250, 6, 70, 198, 38, 166, 102, 230, 22, 150, 214, 54, 182, 118, 246, 14, 142, 78, 206, 46, 174, 110, 238, 30, 94, 222, 62, 190, 126, 254, 193, 33, 161, 97, 17, 145, 81, 209, 49, 177, 113, 241, 9, 137, 73, 201, 41, 105, 233, 25, 153, 89, 217, 57, 121, 249, 5, 133, 69, 197, 37, 101};

//PROGMEM const byte DCDCvalues[LenDCDCvalues] = {101, 37, 197, 69, 133, 5, 249, 121, 57, 217, 89, 153, 25, 233, 105, 41, 201, 73, 137, 9, 241, 113, 177, 49, 209, 81, 145, 17, 97, 161, 33, 193, 254, 126, 190, 62, 222, 94, 30, 238, 110, 174, 46, 206, 78, 142, 14, 246, 118, 182, 54, 214, 150, 22, 230, 102, 166, 38, 198, 70, 6, 250, 122, 186, 58, 218, 90, 26, 234, 106, 170, 42, 202, 74, 138, 10, 242, 114, 178, 50, 82, 146, 18, 226, 98, 162, 34, 194, 130, 2, 252, 124, 188, 60, 220, 92, 28, 236, 108, 172, 44, 204, 76, 140, 12, 244, 116, 180, 52, 84, 148, 20, 228, 100, 164, 36, 68, 132, 4, 248};
int value;
byte TPICvalue;
uint16_t encoderPos = 0;
uint16_t voltage = 0;
int16_t current_sense_values[8];
int16_t voltage_sense_values[8];
int32_t v_out_sense;
int32_t i_out_sense;
void setup()
{
    Wire.begin();
    Serial.begin(9600);
    setupBQ27441();
    while (!Serial)
        ;
    Serial.println("VERSION v1");
    Serial.println("Initilialization Done");
    Serial.println("Insert (a) for automatic ramp or (m) dor manually.-");
    pinMode(C_PIN_EN_DCDC, OUTPUT);
    digitalWrite(C_PIN_EN_DCDC, LOW);
    pinMode(C_PIN_SHIPPING_MOD, OUTPUT);
    digitalWrite(C_PIN_SHIPPING_MOD, HIGH);
}

void loop()
{

    if (Serial.available())
    {
        voltage = 40;
        SetVoltage(voltage);
        char ch1 = toupper(Serial.read());
        if (ch1 == 'A')
        {
            while (1)
            {
                TPICvalue = pgm_read_byte_near(DCDCvalues + encoderPos);
                Write_TPIC2810(ADDR_I2C_DCDC, TPICvalue);
                encoderPos += 1;
                voltage += 1;
                if (encoderPos > LenDCDCvalues)
                {
                    encoderPos = 0;
                    delay(1000);
                    voltage = 40;
                }
                voltage = constrain(voltage, 40, 160);
                Serial.print(" || ");
                Serial.print(encoderPos);
                Serial.print(" || ");
                Serial.println(voltage);

                delay(100);
            }
        }
        else if (ch1 == 'M')
        {
            Serial.println("Pulse 'W' para incrementar el voltage en 1 posicion.\nPulse 'S' para decrementar el voltage en 1 posicion.\n Pulse 'E' para ENCENDER.\n Pulse 'Q' para APAGAR.");
            while (1)
            {

                if (Serial.available())
                {
                    char ch2 = toupper(Serial.read());
                    if (ch2 == 'W')
                    {
                        voltage++;
                        SetVoltage(voltage);
                    }
                    else if (ch2 == 'S')
                    {
                        voltage--;
                        SetVoltage(voltage);
                    }
                    else if (ch2 == 'E')
                    {
                        digitalWrite(C_PIN_EN_DCDC, LOW);
                        SetVoltage(voltage);
                    }
                    else if (ch2 == 'Q')
                    {
                        digitalWrite(C_PIN_EN_DCDC, HIGH);
                    }
                    voltage = constrain(voltage, 40, 160);
                }
            }
        }
    }
}

// ================  Write_TPIC2810 =====================*

void Write_TPIC2810(byte address, byte data)
{
    Wire.beginTransmission(byte(96)); // transmit command to device TPIC2810
    Wire.write(byte(68));             // Command to transfer next value to output register
    Wire.write(byte(data));
    Wire.endTransmission(); // stop transmitting
}

void Arranque(int volt)
{
    volt = constrain(volt, 40, 160);
    encoderPos = volt - 40;
    for (int i = 120; i >= encoderPos; i--)
    {
        TPICvalue = pgm_read_byte_near(DCDCvalues + i);
        Write_TPIC2810(ADDR_I2C_DCDC, TPICvalue);
        delayMicroseconds(2000);
    }
    // Serial.print(encoderPos);
    // Serial.print("\t");
    // Serial.println(volt);
}
void SetVoltage(int volt)
{
    volt = constrain(volt, 40, 160);
    encoderPos = volt - 40;
    TPICvalue = pgm_read_byte_near(DCDCvalues + encoderPos);
    Write_TPIC2810(ADDR_I2C_DCDC, TPICvalue);
    Serial.print(encoderPos);
    Serial.print("\t");
    Serial.print(volt);
    SenseOutput();
    Serial.print("\t");
    Serial.print(v_out_sense);
    Serial.print("\t");
    Serial.println(TPICvalue);
}
void SenseOutput()
{
    i_out_sense = 0;
    v_out_sense = 0;
    //Serial.println("Voltage Sense");
    for (int i = (sizeof(voltage_sense_values) / sizeof(voltage_sense_values[0])) - 1; i >= 1; i--)
    {
        current_sense_values[i] = current_sense_values[i - 1];
        i_out_sense += current_sense_values[i];
        // Serial.println(current_sense_values[i]);
    }

    //Serial.println("Current Sense");
    for (int i = (sizeof(voltage_sense_values) / sizeof(voltage_sense_values[0])) - 1; i >= 1; i--)
    {
        voltage_sense_values[i] = voltage_sense_values[i - 1];
        v_out_sense += voltage_sense_values[i];
        // Serial.println(voltage_sense_values[i]);
    }

    current_sense_values[0] = 0;
    voltage_sense_values[0] = 0;
    for (int i = 0; i < 8; i++)
    {
        current_sense_values[0] += analogRead(C_PIN_I_OUT);
        voltage_sense_values[0] += analogRead(C_PIN_V_OUT);
        delay(1);
    }

    current_sense_values[0] = current_sense_values[0] >> 3;
    voltage_sense_values[0] = voltage_sense_values[0] >> 3;

    v_out_sense = ((voltage_sense_values[0])) * 4.84 * 5000 / 1023;

    i_out_sense = ((current_sense_values[0])) * 5000 / 1023;
}
void setupBQ27441()
{
    if (!lipo.begin()) // begin() will return true if communication is successful
    {
        // If communication fails, print an error message and loop forever.
        Serial.println("Error: Unable to communicate with BQ27441.");
        Serial.println("  Check wiring and try again.");
        Serial.println("  (Battery must be plugged into Battery Babysitter!)");
        while (1)
            ;
    }
    Serial.println("Connected to BQ27441!");
    Serial.println("Configure?(Y/N)");

    lipo.enterConfig();
    lipo.setCapacity(BATTERY_CAPACITY);
    lipo.setDesignEnergy(DESIGNE_ENERGY);
    lipo.setTerminateVoltage(TERMITANTE_VOLTAGE);
    lipo.setTaperRate(TAPER_RATE);

    lipo.exitConfig();
}