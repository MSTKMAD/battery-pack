#include <batt_SlowSoftI2CMaster.h>
#include <EepromBitBang.h>

void setup() {
  Serial5.begin(9600);

  uint8_t data8;
  uint16_t data16;

  writeEEPROM(0, 21);
  delay(1000);
  readEEPROM(0, &data8);
  Serial5.println(data8);

  delay(1000);

  writeEEPROM(1, 22);
  delay(1000);
  readEEPROM(1, &data8);
  Serial5.println(data8);

  delay(500);

  Serial5.println("-----------------");

  readEEPROM(0, &data8);
  Serial5.println(data8);


  for (int i = 0; i < 10; i++) {
    if ( readEEPROM(i, &data8)) {
      Serial5.print(data8);
      //Serial5.print("/");
      //Serial5.print(EEPROMADDR | I2C_WRITE | (i & 0x10000 ? 8 : 0));
      Serial5.print("-");
      // delay(10);

    } else {
      Serial5.println("Fallo de LECTURA!");
    }
  }
  for (int i = 0; i < 10; i++) {
    if (writeEEPROM(i, i)) {
      Serial5.print(i);
      //Serial5.print("/");
      //Serial5.print(EEPROMADDR | I2C_WRITE | (i & 0x10000 ? 8 : 0));
      //Serial5.print("/");
      //Serial5.print((i >> 8) & 0xFF);
      //Serial5.print("/");
      //Serial5.print(i & 0xFF);
      Serial5.print("-");

      //delay(100);
    } else {
      Serial5.println("Fallo de escritura!");
    }
  }
  Serial5.println();

  //delay(1000);

  for (int i = 0; i < 10; i++) {
    if ( readEEPROM(i, &data8)) {
      Serial5.print(data8);
      //Serial5.print("/");
      //Serial5.print(EEPROMADDR | I2C_WRITE | (i & 0x10000 ? 8 : 0));
      Serial5.print("-");
      // delay(100);

    } else {
      Serial5.println("Fallo de LECTURA!");
    }
  }
  Serial5.println();

  for (int i = 0; i < 10; i++) {
    WriteWordEEPROM(i * 2, i*100);
    Serial5.print(i);
    //Serial5.print("/");
    //Serial5.print(EEPROMADDR | I2C_WRITE | (i & 0x10000 ? 8 : 0));
    //Serial5.print("/");
    //Serial5.print((i >> 8) & 0xFF);
    //Serial5.print("/");
    //Serial5.print(i & 0xFF);
    Serial5.print("-");

  }
  Serial5.println();

  //delay(1000);

  for (int i = 0; i < 10; i++) {
   data16 =  ReadWordEEPROM(i * 2);
    Serial5.print(data16);
    //Serial5.print("/");
    //Serial5.print(EEPROMADDR | I2C_WRITE | (i & 0x10000 ? 8 : 0));
    Serial5.print("-");
    // delay(100);

  }
  Serial5.println();

}

void loop() {
  // put your main code here, to run repeatedly:

}
