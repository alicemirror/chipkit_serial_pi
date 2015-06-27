/**
 *	\file Globals.h
 *  \brief Global constants
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

#endif


