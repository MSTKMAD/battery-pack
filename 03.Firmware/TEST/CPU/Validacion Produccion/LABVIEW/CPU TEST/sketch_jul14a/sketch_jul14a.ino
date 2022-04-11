int16_t i;
char data;
const uint16_t C_PIN_SW1 = 2;
const uint16_t C_PIN_SW2 = 3;
const uint16_t C_PIN_LED_3 = 4;
const uint16_t C_PIN_ENABLE_LDO = 5;
const uint16_t C_PIN_LED_1 = 6;
const uint16_t C_PIN_SDA_2 = 7;
const uint16_t C_PIN_SCL_2 = 8;
const uint16_t C_PIN_LED_2 = 9;
const uint16_t C_PIN_ISENSE_RAW = 10;
const uint16_t C_PIN_IOUT_SENSE = A0;
const uint16_t C_PIN_VCC_2 = A1;
const uint16_t C_PIN_VCC = A2;

int16_t iout_sense = 0, vcc = 0, vcc_2 = 0;

void setup()
{
  Serial.begin(9600);
  analogReference(DEFAULT);
  pinMode(C_PIN_LED_1, OUTPUT);
  pinMode(C_PIN_LED_2, OUTPUT);
  pinMode(C_PIN_LED_3, OUTPUT);
  pinMode(C_PIN_ENABLE_LDO, OUTPUT);
  pinMode(C_PIN_ISENSE_RAW, OUTPUT);
  pinMode(C_PIN_SW1, OUTPUT);
  pinMode(C_PIN_SW2, OUTPUT);
  pinMode(C_PIN_IOUT_SENSE, INPUT);
  pinMode(C_PIN_VCC_2, INPUT);
  pinMode(C_PIN_VCC, INPUT);
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(C_PIN_LED_1, LOW);
  digitalWrite(C_PIN_LED_2, LOW);
  digitalWrite(C_PIN_LED_3, LOW);
  digitalWrite(C_PIN_ENABLE_LDO, LOW);
  digitalWrite(C_PIN_ISENSE_RAW, LOW);
  digitalWrite(C_PIN_SW1, LOW);
  digitalWrite(C_PIN_SW2, LOW);

  digitalWrite(C_PIN_ENABLE_LDO, HIGH);
  digitalWrite(C_PIN_ISENSE_RAW, HIGH);
  delay(100);
}

void loop()
{

  digitalRead(C_PIN_SDA_2);
  digitalRead(C_PIN_SCL_2);
  iout_sense = 0;
  // for (int i = 0; i < 8; i++)
  // {
  //   iout_sense += analogRead(C_PIN_IOUT_SENSE);
  //   delay(100);
  // }
  // iout_sense = (iout_sense >> 3);
  // vcc = 0;
  // for (int i = 0; i < 8; i++)
  // {
  //   vcc += analogRead(C_PIN_VCC);
  //   delay(100);
  // }
  // vcc = (vcc >> 3);
  // vcc_2 = 0;
  // for (int i = 0; i < 8; i++)
  // {
  //   vcc_2 += analogRead(C_PIN_VCC_2);
  //   delay(100);
  // // }
  // vcc_2 = (vcc_2 / 8);

  if (Serial.available())
  {
    delay(500);
    digitalWrite(LED_BUILTIN, HIGH);
    data = Serial.read();
    if (data == '1')
    {
      digitalWrite(C_PIN_LED_1, HIGH);
      digitalWrite(C_PIN_SW1, LOW);
      digitalWrite(C_PIN_SW2, LOW);
      delay(200);
    }
    else if (data == '2')
    {
      digitalWrite(C_PIN_LED_2, HIGH);
      digitalWrite(C_PIN_SW1, HIGH);
      digitalWrite(C_PIN_SW2, LOW);
    }
    else if (data == '3')
    {
      digitalWrite(C_PIN_LED_3, HIGH);
      digitalWrite(C_PIN_SW1, LOW);
      digitalWrite(C_PIN_SW2, HIGH);
    }
    else if (data == '0')
    {
      digitalWrite(C_PIN_LED_1, LOW);
      digitalWrite(C_PIN_LED_2, LOW);
      digitalWrite(C_PIN_LED_3, LOW);
    }

    Serial.print(data);
    Serial.print("\t");
    Serial.print(digitalRead(C_PIN_SCL_2));
    Serial.print(';');
    Serial.print(digitalRead(C_PIN_SDA_2));
    Serial.print(';');
    Serial.print(digitalRead(C_PIN_SCL_2));
    Serial.print(';');
    Serial.print(analogRead(C_PIN_IOUT_SENSE));
    Serial.print(';');
    Serial.print(analogRead(C_PIN_VCC));
    Serial.print(';');
    Serial.print(analogRead(C_PIN_VCC_2));
    Serial.print("\n");
    delay(500);
    digitalWrite(LED_BUILTIN, LOW);
  }

  // //-----------------------------------------
  // //  Test LDO
  // digitalWrite(C_PIN_ENABLE_LDO, HIGH);
  // digitalWrite(C_PIN_ISENSE_RAW, HIGH);

  // //-----------------------------------------
  // //  Test Digital Ports
  // digitalWrite(C_PIN_LED_1, HIGH);
  // delay(500);
  // digitalWrite(C_PIN_LED_2, HIGH);
  // delay(500);
  // digitalWrite(C_PIN_LED_3, HIGH);
  // delay(500);
  // digitalWrite(C_PIN_ENABLE_LDO, HIGH);
  // delay(500);
  // digitalWrite(C_PIN_ISENSE_RAW, HIGH);
  // delay(500);
  // digitalWrite(C_PIN_SW1, HIGH);
  // delay(500);
  // digitalWrite(C_PIN_SW2, HIGH);
  // delay(500);
  // while (1)
  // {
  //   /* code */
  // }

  // digitalWrite(C_PIN_LED_1, LOW);
  // digitalWrite(C_PIN_LED_2, LOW);
  // digitalWrite(C_PIN_LED_3, LOW);
  // digitalWrite(C_PIN_ENABLE_LDO, LOW);
  // digitalWrite(C_PIN_ISENSE_RAW, LOW);
  // digitalWrite(C_PIN_SW1, LOW);
  // digitalWrite(C_PIN_SW2, LOW);

  //-----------------------------------------
  //   Test Serial Port
  //
  // if (Serial.available())
  // {
  //   data = Serial.readString();
  //   data.trim();
  //   Serial.print(data);
  // }
  // for (i = 0; i < 10; i++)
  // {
  //   Serial.print("\t");
  //   Serial.print(i);
  //   Serial.print(";");
  //   Serial.print(i + 5);
  //   Serial.print(";");
  //   Serial.print(i * 10);
  //   Serial.print(";");
  //   Serial.print(i * 2);
  //   Serial.print("\n");
  //   delay(500);
  // }
}
