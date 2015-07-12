/**
 *	\file Analog.h
 *  \brief Analog input constants
 *
 *  Constants definitions to manage the analog potentiometer input for digital stethoscope calibration
*/

#ifndef __ANALOG_H__
#define __ANALOG_H__

//! Units to show the potentiometer resistance value.
//! Set to 0 if the resistance should be in Ohm instead of K
#define KOHM  0  
//! Analog potentiometer value in Ohm
#define ANALOGPOT  4700.00
//! Steps divider from analog converter
#define ANALOGDIVIDER 1024.00

//! Minimum gain level mapped to the analog reading
#define MINGAIN 16
#define MAXGAIN 64

#define CALIBRATION_POT A0

#endif
