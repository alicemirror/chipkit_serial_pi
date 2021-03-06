/**
  \file Globals.h
  \brief Global constants
*/

#ifndef __GLOBALS_H__
#define __GLOBALS_H__

//! Lid open status
#define LIDOPEN false
//! Lid closed status
#define LIDCLOSED true

//! PWM fan control speed PIN
#define FAN_SPEED 2
//! ECG enabled PIN
#define ECG_STATUS 3
//! Stethoscope enabled PIN
#define STETHOSCOPE_STATUS 8
//! Camera transmitting enabled PIN
#define CAMERA_STATUS 9
//! Lid switch input pin
#define LIDSTATUS 10

//! LCD Shift control pin - Data signal
//! Define this value accordingly with the available I2C board pins
#define LCDdataPin 11
//! LCD Shift control pin - Clock signal
//! Define this value accordingly with the available I2C board pins
#define LCDclockPin 12
//! LCD Shift control pin - Latch signal
//! Define this value accordingly with the available I2C board pins
#define LCDlatchPin 13

//! Minimum PWM frequency for fan stopped
#define MIN_FANSPEED  35
//! Maximum PWM frequency for fan running
#define MAX_FANSPEED  255
//! PWM frequency for fan stopped (lower values has no effect)
#define STOP_FAN  5

//! Minimum temperature (Celsius) to start the fan (min speed)
//! Lower temperature values does not need the fan starting
#define MIN_TEMP  38.00
//! Maximum temperature (Celsius) to reach the higher fan speed.
//! Higher temperature values should generate an overheating alarm
//! condition
#define MAX_TEMP  60.00

//! Lid open check status frequency (1 sec.)
#define LID_OPEN_TIMEOUT  1

//! Fan cooler interrupt timer update frequency (5 secs.)
#define UPDATE_FAN_SPEED_TIMEOUT 5

//! The serial communication speed with the RPI master
#define SERIAL_SPEED 38400


//! Update the display task every second (in ms)
#define TASK_UPDATEDISPLAY  1000 

#endif


