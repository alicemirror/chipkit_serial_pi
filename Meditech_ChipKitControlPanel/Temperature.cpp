/**
 * \file Temperature.cpp
 * \brief Class to manage the internal temperature of the devices.
 * 
 * The Temperature class is based on the LM35 temperature sensor
 *
 */
 
 #include "Temperature.h"
 
 /**
  * \brief Temperature class constructor. Set the analog pin for data reading
  */
 Temperature::Temperature() {
 } 

/**
 * \brief Convert the analog value to the respective value.
 *
 * As the call to the temperature reading is not a high priority task and does
 * not need very high responsivity, when the analog data are read they are immediately
 * converted to the three temperature scales: Celsius, Kelvin and Farehneit
 */
void Temperature::CalcTemp(float sensor) {

  _sensorValue = sensor;
  _Celsius = (((_sensorValue / 1024) * 5) * 100) - 10; // convert the reading to C
}

float Temperature::Celsius() const {
	return(_Celsius);
}

