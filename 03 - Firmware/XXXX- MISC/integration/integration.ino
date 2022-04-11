/**
   @file integration.ino
   @author Javi (Javier@musotoku.com)
   @brief
   @version 0.1
   @date 2020-10-28

   @copyright Copyright (c) 2020

*/

//---------- Include ----------//
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_IS31FL3731.h>
#include "bitmaps.h"
#include "display.h"
#include <SparkFunBQ27441.h>
#include <SPI.h>
#include <SD.h>
#include <EEPROM.h>

#define NOTE_B0 31
#define NOTE_C1 33
#define NOTE_CS1 35
#define NOTE_D1 37
#define NOTE_DS1 39
#define NOTE_E1 41
#define NOTE_F1 44
#define NOTE_FS1 46
#define NOTE_G1 49
#define NOTE_GS1 52
#define NOTE_A1 55
#define NOTE_AS1 58
#define NOTE_B1 62
#define NOTE_C2 65
#define NOTE_CS2 69
#define NOTE_D2 73
#define NOTE_DS2 78
#define NOTE_E2 82
#define NOTE_F2 87
#define NOTE_FS2 93
#define NOTE_G2 98
#define NOTE_GS2 104
#define NOTE_A2 110
#define NOTE_AS2 117
#define NOTE_B2 123
#define NOTE_C3 131
#define NOTE_CS3 139
#define NOTE_D3 147
#define NOTE_DS3 156
#define NOTE_E3 165
#define NOTE_F3 175
#define NOTE_FS3 185
#define NOTE_G3 196
#define NOTE_GS3 208
#define NOTE_A3 220
#define NOTE_AS3 233
#define NOTE_B3 247
#define NOTE_C4 262
#define NOTE_CS4 277
#define NOTE_D4 294
#define NOTE_DS4 311
#define NOTE_E4 330
#define NOTE_F4 349
#define NOTE_FS4 370
#define NOTE_G4 392
#define NOTE_GS4 415
#define NOTE_A4 440
#define NOTE_AS4 466
#define NOTE_B4 494
#define NOTE_C5 523
#define NOTE_CS5 554
#define NOTE_D5 587
#define NOTE_DS5 622
#define NOTE_E5 659
#define NOTE_F5 698
#define NOTE_FS5 740
#define NOTE_G5 784
#define NOTE_GS5 831
#define NOTE_A5 880
#define NOTE_AS5 932
#define NOTE_B5 988
#define NOTE_C6 1047
#define NOTE_CS6 1109
#define NOTE_D6 1175
#define NOTE_DS6 1245
#define NOTE_E6 1319
#define NOTE_F6 1397
#define NOTE_FS6 1480
#define NOTE_G6 1568
#define NOTE_GS6 1661
#define NOTE_A6 1760
#define NOTE_AS6 1865
#define NOTE_B6 1976
#define NOTE_C7 2093
#define NOTE_CS7 2217
#define NOTE_D7 2349
#define NOTE_DS7 2489
#define NOTE_E7 2637
#define NOTE_F7 2794
#define NOTE_FS7 2960
#define NOTE_G7 3136
#define NOTE_GS7 3322
#define NOTE_A7 3520
#define NOTE_AS7 3729
#define NOTE_B7 3951
#define NOTE_C8 4186
#define NOTE_CS8 4435
#define NOTE_D8 4699
#define NOTE_DS8 4978
#define NOTE_E8 5274
#define NOTE_F8 5588
#define NOTE_FS8 5920
#define NOTE_G8 6272
#define NOTE_GS8 6644
#define NOTE_A8 7040
#define NOTE_AS8 7458
#define NOTE_B8 7902
#define NOTE_C9 8372
#define NOTE_CS9 8870
#define NOTE_D9 9398
#define NOTE_DS9 9956
#define NOTE_E9 10548
#define NOTE_F9 11176
#define NOTE_FS9 11840
#define NOTE_G9 12544
#define NOTE_GS9 13288
#define NOTE_A9 14080
#define NOTE_AS9 14916
#define NOTE_B9 15804
Adafruit_IS31FL3731_Wing ledmatrix = Adafruit_IS31FL3731_Wing();
//---------- Constantes  ----------//
/*      Buttons      */
const int16_t C_LP_CENTER = 0x01;
const int16_t C_LP_RIGHT = 0x02;
const int16_t C_LP_LEFT = 0x03;
const int16_t C_LP_UP = 0x04;
const int16_t C_LP_DOWN = 0x05;
const int16_t C_CLICK_CENTER = 0x06;
const int16_t C_CLICK_RIGTH = 0x07;
const int16_t C_CLICK_LEFT = 0x08;
const int16_t C_CLICK_UP = 0x09;
const int16_t C_CLICK_DOWN = 0x0A;
const int16_t C_NONE_EVENT = 0x0B;

