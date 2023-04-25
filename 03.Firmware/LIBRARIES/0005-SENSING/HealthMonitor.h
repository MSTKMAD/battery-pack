/**
 * @file HealthMonitor.h
 * @author Javi (Javier@musotoku.com)
 * @brief
 * @version 4
 * @date 2020-3-18
 *
 * @copyright Copyright (c) 2020
 *
 */
#include <batt_SAMD_AnalogCorrection.h>

const int16_t C_TIME_HEALTH_MONITORING = 50; // ms
const int16_t C_SAMPLING_TIME = 10;          // ms

const int16_t C_RISE_COUNT = 20;

const int16_t C_COUNTER_LIMIT = C_RISE_COUNT * C_TIME_HEALTH_MONITORING / C_SAMPLING_TIME;

class HealthMonitor
{
  // Class Member Variables
  uint16_t counter; // private, only accesible by setCounter method

public:
  uint16_t threshold;
  uint8_t rampUPinc;
  uint8_t rampDOWNdec;
  uint16_t limit;
  bool alarm;
  int16_t sense_values[8]; // Filter samples

  /**
   * @brief Construct a new Health Monitor object
   *
   * @param thres
   * @param inc
   * @param dec
   * @param lim
   */
  HealthMonitor(uint16_t thres, uint8_t inc, uint8_t dec, uint16_t lim)
  {
    threshold = thres;
    rampUPinc = inc;
    rampDOWNdec = dec;
    limit = lim;
    counter = 0;
  }
  /**
   * @brief Set the Counter object
   *
   * @param value
   */
  void setCounter(uint16_t value)
  {
    counter = constrain(value, 0, UINT16_MAX - 1);
  }

  /**
   * @brief Get the Counter object
   *
   * @return uint16_t
   */
  uint16_t getCounter()
  {
    uint16_t percent;

    percent = counter * 100 / C_COUNTER_LIMIT;

    return percent;
  }

  /**
   * @brief Check the input and return an alarm if the conter exceed the count limit..
   *
   * @param sample
   * @return boolean
   */
  boolean check(uint16_t sample)
  {

    if (sample > threshold)
    {
      counter = constrain(counter, 0, UINT16_MAX - rampUPinc); // prevent counter overflow before inc/dec
      counter = counter + rampUPinc;
    }
    else
    {
      counter = constrain(counter, rampDOWNdec, UINT16_MAX - rampUPinc); // prevent counter overflow before inc/dec
      counter = counter - rampDOWNdec;
    }

    if (counter >= limit)
    {
      counter = constrain(counter, rampDOWNdec, limit);
      alarm = true;
    }
    else
    {
      alarm = false;
    }
    return (alarm);
  }

  /**
   * @brief Get the Sample object from an ADC pin.
   *
   * @param ADCpin
   * @return uint16_t
   */
  uint16_t getSample(uint16_t ADCpin)
  {
    //analogReadCorrection(12, 2055);
    analogReadResolution(12); // Set analog input resolution to max, 12-bits
    int16_t out_sample = 0;
    int16_t sample = 0;

    for (int i = 0; i < 8; i++)
    {
      sample += analogRead(ADCpin);
    }
    sample = sample / 8;

    // for (int i = 7; i >= 1; i--)
    // {
    //   sense_values[i] = sense_values[i - 1];
    // }
    // out_sample += sense_values[7] / 4;
    // out_sample += sense_values[6] / 4;
    // out_sample += sense_values[5] / 4;
    // out_sample += sense_values[4] / 4;
    // out_sample += sense_values[3] / 2;
    // out_sample += sense_values[2] / 2;
    // out_sample += sense_values[1];
    // out_sample += sample;
    

    // sample = out_sample / 4;
    // sense_values[0] = sample;
    return sample;
  }
};