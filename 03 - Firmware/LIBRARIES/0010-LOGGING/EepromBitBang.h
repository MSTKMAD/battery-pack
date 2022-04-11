// Sketch to explore 24AA1024 using SlowSoftI2CMaster

//#include <batt_SlowSoftI2CMaster.h>
#define SDA_PIN 10
#define SCL_PIN 13

SlowSoftI2CMaster si = SlowSoftI2CMaster(SDA_PIN, SCL_PIN, true);

#define EEPROMADDR 0xA0 // set by jumper (A0 and A1 = High)

//------------------------------------------------------------------------------
/*
 * read one byte from address
 */
boolean readEEPROM(unsigned long address, uint8_t *byte)
{
    // issue a start condition, send device address and write direction bit
    if (!si.i2c_start(EEPROMADDR | I2C_WRITE | ((address>>8)*2)))
        return false;
    // send the address
    //if (!si.i2c_write((address >> 8) & 0xFF))
    //return false;
    if (!si.i2c_write(address & 0xFF))
        return false;

    // issue a repeated start condition, send device address and read direction bit
    if (!si.i2c_rep_start(EEPROMADDR | I2C_READ | ((address>>8)*2)))
        return false;

    *byte = si.i2c_read(true);

    si.i2c_stop();
    return true;
}

/*
 * write 1 byte to 'address' in eeprom
 */
boolean writeEEPROM(long unsigned address, uint8_t byte)
{
    // issue a start condition, send device address and write direction bit
    if (!si.i2c_start(EEPROMADDR | I2C_WRITE | ((address>>8)*2)))
        return false;

    // send the address
    //if (!si.i2c_write((address >> 8) & 0xFF))
    // return false;
    if (!si.i2c_write(address & 0xFF))
        return false;

    // send data to EEPROM
    if (!si.i2c_write(byte))
        return false;

    // issue a stop condition
    si.i2c_stop();

    delay(6);

    return true;
}

uint16_t ReadWordEEPROM(int16_t address)
{
    uint8_t low_byte = 0;
    uint8_t high_byte = 0;
    uint16_t value = 0;
    readEEPROM(address, &low_byte);
    readEEPROM(address + 1, &high_byte);
    value = (high_byte << 8) | low_byte;
    return value;
}
void WriteWordEEPROM(int16_t address, int16_t value)
{
    uint8_t low_byte = (byte)value;
    uint8_t high_byte = (byte)(value >> 8);
    writeEEPROM(address, low_byte);
    writeEEPROM(address + 1, high_byte);
}
void ReadArrayEEPROM(uint16_t address, uint16_t *array, uint16_t length_array)
{
    for (int i = 0; i < length_array; i++)
    {
        array[i] = ReadWordEEPROM(address + i * 2);
    }
}
void WriteArrayEEPROM(uint16_t address, uint16_t *array, uint16_t length_array)
{
    for (int i = 0; i < length_array; i++)
    {
        WriteWordEEPROM(address + i * 2, array[i]);
    }
}