const int16_t C_TIMER_REBOUND = 20;
const int32_t C_TIMER_LONGPRESS_1 = 1000;
/*      Display     */
uint8_t brightness = 30;
/*      Battery     */
const unsigned int BATTERY_CAPACITY = 2000; // e.g. 850mAh battery
const uint16_t DESIGNE_ENERGY = BATTERY_CAPACITY * 3.7f;
const uint16_t TERMITANTE_VOLTAGE = 3;
const uint16_t TAPER_CURRENT = 150;
const uint16_t TAPER_RATE = 10 * BATTERY_CAPACITY / TAPER_CURRENT;

/*     TIMER       */
const int32_t C_TIMER_WD = 30000;
const int32_t C_TIMER_STOP = 15 * 60000;
const int32_t C_BREATH_TIME = 2 * 60000;

/*      DCDC    */
const uint16_t LenDCDCvalues = 120;
const byte ADDR_I2C_DCDC = 0;
PROGMEM const byte DCDCvalues[LenDCDCvalues] = {248, 4, 132, 68, 36, 164, 100, 228, 20, 148, 84, 52, 180, 116, 244, 12, 140, 76, 204, 44, 172, 108, 236, 28, 92, 220, 60, 188, 124, 252, 2, 130, 194, 34, 162, 98, 226, 18, 146, 82, 50, 178, 114, 242, 10, 138, 74, 202, 42, 170, 106, 234, 26, 90, 218, 58, 186, 122, 250, 6, 70, 198, 38, 166, 102, 230, 22, 150, 214, 54, 182, 118, 246, 14, 142, 78, 206, 46, 174, 110, 238, 30, 94, 222, 62, 190, 126, 254, 193, 33, 161, 97, 17, 145, 81, 209, 49, 177, 113, 241, 9, 137, 73, 201, 41, 105, 233, 25, 153, 89, 217, 57, 121, 249, 5, 133, 69, 197, 37, 101};

//PROGMEM const byte DCDCvalues[LenDCDCvalues] = {101, 37, 197, 69, 133, 5, 249, 121, 57, 217, 89, 153, 25, 233, 105, 41, 201, 73, 137, 9, 241, 113, 177, 49, 209, 81, 145, 17, 97, 161, 33, 193, 254, 126, 190, 62, 222, 94, 30, 238, 110, 174, 46, 206, 78, 142, 14, 246, 118, 182, 54, 214, 150, 22, 230, 102, 166, 38, 198, 70, 6, 250, 122, 186, 58, 218, 90, 26, 234, 106, 170, 42, 202, 74, 138, 10, 242, 114, 178, 50, 82, 146, 18, 226, 98, 162, 34, 194, 130, 2, 252, 124, 188, 60, 220, 92, 28, 236, 108, 172, 44, 204, 76, 140, 12, 244, 116, 180, 52, 84, 148, 20, 228, 100, 164, 36, 68, 132, 4, 248};

/*      Protections     */

const int16_t C_POWER_PROTECTION = 4000;
const int16_t C_VOLTAGE_PROTECTION = 2000;
const int16_t C_CURRENT_PROTECTION = 450;

const int16_t C_TIME_POWER_PROTECTION = 2000;   //ms
const int16_t C_TIME_CURRENT_PROTECTION = 1500; //ms
const int16_t C_TIME_VOLTAGE_PROTECTION = 1000; //ms

const int16_t C_CONT_POWER_PROTECTION = 200;   //ms
const int16_t C_CONT_CURRENT_PROTECTION = 150; //ms
const int16_t C_CONT_VOLTAGE_PROTECTION = 100; //ms

//---------- Pines ----------//
/*    SD    */
const int chipSelect = 10;
/*      Battery     */
const uint16_t C_PIN_BUZZER = 4;
const uint16_t C_PIN_EN_DCDC = 2;
const uint16_t C_PIN_SHIPPING_MOD = 3;

