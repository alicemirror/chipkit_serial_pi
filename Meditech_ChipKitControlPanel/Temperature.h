/**
 * \file Temperature.h
 * \brief Constants and class prototypes
 */

#ifndef Temperature_h
#define Temperature_h

#include <inttypes.h>

//! Constant for Kelvin temperature conversion
#define KELVINC 273

//! Analog channel for the temperature sensor
#define TEMP_SENSOR A1

class Temperature
{
  public:
    Temperature();
    void CalcTemp(float sensor);
    float Celsius() const;
  private:
    float _Celsius;
    float _sensorValue;

};

#endif

