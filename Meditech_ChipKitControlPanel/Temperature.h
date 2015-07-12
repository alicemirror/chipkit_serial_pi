/**
 * \file Temperature.h
 * \brief Constants and class prototypes
 */

#ifndef Temperature_h
#define Temperature_h

#include <inttypes.h>

//! Absolute zero value for calculation
#define ABSOLUTE_ZERO_CELSIUS -273.15

//! Celsius temperature offset. Should be calibrated depending on the
//! analog reading from the choosen chip. This value is preset for 
//! LM35 temperature sensor.
#define TEMP_OFFSET 10

//! Analog channel for the temperature sensor
#define TEMP_SENSOR A1

class Temperature
{
  public:
    Temperature();
    void CalcTemp(float sensor);
    float Celsius() const;
    float Fahrenheit() const;
    float Kelvin() const;
    float Rankine() const;
  private:
    float _Celsius;
    float _Fahrenheit;
    float _Kelvin;
    float _Rankine;
    float _sensorValue;

};

#endif