/*      Buttons     */
const int16_t C_PIN_BUTT_UP = 9;
const int16_t C_PIN_BUTT_DOWN = 8;
const int16_t C_PIN_BUTT_RIGHT = 6;
const int16_t C_PIN_BUTT_LEFT = 7;
const int16_t C_PIN_BUTT_CENTER = 5;
/*      Sensing     */
const int16_t C_PIN_I_OUT = 15;
const int16_t C_PIN_V_OUT = 16;

//---------- Variables Globales ----------//
/*     DCDC     */
int value;
byte TPICvalue;
uint16_t encoderPos = 0;
int voltage = 0;

/*    Babysitter      */
int16_t socf = 0;                 // Read state-of-charge (%)
int16_t socunf = 0;               // Read state-of-charge (%)
int16_t volts = 0;                // Read battery voltage (mV)
int16_t currentavg = 0;           // Read average current (mA)
int16_t currentstby = 0;          // Read average current (mA)
int16_t currentmax = 0;           // Read average current (mA)
int16_t fullCapacity = 0;         // Read full capacity (mAh)
int16_t availCapacity = 0;        // Read full capacity (mAh)
int16_t availfullCapacity = 0;    // Read full capacity (mAh)
int16_t remainfilCapacity = 0;    // Read full capacity (mAh)
int16_t remainunfiltCapacity = 0; // Read full capacity (mAh)
int16_t fullfiltCapacity = 0;     // Read full capacity (mAh)
int16_t fullunfiltCapacity = 0;   // Read full capacity (mAh)
int16_t designCapacity = 0;       // Read full capacity (mAh)
int16_t capacity = 0;             // Read remaining capacity (mAh)
int16_t temperatura = 0;          // specified temperature measurement in degrees C
int16_t power = 0;                // Read average power draw (mW)
int16_t healthperc = 0;           // Read state-of-health (%)
String toPrint;

/*     TIMER       */
int32_t timer_wd = 0;
bool wd_flag = false;
int32_t timer_stop = 0;
bool stop_flag = false;

/*      Display     */
String data = "";

/*      Buttons     */
int16_t button_event = C_NONE_EVENT;
bool enable_out = false;

/*      Logging     */

int16_t current_sense_values[8];
int16_t voltage_sense_values[8];
int32_t v_out_sense;
int32_t i_out_sense;

/*      Protections     */
int16_t power_cont;
int16_t current_cont;
int16_t voltage_cont;

int16_t cont = 0;
int16_t cont_five = 0;
int16_t bytes_written = 0;

bool flag_cycle = true;
bool flag_arranque = true;

//---------- Timers ----------//

//------------------------------------------------------------------------------------------//
//                                   Main                                                   //
//------------------------------------------------------------------------------------------//
void setup()
{
    Serial.begin(9600);
    Wire.begin();
    timer_wd = millis();
    timer_stop = millis();

    //Serial.println("Pulse 'W' para incrementar el voltage en 1 posicion.\nPulse 'S' para decrementar el voltage en 1 posicion.\n Pulse 'E' para ENCENDER.\n Pulse 'Q' para APAGAR.");
    pinMode(C_PIN_EN_DCDC, OUTPUT);
    pinMode(C_PIN_BUZZER, OUTPUT);
    pinMode(C_PIN_SHIPPING_MOD, OUTPUT);
    pinMode(chipSelect, OUTPUT);

    pinMode(C_PIN_BUTT_UP, INPUT);
    pinMode(C_PIN_BUTT_DOWN, INPUT);
    pinMode(C_PIN_BUTT_RIGHT, INPUT);
    pinMode(C_PIN_BUTT_LEFT, INPUT);
    pinMode(C_PIN_BUTT_CENTER, INPUT);

    digitalWrite(C_PIN_BUZZER, LOW);
    digitalWrite(C_PIN_EN_DCDC, LOW);
    digitalWrite(C_PIN_SHIPPING_MOD, HIGH);
    digitalWrite(C_PIN_EN_DCDC, LOW);
    digitalWrite(chipSelect, LOW);

    tone(C_PIN_BUZZER, 1000, 100); //milisegundos

    setupBQ27441();

    // setupSD();
    if (!SD.begin(14))
    {
        Serial.println("Card failed, or not present");
        // don't do anything more:
        while (1)
            ;
    }
    Serial.println("card initialized.");
    setupDisplay();
    voltage = 92;
    SetVoltage(voltage);
    displayVolts(voltage, ledmatrix);
    delay(200);
}

