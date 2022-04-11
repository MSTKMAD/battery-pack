/**
 * @file TEST-DCDC.ino          
 * @author Javi (Javier@musotoku.com)
 * @brief 
 * @version 0.1
 * @date 2020-10-13
 * 
 * @copyright Copyright (c) 2020
 * 
 */

#include <SparkFunBQ27441.h>
#include <Wire.h> 

// Set BATTERY_CAPACITY to the design capacity of your battery.
const unsigned int BATTERY_CAPACITY = 2000; // e.g. 850mAh battery
const uint16_t DESIGNE_ENERGY = BATTERY_CAPACITY * 3.7f;
const uint16_t TERMITANTE_VOLTAGE = 3;
const uint16_t TAPER_CURRENT = 150;
const uint16_t TAPER_RATE = 10 * BATTERY_CAPACITY / TAPER_CURRENT;

const uint16_t C_PIN_BUZZER = 5;
const uint16_t C_PIN_EN_DCDC = 4;
const uint16_t C_PIN_SHIPPING_MOD = 3;
const uint16_t LenDCDCvalues = 120;
const byte ADDR_I2C_DCDC = 0;
PROGMEM const byte DCDCvalues[LenDCDCvalues] = {248, 4, 132, 68, 36, 164, 100, 228, 20, 148, 84, 52, 180, 116, 244, 12, 140, 76, 204, 44, 172, 108, 236, 28, 92, 220, 60, 188, 124, 252, 2, 130, 194, 34, 162, 98, 226, 18, 146, 82, 50, 178, 114, 242, 10, 138, 74, 202, 42, 170, 106, 234, 26, 90, 218, 58, 186, 122, 250, 6, 70, 198, 38, 166, 102, 230, 22, 150, 214, 54, 182, 118, 246, 14, 142, 78, 206, 46, 174, 110, 238, 30, 94, 222, 62, 190, 126, 254, 193, 33, 161, 97, 17, 145, 81, 209, 49, 177, 113, 241, 9, 137, 73, 201, 41, 105, 233, 25, 153, 89, 217, 57, 121, 249, 5, 133, 69, 197, 37, 101};
byte TPICvalue;

uint16_t i = 0;

void setup()
{
    Serial.begin(115200);
    setupBQ27441();
    pinMode(C_PIN_BUZZER, OUTPUT);
    pinMode(C_PIN_EN_DCDC, OUTPUT);
    pinMode(C_PIN_SHIPPING_MOD, OUTPUT);

    digitalWrite(C_PIN_BUZZER, LOW);
    digitalWrite(C_PIN_EN_DCDC, LOW);
    digitalWrite(C_PIN_SHIPPING_MOD, LOW);
}

void loop()
{
    printBatteryStats();
    if(i > LenDCDCvalues){
        i = 0;
    }else
    {
        i++;
    }
    
    TPICvalue = pgm_read_byte_near(DCDCvalues + i );
    Serial.println(TPICvalue);
    Write_TPIC2810(ADDR_I2C_DCDC, TPICvalue);
    delay(1000);
}

//-----------------------------------------------------------------------------------//

void setupBQ27441(void)
{
    // Use lipo.begin() to initialize the BQ27441-G1A and confirm that it's
    // connected and communicating.
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
    Serial.println("Configure?");
    while (!Serial.available())
        ;
    char c = toupper(Serial.read());
    if (c == 'Y')
    {
        // Uset lipo.setCapacity(BATTERY_CAPACITY) to set the design capacity
        // of your battery.
        lipo.enterConfig();
        lipo.setCapacity(BATTERY_CAPACITY);
        lipo.setDesignEnergy(DESIGNE_ENERGY);
        lipo.setTerminateVoltage(TERMITANTE_VOLTAGE);
        lipo.setTaperRate(TAPER_RATE);

        lipo.exitConfig();
    }
}

void printBatteryStats()
{
    // Read battery stats from the BQ27441-G1A
    unsigned int socf = lipo.soc(FILTERED);                       // Read state-of-charge (%)
    unsigned int socunf = lipo.soc(UNFILTERED);                   // Read state-of-charge (%)
    unsigned int volts = lipo.voltage();                          // Read battery voltage (mV)
    int currentavg = lipo.current(AVG);                           // Read average current (mA)
    int currentstby = lipo.current(STBY);                         // Read average current (mA)
    int currentmax = lipo.current(MAX);                           // Read average current (mA)
    unsigned int fullCapacity = lipo.capacity(FULL);              // Read full capacity (mAh)
    unsigned int availCapacity = lipo.capacity(AVAIL);            // Read full capacity (mAh)
    unsigned int availfullCapacity = lipo.capacity(AVAIL_FULL);   // Read full capacity (mAh)
    unsigned int remainfilCapacity = lipo.capacity(REMAIN_F);     // Read full capacity (mAh)
    unsigned int remainunfiltCapacity = lipo.capacity(REMAIN_UF); // Read full capacity (mAh)
    unsigned int fullfiltCapacity = lipo.capacity(FULL_F);        // Read full capacity (mAh)
    unsigned int fullunfiltCapacity = lipo.capacity(FULL_UF);     // Read full capacity (mAh)
    unsigned int designCapacity = lipo.capacity(DESIGN);          // Read full capacity (mAh)
    unsigned int capacity = lipo.capacity(REMAIN);                // Read remaining capacity (mAh)
    int power = lipo.power();                                     // Read average power draw (mW)
    int healthperc = lipo.soh();                                  // Read state-of-health (%)
    int health = lipo.soh(5);                                     // Read state-of-health (%)

    // Now print out those values:
    String toPrint = String(socf) + "\t";
    toPrint += String(socunf) + "\t";
    toPrint += String(volts) + "\t";
    toPrint += String(currentavg) + "\t";
    toPrint += String(currentstby) + "\t";
    toPrint += String(currentmax) + "\t";
    toPrint += String(capacity) + "\t";
    toPrint += String(fullCapacity) + "\t";
    toPrint += String(availCapacity) + "\t";
    toPrint += String(availfullCapacity) + "\t";
    toPrint += String(remainfilCapacity) + "\t";
    toPrint += String(remainunfiltCapacity) + "\t";
    toPrint += String(fullfiltCapacity) + "\t";
    toPrint += String(fullunfiltCapacity) + "\t";
    toPrint += String(designCapacity) + "\t";
    toPrint += String(power) + "\t";
    toPrint += String(healthperc) + "\t";
    toPrint += String(health);

    uint16_t Control_status = lipo.status();
    uint16_t Flags = lipo.flags();
   // Serial.print("F:");
    //Serial.println(Flags, BIN);
    if (Flags & 32)
    {

        if (lipo.begin())
        {
            lipo.enterConfig();
            lipo.setCapacity(BATTERY_CAPACITY);
            lipo.setDesignEnergy(DESIGNE_ENERGY);
            lipo.setTerminateVoltage(TERMITANTE_VOLTAGE);
            lipo.setTaperRate(TAPER_RATE);
            lipo.exitConfig();
            Serial.println("Capacity changed!");
        }
        else
        {
            Serial.println("Capacity NOT changed!");
        }
    }
    //Serial.print("CS:");
   // Serial.println(Control_status, BIN);
    Serial.println(toPrint);
}
// ================  Write_TPIC2810 =====================*

void Write_TPIC2810(byte address, byte data)
{
    Wire.beginTransmission(byte(96)); // transmit command to device TPIC2810
    Wire.write(byte(68));             // Command to transfer next value to output register
    Wire.write(byte(data));
    Wire.endTransmission(); // stop transmitting
}
