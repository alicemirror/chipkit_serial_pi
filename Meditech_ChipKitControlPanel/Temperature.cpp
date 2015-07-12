/**
  \file Temperature.cpp
  \brief Class to manage the internal temperature of the devices.
  
  The Temperature class acquires data from an LM35 temperature sensor in this specific case.
  
  \page temp_convert Temperature conversion formulas
  The <CODE>Temperature</CODE> class manages the analog readings from an <EM>LM35</EM> temperature sensor device to control the
  cooler fan speed.\n
  The base calculation for the temperature after the sensor reading is managed by the <CODE>CalcTemp</CODE> method of the
  Temperature class; the 10-bit AD conversion generates 1024 possible values (between 0 and 1023) so the base
  Celsius float conversion formula is:\n
  
  <CENTER><B>[sensorValue / 1024] * 5) * 100 </B></CENTER>
  
  From this first conversion calculation are derived all the other units values: Fahrenheit, Kelvin and Rankine
  following the formulas below:\n
  
  <CENTER><B>
    Fahrenheit = (Celsius * 9) / 5) + 32<BR>
    Kelvin = Celsius - ABSOLUTE_ZERO_CELSIUS<BR>
    Rankine = (Celsius - ABSOLUTE_ZERO_CELSIUS) * 9) / 5<BR>
  </B></CENTER>
  
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
 * converted to the three temperature scales: Celsius, Kelvin and Farehneit.
 */
void Temperature::CalcTemp(float sensor) {

  _sensorValue = sensor;
  _Celsius = (((_sensorValue / 1024) * 5) * 100) - TEMP_OFFSET; // convert the reading to C
  _Fahrenheit = _Celsius * 1.8 + 32.0; // convert the reading to F
  _Kelvin = _Celsius - ABSOLUTE_ZERO_CELSIUS; // convert the reading to K
  _Rankine = ((_Celsius - ABSOLUTE_ZERO_CELSIUS) * 9) / 5; // convert the reading do R
}

//! \brief Return the last temperature read in Celsius deg
float Temperature::Celsius() const {
	return(_Celsius);
}

//! \brief Return the last temperature read in Fahrenheit deg
float Temperature::Fahrenheit() const {
	return(_Fahrenheit);
}

//! \brief Return the last temperature read in Kelvin deg
float Temperature::Kelvin() const {
	return(_Kelvin);
}

//! \brief Return the last temperature read in Rankine deg
float Temperature::Rankine() const {
	return(_Rankine);
}

