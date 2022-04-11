/******************************************************************************
  BQ27441_Basic
  BQ27441 Library Basic Example
  Jim Lindblom @ SparkFun Electronics
  May 9, 2016
  https://github.com/sparkfun/SparkFun_BQ27441_Arduino_Library

  Demonstrates how to set up the BQ27441 and read state-of-charge (soc),
  battery voltage, average current, remaining capacity, average power, and
  state-of-health (soh).

  After uploading, open up the serial monitor to 115200 baud to view your
  battery's stats.

  Hardware Resources:
  - Arduino Development Board
  - SparkFun Battery Babysitter

  Development environment specifics:
  Arduino 1.6.7
  SparkFun Battery Babysitter v1.0
  Arduino Uno (any 'duino should do)
******************************************************************************/
#include <SparkFunBQ27441.h>

// Set BATTERY_CAPACITY to the design capacity of your battery.
const unsigned int BATTERY_CAPACITY = 2010; // e.g. 850mAh battery
const uint16_t DESIGNE_ENERGY = BATTERY_CAPACITY * 3.7f;
const uint16_t TERMITANTE_VOLTAGE = 3;
const uint16_t TAPER_CURRENT = 150;
const uint16_t TAPER_RATE = 10*BATTERY_CAPACITY/TAPER_CURRENT;



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
    while (1) ;
  }
  Serial.println("Connected to BQ27441!");
  Serial.println("Configure?");
  while (!Serial.available());
  char c = toupper(Serial.read());
  if ( c == 'Y') {
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
  unsigned int socf = lipo.soc(FILTERED);  // Read state-of-charge (%)
  unsigned int socunf = lipo.soc(UNFILTERED);  // Read state-of-charge (%)
  unsigned int volts = lipo.voltage(); // Read battery voltage (mV)
  int currentavg = lipo.current(AVG); // Read average current (mA)
  int currentstby = lipo.current(STBY); // Read average current (mA)
  int currentmax = lipo.current(MAX); // Read average current (mA)
  unsigned int fullCapacity = lipo.capacity(FULL); // Read full capacity (mAh)
  unsigned int availCapacity = lipo.capacity(AVAIL); // Read full capacity (mAh)
  unsigned int availfullCapacity = lipo.capacity(AVAIL_FULL); // Read full capacity (mAh)
  unsigned int remainfilCapacity = lipo.capacity(REMAIN_F); // Read full capacity (mAh)
  unsigned int remainunfiltCapacity = lipo.capacity(REMAIN_UF); // Read full capacity (mAh)
  unsigned int fullfiltCapacity = lipo.capacity(FULL_F); // Read full capacity (mAh)
  unsigned int fullunfiltCapacity = lipo.capacity(FULL_UF); // Read full capacity (mAh)
  unsigned int designCapacity = lipo.capacity(DESIGN); // Read full capacity (mAh)
  unsigned int capacity = lipo.capacity(REMAIN); // Read remaining capacity (mAh)
  int power = lipo.power(); // Read average power draw (mW)
  int healthperc = lipo.soh(); // Read state-of-health (%)
  int health = lipo.soh(5); // Read state-of-health (%)

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
  Serial.print("F:");
  Serial.println(Flags, BIN);
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
    } else {
      Serial.println("Capacity NOT changed!");
    }

  }
  Serial.print("CS:");
  Serial.println(Control_status, BIN);
  Serial.println(toPrint);
}

void setup()
{
  Serial.begin(115200);
  Serial.println("Iniciando...");
  setupBQ27441();
}

void loop()
{
  printBatteryStats();
  delay(1000);
}