void loop()
{
    ReadBatteryStats();
    SenseOutput();
    toPrint += String(v_out_sense) + ";";
    toPrint += String(i_out_sense) + ";";
    Serial.println(toPrint);

    if ((socf >= 95 && flag_cycle == false) && currentavg == 0)
    {
        flag_cycle = true;
        while (button_event != C_CLICK_CENTER)
        {
            button_event = ReadDirPad();
            digitalWrite(C_PIN_SHIPPING_MOD, LOW);
            tone(C_PIN_BUZZER, NOTE_B5, 100);
            delay(100);
            tone(C_PIN_BUZZER, NOTE_E6, 350);
            delay(500);
            noTone(8);
            delay(3000);
        }
    }
    else if (volts <= 3200)
    {
        cont_five++;
        if (cont_five == 5)
        {
            flag_cycle = false;
            while (button_event != C_CLICK_CENTER)
            {
                button_event = ReadDirPad();
                digitalWrite(C_PIN_SHIPPING_MOD, LOW);
                tone(C_PIN_BUZZER, NOTE_B5, 100);
                delay(100);
                tone(C_PIN_BUZZER, NOTE_E6, 350);
                delay(500);
                noTone(8);
                delay(3000);
            }
            cont_five = 0;
        }
    }
    if (cont == 60 * 30)
    {
        digitalWrite(C_PIN_SHIPPING_MOD, LOW);
    }
    else if (cont == 60 * 30 + 30)
    {
        SetVoltage(40);
        digitalWrite(C_PIN_SHIPPING_MOD, HIGH);
        Arranque(voltage);
        cont = 0;
    }

    File dataFile = SD.open("datalog.csv", FILE_WRITE);
    if (dataFile)
    {
        bytes_written = dataFile.println(toPrint);
        // if (bytes_written == 0)
        // {
        //     Serial.println("WRITE FAIL");
        // }
        // else
        // {
        //    // Serial.println(bytes_written);
        // }
        dataFile.close();
    }
    else
    {
        Serial.println("OPEN FAIL");
    }
    cont++;
    SetVoltage(voltage);
    //VoltageProtection();
    //CurrentProteccion();
    //PowerProtection();
    delay(1000);

    // /*  Buttons State Machine   */
    // if (button_event == C_CLICK_UP)
    // {
    //     voltage += 1;
    //     voltage = constrain(voltage, 40, 160);
    //     //Serial.print("Voltage: ");
    //     Serial.println(voltage);
    //     tone(C_PIN_BUZZER, 1000, 100);
    //     tone(C_PIN_BUZZER, 2000, 100);
    //     displayVolts(voltage, ledmatrix);
    //     delay(200);

    //     SetVoltage(voltage);
    //     //Serial.println(button_event);
    // }

    // if (button_event == C_LP_UP)
    // {
    //     voltage += 10;
    //     voltage = constrain(voltage, 40, 160);
    //     //Serial.print("Voltage: ");
    //     Serial.println(voltage);
    //     tone(C_PIN_BUZZER, 1000, 100);
    //     tone(C_PIN_BUZZER, 2000, 100);
    //     displayVolts(voltage, ledmatrix);
    //     delay(200);

    //     SetVoltage(voltage);
    //     // Serial.println(button_event);
    // }
    // if (button_event == C_LP_DOWN)
    // {
    //     voltage -= 10;
    //     voltage = constrain(voltage, 40, 160);
    //     //Serial.print("Voltage: ");
    //     Serial.println(voltage);
    //     tone(C_PIN_BUZZER, 2000, 100);
    //     tone(C_PIN_BUZZER, 1000, 100);
    //     displayVolts(voltage, ledmatrix);
    //     delay(200);
    //     SetVoltage(voltage);
    //     // Serial.println(button_event);
    // }

    // if (button_event == C_CLICK_DOWN)
    // {
    //     voltage -= 1;
    //     voltage = constrain(voltage, 40, 160);
    //     //Serial.print("Voltage: ");
    //     Serial.println(voltage);
    //     tone(C_PIN_BUZZER, 2000, 100);
    //     tone(C_PIN_BUZZER, 1000, 100);
    //     displayVolts(voltage, ledmatrix);
    //     delay(200);
    //     SetVoltage(voltage);
    //     //Serial.println(button_event);
    // }
    // if (button_event == C_CLICK_CENTER)
    // {
    //     tone(C_PIN_BUZZER, 2000, 100);
    //     tone(C_PIN_BUZZER, 1000, 100);
    //     if (enable_out == true)
    //     {
    //         enable_out = false;
    //         digitalWrite(C_PIN_SHIPPING_MOD, LOW);
    //         data = "OFF";
    //         displayTextBlock(data, ledmatrix);
    //         delay(200);

    //         Serial.println("OFF");
    //     }
    //     else if (enable_out == false)
    //     {
    //         enable_out = true;
    //         digitalWrite(C_PIN_SHIPPING_MOD, HIGH);
    //         Arranque(voltage);
    //         displayVolts(voltage, ledmatrix);
    //         delay(200);

    //         Serial.println(voltage);
    //     }
    // }

    // /*  Serial Port  */

    // if (Serial.available())
    // {
    //     char ch2 = toupper(Serial.read());
    //     if (ch2 == 'E')
    //     {
    //         digitalWrite(C_PIN_SHIPPING_MOD, HIGH);
    //         Arranque(voltage);
    //         displayVolts(80, ledmatrix);
    //         delay(200);
    //     }
    //     else if (ch2 == 'Q')
    //     {
    //         digitalWrite(C_PIN_SHIPPING_MOD, LOW);
    //         data = "OFF";
    //         displayTextBlock(data, ledmatrix);
    //         delay(200);
    //     }
    // }
}
//------------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------------//

