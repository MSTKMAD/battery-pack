/**
 * @file PCB early test.ino
 * @author Javi (Javier@musotoku.com)
 * @brief 
 * @version 1
 * @date 2021-08-30
 * 
 * @copyright Copyright (c) 2021
 * 
 */

int16_t C_PIN_SDA = 10;
int16_t C_PIN_SCL = 13;
int16_t C_PIN_FLAG_CHARGE = 5;
int16_t C_PIN_FLAG_BATT_LOW = 12;
int16_t C_PIN_VIN_SENSE = A5;
int16_t C_PIN_VO_SENSE = A4;
int16_t C_PIN_IO_SENSE = A1;
int16_t C_PIN_ENABLE_LDO_VCC_2 = 0;

int32_t v_in;
int32_t v_out;
int32_t i_o;

int16_t version = 5;
void setup()
{
    // Pines en modo salida.
    pinMode(C_PIN_SDA, OUTPUT);
    pinMode(C_PIN_SCL, OUTPUT);
    pinMode(C_PIN_ENABLE_LDO_VCC_2, OUTPUT);
    pinMode(C_PIN_FLAG_CHARGE, INPUT_PULLUP);
    pinMode(C_PIN_FLAG_BATT_LOW, INPUT_PULLUP);
    pinMode(C_PIN_VIN_SENSE, INPUT_PULLDOWN);
    pinMode(C_PIN_IO_SENSE, INPUT_PULLDOWN);
    pinMode(C_PIN_IO_SENSE, INPUT_PULLDOWN);

    digitalWrite(C_PIN_ENABLE_LDO_VCC_2, HIGH);

    // Inicialiar el peurto serie.
    Serial.begin(9600);
    Serial5.begin(9600);
    // Resolucion del ADC 12 bits.
    analogReadResolution(12);
    analogReference(AR_DEFAULT);
}

void loop()
{
    // Blink de los pines SDA y SCL.
    digitalWrite(C_PIN_SDA, LOW);
    digitalWrite(C_PIN_SCL, LOW);
    delay(500);
    digitalWrite(C_PIN_SDA, HIGH);
    digitalWrite(C_PIN_SCL, HIGH);
    delay(500);
    // Comunicacion por el puerto serie a modo de prueba.
    Serial5.printf("\n------------------------------------------------------------------------\n");
    Serial5.printf("\nTest de validacion del PCB de la CPU del proyecto Battery Pack Rover 2k.\n");
    Serial5.printf("Version de la placa : %d \n\n",version);
    Serial5.printf("\t1) Test 1: Flag Charge.\n");
    Serial5.printf("\t2) Test 2: Flag BatLowe.\n");
    Serial5.printf("\t3) Test 3: Isense. \n");
    Serial5.printf("\t4) Off.");
    Serial5.printf("\n------------------------------------------------------------------------\n");
    // Lectura de los puertos del ADC correspondientes a Vin e Io.
    v_in = analogRead(C_PIN_VIN_SENSE)*250/150*3000/4096;
    i_o = analogRead(C_PIN_IO_SENSE)*3000/4096;
    v_out = analogRead(C_PIN_VO_SENSE)*3000/4096;
    Serial5.printf("Vin:%d\n", v_in);
    Serial5.printf("Vo:%d\n", v_out);
    Serial5.printf("Io: %d\n", i_o);
    // Lectura de los Flags de Carga y Bateria Baja.
    Serial5.printf("Flag_charge: %d \t\t Flag_Bat_low: %d \n", digitalRead(C_PIN_FLAG_CHARGE), digitalRead(C_PIN_FLAG_BATT_LOW));
}