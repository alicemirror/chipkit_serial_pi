/**
 * \file Version.h
 * \brief Version and Build Number Helper Class
 *
 * This helper macros exposes the static methods to get the firmware version and the build number.
 * Use the build() and version() methods anywhere in the program including this file
*/
#ifndef __VERSION_H__
#define __VERSION_H__

//! Incremental build number
#define build() "0035"
//! Firmware version
#define version() "1.0"
//! Project name
#define project() "Meditech beta"

#endif //__VERSION_H__