//---------- Funciones ----------//
void Write_TPIC2810(byte address, byte data)
{
    Wire.beginTransmission(byte(96)); // transmit command to device TPIC2810
    Wire.write(byte(68));             // Command to transfer next value to output register
    Wire.write(byte(data));
    Wire.endTransmission(); // stop transmitting
}

void Arranque(int volt)
{
    flag_arranque = true;
    volt = constrain(volt, 40, 160);
    encoderPos = volt - 40;
    for (int i = 120; i >= encoderPos; i--)
    {
        TPICvalue = pgm_read_byte_near(DCDCvalues + i);
        Write_TPIC2810(ADDR_I2C_DCDC, TPICvalue);
        delayMicroseconds(2000);
    }
    flag_arranque = false;
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
    // Serial.print(encoderPos);
    // Serial.print("\t");
    // Serial.println(volt);
}

void ReadBatteryStats()
{
    socf = lipo.soc(FILTERED);                       // Read state-of-charge (%)
    socunf = lipo.soc(UNFILTERED);                   // Read state-of-charge (%)
    volts = lipo.voltage();                          // Read battery voltage (mV)
    currentavg = lipo.current(AVG);                  // Read average current (mA)
    currentstby = lipo.current(STBY);                // Read average current (mA)
    currentmax = lipo.current(MAX);                  // Read average current (mA)
    fullCapacity = lipo.capacity(FULL);              // Read full capacity (mAh)
    availCapacity = lipo.capacity(AVAIL);            // Read full capacity (mAh)
    availfullCapacity = lipo.capacity(AVAIL_FULL);   // Read full capacity (mAh)
    remainfilCapacity = lipo.capacity(REMAIN_F);     // Read full capacity (mAh)
    remainunfiltCapacity = lipo.capacity(REMAIN_UF); // Read full capacity (mAh)
    fullfiltCapacity = lipo.capacity(FULL_F);        // Read full capacity (mAh)
    fullunfiltCapacity = lipo.capacity(FULL_UF);     // Read full capacity (mAh)
    designCapacity = lipo.capacity(DESIGN);          // Read full capacity (mAh)
    capacity = lipo.capacity(REMAIN);                // Read remaining capacity (mAh)
    power = lipo.power();                            // Read average power draw (mW)
    healthperc = lipo.soh();                         // Read state-of-health (%)
    temperatura = lipo.temperature(1);

    toPrint = String(socf) + ";";
    toPrint += String(volts) + ";";
    toPrint += String(currentavg) + ";";
    toPrint += String(capacity) + ";";
    toPrint += String(fullCapacity) + ";";
    toPrint += String(designCapacity) + ";";
    toPrint += String(power) + ";";
    toPrint += String(temperatura) + ";";
    toPrint += String(healthperc) + ";";
}
// void setupSD()
// {
//     Serial.println("Initializing SD card...");
//     if (SD.begin(14))
//     {
//         Serial.println("Connected to SD Card!");
//         while (Serial.available())
//         {
//             Serial.read();
//         }
//         Serial.println("Erase the SD card ?(Y/N)");
//         while (!Serial.available())
//             ;

