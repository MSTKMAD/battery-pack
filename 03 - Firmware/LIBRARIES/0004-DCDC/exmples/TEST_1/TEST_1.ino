/**
 * @file TEST_1.ino
 * @author Javi (Javier@musotoku.com)
 * @brief 
 * @version 0.1
 * @date 2021-09-03
 * 
 * @copyright Copyright (c) 2021
 * 
 */
#include <DCDC.h>
const uint16_t C_PIN_EN_DCDC = 2;
dcdc_controler DCDC(C_PIN_EN_DCDC);
void setup()
{
    Wire.begin();
}

void loop()
{
    DCDC.SetVoltage(100, C_NON_BOOST_MODE);
    delay(500);
}