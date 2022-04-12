/**
   @file cyclator.ino
   @author Javi (Javier@musotoku.com)
   @brief
   @version 0.1
   @date 2020-09-28

   @copyright Copyright (c) 2020

*/
#include <SparkFunBQ27441.h>
#include <SPI.h>
#include <SD.h>
#include <EEPROM.h>
#include <MilliTimer.h>

//------------ Constants----------------//
/*    SD    */
const int chipSelect = 10;
//

/*    Babysitter      */
const uint16_t BATTERY_CAPACITY = 2010; // e.g. 850mAh battery
const uint16_t DESIGNE_ENERGY = BATTERY_CAPACITY * 3.7f;
const uint16_t TERMITANTE_VOLTAGE = 3;
const uint16_t TAPER_CURRENT = 150;
const uint16_t TAPER_RATE = 10 * BATTERY_CAPACITY / TAPER_CURRENT;

/*    cyclator      */
const int16_t PIN_LOAD_1 = 7;
const int16_t PIN_LOAD_2 = 6;
const int16_t PIN_CHARGE_ON = 5;

const int16_t CHARGING = 0xAA;
const int16_t DISCHARGE = 0x55;
//
/*     TIMER       */
const int32_t C_TIMER_WD = 5 * 60000;
const int32_t C_TIMER_STOP = 15 * 60000;
const int32_t C_BREATH_TIME = 2 * 60000;

const int32_t C_TIMER_LOGGING = 60;
const int32_t C_TIMER_CHARGE = 3 * 3600;
const int32_t C_TIMER_DISCHARGE = 3 * 3600;

//--------------- VARIABLE --------------//
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
uint32_t power_homemade = 0;
String toPrint;

/*    cyclator      */
int16_t cycle = 0;
bool flag_cycle = false;
int32_t cont = 0;
int32_t cont_cycle = 0;
int state = CHARGING;
int16_t cont_five = 0;
uint16_t theory_cap = 0;
/*     TIMER       */
int32_t timer_wd = 0;
bool wd_flag = false;
int32_t timer_stop = 0;
bool stop_flag = false;

byte check_EEPROM = 0;
byte check_EEPROM_add = 3;
byte C_EEPROM_CHECK = 0x86;

MilliTimer timer;