//         if (Serial.available())
//         {

//             char c = toupper(Serial.read());
//             Serial.println(c);
//             if (c == 'Y')
//             {
//                 SD.remove("datalog.csv");
//                 File dataFile = SD.open("datalog.csv", FILE_WRITE);
//                 if (dataFile)
//                 {
//                     dataFile.println("Percent;Volts;Current AVG;Capacity;FullCapacity;DesignCapacity;Power;Temp;Health;Cycles");
//                     dataFile.close();
//                     Serial.println("Percent;Volts;Current AVG;Capacity;FullCapacity;DesignCapacity;Power;Temp;Health;Cycles");
//                     Serial.println("Card Erased.");
//                 }
//             }
//             else
//             {
//                 Serial.println("Card NOT Erased.");
//             }
//         }
//         else
//         {
//             while (1)
//                 ;
//         }
//     }
// }
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
    while (!Serial.available() && ((millis() - timer_wd) < C_TIMER_WD))
    {
        // Serial.print((millis() - timer_wd));
        // Serial.print("/");
        // Serial.println(C_TIMER_WD);
    }
    if (Serial.available())
    {
        char c = toupper(Serial.read());
        if (c == 'Y')
        {
            lipo.enterConfig();
            lipo.setCapacity(BATTERY_CAPACITY);
            lipo.setDesignEnergy(DESIGNE_ENERGY);
            lipo.setTerminateVoltage(TERMITANTE_VOLTAGE);
            lipo.setTaperRate(TAPER_RATE);

            lipo.exitConfig();
        }
        while (Serial.available())
        {
            Serial.read();
        }
    }
}
void setupDisplay()
{
    Serial.println("ISSI manual animation test");
    if (!ledmatrix.begin())
    {
        Serial.println("IS31 not found");
        while (1)
            ;
    }
    Serial.println("IS31 Found!");
    ledmatrix.setTextWrap(false);
    ledmatrix.setTextColor(100); // quarter brightness
    ledmatrix.setTextSize(1);
    uint8_t brightness = 30;
}
int16_t ReadDirPad()
{
    int event = C_NONE_EVENT;
    int16_t cont_changes = 0;

    static bool up_pressed = false;
    static bool up_released = false;
    static bool down_pressed = false;
    static bool down_released = false;

    static bool up_fast_mode_flag = false;
    static bool down_fast_mode_flag = false;

    static int32_t up_rebound_timer = 0;
    static int32_t up_longpress_timer = 0;
    static int32_t down_rebound_timer = 0;
    static int32_t down_longpress_timer = 0;

    static bool up_butt_last_state = false;
    static bool up_butt_state = false;
    static bool down_butt_last_state = false;
    static bool down_butt_state = false;
    static bool left_butt_last_state = false;
    static bool left_butt_state = false;
    static bool right_butt_last_state = false;
    static bool right_butt_state = false;
    static bool center_butt_last_state = false;
    static bool center_butt_state = false;

    up_butt_state = digitalRead(C_PIN_BUTT_UP);
    down_butt_state = digitalRead(C_PIN_BUTT_DOWN);
    left_butt_state = digitalRead(C_PIN_BUTT_LEFT);
    right_butt_state = digitalRead(C_PIN_BUTT_RIGHT);
    center_butt_state = digitalRead(C_PIN_BUTT_CENTER);

    //  UP
    /* Falling Edge detection.*/
    if (up_butt_state == false && up_butt_last_state == true)
    {
        up_pressed = true;
        //Serial.print("fall");
    }
    if (up_pressed == true)
    {
        if ((millis() - up_rebound_timer) >= C_TIMER_REBOUND)
        {
            if ((millis() - up_longpress_timer) < C_TIMER_LONGPRESS_1)
            {
                /* Rising Edge detection.*/
                if (up_butt_state == true && up_butt_last_state == false)
                {
                    up_released = true;
                    up_rebound_timer = millis();
                }
            }
            else
            {
                // Serial.print("lp");
                up_fast_mode_flag = true;
                up_longpress_timer = millis();
                event = C_LP_UP;
                cont_changes += 1;
            }
        }
    }
    else
    {
        up_rebound_timer = millis();
        up_longpress_timer = millis();
    }
    if (up_released == true)
    {
        if ((millis() - up_rebound_timer) >= C_TIMER_REBOUND)
        {
            // Serial.print("rise");
            if (up_fast_mode_flag)
            {
                event = C_NONE_EVENT;
                up_fast_mode_flag = false;
            }
            else
            {
                event = C_CLICK_UP;
            }
            up_pressed = false;
            up_released = false;
            cont_changes += 1;
        }
    }

    //  DOWN
    if (down_butt_state == false && up_butt_last_state == true)
    {
        down_pressed = true;
        //Serial.print("fall");
    }
    if (down_pressed)
    {
        if ((millis() - down_rebound_timer) >= C_TIMER_REBOUND)
        {
            if ((millis() - down_longpress_timer) < C_TIMER_LONGPRESS_1)
            {
                if (down_butt_state == true && down_butt_last_state == false)
                {
                    down_released = true;
                    down_rebound_timer = millis();
                }
            }
            else
            {
                // Serial.print("lp");
                down_fast_mode_flag = true;
                down_longpress_timer = millis();
                event = C_LP_DOWN;
                cont_changes += 1;
            }
        }
    }
    else
    {
        down_rebound_timer = millis();
        down_longpress_timer = millis();
    }
    if (down_released)
    {
        if ((millis() - down_rebound_timer) >= C_TIMER_REBOUND)
        {
            // Serial.print("rise");
            if (down_fast_mode_flag)
            {
                event = C_NONE_EVENT;
                down_fast_mode_flag = false;
            }
            else
            {
                event = C_CLICK_DOWN;
            }
            down_pressed = false;
            down_released = false;
            cont_changes += 1;
        }
    }
    // RIGHT
    if (right_butt_state == true && right_butt_last_state == false)
    {
        event = C_CLICK_RIGTH;
        cont_changes += 1;
    }
    if (left_butt_state == true && left_butt_last_state == false)
    {
        event = C_CLICK_LEFT;
        cont_changes += 1;
    }
    if (center_butt_state == true && center_butt_last_state == false)
    {
        event = C_CLICK_CENTER;
        cont_changes += 1;
    }
    //Serial.println(cont_changes);

    center_butt_last_state = center_butt_state;
    left_butt_last_state = left_butt_state;
    right_butt_last_state = right_butt_state;
    up_butt_last_state = up_butt_state;
    down_butt_last_state = down_butt_state;

    if (cont_changes == 1)
    {
        return event;
    }
    else
    {
        return C_NONE_EVENT;
    }
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

    v_out_sense = ((v_out_sense + voltage_sense_values[0]) >> 3) * 4.84 * 5000 / 1023;

    i_out_sense = ((i_out_sense + current_sense_values[0]) >> 3) * 5000 / 1023;
}
void PowerProtection()
{
    int16_t instant_power = v_out_sense * i_out_sense / 1000;
    if (flag_arranque != true)
    {
        if (instant_power <= C_POWER_PROTECTION)
        {
            power_cont += 5;
        }
        else
        {
            power_cont -= 1;
        }

        if (power_cont >= C_CONT_POWER_PROTECTION)
        {
            digitalWrite(C_PIN_SHIPPING_MOD, LOW);
        }
    }
}
void VoltageProtection()
{
    if (voltage - v_out_sense >= C_VOLTAGE_PROTECTION)
    {
        voltage_cont += 5;
    }
    else
    {
        voltage_cont -= 1;
    }
    if (voltage_cont >= C_CONT_VOLTAGE_PROTECTION)
    {
        digitalWrite(C_PIN_SHIPPING_MOD, LOW);
    }
}
void CurrentProteccion()
{
    if (i_out_sense >= C_CURRENT_PROTECTION)
    {
        current_cont += 5;
    }
    else
    {
        current_cont -= 1;
    }
    if (current_cont >= C_CONT_CURRENT_PROTECTION)
    {
        digitalWrite(C_PIN_SHIPPING_MOD, LOW);
    }
}