void initialization()
{
  check_EEPROM = EEPROM.read(check_EEPROM_add);
  if (check_EEPROM != C_EEPROM_CHECK)
  {
    EEPROM.write(0, 150);
    EEPROM.write(1, 0);
    EEPROM.write(2, 0);
    EEPROM.write(3, C_EEPROM_CHECK);
  }
  timer_wd = millis();
  timer_stop = millis();
  pinMode(PIN_LOAD_1, OUTPUT);
  pinMode(PIN_LOAD_2, OUTPUT);
  pinMode(PIN_CHARGE_ON, OUTPUT);
  digitalWrite(PIN_CHARGE_ON, LOW);
  digitalWrite(PIN_LOAD_1, LOW);
  digitalWrite(PIN_LOAD_2, LOW);
  Serial.begin(9600);
  setupBQ27441();
  setupSD();
  cycle = EEPROM.read(0);
}
void setupSD()
{
  Serial.println("Initializing SD card...");
  if (SD.begin(chipSelect))
  {
    Serial.println("Connected to SD Card!");
    while (Serial.available())
    {
      Serial.read();
    }
    Serial.println("Erase the SD card ?(Y/N)");
    while (!Serial.available() && ((millis() - timer_wd) < C_TIMER_WD))
    {
      Serial.print((millis() - timer_wd));
      Serial.print("/");
      Serial.println(C_TIMER_WD);
    }
    if (Serial.available())
    {
      char c = toupper(Serial.read());
      if (c == 'Y')
      {
        SD.remove("datalog.csv");
        File dataFile = SD.open("datalog.csv", FILE_WRITE);
        if (dataFile)
        {
          dataFile.println("Percent;Volts;Current AVG;Capacity;FullCapacity;DesignCapacity;Power;Temp;Health;Cycles");
          dataFile.close();
          Serial.println("Percent;Volts;Current AVG;Capacity;FullCapacity;DesignCapacity;Power;Temp;Health;Cycles");
          Serial.println("Card Erased.");
        }
      }
    }
    else
    {
      while (1)
        ;
    }
  }
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

  // toPrint = String(socf) + ";";
  toPrint = String(volts) + ";";
  toPrint += String(currentavg) + ";";
  // toPrint += String(capacity) + ";";
  // toPrint += String(fullCapacity) + ";";
  // toPrint += String(designCapacity) + ";";
  // toPrint += String(power) + ";";
  // toPrint += String(temperatura) + ";";
  // toPrint += String(healthperc) + ";";
  toPrint += String(cycle) + ";";
  power_homemade = volts * currentavg / 1000000;
  toPrint += String(power_homemade) + ";";
}
void setup()
{
  initialization();

  digitalWrite(PIN_CHARGE_ON, HIGH);
}
void loop()
{
  if (timer.poll(1000) != C_TIMER_NOT_EXPIRED)
  {
    cont++;
    cont_cycle++;
    // Serial.print(state);
    // Serial.print(";");
    // Serial.print(cont);
    // Serial.print(";");
    ReadBatteryStats();
    Serial.println(toPrint);



    if (state == CHARGING)
    {

      digitalWrite(PIN_CHARGE_ON, HIGH);
      digitalWrite(PIN_LOAD_1, LOW);
      digitalWrite(PIN_LOAD_2, LOW);

      if ((theory_cap == 100) && (currentavg == 0))
      {
        state = DISCHARGE;
        cycle++;
        EEPROM.write(0, cycle);
        cont_cycle = 0.0;
      }
    }
    if (flag_cycle == true)
    {
      flag_cycle = false;
      Serial.print(cont_cycle / 60);
    }

    if (cont == C_TIMER_LOGGING)
    {
      cont = 0;
      //    if (dataFile)
      //    {
      //      dataFile.println(toPrint);
      //      dataFile.close();
      //    }
      bool state_load_1 = digitalRead(PIN_LOAD_1);
      bool state_load_2 = digitalRead(PIN_LOAD_2);
      bool state_charge = digitalRead(PIN_CHARGE_ON);
      digitalWrite(PIN_LOAD_1, LOW);
      digitalWrite(PIN_LOAD_2, LOW);
      digitalWrite(PIN_CHARGE_ON, LOW);
      delay(5000);
      ReadBatteryStats();
      if ( volts >= 3600) {
        theory_cap = (volts - 3600);
        theory_cap = theory_cap * 85;
        theory_cap = theory_cap / 500;
        theory_cap = theory_cap  + 15;
        theory_cap = constrain(theory_cap, 15, 100);
      } else {
        theory_cap = constrain(((volts - 3300) * 15 / 300), 0, 15);
      }
      toPrint += String(theory_cap) + ";";
      Serial.println(toPrint);
      File dataFile = SD.open("datalog.csv", FILE_WRITE);
      if (dataFile)
      {
        dataFile.println(toPrint);
        dataFile.close();
      }
      digitalWrite(PIN_LOAD_1, state_load_1);
      digitalWrite(PIN_LOAD_2, state_load_2);
      digitalWrite(PIN_CHARGE_ON, state_charge);
      delay(2000);
      if (state == DISCHARGE)
      {

        digitalWrite(PIN_CHARGE_ON, LOW);
        digitalWrite(PIN_LOAD_1, HIGH);
        digitalWrite(PIN_LOAD_2, HIGH);

        if (volts <= 3300)
        {
          cont_five++;
          if (cont_five == 2)
          {
            flag_cycle = true;
            state = CHARGING;
            cont_five = 0;
          }
        }
      }
    }
  }
}
